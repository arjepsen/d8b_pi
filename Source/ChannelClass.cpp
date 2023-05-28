/*
  ==============================================================================

    ChannelClass.cpp
    Created: 14 Apr 2023 9:48:22pm
    Author:  anders

  ==============================================================================
*/

#include "ChannelClass.h"

// UNCOMMENT TO ENABLE DEBUG MESSAGES.
#define CHANNEL_DEBUG_MESSAGES

#ifdef CHANNEL_DEBUG_MESSAGES
#define DEBUG_MSG(format, ...) printf("IO_INIT_DBG: " format, ##__VA_ARGS__)
#else
#define DEBUG_MSG(format, ...) ((void)0) // do {} while (0)
#endif

// Set first channel ID. This will increment with every channel object constructed.
uint8_t Channel::nextID = 0;  

// Constructor.
Channel::Channel() : channelID{nextID++}
{
    DEBUG_MSG("\n=============== CHANNEL CONSTRUCTOR =====================\n");

    // VALUE COMMANDS ARE STILL SENT AS HEX STRINGS..... WHEN SHOULD A CONVERSION FROM INT TO CHAR HAPPEN?

    volume = 0;
    mute = false;
    solo = false;

    pan = 127;  // Center (0x7F)

    assignments[0] = true;  // Assign to L-R. All others should default to false.


    // Initialize other member variables to default values...
}

// Implement other member functions...
