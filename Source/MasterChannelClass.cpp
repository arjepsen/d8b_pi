/*
  ==============================================================================

    MasterChannel.cpp
    Created: 11 Jun 2023 12:14:52pm
    Author:  anders

  ==============================================================================
*/

#include "MasterChannelClass.h"
//#include <unistd.h>

// Constructor
MasterChannel::MasterChannel()
    : eventBus(EventBus::getInstance()), // Get the singleton instances
      brainCom(BrainCom::getInstance()),
      dspCom(DspCom::getInstance())
{

    // Subscribe to events (callbacks will ignore ID and Bank)
    eventBus.bankEventSubscribe(
        LINE_BANK,
        MASTER_STRIP_ID,
        [this](const std::string &faderValue, const Bank bank, const std::string &channelStripID, EventSource source)
        { this->masterStripFaderEventCallback(faderValue, bank, channelStripID, source); },
        [this](const std::string &vpotValue, const Bank bank, const std::string &channelStripID, EventSource source)
        { this->masterStripVpotEventCallback(vpotValue, bank, channelStripID, source); },
        [this](const std::string &buttonValue, const Bank bank, const std::string &channelStripID, EventSource source)
        { this->masterStripVpotEventCallback(buttonValue, bank, channelStripID, source); },
        [this](const Bank bank, const std::string &channelStripID)
        { this->removeMasterStripAssociationCallback(bank, channelStripID); });

}

void MasterChannel::setMasterVolume(std::string volumeString, int dspDescriptor)
{
    // Construct DSP command, and send it.
    std::string dspMasterVolumeCommand = "4Cc9X" + volumeString + "QAX" + volumeString + "Q";
    // write(dspDescriptor, dspMasterVolumeCommand.c_str(), dspMasterVolumeCommand.length());
	dspCom.send(dspMasterVolumeCommand);


	//SO - IF WE SEND 4Cc9XC1Q   would that only update volume in one side?

    // Update the member.
    masterVolume = volumeString;
}


// ###########################################################################################################################
// This is a callback function that is fired by the EventBus, when the master fader is moved.
// First it sends a DSP command to update the volume. Then it will iterate over all associalted channelstrips
// on the console, and update them. Finaly it will post another event, which will update the channelStripComponents in the UI
// ###########################################################################################################################
void MasterChannel::masterStripFaderEventCallback(const std::string faderValue,
                                             const Bank bank,
                                             const std::string &channelStripID,
                                             EventSource source)
{
    // Construct DSP volume command, and send it.
    // Interestingly, this command can be divided: 4Cc9XyyQ for left side and 4CcAXyyQ for right side. (yy is value)
    // TODO: is it always the same value for both sideS???
    std::string dspMasterVolumeCommand = "4Cc9X" + faderValue + "QAX" + faderValue + "Q";

	dspCom.send(dspMasterVolumeCommand);

    // Update volume member
    masterVolume = faderValue;

	// If Console fader was moved, update UI, else update console.
    if (source == CONSOLE_EVENT)
    {
        eventBus.associateMasterEventPost(FADER_EVENT, faderValue);
    }
    else
    {
        std::string brainMasterVolumeCommand = "18" + faderValue + "f";
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

}