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
MixerManager::MixerManager() : channels{} {}


const Channel &MixerManager::getChannel(uint8_t id) const
{
    if (id < CHANNEL_COUNT)
        return channels[id];
    throw std::out_of_range("Invalid channel ID");
}
// Implement other methods...
