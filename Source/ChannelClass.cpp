/*
  ==============================================================================

    ChannelClass.cpp
    Created: 14 Apr 2023 9:48:22pm
    Author:  anders

  ==============================================================================
*/

#include "ChannelClass.h"
#include "ChannelIDMap.h"
#include <iomanip>
#include <sstream>
#include <unistd.h>

// UNCOMMENT TO ENABLE DEBUG MESSAGES.
#define CHANNEL_DEBUG_MESSAGES

#ifdef CHANNEL_DEBUG_MESSAGES
#define DEBUG_MSG(format, ...) printf("CHNL_DBG: " format, ##__VA_ARGS__)
#else
#define DEBUG_MSG(format, ...) ((void)0) // do {} while (0)
#endif

// Set first channel ID. This will increment with every channel object constructed.
uint8_t Channel::nextChannelNumber = 0;

// ############################### CONSTRUCTOR #########################################
// Channel Constructor. Set up channel number and get channel dsp ID from channelIDMap.h
// Convert channel number to channelStripID for the associate map.
// #####################################################################################
Channel::Channel()
    : channelID{dspChannelIDs[nextChannelNumber]}, // Get unique channel ID for dsp commands.
      channelNumber{nextChannelNumber},            // Set unique channel number.
      eventBus(EventBus::getInstance()),           // Get the singleton instances
      brainCom(BrainCom::getInstance()),
      dspCom(DspCom::getInstance())
{
    // Set up variables for initial channel strip event subscription
    int initialAssociateStripNumber = channelNumber;
    Bank initialAssociateBank = LINE_BANK;

    // TODO: Adjust for channels above 24.
    if (channelNumber > 23)
    {
        initialAssociateStripNumber -= 23;
        initialAssociateBank = TAPE_BANK;
    }

    // Convert Channel Number to a corresponding channel strip ID.
    std::stringstream stream;
    stream << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << initialAssociateStripNumber;
    std::string initialAssociateChannelStripID = stream.str();

    // Put the initial channel strip ID in the associate map, for the initial bank:
    std::unordered_set<std::string> initialStripSet{initialAssociateChannelStripID};
    associatedChannelStrips[initialAssociateBank] = initialStripSet;

    // Subscribe to events on the given bank and strip.
    eventBus.bankEventSubscribe
    (
        initialAssociateBank,
        initialAssociateChannelStripID,
        [this](const std::string &faderValue, const Bank bank, const std::string &channelStripID, EventSource source)
        { this->channelStripFaderEventCallback(faderValue, bank, channelStripID, source); },
        [this](const std::string &vpotValue, const Bank bank, const std::string &channelStripID, EventSource source)
        { this->channelStripVpotEventCallback(vpotValue, bank, channelStripID, source); },
        [this](const std::string &buttonValue, const Bank bank, const std::string &channelStripID, EventSource source)
        { this->channelStripButtonEventCallback(buttonValue, bank, channelStripID, source); },
        [this](const Bank bank, const std::string &channelStripID)
        { this->removeChStripAssociationCallback(bank, channelStripID); }
    );

    // Set initial Vpot handler method
    currentVpotFunction = &Channel::handleVpotPan;



    // INITIAL SETTINGS - WE CAN SET THEM HERE ARBITRARILY, OR USE A SPECIFIC CALL??
    // TODO: We need to be able to save and recall settings across reboots.
    // On construction, we should read saved values and set them.
    // Default saved values must be present, and recreated if not present.
    mute = false;

    pan = 0x7F; // Center
    panDotCenter = true;

    // solo = false;



    // assignments[0] = true;  // Assign to L-R. All others should default to false.

    // Initialize other member variables to default values...

    // Increment the nextChannelNumber for the next channel construction.
    nextChannelNumber++;


    // TODO: HERE AT THE BOTTOM MAYBE WE SHOULD SEND INITIAL SETTINGS TO THE DSP BOARD??
}

// Implement other member functions...

// #######################################################################################
// This method updates the channel object's volume member, and sends a command to the DSP
// #######################################################################################
void Channel::setVolume(std::string volumeValue)
{
    // Update volume member
    volume = volumeValue;

    // Construct DSP command, and send it.
    std::string volumeCommand = channelID + "cX" + volumeValue + "Q";
    if (!mute)
        // write(*dspDescriptorPtr, volumeCommand.c_str(), volumeCommand.length());
        dspCom.send(volumeCommand);
}

// ###########################################################################################################################
// This is a callback function that is fired by the EventBus, when a fader is moved.
// First it sends a DSP command to update the volume. Then it will iterate over all associalted channelstrips
// on the console, and update them. Finaly it will post another event, which will update the channelStripComponents in the UI
// ###########################################################################################################################
void Channel::channelStripFaderEventCallback(const std::string faderValue,
                                             const Bank bank,
                                             const std::string &channelStripID,
                                             EventSource source)
{

    // TODO: THIS MAY NEED TO CHANGE A BIT, IF WE CONTINUE TO USE THE CHANNEL CLASS FOR EFFECTS, MIDI, BUS, GROUPS, ETC.

    // Construct DSP volume command, and send it.
    std::string volumeCommand = channelID + "cX" + faderValue + "Q";

    // Only send dsp command if channel is not muted.
    if (!mute)
        // write(*dspDescriptorPtr, volumeCommand.c_str(), volumeCommand.length());
        dspCom.send(volumeCommand);

    // Update volume member - even if muted, so we know what level to go at, when unmuting.
    volume = faderValue;

    // Retrieve the set of associated CONSOLE channel strips on the current bank.
    // (the other banks will have their stuff set, when bank is changed.)
    std::unordered_set<std::string> associatedConsoleStrips = associatedChannelStrips[bank];

    // Make a copy for the associated UI channel strips on the current bank.
    std::unordered_set<std::string> associatedUiStrips = associatedConsoleStrips;

    // Depending on whether the event was triggered by CONSOLE or UI, remove the ID that was activated
    if (source == CONSOLE_EVENT)
    {
        associatedConsoleStrips.erase(channelStripID);
        
    }
    else
    {
        associatedUiStrips.erase(channelStripID);
    }

    // Iterate through the set, send move command to move the console faders
    for (auto &stripID : associatedConsoleStrips)
    {
        std::string faderCommand = stripID + faderValue + "f";
        brainCom.send(faderCommand);
    }

    // Now make an event post, for the UI strips to get updated
    // It runs a for loop, so works even if set is empty.
    eventBus.associateChStripUiEventPost(associatedUiStrips, FADER_EVENT, faderValue);
}

void Channel::channelStripVpotEventCallback(const std::string vpotValue,
                                            const Bank bank,
                                            const std::string &channelStripID,
                                            EventSource source)
{
    // The supplied value is likely not a specific placement, but rather a number of how fast/far the pot was turned.
    // So the procedure here is:
    // 1 - DEDUCE WHAT THE POT IS CONTROLING (done by pointer to current function)
    // 2 - CALCULATE NEW VALUE FROM OLD VALUE ON SUPPLIED CHANGE-VALUE (done in specific function)
    // 3 - SEND DSP (OR BRAIN?) COMMAND TO UPDATE WHAT THE VPOT IS CHOSEN TO CONTROL
    // 4 - UPDATE PRIVATE MEMBER
    // 5 - SEND AN ASSOCIATE EVENT POST FOR UPDATING THE UI, AND THE LED's ON THE CONSOLE
    // --------------------------------------------------------------------------------

    // So , I guess here it could make sense to use a pointer to a function, depending on what mode is selected:
    // A - Master Pan (pan button below master vpot) - ch. 1-72 + 81-88 pan control. (no pan for groups and busses)
    // B - Aux Send Level (Aux1-8 buttons) (no aux for groups, midis and busses)
    // C - Aux 9-10 / 11-12 send level for the stereo pair
    // D - Aux 9-10 / 11-12 PAN pan control for the stereo pair.
    // E - Level to tape
    // F - Digital trim. (only for the 48 audio channels)

    //The pots send values in same form as the faders. BUT the value is Fy for CCW and 0y for CW rotation, where y is the value depending on how fast the pot was turned.
    // ---------------------------------------------------------------------------------------------

    // So, channelstrip vpot functionality is global, so same across all banks. (except for channels that dont support - like aux for bus)

    // Here's a list of what the different channels support:
    // Ch. 1- 48: all.
    // Ch. 49 - 



    // This points to the method, related to current selected vpots functionality.
    // It is only responsible for sending the relevant DSP command.    
    (this->*currentVpotFunction)(vpotValue, bank, channelStripID, source);

    // DSP command was sent. Now we need to update the console and UI, including associated channelstrips.
    // HOW DO WE HANDLE SEPERATION BETWEEN SUPPORTED FUNCTIONALITY?
    
    // ALL CHANNELS WILL SUPPORT _SOME_ POT ACTION, SO THEY SHOULD SUBSCRIBE TO THE EVENT.
    // THEN WE CAN MANAGE IN-CLASS, BY MANAGING WHICH FUNCTIONS AR POINTED TO?
    // PRESUMABLY THIS IS IN THE METHOD THAT CHANGES THE METHOD POINTED TO.

    // This also means, that associated channelstrips should get updated by the pointed function, not here.
}

void Channel::channelStripButtonEventCallback(const std::string buttonID,
                                              const Bank bank,
                                              const std::string &channelStripID,
                                              EventSource source)
{
    // TODO: SOME WORK TO BE DONE HERE...

    // So what we wanna do here is:
    // 1: check which button.
    printf("BUTTON CLICKED");
}



// ####################################################################################################
// This method is a callback for removing the supplied channel strip from the set of associated strips.
// This is called from the event bus, when an association is updated.
// ####################################################################################################
void Channel::removeChStripAssociationCallback(const Bank bank, const std::string channelStripID)
{
    associatedChannelStrips[bank].erase(channelStripID);
}

// ARE WE USING THIS??
std::string Channel::getID()
{
    return channelID;
}

// ===========================================  VPOT EVENT HANDLERS  =======================================

// ####################################################################################################################
// The Brain board sends a message with the value "Fy" where "y" is the value to change, when the pan pot is moved CCW.
// On a CW movement, the value is in the form "0y".
// The value "y" is higher, the faster the pot is turned.
// This method adds the current pan value with the received value. Since it's a byte, it wraps around, so Fy values 
// becomes subtractive.
// ####################################################################################################################
void Channel::handleVpotPan(const std::string& vpotValue, const Bank bank, std::string channelStripID, EventSource &source)
{
    uint8_t newPanValue;

    // Use a stringstream to construct the DSP command  (xxFEFFXyyOFDFFXP, x is channelID, y is pan value)
    std::stringstream ss;
    std::string panValueString;

    // Handle the event one way if it was fired by the console.
    if (source == CONSOLE_EVENT)
    {    

        printf("pot moved: ");
        // The console sends a value showing how fast the pot was turned.
        // Convert the value to integer, then calculate new value from last, and convert back to 2-digit hex string.

        // Convert hex string to signed int8_t.
        int8_t panChangeValue = static_cast<int8_t>(std::stoi(vpotValue, nullptr, 16));
        
        // Perform the arithmetic operation using an signed 16-bit integer.
        int16_t tempCalc = static_cast<int16_t>(pan) + panChangeValue;
        
        // Clamp the new value to the range 0-254.
        newPanValue = static_cast<uint8_t>(std::max(0, std::min(254, static_cast<int>(tempCalc))));

        // Convert to 2-digit hex string.
        ss << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(newPanValue);
        
        // Write to std::string variable.
        panValueString = ss.str();

        // Clear the stringstream
        ss = std::stringstream();
    }
    else
    {
        // This is a UI event, so we're receiving the value that the pot has been moved to as a hex string.
        panValueString = vpotValue;

        // Convert to uint8_t for saving to pan member.
        newPanValue = static_cast<uint8_t>(std::stoi(vpotValue, nullptr, 16));
    }

    // Add ID and first part
    ss << channelID << "dFEFFX" << panValueString << "OFDFFXP";

    // Send the DSP command
    dspCom.send(ss.str());

    // Update internal member value
    pan = newPanValue;

    // Determine which LED's should be lit
    ChStripLed newRingLED;
    if (pan < 28) newRingLED = RING_1;
    else if (pan < 54) newRingLED = RING_2;
    else if (pan < 78) newRingLED = RING_3;
    else if (pan < 100) newRingLED = RING_4;
    else if (pan < 120) newRingLED = RING_5;
    else if (pan < 136) newRingLED = RING_6;
    else if (pan < 156) newRingLED = RING_7;
    else if (pan < 178) newRingLED = RING_8;
    else if (pan < 202) newRingLED = RING_9;
    else if (pan < 228) newRingLED = RING_10;
    else newRingLED = RING_11;

    // Check dead center for dot LED.
    bool updateDot = false;
    if (pan == 127)
    {
        panDotCenter = true;
        updateDot = true;
    }
    else if (panDotCenter)
    {
        panDotCenter = false;
        updateDot = true;
    }

    // Retreive the set of associated channelStrips on the current bank
    std::unordered_set<std::string> associatedStrips = associatedChannelStrips[bank];   

    // If ring LED needs to change, turn on the new one, an turn off the old one.
    if (currentRingLED != newRingLED || updateDot)
    {
        for (auto &stripID : associatedStrips)
        {
            // Look up the ID for the led to turn off, and the one to turn on.
            // Construct the Brain command to send.
            std::string ledOff = CH_STRIP_LED_MAP.at(stripID)[currentRingLED] + "j";
            std::string ledOn = CH_STRIP_LED_MAP.at(stripID)[newRingLED] + "i";

            brainCom.send(ledOff);
            brainCom.send(ledOn);

            // Check for need to update dot
            if (updateDot)
            {
                // Start the command string.
                std::string dotCommand = CH_STRIP_LED_MAP.at(stripID)[RING_DOT];

                // Append last part, depending on whether we need to turn on or off.
                dotCommand.append(panDotCenter ? "i" : "j");
                brainCom.send(dotCommand);
            }
        }
        // Update the registry
        currentRingLED = newRingLED;
    }

    // If this was a console event, post event for UI update
    if (source == UI_EVENT)
    {
        // Remove the calling channel strip ID from the associate set
        associatedStrips.erase(channelStripID);
    }

    // Post event for the associated strips.
    eventBus.associateChStripUiEventPost(associatedStrips, VPOT_EVENT, panValueString);
}  


void Channel:: handleVpotAuxSend(const std::string& vpotValue, const Bank bank, EventSource& source)
{
    // TODO: WORK ON THESE


}
