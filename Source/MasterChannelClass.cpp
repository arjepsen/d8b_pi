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

    DEBUG_MSG("MASTER CHANNEL CLASS CONSTRUCTOR\n");
    // // Subscribe to events (callbacks will ignore ID and Bank)
    // eventBus.bankEventSubscribe(
    //     LINE_BANK,
    //     MASTER_STRIP_ID,
    //     [this](const std::string &faderValue, const Bank bank, const std::string &channelStripID, EventSource source)
    //     { this->masterStripFaderEventCallback(faderValue, bank, channelStripID, source); },
    //     [this](const std::string &vpotValue, const Bank bank, const std::string &channelStripID, EventSource source)
    //     { this->masterStripVpotEventCallback(vpotValue, bank, channelStripID, source); },
    //     [this](const std::string &buttonValue, const Bank bank, const std::string &channelStripID, EventSource source)
    //     { this->masterStripVpotEventCallback(buttonValue, bank, channelStripID, source); },
    //     [this](const Bank bank, const std::string &channelStripID)
    //     { this->removeMasterStripAssociationCallback(bank, channelStripID); });

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

    DEBUG_MSG("END OF MASTER CHANNEL CONSTRUCTOR\n");
}

// void MasterChannel::setMasterVolume(std::string volumeString, int dspDescriptor)
// {
//     // Construct DSP command, and send it.
//     std::string dspMasterVolumeCommand = "4Cc9X" + volumeString + "QAX" + volumeString + "Q";
//     // write(dspDescriptor, dspMasterVolumeCommand.c_str(), dspMasterVolumeCommand.length());
// 	dspCom.send(dspMasterVolumeCommand);


// 	//SO - IF WE SEND 4Cc9XC1Q   would that only update volume in one side?

//     // Update the member.
//     masterVolume = volumeString;
// }


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

    // TODO: check if 13 is enough... not transmittion null terminator.
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
        brainCom.send(brainMasterVolumeCommand);
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

