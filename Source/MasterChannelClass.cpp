/*
  ==============================================================================

    MasterChannel.cpp
    Created: 11 Jun 2023 12:14:52pm
    Author:  anders

  ==============================================================================
*/

#include "MasterChannelClass.h"
//#include <unistd.h>
#include "Debug.h"


// Constructor
MasterChannel::MasterChannel()
    : eventBus(EventBus::getInstance()), // Get the singleton instances
      brainCom(BrainCom::getInstance()),
      dspCom(DspCom::getInstance()),
      hexToIntLookup(HexToIntLookup::getInstance())
{

    // Make a lambda expression for subscribing the callback
    FaderCallback masterFaderCallback = [this](const char(&faderValue)[2],
                                                Bank bank,
                                                ChStripID channelStripID,
                                                EventSource source)
    {
        this->masterFaderEventCallback(faderValue, bank, channelStripID, source);
    };

    // Use it to subscribe.
    eventBus.masterFaderEventSubscribe(masterFaderCallback);

}




// ###########################################################################################################################
// This is a callback function that is fired by the EventBus, when the master fader is moved.
// First it sends a DSP command to update the volume. Then it will iterate over all associalted channelstrips
// on the console, and update them. Finaly it will post another event, which will update the channelStripComponents in the UI
// ###########################################################################################################################


/******************************************************************************
 * @brief This is the callback method for the master fader.
 *        First we construct a DSP command to update the volume. 
 *        Then, depending on whether the fader was moved on the console or in
 *        the UI, we update the other.
 * 
 * @param faderValue 
 * @param bank 
 * @param channelStripID 
 * @param source 
 *****************************************************************************/
void MasterChannel::masterFaderEventCallback(const char (&faderValue)[2],
                                             Bank bank,
                                             ChStripID channelStripID,
                                             EventSource source)
{
    // Construct DSP volume command, and send it.
    // Interestingly, this command can be divided: 4Cc9XyyQ for left side and 4CcAXyyQ for right side. (yy is value)
    // TODO: is it always the same value for both sideS???

    // Update the master volume member
    masterVolume[0] = faderValue[0];
    masterVolume[1] = faderValue[1];

    char dspMasterVolumeCommand[] = "4Cc9X--QAX--Q";
    dspMasterVolumeCommand[5] = faderValue[0];
    dspMasterVolumeCommand[10] = faderValue[0];
    dspMasterVolumeCommand[6] = faderValue[1];
    dspMasterVolumeCommand[11] = faderValue[1];

    //std::string dspMasterVolumeCommand = "4Cc9X" + faderValue + "QAX" + faderValue + "Q";

	dspCom.send(dspMasterVolumeCommand, 13);    

	// If Console fader was moved, update UI, else update console.
    if (source == CONSOLE_EVENT)
    {
        int masterFaderValue = hexToIntLookup.hexToInt(faderValue);
        eventBus.updateUiMasterFaderEventPost(masterFaderValue);
    }
    else
    {
        char brainMasterVolumeCommand[] = "18--f"; //"18" + faderValue + "f";
        brainMasterVolumeCommand[2] = masterVolume[0];
        brainMasterVolumeCommand[3] = masterVolume[1];
        brainCom.send(brainMasterVolumeCommand, 5);
    }
}

void MasterChannel::masterStripVpotEventCallback(const std::string vPotValue,
                                             const Bank bank,
                                             const std::string &channelStripID,
                                             EventSource source)
{
}

void MasterChannel::masterStripButtonEventCallback(const std::string buttonValue,
                                             const Bank bank,
                                             const std::string &channelStripID,
                                             EventSource source)
{
}

void MasterChannel::removeMasterStripAssociationCallback(const Bank bank, const std::string channelStripID)
{
    // No reason for this... the Master will not be removed....
}

void MasterChannel::initializeMasterChannel()
{
    // TODO: This is probably where we would load saved settings.

    printf("initializing master channel\n");

    // Set volume to 0
    char dspMasterVolumeCommand[] = "4Cc9X00QAX00Q";
    dspCom.send(dspMasterVolumeCommand, 13);


    // Pull fader to 0
    char brainMasterVolumeCommand[] = "1800f";
    brainCom.send(brainMasterVolumeCommand, 5);


    // UI is initially already at 0 and center.
}