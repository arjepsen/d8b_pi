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

// ############################ CONSTRUCTOR ####################################
// std::array automatically initializes elements to default value -
// so Channel constructor gets called automatically.
MixerManager::MixerManager() : channels{}, settings(Settings::getInstance())
{
    std::cout << "MixerManger Constructor" << std::endl;
    // assign default values to each channel's members,
    // And at the same time, send the corresponding commands to the mixer.
    // When this program runs, and this constructor is run, we can have had
    // The initial bootscript run already.
    // ALSO - we might want this constructor to read a saved configuration file, from last "session"
    // So the user starts out where he left?
    for (auto &channel : channels)
    {
        // Initialize channel member variables to their default (OR saved) values
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
    // Just for laughs - print out usb devices
    settings.printUsbDevices();
}

// ############################### METHODS ####################################
const Channel &MixerManager::getChannel(uint8_t id) const
{
    if (id < CHANNEL_COUNT)
        return channels[id];
    throw std::out_of_range("Invalid channel ID");
}

// Fetch the settings object
const Settings &MixerManager::getSettings() const
{
    return settings;
}

bool MixerManager::setBrainPort(std::string deviceString)
{
    std::string deviceStringStart = "/dev/ttyUSB";
    if (deviceString.substr(0, deviceStringStart.length()) == deviceStringStart)
    {
        settings.settingSetBrainPort(deviceString);
        return true;
    }
    else
        return false;
}

bool MixerManager::setDspPort(std::string deviceString)
{
    std::string deviceStringStart = "/dev/ttyUSB";
    if (deviceString.substr(0, deviceStringStart.length()) == deviceStringStart)
    {
        settings.settingSetBrainPort(deviceString);
        return true;
    }
    else
        return false;
}

std::string MixerManager::getBrainPort()
{
    return settings.settingsGetBrainPort();
}

std::string MixerManager::getDspPort()
{
    return settings.settingsGetDspPort();
}

// Other things to be aware of in constructor:
// Which Bank to select?
// config file
//