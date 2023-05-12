/*
  ==============================================================================

    MixerManagerClass.h
    Created: 14 Apr 2023 10:12:41pm
    Author:  anders

    The MixerManager class is where the UI is tied together with the rest
    of the program. Everything should pass through here, so we have
    seperation of concerns.
    This should be a "singleton" class, so only one instance can exist.
  ==============================================================================
*/

#pragma once
#include "ChannelClass.h"
#include "SettingsClass.h"
#include <array>
#include <cstdint>

class MixerManager
{
private:
    static constexpr uint8_t CHANNEL_COUNT = 56;
    std::array<Channel, CHANNEL_COUNT> channels;

    Settings &settings;

    MixerManager(); // Constructor

    // Delete copy constructor and assignment operator, to avoid copying the singleton.
    MixerManager(const MixerManager &) = delete;
    MixerManager &operator=(const MixerManager &) = delete;

public:
    static MixerManager &getInstance(); // Returns a reference to the instance.

    // Public methods for interacting with channels
    const Channel &getChannel(uint8_t id) const; // Ensure channels stay in their place in the array.

    // Public method for providing access to the Settings instance
    const Settings &getSettings() const;

    // TODO: Add methods to handle communication with the Brain and DSP boards
};

// Singleton modifications
inline MixerManager &MixerManager::getInstance()
{
    static MixerManager instance;
    return instance;
}

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