/*
  ==============================================================================

    MixerManagerClass.cpp
    Created: 14 Apr 2023 10:12:41pm
    Author:  anders

  ==============================================================================
*/

#include "MixerManagerClass.h"
#include <algorithm>
#include <numeric>
#include <stdexcept>

// std::array automatically initializes elements to default value - 
// so Channel constructor gets called automatically.
MixerManager::MixerManager() : channels{} 
{
  // assign default values to each channel's members, 
  // And at the same time, send the corresponding commands to the mixer.
  // When this program runs, and this constructor is run, we can have had
  // The initial bootscript run already.
  // ALSO - we might want this constructor to read a saved configuration file, from last "session"
  // So the user starts out where he left?
    for (auto &channel : channels)
    {
        // Initialize channel member variables to their default values
        // For example:
        // channel.setVolume(defaultVolume);
        // channel.setPan(defaultPan);
        // ...
        // ... or maybe: channel.setDefaults() / channel.initialize.

        // Send the corresponding commands to the mixer
        // For example:
        // sendVolumeCommand(channel.getChannelID(), defaultVolume);
        // sendPanCommand(channel.getChannelID(), defaultPan);
        // ...

    }
}

const Channel &MixerManager::getChannel(uint8_t id) const
{
    if (id < CHANNEL_COUNT)
        return channels[id];
    throw std::out_of_range("Invalid channel ID");
}
// Implement other methods...


// Other things to be aware of in constructor:
// Which Bank to select? 
// config file
// 