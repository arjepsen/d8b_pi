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
        eventBus.associateMasterEventPost(FADER_EVENT, faderValue);
        instead: uiMasterFaderEventPost
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

}

//////////////////////////////////////////////////////////////////
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