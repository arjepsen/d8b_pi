/*
  ==============================================================================

    MixerManagerClass.h
    Created: 14 Apr 2023 10:12:41pm
    Author:  anders

  ==============================================================================
*/

#pragma once
#include <cstdint>
#include <array>
#include "ChannelClass.h"

class MixerManager
{
private:
  static constexpr uint8_t CHANNEL_COUNT = 56;
  std::array<Channel, CHANNEL_COUNT> channels;

public:
  MixerManager();

  // Public methods for interacting with channels
  const Channel &getChannel(uint8_t id) const; // Ensure channels stay in their place in the array.

  // Add methods to handle communication with the Brain and DSP boards
};


//////////////////////////////////
// VARIOUS NOTES //

// Mixermanager object should be instantiated in MainComponent. (So it's accessible for everything else...)
// Remaining classes should be instantiated in mixermanager.
// So channels gets instantiated in mixermanager.
// But channelstripcomponent are also instantiated in maincomponent, since they are a gui element.
// Channels are data storage. When updated, the mixermanager also updates the channelstripcomponent.

// There are four banks - and the two first are channels.
// SO MAYBE THE CONNECTION BETWEEN CHANNELSTRIP AND CHANNEL LIES ENTIRELY IN MIXERMANAGER?

// This could make BANK selection easier, since it's simply an update in mixermanager, which then updates the
// channelstrip graphically.

// There should be a "ChangeBank" method here  in mixermanager, which is called, when a bank button is pressed
// (either physically, or though gui)

// There are several sections on the mixer - but some of them are maybe just purely variables?
// for example "Assignments" only contain buttons...

// SELECT button is probably a variable in MixerManager.