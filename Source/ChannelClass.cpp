/*
  ==============================================================================

    ChannelClass.cpp
    Created: 14 Apr 2023 9:48:22pm
    Author:  anders

  ==============================================================================
*/

#include "ChannelClass.h"
#include "ChannelIDMap.h"
#include <unistd.h>

// UNCOMMENT TO ENABLE DEBUG MESSAGES.
#define CHANNEL_DEBUG_MESSAGES

#ifdef CHANNEL_DEBUG_MESSAGES
#define DEBUG_MSG(format, ...) printf("CHNL_DBG: " format, ##__VA_ARGS__)
#else
#define DEBUG_MSG(format, ...) ((void)0) // do {} while (0)
#endif

// Set first channel ID. This will increment with every channel object constructed.
uint8_t Channel::nextChannelNumber = 0;

// Constructor. Set up channel number and get channel dsp ID from channelIDMap.h
Channel::Channel()
    : channelID{dspChannelIDs[nextChannelNumber]}, channelNumber{nextChannelNumber}
{
    DEBUG_MSG("\n=============== CHANNEL CONSTRUCTOR =====================\n");

    // VALUE COMMANDS ARE STILL SENT AS HEX STRINGS..... WHEN SHOULD A CONVERSION FROM INT TO CHAR HAPPEN?

	mute = false;
    
    // solo = false;

    // pan = 127;  // Center (0x7F)

    // assignments[0] = true;  // Assign to L-R. All others should default to false.

    // Initialize other member variables to default values...

	// Increment the nextChannelNumber for the next channel construction.
	nextChannelNumber++;
}

// Implement other member functions...
std::string Channel::getID()
{
	return channelID;
}

// #######################################################################################
// This method updates the channel object's volume member, and sends a command to the DSP
// #######################################################################################
void Channel::setVolume(std::string volumeValue, int dspDescriptor)
{
	// Update volume member
	volume = volumeValue;

	// Construct DSP command, and send it.
	std::string volumeCommand = channelID + "cX" + volumeValue + "Q";
	if (!mute)
		write(dspDescriptor, volumeCommand.c_str(), volumeCommand.length());
}