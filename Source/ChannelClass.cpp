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
//#include "LEDClass.h"
#include <iomanip>
#include <sstream>
#include <string.h>
#include <unistd.h>
#include "Debug.h"



constexpr size_t LED_CMD_LENGTH = 5; // Number of chars in the LED DSP commands, including null terminator.
constexpr int STRIP_ID_LENGTH = 3;   // Number of chars in the channel strip ID, incl. null.
constexpr int VOL_VALUE_LENGTH = 3;  // Max chars in the volume value string. (one or two.) Incl. null.

// constexpr char DSP_VOL_CMD_PRT1[] = "cX";
// constexpr char DSP_CMD_END[] = "Q";
constexpr int DSP_VOL_CMD_LENGTH = 8;     // Max dsp volume command length. (for setting array). ie. "0AcXC8Q" (can be only 7, when just one hex digit for volume)
constexpr int DSP_PAN_CMD_LENGTH = 16;
//constexpr int BRAIN_LED_CMD_LENGTH = 4;

constexpr int DSP_ID_LENGTH = 3;

// Array of DSP command channel ID's. Note that these are dependant on what type of command is sent.
// Note also that the bank is just an initial settings, as any channel can be moved to any channelstrip on any bank.
// These primarily correspond to fader moves.
const char DSP_CH_ID_ARRAY[][DSP_ID_LENGTH] = 
{
    // LINE BANK
	"06", "1E", "07", "1F", "08", "20", "09", "21", "0A", "22", "0B", "23",	// ch. 1-12
	"00", "18", "01", "19", "02", "1A", "03", "1B", "04", "1C", "05", "1D",	// ch. 13-24

    // TAPE BANK
	"14", "2C", "15", "2D", "16", "2E", "17", "2F", "10", "28", "11", "29", // ch. 25-36
	"12", "2A", "13", "2B", "0C", "24", "0D", "25", "0E", "26", "0F", "27",	// ch. 37-48

    // EFFECTS BANK
    "30", "3E", "31", "3F", "32", "40", "33", "41", "34", "42", "35", "43", "36", "44", "37", "45", // FX 1 - 16
    "38", "46", "39", "47", "3A", "48", "3B", "49", // RET 1 - 8

    // MASTERS BANK 
    // TODO: unsure about these, but at least we need placeholders for now.
    "X1", "X2", "X3", "X4", "X5", "X6", "X7", "X8", // Group 1 - 8
    "__", "__", "__", "__", "__", "__", "__", "__", // MIDI... these are currently just placeholders.
    "8X", "7X", "6X", "5X", "4X", "3X", "2X", "1X"

};



// Set first channel ID. This will increment with every channel object constructed.
uint8_t Channel::nextChannelNumber = 0;

// ############################### CONSTRUCTOR #########################################
// Channel Constructor. Set up channel number and get channel dsp ID from channelIDMap.h
// Convert channel number to channelStripID for the associate map.
// #####################################################################################
Channel::Channel()
    : DSP_CH_ID_STR{DSP_CH_ID_ARRAY[nextChannelNumber]}, // Get unique channel ID for dsp commands.
      CH_NUMBER{nextChannelNumber},                  // Set unique channel number.
      //eventBus(EventBus::getInstance()),             // Get the singleton instances
      //brainCom(BrainCom::getInstance()),
      dspCom(DspCom::getInstance()),
      intToHexLookup(IntToHexLookup::getInstance()),
      hexToIntLookup(HexToIntLookup::getInstance())
      //ledRingLookup(LEDringLookup::getInstance())
{
    // TODO: Get saved settings....

    // Set the two bits of the LED bitmask from the channel number.
    ChannelLeds chLeds = CH_LEDS_NONE;  
    if (CH_NUMBER < 24)
        chLeds = CH_LEDS_GREEN;
    else if (CH_NUMBER < 48)
        chLeds = CH_LEDS_RED;
    else if (CH_NUMBER < 72)
        chLeds = CH_LEDS_BOTH;

    ledOnBitmap = static_cast<uint32_t>(chLeds) << 18;  // Shift enum to set bits.


    // // TODO: OTHER LEDS??


    // TODO How to handle other associates....?

    // ============================ SET INITIAL SETTINGS ============================
    // TODO: We need to be able to save and recall settings across reboots.
    // On construction, we should read saved values and set them.
    // Default saved values must be present, and recreated if not present.
    //mute = false;


    //pan = 0x7F; // Center
    // Make a call to set volume (and fader) to 0, and pan center
    // Cant use the fader callback here, since it will exclude the "calling" control (ui/console)
    // Make a method that does it manually for all controls

    // solo = false;

    // assignments[0] = true;  // Assign to L-R. All others should default to false.

    // Initialize other member variables to default values...

    // Increment the nextChannelNumber for the next channel construction.
    nextChannelNumber++;

    // TODO: HERE AT THE BOTTOM MAYBE WE SHOULD SEND INITIAL SETTINGS TO THE DSP BOARD??
}


void Channel::updateVolume(const char (&faderValue)[2])
{
    // Update the channel volume member
    volume[0] = faderValue[0];
    volume[1] = faderValue[1];

    // Only send dsp command if channel is not muted.
    if (!mute)
    {
        // Create command string.
        char dspVolumeCommand[DSP_VOL_CMD_LENGTH] = "--cX--Q";
        dspVolumeCommand[0] = DSP_CH_ID_STR[0];
        dspVolumeCommand[1] = DSP_CH_ID_STR[1];
        dspVolumeCommand[4] = volume[0];
        dspVolumeCommand[5] = volume[1];

        // Send command.
        dspCom.send(dspVolumeCommand);
    }
}




// /*********************************************************************************
//  * @brief This method is used when we make a new association, i.e. when we want
//  *        to set up some specific channelstrip to control this channel.
//  *        (any channelstrip on any bank can be configured to control any channel).
//  *        When a new association is made, we first need to let the old associated
//  *        channel know that it is no longer associated with that strip, so it can
//  *        update it's register of associated channels - done by this callback.
//  * 
//  * @param bank
//  * @param chStripID 
//  *********************************************************************************/
// void Channel::removeChStripAssociationCallback(Bank bank, ChStripID chStripID)
// {
//     associatedChannelStripBitmask[bank] &= ~(1U << chStripID);
// }



// // ===========================================  VPOT EVENT HANDLERS  =======================================

// /*********************************************************************************
//  * @brief Vpot pan event handler. 
//  *        When a channelstrip vpot is turned, the Brain sends a message in the same 
//  *        format as the faders: XXYYv, where XX is the channelstrip ID, and YY 
//  *        is the value. The value will be Fy for CCW movement, and 0y for CW 
//  *        movement, where y is a value depending on how fast the pot is turned.
//  *        When converting this to an int8_t, it becomes negative for Fy values,
//  *        which means we can simply add this value to the previously stored pan.
//  * 
//  * @param panValue 
//  * @param bank 
//  * @param channelStripID 
//  * @param source 
//  **********************************************************************************/
// void Channel::handleVpotPan(const char (&panValue)[2], Bank bank, ChStripID channelStripID, EventSource source)
// {
//     // If old pan value was 127, then we are moving away from dead center
//     // And need to turn the dot off.
//     bool handleDot = (pan == 127);

//     // Create initial DSP command string.
//     char dspPanCommand[] = "--dFEFFX--OFDFFXP";

//     // Write in the dsp channel ID code.
//     dspPanCommand[0] = DSP_CH_ID_STR[0];
//     dspPanCommand[1] = DSP_CH_ID_STR[1];

//     // The pan value needs to be handled depending on whether it was console 
//     // or UI
//     if (source == CONSOLE_EVENT)
//     {
//         // Convert hex string to signed int8_t. (CCW becomes negative)
//         int panChangeValue = static_cast<int8_t>(hexToIntLookup.hexToInt(panValue));

//         // Adding to the current value will result in the new value to write.
//         // Clamp the value, so it stays within 0 to 254 when turning at max/min 
//         // values. Write to class member.
//         pan = std::max(0, std::min(254, static_cast<int>(pan + panChangeValue)));
//     }
//     else
//     {
//         // This is a UI event, so we're receiving the value that the pot has 
//         // been moved _TO_ as a hex string.
//         // Update the pan member of this channel
//         pan = hexToIntLookup.hexToInt(panValue);
//     }

//     // Convert pan to a 2-digit hex string
//     const char *panHexValue = intToHexLookup.getHexValue(pan);

//     // Write the hex value to the dsp command string
//     dspPanCommand[8] = panHexValue[0];
//     dspPanCommand[9] = panHexValue[1];

//     // Send the DSP command
//     dspCom.send(dspPanCommand);

//     // Handle dot also, if we move TO 127 (then turn on instead of off.)
//     // Check up against the new updated pan value.
//     handleDot = (pan == 127) ? true : handleDot;




//     // // Next, update the LED's around the vpot.
//     // // Do a lookup of the vPot LED corresponding to the new pan value.
//     // ChStripLED newRingLED = ledRingLookup.getRingID(pan);


//     // bool updateRing = (newRingLED != currentRingLED);

//     // // Check it up against last saved - update if necessary.
//     // if (updateRing || handleDot)
//     // {
//     //     // New pan value requires an LED change.
//     //     // Iterate through all associated channels (including this one)
//     //     // and turn off the current LED and turn on the new one.

//     //     // First, set up a char array for the command
//     //     char brainLedCommand[4];

//     //     // Copy the bitmask, and iterate over the bits
//     //     uint32_t channelMask = associatedChannelStripBitmask[bank];
//     //     while (channelMask)
//     //     {
//     //         // Get the index of the lowest set bit
//     //         ChStripID stripID = static_cast<ChStripID>(__builtin_ctz(channelMask));

//     //         if (updateRing)
//     //         {
//     //             // Copy and send command from set, to turn off previous LED
//     //             brainLedCommand[0] = CH_STRIP_LED_MAP[stripID][currentRingLED][0];
//     //             brainLedCommand[1] = CH_STRIP_LED_MAP[stripID][currentRingLED][1];
//     //             brainLedCommand[2] = CH_STRIP_LED_MAP[stripID][currentRingLED][2];
//     //             brainLedCommand[3] = LED_OFF_CMD;
//     //             brainCom.send(brainLedCommand, BRAIN_LED_CMD_LENGTH);

//     //             // Likewise, turn on new LED
//     //             brainLedCommand[0] = CH_STRIP_LED_MAP[stripID][newRingLED][0];
//     //             brainLedCommand[1] = CH_STRIP_LED_MAP[stripID][newRingLED][1];
//     //             brainLedCommand[2] = CH_STRIP_LED_MAP[stripID][newRingLED][2];
//     //             brainLedCommand[3] = LED_ON_CMD;
//     //             brainCom.send(brainLedCommand, BRAIN_LED_CMD_LENGTH);

//     //             // Update the current LED record
//     //             currentRingLED = newRingLED;
//     //         }

//     //         // Update center dot if necessary.
//     //         if (handleDot)
//     //         {
//     //             brainLedCommand[0] = CH_STRIP_LED_MAP[stripID][RING_DOT][0];
//     //             brainLedCommand[1] = CH_STRIP_LED_MAP[stripID][RING_DOT][1];
//     //             brainLedCommand[2] = CH_STRIP_LED_MAP[stripID][RING_DOT][2];
//     //             brainLedCommand[3] = (pan == 127) ? LED_ON_CMD : LED_OFF_CMD;
//     //             brainCom.send(brainLedCommand, BRAIN_LED_CMD_LENGTH);
//     //         }

//     //         // Clear lowest set bit
//     //         channelMask &= channelMask - 1;
//     //     }
//     // }

//     // // Finally, let's update the ui. 
//     // // If this was a UI event, first remove the calling stripID.
//     // // Instead of using conditional check, use the fact that "UI_EVENT" = 1
//     // // and CONSOLE_EVENT = 0;
    
//     // // First, get the bitmask
//     // int uiMask = associatedChannelStripBitmask[bank];

//     // // Update the bit for the calling channel according to what source was.
//     // uiMask |= (source << channelStripID);

//     // // Post a ui event
//     // eventBus.associateUiStripVpotEventPost(uiMask, pan);
// }

// void Channel::handleVpotAuxSend(const char (&panValue)[2], const Bank bank, ChStripID channelStripID, EventSource source)
// {
//     // TODO

// }

// void Channel::handleVpotAuxStereoSend(const char (&panValue)[2], const Bank bank, ChStripID channelStripID, EventSource source)
// {
//     // TODO
// }

// void Channel::handleVpotLevelToTape(const char (&panValue)[2], const Bank bank, ChStripID channelStripID, EventSource source)
// {
//     // TODO
// }

// void Channel::handleVpotDigitalTrim(const char (&panValue)[2], const Bank bank, ChStripID channelStripID, EventSource source)
// {
//     // TODO
// }

// void Channel::handleVpotAuxStereoPan(const char (&panValue)[2], const Bank bank, ChStripID channelStripID, EventSource source)
// {
//     // TODO
// }


// ===========================================  BUTTON EVENT CALLBACKS  =======================================

/**************************************************************************
 * @brief This is the callback for the mute button.
 *        First we send a DSP volume command.
 *        Next, we handle the LED's on all associated channelstrip buttons.
 * 
 * @param btnAction 
 * @param currentBank 
 **************************************************************************/
void Channel::muteBtnCallback(ButtonAction btnAction, Bank currentBank)
{
    printf("MUTE BUTTON\n");

    // Only act on presses, not on releases.
    if (btnAction == BTN_PRESS)
    {
        // Set up a char for the 'i' or 'j', to turn on or off the mute LED
        LedStateCommand stateCmd;

        // Create the command string for volume. 
        char dspVolumeCommand[DSP_VOL_CMD_LENGTH] = "--cX--Q";
        
        // Copy in the Channel ID
        dspVolumeCommand[0] = DSP_CH_ID_STR[0];
        dspVolumeCommand[1] = DSP_CH_ID_STR[1];

        if (mute)
        {
            // Copy in the volume value.
            dspVolumeCommand[4] = volume[0];
            dspVolumeCommand[5] = volume[1];

            // Update the member flag
            mute = false;

            // Set last part of LED command.
            stateCmd = LED_OFF_CMD;
        }
        else
        {
            // Muting, so set volume command to "00"
            dspVolumeCommand[4] = '0';
            dspVolumeCommand[5] = '0';

            // Update the member flag
            mute = true;

            // Set last part of LED command.
            stateCmd = LED_ON_CMD;
        }

        // Send DSP command.
        dspCom.send(dspVolumeCommand, 7);

    }
}


void Channel::soloBtnCallback(ButtonAction btnAction, Bank currentBank)
{
    // TODO
    printf("solo button\n");
}

void Channel::selectBtnCallback(ButtonAction btnAction, Bank currentBank)
{
    // TODO
    printf("select button\n");
}

void Channel::writeBtnCallback(ButtonAction btnAction, Bank currentBank)
{
    // TODO
    printf("write button\n");
}

void Channel::assignBtnCallback(ButtonAction btnAction, Bank currentBank)
{
    // TODO
    printf("assign button\n");
}
void Channel::recRdyBtnCallback(ButtonAction btnAction, Bank currentBank)
{
    // TODO
    printf("recrdy button\n");
}


void Channel::initializeChannel()
{
    // TODO: The channels are constructed at program start, but the console
    // is booted later. This means that any settings for the console - be it Brain or DSP
    // has to happen later.
    // So this method is called after the console is up an running, to do any commands to 
    // these two as initialization for the channel.

    // TODO: BUT - is this necessary? Isn't this the standard settings?

    // ================ SET VOLUME TO 0 =======================
    char dspVolumeCommand[DSP_VOL_CMD_LENGTH] = "--cX--Q";
    dspVolumeCommand[0] = DSP_CH_ID_STR[0];
    dspVolumeCommand[1] = DSP_CH_ID_STR[1];
    dspVolumeCommand[4] = volume[0];
    dspVolumeCommand[5] = volume[1];
    dspCom.send(dspVolumeCommand, 7);   // 7 chars to send.

    // =============== SET PAN TO CENTER ======================
    // TODO: Also change this to fetch the pan value - fits better with saved settings.
    pan = 0x7F; // Center
    char dspPanCommand[] = "--dFEFFX7FOFDFFXP";
    dspPanCommand[0] = DSP_CH_ID_STR[0];
    dspPanCommand[1] = DSP_CH_ID_STR[1];
    dspCom.send(dspPanCommand,17);

}
