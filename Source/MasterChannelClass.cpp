/*
  ==============================================================================

    MasterChannel.cpp
    Created: 11 Jun 2023 12:14:52pm
    Author:  anders

  ==============================================================================
*/

#include "MasterChannelClass.h"
#include "SharedDataStructures.h"
//#include <unistd.h>
#include "Debug.h"

#define MASTER_FADER_CMD "--18f"    // String litteral for fader command initialization.

constexpr int DSP_MASTER_VOL_CMD_LENGTH = 8; 
//constexpr char MASTER_STRIP_ID_STR[3] = "18";


// Constructor
MasterChannel::MasterChannel()
 : brainCom(BrainCom::getInstance()),
   dspCom(DspCom::getInstance())
    //   //hexToIntLookup(HexToIntLookup::getInstance(),
    //   ledRingLookup(LEDringLookup::getInstance())
{

    // // Make a lambda expression for subscribing the callback
    // FaderCallback masterFaderCallback = [this](const char(&faderValue)[2],
    //                                             Bank bank,
    //                                             ChStripID channelStripID,
    //                                             EventSource source)
    // {
    //     this->masterFaderEventCallback(faderValue, bank, channelStripID, source);
    // };

    // Use it to subscribe.
    //eventBus.masterFaderEventSubscribe(masterFaderCallback);

}



void MasterChannel::updateChannelVolume(Bank irrelevantOnMasterChannel, const char (&faderValue)[2])
{
    // Need bank argument to align with interface.

    // Update the volume member
    masterVolume[0] = faderValue[0];
    masterVolume[1] = faderValue[1];

    // Construct and send DSP command.
    //char dspMasterVolumeCommand[] = "4Cc9X--QAX--Q";
    dspMasterVolumeCommand[5] = faderValue[0];
    dspMasterVolumeCommand[10] = faderValue[0];
    dspMasterVolumeCommand[6] = faderValue[1];
    dspMasterVolumeCommand[11] = faderValue[1];
    dspCom.send(dspMasterVolumeCommand, MASTER_VOL_DSP_CMD_LENGTH); 
}



void MasterChannel::updateFaderPosition(Bank irrelevantOnMasterChannel)
{
    faderMoveCmd[2] = masterVolume[0];
    faderMoveCmd[3] = masterVolume[1];
    brainCom.send(faderMoveCmd, BRAIN_FADER_CMD_LENGTH);
}

// ###########################################################################################################################
// This is a callback function that is fired by the EventBus, when the master fader is moved.
// First it sends a DSP command to update the volume. Then it will iterate over all associalted channelstrips
// on the console, and update them. Finaly it will post another event, which will update the channelStripComponents in the UI
// ###########################################################################################################################


// /******************************************************************************
//  * @brief This is the callback method for the master fader.
//  *        First we construct a DSP command to update the volume. 
//  *        Then, depending on whether the fader was moved on the console or in
//  *        the UI, we update the other.
//  * 
//  * @param faderValue 
//  * @param bank 
//  * @param channelStripID 
//  * @param source 
//  *****************************************************************************/
// void MasterChannel::masterFaderEventCallback(const char (&faderValue)[2],
//                                              Bank bank,
//                                              ChStripID channelStripID,
//                                              EventSource source)
// {
//     // Construct DSP volume command, and send it.
//     // Interestingly, this command can be divided: 4Cc9XyyQ for left side and 4CcAXyyQ for right side. (yy is value)
//     // TODO: is it always the same value for both sideS???

//     // Update the master volume member
//     masterVolume[0] = faderValue[0];
//     masterVolume[1] = faderValue[1];

//     char dspMasterVolumeCommand[] = "4Cc9X--QAX--Q";
//     dspMasterVolumeCommand[5] = faderValue[0];
//     dspMasterVolumeCommand[10] = faderValue[0];
//     dspMasterVolumeCommand[6] = faderValue[1];
//     dspMasterVolumeCommand[11] = faderValue[1];

//     //std::string dspMasterVolumeCommand = "4Cc9X" + faderValue + "QAX" + faderValue + "Q";

// 	dspCom.send(dspMasterVolumeCommand, 13);    

// 	// If Console fader was moved, update UI, else update console.
//     if (source == CONSOLE_EVENT)
//     {
//         int masterFaderValue = hexToIntLookup.hexToInt(faderValue);
//         eventBus.updateUiMasterFaderEventPost(masterFaderValue);
//     }
//     else
//     {
//         char brainMasterVolumeCommand[] = "18--f"; //"18" + faderValue + "f";
//         brainMasterVolumeCommand[2] = masterVolume[0];
//         brainMasterVolumeCommand[3] = masterVolume[1];
//         brainCom.send(brainMasterVolumeCommand, 5);
//     }
// }

// void MasterChannel::masterStripVpotEventCallback(const std::string vPotValue,
//                                              const Bank bank,
//                                              const std::string &channelStripID,
//                                              EventSource source)
// {
// }

// void MasterChannel::masterStripButtonEventCallback(const std::string buttonValue,
//                                              const Bank bank,
//                                              const std::string &channelStripID,
//                                              EventSource source)
// {
// }

// void MasterChannel::removeMasterStripAssociationCallback(const Bank bank, const std::string channelStripID)
// {
//     // No reason for this... the Master will not be removed....
// }

// void MasterChannel::initializeMasterChannel()
// {
//     // TODO: This is probably where we would load saved settings.

//     printf("initializing master channel\n");

//     // Set volume to 0
//     char dspMasterVolumeCommand[] = "4Cc9X00QAX00Q";
//     dspCom.send(dspMasterVolumeCommand, 13);


//     // Pull fader to 0
//     char brainMasterVolumeCommand[] = "1800f";
//     brainCom.send(brainMasterVolumeCommand, 5);


//     // UI is initially already at 0 and center.
// }


// void MasterChannel::bankChangeCallback(ChStripID chStripID)
// {
//     1: construct fader command from stripID and send it.
//     2: send ui update event post for same.
//     3: construct led commands for the vpot, to turn on the correct one, and the others off.
//     4: construct and send the led command for channel
//     5: same for button led's
// }

void MasterChannel::updateChStrip(Bank bank)
{
    // Not using the bank for master.

    // // THE CURRENT STATE IS SAVED IN THE OBJECT. COMPARE FOR CHECKING WHAT TO CHANGE
    // uint32_t newLedOnStates = channelPtrs[bank]->getLedOnBitmap();
    // uint32_t newLedBlinkStates = channelPtrs[bank]->getLedBlinkBitmap();

    // // XOR the bitmaps with the new channel's bitmaps, to get a bitmap of changes.
    // uint32_t changeOnBitmap = currentLedOnStates ^ newLedOnStates;
    // uint32_t changeBlinkBitmap = currentLedBlinkStates ^ newLedBlinkStates;

    // // Set up a char string for the brain LED commands
    // char brainLedCommand[LED_COMMAND_LENGTH];

    // // Iterate through the bitmaps and send brain messages to update LED's
    // while(changeOnBitmap)
    // {
    //     // Get the index of the lowest set bit. (corresponds to ChStripLed)
    //     int bitIndex = __builtin_ctz(changeOnBitmap);

    //     // Get bit value (0 or 1)
    //     uint32_t bitValue = (changeOnBitmap >> bitIndex) & 1;

    //     // Add the value to get the ON or OFF command for the LED
    //     LedStateCommand stateCmd = static_cast<LedStateCommand>(LED_OFF_CMD - bitValue);    // off=106, on=105

    //     // Construct Brain command, and send it.
    //     brainLedCommand[0] = CH_STRIP_LED_MAP[CH_STRIP_INDEX][bitIndex][0];
    //     brainLedCommand[1] = CH_STRIP_LED_MAP[CH_STRIP_INDEX][bitIndex][1];
    //     brainLedCommand[2] = CH_STRIP_LED_MAP[CH_STRIP_INDEX][bitIndex][2];
    //     brainLedCommand[3] = stateCmd;
    //     brainCom.send(brainLedCommand, LED_COMMAND_LENGTH);

    //     // Clear lowest set bit
    //     changeOnBitmap &= changeOnBitmap - 1;
    // }

    // // For the blink bitmap, all 1's means activate blink.
    // while(changeBlinkBitmap)
    // {
    //     int blinkBitIndex = __builtin_ctz(changeBlinkBitmap);
    //     brainLedCommand[0] = CH_STRIP_LED_MAP[CH_STRIP_INDEX][blinkBitIndex][0];
    //     brainLedCommand[1] = CH_STRIP_LED_MAP[CH_STRIP_INDEX][blinkBitIndex][1];
    //     brainLedCommand[2] = CH_STRIP_LED_MAP[CH_STRIP_INDEX][blinkBitIndex][2];
    //     brainLedCommand[3] = LED_BLINK_CMD;
    //     brainCom.send(brainLedCommand, LED_COMMAND_LENGTH);
    //     changeBlinkBitmap &= changeBlinkBitmap - 1;
    // }

    // // Update the ChannelStrip's record of LED's
    // currentLedOnStates = newLedOnStates;
    // currentLedBlinkStates = newLedBlinkStates;

    // Now update the fader position.
    // Create a fader command string from the new channel's volume string,
    // and send it.
    // char brainFaderCommand[BRAIN_FADER_CMD_LENGTH]; 
    // brainFaderCommand[0] = CH_STRIP_ID_STR[0];
    // brainFaderCommand[1] = CH_STRIP_ID_STR[1];
    // brainFaderCommand[2] = channelPtrs[bank]->getVolume()[0];
    // brainFaderCommand[3] = channelPtrs[bank]->getVolume()[1];
    // brainFaderCommand[4] = 'f';
    // brainCom.send(brainFaderCommand, BRAIN_FADER_CMD_LENGTH);

}