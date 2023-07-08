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

    // Adjust for channels above 24.
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
    mute = false;

    pan = 0x7F; // Center

    // solo = false;



    // assignments[0] = true;  // Assign to L-R. All others should default to false.

    // Initialize other member variables to default values...

    // Increment the nextChannelNumber for the next channel construction.
    nextChannelNumber++;


    // HERE AT THE BOTTOM MAYBE WE SHOULD SEND INITIAL SETTINGS TO THE DSP BOARD??
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

    // TODO: Master fader...??


    // Construct DSP volume command, and send it.
    std::string volumeCommand = channelID + "cX" + faderValue + "Q";

    if (!mute)
        // write(*dspDescriptorPtr, volumeCommand.c_str(), volumeCommand.length());
        dspCom.send(volumeCommand);

    // Update volume member
    volume = faderValue;

    // Retrieve the set of associated CONSOLE channel strips on the current bank.
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
    eventBus.associateChStripEventPost(associatedUiStrips, FADER_EVENT, faderValue);
}

void Channel::channelStripVpotEventCallback(const std::string vpotValue,
                                            const Bank bank,
                                            const std::string &channelStripID,
                                            EventSource source)
{
    // The supplied value is likely not a specific placement, but rather a number of how fast/far the pot was turned.
    // So the procedure here is:
    // 1 - DEDUCE WHAT THE POT IS CONTROLING
    // 2 - CALCULATE NEW VALUE FROM OLD VALUE ON SUPPLIED CHANGE-VALUE
    // 3 - SEND DSP (OR BRAIN?) COMMAND TO UPDATE WHAT THE VPOT IS CHOSEN TO CONTROL
    // 4 - UPDATE PRIVATE MEMBER
    // 5 - SEND AN ASSOCIATE EVENT POST FOR UPDATING THE UI
    // --------------------------------------------------------------------------------

    // So , I guess here it could make sense to use a pointer to a function, depending on what mode is selected:
    // A - Master Pan (pan button below master vpot) - ch. 1-72 + 81-88 pan control.
    // B - Aux Send Level (Aux1-8 buttons)
    // C - Aux 9-10 / 11-12 send level for the stereo pair
    // D - Aux 9-10 / 11-12 PAN pan control for the stereo pair.
    // E - Level to tape
    // F - Digital trim.

    //The pots send values in same form as the faders. BUT the value is Fy for CCW and 0y for CW rotation, where y is the value depending on how fast the pot was turned.


    // We may need to also do some map or array here, depending on bank?
    (this->*currentVpotFunction)(vpotValue);

}

void Channel::channelStripButtonEventCallback(const std::string buttonID,
                                              const Bank bank,
                                              const std::string &channelStripID,
                                              EventSource source)
{
    // SOME WORK TO BE DONE HERE...
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
void Channel::handleVpotPan(const std::string& vpotValue)
{
    // Convert hex string to signed int8_t.
    int8_t panChangeValue = static_cast<int8_t>(std::stoi(vpotValue, nullptr, 16));
    
    // Perform the arithmetic operation using an signed 16-bit integer.
     int16_t tempCalc = static_cast<int16_t>(pan) + panChangeValue;
    
    // Clamp the new value to the range 0-254.
    uint8_t newPanValue = static_cast<uint8_t>(std::max(0, std::min(254, static_cast<int>(tempCalc))));

    // Only continue if pan value is different than before
    if (newPanValue != pan)
    {        
        // Use a stringstream to construct the DSP command  (xxFEFFXyyOFDFFXP, x is channelID, y is pan value)
        std::stringstream ss;

        // Add ID and first part
        ss << channelID << "dFEFFX";

        // Add the pan value
        ss << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(newPanValue);

        // Add last part of the command.
        ss << "OFDFFXP";

        // Send the DSP command
        dspCom.send(ss.str());

        // Update internal member value
        pan = newPanValue;

        // Post a ui event, for updating ..well..you know...the...uhm.....ui....
        ASSOICIATIVE EventSource
        UI EVENT POST..
        check up against fader work, to see how it's done.
    }
}


void Channel:: handleVpotAuxSend(const std::string& vpotValue)
{
    //WORK ON THESE


}
