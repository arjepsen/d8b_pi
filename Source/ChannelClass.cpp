/*
  ==============================================================================

    ChannelClass.cpp
    Created: 14 Apr 2023 9:48:22pm
    Author:  anders

  ==============================================================================
*/

#include "ChannelClass.h"


uint8_t Channel::nextID = 0;


// Constructor
Channel::Channel() : channelID{nextID++}
{
  // Initialize other member variables to default values...
}

// Implement other member functions...
