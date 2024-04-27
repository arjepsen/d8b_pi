/*
  ==============================================================================

    ChannelClass.cpp
    Created: 14 Apr 2023 9:48:22pm
    Author:  anders

  ==============================================================================
*/

// TODO: Change from std::string to c-style strings where applicable.
#include "ChannelClass.h"
#include "ButtonLookupTable.h"
#include "ChannelIDMap.h"
#include "LedIDMaps.h"
#include <iomanip>
#include <sstream>
#include <string.h>
#include <unistd.h>

// UNCOMMENT TO ENABLE DEBUG MESSAGES.
#define CHANNEL_DEBUG_MESSAGES

#ifdef CHANNEL_DEBUG_MESSAGES
#define DEBUG_MSG(format, ...) printf("CHNL_DBG: " format, ##__VA_ARGS__)
#else
#define DEBUG_MSG(format, ...) ((void)0) // do {} while (0)
#endif

constexpr size_t LED_CMD_LENGTH = 5; // Number of chars in the LED DSP commands, including null terminator.
constexpr int STRIP_ID_LENGTH = 3;   // Number of chars in the channel strip ID, incl. null.
constexpr int VOL_VALUE_LENGTH = 3;  // Max chars in the volume value string. (one or two.) Incl. null.

constexpr char DSP_VOL_CMD_PRT1[] = "cX";
constexpr char DSP_CMD_END[] = "Q";
constexpr int DSP_VOL_CMD_LENGTH = 8; // Max dsp volume command length. (for setting array). ie. "0AcXC8Q" (can be only 7, when just one hex digit for volume)
constexpr int BRAIN_FADER_CMD_LENGTH = 5; // Max brain fader command length. i.e. "22ABf" excluding the null terminator

// Set first channel ID. This will increment with every channel object constructed.
uint8_t Channel::nextChannelNumber = 0;

// ############################### CONSTRUCTOR #########################################
// Channel Constructor. Set up channel number and get channel dsp ID from channelIDMap.h
// Convert channel number to channelStripID for the associate map.
// #####################################################################################
Channel::Channel()
    : CH_ID_STR{DSP_CH_ID_ARRAY[nextChannelNumber]}, // Get unique channel ID for dsp commands.
      CH_NUMBER{nextChannelNumber},                  // Set unique channel number.
      eventBus(EventBus::getInstance()),             // Get the singleton instances
      brainCom(BrainCom::getInstance()),
      dspCom(DspCom::getInstance())
{
    // Set up variables for initial channel strip event subscription.
    // TODO: Upon boot, this should be loaded from saved file.
    int initialAssociateStripNumber = CH_NUMBER;
    Bank initialAssociateBank = LINE_BANK;

    // TODO: Adjust for channels above 24.
    if (CH_NUMBER > 23)
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
    eventBus.bankEventSubscribe(
        initialAssociateBank,
        initialAssociateChannelStripID,
        [this](const std::string &faderValue, const Bank bank, const std::string &channelStripID, EventSource source)
        { this->channelStripFaderEventCallback(faderValue, bank, channelStripID, source); },
        [this](const std::string &vpotValue, const Bank bank, const std::string &channelStripID, EventSource source)
        { this->channelStripVpotEventCallback(vpotValue, bank, channelStripID, source); },
        [this](const std::string &buttonValue, const Bank bank, const std::string &channelStripID, EventSource source)
        { this->channelStripButtonEventCallback(buttonValue, bank, channelStripID, source); },
        [this](const Bank bank, const std::string &channelStripID)
        { this->removeChStripAssociationCallback(bank, channelStripID); });

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
// void Channel::setVolume(std::string volumeValue)
void Channel::setVolume(char *volumeValue)
{
    // Update volume member
    // volume = volumeValue;
    strncpy(volume, volumeValue, 2); // Copy two chars.
    volume[2] = '\0';                // Ensure third index is null terminator.

    // Construct DSP command, and send it (if not muted)    (i.e. 22cX4FQ)
    // std::string volumeCommand = CH_ID_STR + "cX" + volumeValue + "Q";

    if (!mute)
    {
        // write(*dspDescriptorPtr, volumeCommand.c_str(), volumeCommand.length());
        char volumeCommand[DSP_VOL_CMD_LENGTH];
        // strcpy(volumeCommand, CH_ID_STR);
        // strcat(volumeCommand, "cX");
        // strcat(volumeCommand, volume);
        // strcat(volumeCommand, "Q");
        snprintf(volumeCommand, DSP_VOL_CMD_LENGTH, "%s%s%sQ", CH_ID_STR, "cX", volume);
        dspCom.send(volumeCommand);
    }
}

// ###########################################################################################################################
// This is a callback function that is fired by the EventBus, when a fader is moved.
// First it sends a DSP command to update the volume. Then it will iterate over all associalted channelstrips
// on the console, and update them. Finaly it will post another event, which will update the channelStripComponents in the UI
// ###########################################################################################################################
void Channel::channelStripFaderEventCallback(const char (&faderValue)[2],
                                             Bank bank,
                                             const ChStripID channelStripID,
                                             EventSource source)
{
    // TODO: THIS MAY NEED TO CHANGE A BIT, IF WE CONTINUE TO USE THE CHANNEL CLASS FOR EFFECTS, MIDI, BUS, GROUPS, ETC.

    // Update the channel volume member
    volume[0] = faderValue[0];
    volume[1] = faderValue[1];

    // Only send dsp command if channel is not muted.
    if (!mute)
    {
        // Not muted, create string and send it.
        // The form of the command is "XXcXYYQ", where XX is dsp channel id,
        // and YY is the volume value (hex string)
        char dspVolumeCommand[DSP_VOL_CMD_LENGTH];
        snprintf(dspVolumeCommand, DSP_VOL_CMD_LENGTH, "%s%s%sQ", CH_ID_STR, "cX", volume);
        dspCom.send(dspVolumeCommand);

        // Copy associated channelstrip bitmask.
        int consoleMask = associatedChannelStripBitmask;
        int uiMask = associatedChannelStripBitmask;

        // Disable "calling" strip, depending on whether ui or console.
        if (source == CONSOLE_EVENT)
        {
            // Clear the bit of the calling channelstrip
            consoleMask = associatedChannelStripBitmask & ~(1U << channelStripID);
        }
        else
        {
            // UI event, clear the ui caller bit.
            uiMask = associatedChannelStripBitmask & ~(1U << channelStripID);
        }

        // Create and send commands for the associated faders.
        // The char array is not initialized, since we write to it right after,
        // and the remaining two chars are written in the while loop, before using it.
        char brainFaderCommand[BRAIN_FADER_CMD_LENGTH];
        brainFaderCommand[2] = volume[0];
        brainFaderCommand[3] = volume[1];
        brainFaderCommand[4] = 'f';
        //brainFaderCommand[5] = '\0';  // Write command does not send null terminator anyway.

        // Now the char array contains "__XXf\0", where XX is the volume value.
        // Iterate over the bitmask of associated strips, 
        // write the strip id in the first two indices, send commands.
        while(consoleMask)
        {
            // Get the index of the lowest set bit
            int stripID = __builtin_ctz(consoleMask);
            
            // Use array lookup to convert the id to two-char hex.
            brainFaderCommand[0] = CH_STRIP_ID_ARRAY[stripID][0];
            brainFaderCommand[1] = CH_STRIP_ID_ARRAY[stripID][1];

            // Send. We know the length, so use that method.
            brainCom.send(brainFaderCommand, BRAIN_FADER_CMD_LENGTH);

            // Clear lowest set bit
            consoleMask &= consoleMask - 1;
        }
        
        // Now post a ui event with the mask for the ui.


        HANDLE THE UI STUFF

        THEN HANDLE THE REMAINING ONE

        THIS MEANS EDIT THE CALLBACKS IN THE UI.






//========================================================


        // Make a copy for the associated UI channel strips on the current bank.
        std::unordered_set<std::string> associatedUiStrips = associatedConsoleStrips;

        // Depending on whether the event was triggered by CONSOLE or UI, remove the ID that was activated
        if (source == CONSOLE_EVENT)
        {
            //associatedConsoleStrips.erase(channelStripID);
            associatedChannelStrips[bank][channelStripID] = false;

        }
        else
        {
            //associatedUiStrips.erase(channelStripID);

            // 1 - Temporarily "un-associate"
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

        // The pots send values in same form as the faders. BUT the value is Fy for CCW and 0y for CW rotation, where y is the value depending on how fast the pot was turned.
        //  ---------------------------------------------------------------------------------------------

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

    void Channel::channelStripButtonEventCallback(const int chStripNumber,
                                                  const Bank bank,
                                                  const ButtonType btnType,
                                                  const ButtonAction btnAction)
    {
        // TODO: SOME WORK TO BE DONE HERE...
        printf("BUTTON CLICKED");

        // We receive the strip number, to handle associated strips.
        // The button type (mute, solo, etc.)
        // the action (pressed/released)
        // And the source of the event - console or ui.

        // Maybe create a map of dsp commands before-hand?
        // So how does bank selection affect the channelstrip buttons?

        // Handle the button event, depending on which button:
        switch (btnType)
        {
            case MUTE_BTN:
                // Do nothing on button release
                if (btnAction == 'u') return; // TODO: can this be handled better? without conditional?

                // Ok, we first send the mute/unmute command to the DSP board for this channel.
                // Then we must send the command to turn on/off the LED on the mute button
                // on all associated channelstrips.
                // finally we also need to reflect this in the UI.

                // Variable for the LED brain command
                LedStateCommand stateCmd = LED_OFF_CMD;

                // 1: Construct and send the DSP command for muting or unmuting the channel. i.e. "22cXC5Q"
                // std::string dspCmd = CH_ID_STR + "cX";
                char dspCmd[DSP_VOL_CMD_LENGTH]; // Set up the string array

                if (mute) // Check whether we're muting or unmuting
                {
                    // Muted. Get the saved volume value, and send the dsp command. (LED command set to off above)
                    mute = false;
                    snprintf(dspCmd, DSP_VOL_CMD_LENGTH, "%s%s%sQ", CH_ID_STR, "cX", volume);
                }
                else
                {
                    // Muting, send a value of 0 to the DSP, and set the command for turning ON the mute LED.
                    mute = true;
                    snprintf(dspCmd, DSP_VOL_CMD_LENGTH, "%s%s0Q", CH_ID_STR, "cX");
                    stateCmd = LED_ON_CMD; // Prepare for the LED brain command.
                }

                // Send the DSP volume command.
                dspCom.send(dspCmd); // TODO: IS THIS SOMETHING THAT ALL CASES DO??

                // Now, send the on/off command to all associated strip button led's
                // TODO: Maybe also something all cases do??
                // Use the LED lookup map, and index the array using the enumeration
                // TODO: the loop would insert the bank key if it does not already exists - should we put in some safety check? but that would mess with performance?
                for (const std::string &stripID : associatedChannelStrips[bank])
                {
                    std::string ledCommand = CH_STRIP_LED_MAP[stripID][MUTE_LED];
                    ledCommand += stateCmd;
                }

                CHANGING TO USE CHAR ARRAYS INSTEAD.
                MOVE ID ENUMERATION FROM LEDIDMAPS, SINCE IT IS USED MORE PLACES

                // MAYBE INSTEAD WE COULD MAKE A MAP OF COMMANDS, SO THE CALL SIMPLY FETCHES THE CORRECT COMMAND TO SEND????

                // 1: Send a dsp command for setting volume to 0 on this channel.
                //    xxcXC0Q where xx is the channelID string (CH_ID_STR).
                //    BUT, dont update the channels' volume record. Keep this value for unmuting.
                //    REMEMBER - master channel volume command is different (4Cc9X0QAX0Q)
                //    // but... no mute on master....?
                //   TODO: Cheac what other "channels" do (master, effect, midi etc.)
                // 2: Do update the mute variable in the channel object.
                // 3: retrieve associated channels (only current bank - when do we update the others?)
                std::unordered_set<std::string>
                    associatedConsoleStrips = associatedChannelStrips[bank];

                // 4: Turn on/off the led on all the associated channels, including the "activated" on

                // Iterate through the set of associated channelStrips,
                // and activate/deactivate their mute button LED
                for (auto &stripID : associatedChannelStrips[bank])
                {
                    std::string faderCommand = stripID + faderValue + "f";
                    brainCom.send(faderCommand);
                }
        }
    }

    // ####################################################################################################
    // This method is a callback for removing the supplied channel strip from the set of associated strips.
    // This is called from the event bus, when an association is updated.
    // ####################################################################################################
    void Channel::removeChStripAssociationCallback(const Bank bank, const std::string channelStripID)
    {
        associatedChannelStrips[bank].erase(channelStripID);
    }

    // TODO: ARE WE USING THIS??
    std::string Channel::getID()
    {
        return CH_ID_STR;
    }

    // ===========================================  VPOT EVENT HANDLERS  =======================================

    // ####################################################################################################################
    // The Brain board sends a message with the value "Fy" where "y" is the value to change, when the pan pot is moved CCW.
    // On a CW movement, the value is in the form "0y".
    // The value "y" is higher, the faster the pot is turned.
    // This method adds the current pan value with the received value. Since it's a byte, it wraps around, so Fy values
    // becomes subtractive.
    // ####################################################################################################################
    void Channel::handleVpotPan(const std::string &vpotValue, const Bank bank, std::string channelStripID, EventSource &source)
    {
        uint8_t newPanValue;

        // Use a stringstream to construct the DSP command  (xxFEFFXyyOFDFFXP, x is channelID, y is pan value)
        std::stringstream ss;
        std::string panValueString;

        // Handle the event one way if it was fired by the console.
        if (source == CONSOLE_EVENT)
        {
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
        ss << CH_ID_STR << "dFEFFX" << panValueString << "OFDFFXP";

        // Send the DSP command
        dspCom.send(ss.str());

        // Update internal member value
        pan = newPanValue;

        // Determine which LED's should be lit
        ChStripLed newRingLED;
        if (pan < 28)
            newRingLED = RING_1;
        else if (pan < 54)
            newRingLED = RING_2;
        else if (pan < 78)
            newRingLED = RING_3;
        else if (pan < 100)
            newRingLED = RING_4;
        else if (pan < 120)
            newRingLED = RING_5;
        else if (pan < 136)
            newRingLED = RING_6;
        else if (pan < 156)
            newRingLED = RING_7;
        else if (pan < 178)
            newRingLED = RING_8;
        else if (pan < 202)
            newRingLED = RING_9;
        else if (pan < 228)
            newRingLED = RING_10;
        else
            newRingLED = RING_11;

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

    void Channel::handleVpotAuxSend(const std::string &vpotValue, const Bank bank, EventSource &source)
    {
        // TODO: WORK ON THESE
    }

    // ===========================================  BUTTON EVENT HANDLERS  =======================================

    void channelStripButtonEventCallback(const std::string buttonID, const Bank bank, const std::string &channelStripID, EventSource source)
    {
        // 1 - make a map of
    }
