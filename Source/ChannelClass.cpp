/*
  ==============================================================================

    ChannelClass.cpp
    Created: 14 Apr 2023 9:48:22pm
    Author:  anders

  ==============================================================================
*/

#include "ChannelClass.h"
#include "ChannelIDMap.h"
// #include <iomanip>
// #include <sstream>
#include <cstring>
#include <unistd.h>
#include <algorithm>    // for std::clamp
#include "Debug.h"



constexpr size_t LED_CMD_LENGTH = 5; // Number of chars in the LED DSP commands, including null terminator.
constexpr int STRIP_ID_LENGTH = 3;   // Number of chars in the channel strip ID, incl. null.
constexpr int VOL_VALUE_LENGTH = 3;  // Max chars in the volume value string. (one or two.) Incl. null.
constexpr int DSP_VOL_CMD_LENGTH = 8;     // Max dsp volume command length. (for setting array). ie. "0AcXC8Q" (can be only 7, when just one hex digit for volume)
constexpr int DSP_PAN_CMD_LENGTH = 16;
constexpr int DSP_ID_LENGTH = 3;

const char Channel::DSP_PAN_CMD[18] = "--dFEFFX--OFDFFXP";

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
      dspCom(DspCom::getInstance()),
      intToHexLookup(IntToHexLookup::getInstance()),
      hexToIntLookup(HexToIntLookup::getInstance()),
      ledRingLookup(LEDringLookup::getInstance())
{
    // Set the two bits of the LED bitmask from the channel number.
    ChannelLeds chLeds = CH_LEDS_NONE;  
    if (CH_NUMBER < 24)
        chLeds = CH_LEDS_GREEN;
    else if (CH_NUMBER < 48)
        chLeds = CH_LEDS_RED;
    else if (CH_NUMBER < 72)
        chLeds = CH_LEDS_BOTH;

    desiredLedOnBitmap = static_cast<uint32_t>(chLeds) << 18;  // Shift enum to set bits.


    // // TODO: OTHER LEDS??

    // Initialize the method pointers to their corresponding member functions
    vPotMethods[VPOT_PAN] = &Channel::updatePan;
    vPotMethods[VPOT_AUXSEND1] = &Channel::updateAuxSend1;
    vPotMethods[VPOT_AUXSEND2] = &Channel::updateAuxSend2;
    vPotMethods[VPOT_AUXSEND3] = &Channel::updateAuxSend3;
    vPotMethods[VPOT_AUXSEND4] = &Channel::updateAuxSend4;
    vPotMethods[VPOT_AUXSEND5] = &Channel::updateAuxSend5;
    vPotMethods[VPOT_AUXSEND6] = &Channel::updateAuxSend6;
    vPotMethods[VPOT_AUXSEND7] = &Channel::updateAuxSend7;
    vPotMethods[VPOT_AUXSEND8] = &Channel::updateAuxSend8;
    vPotMethods[VPOT_AUXSEND9_10] = &Channel::updateAuxSend9_10;
    vPotMethods[VPOT_AUXPAN9_10] = &Channel::updateAuxPan9_10;
    vPotMethods[VPOT_AUXSEND11_12] = &Channel::updateAuxSend11_12;
    vPotMethods[VPOT_AUXPAN11_12] = &Channel::updateAuxPan11_12;
    vPotMethods[VPOT_LVL2TAPE] = &Channel::updateLvl2Tape;
    vPotMethods[VPOT_DIGITAL_TRIM] = &Channel::updateDigitalTrim;

    // Set initial pan value to center
    vPotFunctionValues[VPOT_PAN] = 127;

    // Set stereo aux pan value to center
    vPotFunctionValues[VPOT_AUXPAN9_10] = 127;
    vPotFunctionValues[VPOT_AUXPAN11_12] = 127;




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



// ===========================================  VPOT EVENT HANDLERS  =======================================



int Channel::updatePan(int vPotValue, EventSource source)
{
    // If the event is from UI, we can use the value directly.
    // If it is from console, it is a CHANGE value.
    // Use the source enumeration to choose - avoid conditionals.

    // Do calculations for the cases where the pot was turned on the console
    // The value is indicating how much the pot was turned. Add the value to the 
    // current pan value, and clamp it within 0 - 254
    int panChangeValue = std::clamp(static_cast<int>(vPotFunctionValues[VPOT_PAN] + vPotValue), 0, 254);

    // Use source enum in calculation to store the correct value, and avoid conditionals.
    // This is because UI sends new pan value, and console sends amount of change.
    //vPotFunctionValues[VPOT_PAN] = (panChangeValue * !source) + (vPotValue * source);
    int newPanValue = (panChangeValue * !source) + (vPotValue * source);    // Seperate variable for readable code.
    
    vPotFunctionValues[VPOT_PAN] = newPanValue;

    // Convert to 2-char hex value for command.
    const char *panHexValue = intToHexLookup.getHexValue(newPanValue);

    // Create initial DSP command string.
    char dspPanCommand[18];
    memcpy(dspPanCommand, DSP_PAN_CMD, DSP_PAN_CMD_LENGTH); // "--dFEFFX--OFDFFXP"

    // Copy in ID and pan value.
    dspPanCommand[0] = DSP_CH_ID_STR[0];
    dspPanCommand[1] = DSP_CH_ID_STR[1];
    dspPanCommand[8] = panHexValue[0];
    dspPanCommand[9] = panHexValue[1];

    dspCom.send(dspPanCommand, 17); // Ignoring terminator.

    // Now update the LED bitmap. First, look up which ring LED should be lit.
    ChStripLED ringBit = ledRingLookup.getRingID(newPanValue);

    // Clear all ring bits, then set the one we looked up.
    desiredLedOnBitmap &= CLEAR_RING_MASK;
    desiredLedOnBitmap |= 1 << ringBit;

    // Set dot LED bit if pan is centered
    desiredLedOnBitmap |= ((newPanValue == 127) << RING_DOT); 

    return newPanValue;
}

// TODO:
int Channel::updateAuxSend1(int vPotValue, EventSource source) {return 0;}
int Channel::updateAuxSend2(int vPotValue, EventSource source) {return 0;}
int Channel::updateAuxSend3(int vPotValue, EventSource source) {return 0;}
int Channel::updateAuxSend4(int vPotValue, EventSource source) {return 0;}
int Channel::updateAuxSend5(int vPotValue, EventSource source) {return 0;}
int Channel::updateAuxSend6(int vPotValue, EventSource source) {return 0;}
int Channel::updateAuxSend7(int vPotValue, EventSource source) {return 0;}
int Channel::updateAuxSend8(int vPotValue, EventSource source) {return 0;}
int Channel::updateAuxSend9_10(int vPotValue, EventSource source) {return 0;}
int Channel::updateAuxPan9_10(int vPotValue, EventSource source) {return 0;}
int Channel::updateAuxSend11_12(int vPotValue, EventSource source) {return 0;}
int Channel::updateAuxPan11_12(int vPotValue, EventSource source) {return 0;}
int Channel::updateLvl2Tape(int vPotValue, EventSource source) {return 0;}
int Channel::updateDigitalTrim(int vPotValue, EventSource source) {return 0;}


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


/****************************************************************************
 * @brief This method is called by the "loadSettings" method in eventbus,
 *        which in turn is called by the mixermanager when the console
 *        has been booted and comms are set up. 
 *        LoadSettings is responsible for loading the settings saved to file,
 *        and then run this method to handle the channel side of things.
 ****************************************************************************/
void Channel::initializeChannel(VpotFunction currentVPotFunction)
{

    // TODO: For now, ignore midi channels - THEY PROLLY HAVE DIFFERENT COMMAND??
    // right now we are sending stuff that aint right at bootup....

    

    // ================ SET VOLUME TO 0 =======================
    char dspVolumeCommand[DSP_VOL_CMD_LENGTH] = "--cX--Q";
    dspVolumeCommand[0] = DSP_CH_ID_STR[0];
    dspVolumeCommand[1] = DSP_CH_ID_STR[1];
    dspVolumeCommand[4] = volume[0];
    dspVolumeCommand[5] = volume[1];
    dspCom.send(dspVolumeCommand, 7);   // 7 chars to send.

    // =============== SET PAN TO CENTER ======================
    // TODO: Also change this to fetch the pan value - fits better with saved settings.

    char dspPanCommand[] = "--dFEFFX7FOFDFFXP";
    dspPanCommand[0] = DSP_CH_ID_STR[0];
    dspPanCommand[1] = DSP_CH_ID_STR[1];
    dspCom.send(dspPanCommand, 17);



    // ============= REGISTER LED's ==========================
    // Get the vPot Value for the registered function.
    int vPotValue = vPotFunctionValues[currentVPotFunction];
    
    // Update the LED bitmap. First, look up which ring LED should be lit.
    ChStripLED ringBit = ledRingLookup.getRingID(vPotValue);

    // Clear all ring bits, then set the one we looked up.
    desiredLedOnBitmap &= CLEAR_RING_MASK;
    desiredLedOnBitmap |= 1 << ringBit;

    // Set dot LED bit if pan is centered
    // TODO: This might need more work, depending on function?
    desiredLedOnBitmap |= ((vPotValue == 127) << RING_DOT); 


    // TODO: Button LED's
}
