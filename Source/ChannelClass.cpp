/*
  ==============================================================================

    ChannelClass.cpp
    Created: 14 Apr 2023 9:48:22pm
    Author:  anders

  ==============================================================================
*/

// TODO: Change from std::string to c-style strings where applicable.
#include "ChannelClass.h"
//#include "ButtonLookupTable.h"
#include "ChannelIDMap.h"
#include "LEDClass.h"
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

// constexpr char DSP_VOL_CMD_PRT1[] = "cX";
// constexpr char DSP_CMD_END[] = "Q";
constexpr int DSP_VOL_CMD_LENGTH = 8;     // Max dsp volume command length. (for setting array). ie. "0AcXC8Q" (can be only 7, when just one hex digit for volume)
constexpr int BRAIN_FADER_CMD_LENGTH = 5; // Max brain fader command length. i.e. "22ABf" excluding the null terminator
constexpr int DSP_PAN_CMD_LENGTH = 16;
constexpr int BRAIN_LED_CMD_LENGTH = 4;

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
      dspCom(DspCom::getInstance()),
      intToHexLookup(IntToHexLookup::getInstance()),
      ledRingLookup(LEDringLookup::getInstance())
{
    // Set up variables for initial channel strip event subscription.
    // TODO: Upon boot, this should be loaded from saved file.
    unsigned int initialAssociateStripNumber = CH_NUMBER;
    Bank initialAssociateBank = LINE_BANK;

    // TODO: Adjust for channels above 48.... maybe we make different classes for that....
    if (CH_NUMBER > 23)
    {
        initialAssociateStripNumber -= 23;
        initialAssociateBank = TAPE_BANK;
    }

    // Set up using enumeration (remember they start at 0...)
    ChStripID initialAssociateStrip = static_cast<ChStripID>(initialAssociateStripNumber - 1);

    // // Convert Channel Number to a corresponding channel strip ID.
    // std::stringstream stream;
    // stream << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << initialAssociateStripNumber;
    // std::string initialAssociateChannelStripID = stream.str();

    // // Put the initial channel strip ID in the associate map, for the initial bank:
    // std::unordered_set<std::string> initialStripSet{initialAssociateChannelStripID};
    // associatedChannelStrips[initialAssociateBank] = initialStripSet;

    // TODO: This should also eventually be loaded from file on boot.
    
    // Clear all association
    associatedChannelStripBitmask[LINE_BANK] = 0;
    associatedChannelStripBitmask[TAPE_BANK] = 0;
    associatedChannelStripBitmask[EFFECTS_BANK] = 0;
    associatedChannelStripBitmask[MASTERS_BANK] = 0;

    // Set the bit for the initially associated channelstrip.
    associatedChannelStripBitmask[initialAssociateBank] |= (1U << initialAssociateStripNumber);

    // // Subscribe to events on the given bank and strip.
    // eventBus.bankEventSubscribe(
    //     initialAssociateBank,
    //     initialAssociateChannelStripID,
    //     [this](const std::string &faderValue, const Bank bank, const std::string &channelStripID, EventSource source)
    //     { this->channelStripFaderEventCallback(faderValue, bank, channelStripID, source); },
    //     [this](const std::string &vpotValue, const Bank bank, const std::string &channelStripID, EventSource source)
    //     { this->channelStripVpotEventCallback(vpotValue, bank, channelStripID, source); },
    //     [this](const std::string &buttonValue, const Bank bank, const std::string &channelStripID, EventSource source)
    //     { this->channelStripButtonEventCallback(buttonValue, bank, channelStripID, source); },
    //     [this](const Bank bank, const std::string &channelStripID)
    //     { this->removeChStripAssociationCallback(bank, channelStripID); });

    // Subscribe to events on the initial channelstrip.
    subscribeToChStrip(initialAssociateBank, initialAssociateStrip);

    // Set initial Vpot handler method.
    currentVpotFunction = &Channel::handleVpotPan;

    // Get the base number for the buttons on the initial associate strip.
    unsigned int buttonBase = eventBus.channelStripButtonBase[initialAssociateStrip];

    // TODO How to handle other associates....?

///////////////////////////////////////////////////////////////

    // INITIAL SETTINGS - WE CAN SET THEM HERE ARBITRARILY, OR USE A SPECIFIC CALL??
    // TODO: We need to be able to save and recall settings across reboots.
    // On construction, we should read saved values and set them.
    // Default saved values must be present, and recreated if not present.
    muted = false;

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

// // #######################################################################################
// // This method updates the channel object's volume member, and sends a command to the DSP
// // #######################################################################################
// // void Channel::setVolume(std::string volumeValue)
// void Channel::setVolume(char *volumeValue)
// {
//     // Update volume member
//     // volume = volumeValue;
//     strncpy(volume, volumeValue, 2); // Copy two chars.
//     volume[2] = '\0';                // Ensure third index is null terminator.

//     // Construct DSP command, and send it (if not muted)    (i.e. 22cX4FQ)
//     // std::string volumeCommand = CH_ID_STR + "cX" + volumeValue + "Q";

//     if (!mute)
//     {
//         // write(*dspDescriptorPtr, volumeCommand.c_str(), volumeCommand.length());
//         char volumeCommand[DSP_VOL_CMD_LENGTH];
//         // strcpy(volumeCommand, CH_ID_STR);
//         // strcat(volumeCommand, "cX");
//         // strcat(volumeCommand, volume);
//         // strcat(volumeCommand, "Q");
//         snprintf(volumeCommand, DSP_VOL_CMD_LENGTH, "%s%s%sQ", CH_ID_STR, "cX", volume);
//         dspCom.send(volumeCommand);
//     }
// }



/*******************************************************************************
 * @brief This is the fader callback method that is fired off when a fader on an
 *        associated channelstrip is moved.
 *        First we send a DSP command to update the volume, then a Brain command
 *        to move all associated faders, and last we post a UI event for moving
 *        the associated faders in the UI.
 * 
 * @param faderValue 
 * @param bank 
 * @param channelStripID 
 * @param source 
 *******************************************************************************/
void Channel::channelStripFaderEventCallback(const char (&faderValue)[2],
                                             Bank bank,
                                             ChStripID channelStripID,
                                             EventSource source)
{
    // TODO: THIS MAY NEED TO CHANGE A BIT, IF WE CONTINUE TO USE THE CHANNEL CLASS FOR EFFECTS, MIDI, BUS, GROUPS, ETC.

    // Update the channel volume member
    volume[0] = faderValue[0];
    volume[1] = faderValue[1];

    // Only send dsp command if channel is not muted.
    if (!muted)
    {
        // Not muted, create string and send it.
        // The form of the command is "XXcXYYQ", where XX is dsp channel id,
        // and YY is the volume value (hex string)
        char dspVolumeCommand[DSP_VOL_CMD_LENGTH] = "--cX--Q";
        //snprintf(dspVolumeCommand, DSP_VOL_CMD_LENGTH, "%s%s%sQ", CH_ID_STR, "cX", volume);
        dspVolumeCommand[0] = CH_ID_STR[0];
        dspVolumeCommand[1] = CH_ID_STR[1];
        dspVolumeCommand[4] = volume[0];
        dspVolumeCommand[5] = volume[1];

        dspCom.send(dspVolumeCommand);

        // Copy associated channelstrip bitmask.
        int consoleMask = associatedChannelStripBitmask[bank];
        int uiMask = associatedChannelStripBitmask[bank];

        // Disable "calling" strip, depending on whether ui or console.
        if (source == CONSOLE_EVENT)
        {
            // Clear the bit of the calling channelstrip
            consoleMask = consoleMask & ~(1U << channelStripID);
        }
        else
        {
            // UI event, clear the ui caller bit.
            uiMask = uiMask & ~(1U << channelStripID);
        }

        // Create and send commands for the associated faders.
        // The char array is not initialized, since we write to it right after,
        // and the remaining two chars are written in the while loop, before using it.
        char brainFaderCommand[BRAIN_FADER_CMD_LENGTH];
        brainFaderCommand[2] = volume[0];
        brainFaderCommand[3] = volume[1];
        brainFaderCommand[4] = 'f';
        // brainFaderCommand[5] = '\0';  // Write command does not send null terminator anyway.

        // Now the char array contains "__XXf\0", where XX is the volume value.
        // Iterate over the bitmask of associated strips,
        // write the strip id in the first two indices, send commands.
        while (consoleMask)
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
        // The eventBus method will iterate over the mask, and call the callbacks.
        const char volumeValue[2] = {volume[0], volume[1]};
        eventBus.associateUiStripFaderEventPost(uiMask, volumeValue);
    }
}

/*******************************************************************************
 * @brief This is the callback for vpot events.
 *        The work is handed off to the currently selected vpot function method.
 * 
 * @param vPotValue 
 * @param bank 
 * @param channelStripID 
 * @param source 
 ******************************************************************************/
void Channel::channelStripVpotEventCallback(const char (&vPotValue)[2],
                                             Bank bank,
                                             const ChStripID channelStripID,
                                             EventSource source)
{
    (this->*currentVpotFunction)(vPotValue, bank, channelStripID, source);
}

/*********************************************************************************
 * @brief This method is used when we make a new association, i.e. when we want
 *        to set up some specific channelstrip to control this channel.
 *        (any channelstrip on any bank can be configured to control any channel).
 *        When a new association is made, we first need to let the old associated
 *        channel know that it is no longer associated with that strip, so it can
 *        update it's register of associated channels - done by this callback.
 * 
 * @param bank
 * @param chStripID 
 *********************************************************************************/
void Channel::removeChStripAssociationCallback(Bank bank, ChStripID chStripID)
{
    associatedChannelStripBitmask[bank] &= ~(1U << chStripID);
}



// ===========================================  VPOT EVENT HANDLERS  =======================================

/*********************************************************************************
 * @brief Vpot pan event handler. 
 *        When a channelstrip vpot is turned it sends a message in the same format
 *        as the faders: XXYYv. XX is the channelstrip ID, and YY is the value.
 *        The value will be Fy for CCW movement, and 0y for CW movement, where
 *        y is a value depending on how fast the pot is turned.
 *        When converting this to an int8_t, it becomes negative for Fy values,
 *        which means we can simply add this value to the previously stored pan.
 * 
 * @param panValue 
 * @param bank 
 * @param channelStripID 
 * @param source 
 **********************************************************************************/
inline void Channel::handleVpotPan(const char (&panValue)[2], Bank bank, ChStripID channelStripID, EventSource source)
{
    // If old pan value was 127, then we are moving away from dead center
    // And need to turn the dot off.
    bool handleDot = (pan == 127);

    // Create initial DSP command string.
    char dspPanCommand[] = "--dFEFFX--OFDFFXP";

    // Write in the dsp channel ID code.
    dspPanCommand[0] = CH_ID_STR[0];
    dspPanCommand[1] = CH_ID_STR[1];

    // Handle the event one way if it was fired by the console.
    if (source == CONSOLE_EVENT)
    {
        // Convert hex string to signed int8_t. (CCW becomes negative)
        int panChangeValue = static_cast<int8_t>(hexToInt(panValue));

        // Adding to the current value will result in the new value to write.
        // Clamp the value, so it stays within 0 to 254. Write to class member.
        pan = std::max(0, std::min(254, static_cast<int>(pan + panChangeValue)));
    }
    else
    {
        // This is a UI event, so we're receiving the value that the pot has 
        // been moved TO as a hex string.
        // Update the pan member of this channel
        pan = hexToInt(panValue);
    }

    // Convert pan to a 2-digit hex string
    const char *panHexValue = intToHexLookup.getHexValue(pan);

    // Write the hex value to the dsp command string
    dspPanCommand[8] = panHexValue[0];
    dspPanCommand[9] = panHexValue[1];

    // Send the DSP command
    dspCom.send(dspPanCommand);

    // Next, update the LED's around the vpot.
    // Do a lookup of the vPot LED corresponding to the new pan value.
    ChStripLED newRingLED = ledRingLookup.getRingID(pan);

    // Handle dot also, if we move TO 127 (then turn on instead of off.)
    handleDot = (pan == 127) ? true : handleDot;
    bool updateRing = (newRingLED != currentRingLED);

    // Check it up against last saved - update if necessary.
    if (updateRing || handleDot)
    {
        // New pan value requires an LED change.
        // Iterate through all associated channels (including this one)
        // and turn off the current LED and turn on the new one.

        // First, set up a char array for the command
        char brainLedCommand[4];

        // Copy the bitmask, and iterate over the bits
        int channelMask = associatedChannelStripBitmask[bank];
        while (channelMask)
        {
            // Get the index of the lowest set bit
            ChStripID stripID = static_cast<ChStripID>(__builtin_ctz(channelMask));

            if (updateRing)
            {
                // Copy and send command from set, to turn off previous LED
                brainLedCommand[0] = CH_STRIP_LED_MAP[stripID][currentRingLED][0];
                brainLedCommand[1] = CH_STRIP_LED_MAP[stripID][currentRingLED][1];
                brainLedCommand[2] = CH_STRIP_LED_MAP[stripID][currentRingLED][2];
                brainLedCommand[3] = LED_OFF_CMD;
                brainCom.send(brainLedCommand, BRAIN_LED_CMD_LENGTH);

                // Likewise, turn on new LED
                brainLedCommand[0] = CH_STRIP_LED_MAP[stripID][newRingLED][0];
                brainLedCommand[1] = CH_STRIP_LED_MAP[stripID][newRingLED][1];
                brainLedCommand[2] = CH_STRIP_LED_MAP[stripID][newRingLED][2];
                brainLedCommand[3] = LED_ON_CMD;
                brainCom.send(brainLedCommand, BRAIN_LED_CMD_LENGTH);
            }

            // Update center dot if necessary.
            if (handleDot)
            {
                brainLedCommand[0] = CH_STRIP_LED_MAP[stripID][RING_DOT][0];
                brainLedCommand[1] = CH_STRIP_LED_MAP[stripID][RING_DOT][1];
                brainLedCommand[2] = CH_STRIP_LED_MAP[stripID][RING_DOT][2];
                brainLedCommand[3] = (pan == 127) ? LED_ON_CMD : LED_OFF_CMD;
                brainCom.send(brainLedCommand, BRAIN_LED_CMD_LENGTH);
            }

            // Clear lowest set bit
            channelMask &= channelMask - 1;
        }
    }

    // Finally, let's update the ui. 
    // If this was a UI event, first remove the calling stripID.
    // Instead of using conditional check, use the fact that "UI_EVENT" = 1
    // and CONSOLE_EVENT = 0;
    
    // First, get the bitmask
    int uiMask = associatedChannelStripBitmask[bank];

    // Update the bit for the calling channel according to what source was.
    uiMask |= (source << channelStripID);

    // Post a ui event
    eventBus.associateUiStripVpotEventPost(uiMask, pan);
}

inline void Channel::handleVpotAuxSend(const char (&panValue)[2], const Bank bank, ChStripID channelStripID, EventSource source)
{
    // TODO
}

inline void Channel::handleVpotAuxStereoSend(const char (&panValue)[2], const Bank bank, ChStripID channelStripID, EventSource source)
{
    // TODO
}

inline void Channel::handleVpotLevelToTape(const char (&panValue)[2], const Bank bank, ChStripID channelStripID, EventSource source)
{
    // TODO
}

inline void Channel::handleVpotDigitalTrim(const char (&panValue)[2], const Bank bank, ChStripID channelStripID, EventSource source)
{
    // TODO
}

inline void Channel::handleVpotAuxStereoPan(const char (&panValue)[2], const Bank bank, ChStripID channelStripID, EventSource source)
{
    // TODO
}


// ===========================================  BUTTON EVENT CALLBACKS  =======================================

/**************************************************************************
 * @brief This is the callback for the mute button.
 *        First we send a DSP volume command.
 *        Next, we handle the LED's on all associated channelstrip buttons.
 * 
 * @param btnAction 
 * @param currentBank 
 **************************************************************************/
inline void Channel::muteBtnCallback(ButtonAction btnAction, Bank currentBank)
{
    // Only act on presses, not on releases.
    if (btnAction == BTN_PRESS)
    {
        // Set up a char for the 'i' or 'j', to turn on or off the mute LED
        LedStateCommand stateCmd;

        // Create the command string for volume. 
        char dspVolumeCommand[DSP_VOL_CMD_LENGTH] = "--cX--Q";
        
        // Copy in the Channel ID
        dspVolumeCommand[0] = CH_ID_STR[0];
        dspVolumeCommand[1] = CH_ID_STR[1];

        if (muted)
        {
            // Copy in the volume value.
            dspVolumeCommand[4] = volume[0];
            dspVolumeCommand[5] = volume[1];

            // Update the member flag
            muted = false;

            // Set last part of LED command.
            stateCmd = LED_OFF_CMD;
        }
        else
        {
            // Muting, so set volume command to "00"
            dspVolumeCommand[4] = '0';
            dspVolumeCommand[5] = '0';

            // Update the member flag
            muted = true;

            // Set last part of LED command.
            stateCmd = LED_ON_CMD;
        }

        // Send DSP command.
        dspCom.send(dspVolumeCommand);

        // Construct the LED command. Iterate through associated channels
        // and send it.
        char brainLedCommand[4];
        int channelMask = associatedChannelStripBitmask[currentBank];
        while (channelMask)
        {
            // Get the index of the lowest set bit
            ChStripID stripID = static_cast<ChStripID>(__builtin_ctz(channelMask));

            brainLedCommand[0] = CH_STRIP_LED_MAP[stripID][MUTE_LED][0];
            brainLedCommand[1] = CH_STRIP_LED_MAP[stripID][MUTE_LED][1];
            brainLedCommand[2] = CH_STRIP_LED_MAP[stripID][MUTE_LED][2];
            brainLedCommand[3] = stateCmd;

            // Clear lowest set bit
            channelMask &= channelMask - 1;
        }
    }
}

/**********************************************************************
 * @brief This method is used for subscribing to a channelstrip.
 *        We collect all the callbacks in a struct, and then call the 
 *        subscribe method from the eventbus.
 * 
 * @param bank 
 * @param chStripID 
 **********************************************************************/
void Channel::subscribeToChStrip(Bank bank, ChStripID chStripID)
{
    // Make a lambda expression for subscribing the callback
    FaderCallback faderCallback = [this](const char(&faderValue)[2],
                                                Bank bank,
                                                ChStripID channelStripID,
                                                EventSource source)
    {
        this->channelStripFaderEventCallback(faderValue, bank, channelStripID, source);
    };

    // Same for the vpot callback
    VpotCallback vPotCallback = [this](const char(&vPotValue)[2],
                                                Bank bank,
                                                ChStripID channelStripID,
                                                EventSource source)
    {
        this->channelStripVpotEventCallback(vPotValue, bank, channelStripID, source);
    };

    // And for the buttons
    ButtonCallback muteBtnCallback = [this](ButtonAction btnAction, Bank currentBank)
    {
        this->muteBtnCallback(btnAction, currentBank);
    };
    ButtonCallback soloBtnCallback = [this](ButtonAction btnAction, Bank currentBank)
    {
        this->soloBtnCallback(btnAction, currentBank);
    };
    ButtonCallback selectBtnCallback = [this](ButtonAction btnAction, Bank currentBank)
    {
        this->selectBtnCallback(btnAction, currentBank);
    };
    ButtonCallback writeBtnCallback = [this](ButtonAction btnAction, Bank currentBank)
    {
        this->writeBtnCallback(btnAction, currentBank);
    };
    ButtonCallback assignBtnCallback = [this](ButtonAction btnAction, Bank currentBank)
    {
        this->assignBtnCallback(btnAction, currentBank);
    };
    ButtonCallback recRdyBtnCallback = [this](ButtonAction btnAction, Bank currentBank)
    {
        this->recRdyBtnCallback(btnAction, currentBank);
    };

    // And finally the unsubscribe callback
    UnSubscribeCallback unsubscribeCallback = [this](Bank bank, ChStripID chStripID)
    {
        this->removeChStripAssociationCallback(bank, chStripID);
    };

    // Collect the callbacks in the struct, to hand it to the eventbus
    EventBus::ChannelStripCallbacks callbacks;

    callbacks.faderCallback = faderCallback;
    callbacks.vPotCallback = vPotCallback;
    callbacks.muteBtnCallback = muteBtnCallback;
    callbacks.soloBtnCallback = soloBtnCallback;
    callbacks.selectBtnCallback = selectBtnCallback;
    callbacks.writeBtnCallback = writeBtnCallback;
    callbacks.assignBtnCallback = assignBtnCallback;
    callbacks.recRdyBtnCallback = recRdyBtnCallback;
    callbacks.unSubScribeCallback = unsubscribeCallback;

    // Register with the eventbus.
    eventBus.channelStripEventSubscribe(bank,chStripID, callbacks);

}