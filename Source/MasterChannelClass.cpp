/*
  ==============================================================================

    MasterChannel.cpp
    Created: 11 Jun 2023 12:14:52pm
    Author:  anders

  ==============================================================================
*/

#include "MasterChannelClass.h"
#include <unistd.h>

// Constructor
MasterChannel::MasterChannel()
{
}

void MasterChannel::setMasterVolume(std::string volumeString, int dspDescriptor)
{
	// Construct DSP command, and send it.
	std::string dspMasterVolumeCommand = "4Cc9X" + volumeString + "QAX" + volumeString + "Q";
	write(dspDescriptor, dspMasterVolumeCommand.c_str(), dspMasterVolumeCommand.length());

	// Update the member.
	masterVolume = volumeString;
}