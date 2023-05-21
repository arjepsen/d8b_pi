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
MixerManager::MixerManager() : channels{}, settings(Settings::getInstance()), isInitializing(false)
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

MixerManager::~MixerManager()
{
    delete fxSlotA;
}

// ############################### METHODS ####################################
const Channel &MixerManager::getChannel(uint8_t id) const
{
    if (id < CHANNEL_COUNT)
        return channels[id];
    throw std::out_of_range("Invalid channel ID");
}

// Fetch the settings object
// const Settings &MixerManager::getSettings() const
// {
//     return settings;
// }

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
        settings.settingSetDspPort(deviceString);
        return true;
    }
    else
        return false;
}

const std::string MixerManager::getBrainPort() const
{
    return settings.settingsGetBrainPort();
}

const std::string MixerManager::getDspPort() const
{
    return settings.settingsGetDspPort();
}

const std::map<std::string, std::string> MixerManager::getUsbPortMap()
{
    // Tell settings to update the device map.
    settings.findUSBDevices();

    // Get the map of devices and return it.
    const std::map<std::string, std::string> usbDeviceMap = settings.getUSBDevices();
    return usbDeviceMap;
}

bool MixerManager::getBrainBoostState()
{
    return settings.settingsGetBrainBoostState();
}

void MixerManager::setBrainBoostState(bool doWeWantTurbo)
{
    settings.settingsSetBrainBoostState(doWeWantTurbo);
}

void MixerManager::initIOSlot(IOSlot *ioSlotPtr, IOSlotID ioSlotID)
{
    switch (ioSlotID)
    {
        case TAPEIO_SLOT_A:
            ioSlotA = ioSlotPtr;
            break;
        case TAPEIO_SLOT_B:
            ioSlotB = ioSlotPtr;
            break;
        case TAPEIO_SLOT_C:
            ioSlotC = ioSlotPtr;
            break;
        case TAPEIO_SLOT_ALTIO:
            ioSlotAlt = ioSlotPtr;
            break;
        case CLOCK_IO_SLOT:
            ioSlotClock = ioSlotPtr;
            break;
        case DIGITAL_IO_SLOT:
            ioSlotDigital = ioSlotPtr;
            break;

        default:
            printf("INVALID IO SLOT INIT");
            exit(1);
    }
}


void MixerManager::initFXSlot(FXSlot *fxSlotPtr, FXSlotID fxSlotID)
{
    switch (fxSlotID)
    {
        case FX_SLOT_A:
            fxSlotA = fxSlotPtr;
            break;
        case FX_SLOT_B:
            fxSlotB = fxSlotPtr;
            break;
        case FX_SLOT_C:
            fxSlotC = fxSlotPtr;
            break;
        case FX_SLOT_D:
            fxSlotD = fxSlotPtr;
            break;
        default:
            printf("INVALID SLOT INIT");
            exit(1);
    }
}

void MixerManager::initMixer(juce::Button *initMixerBtn)
{
    std::cout << "Calling initializemixer from mixermanager" << std::endl;

    if (isInitializing)
        printf("INITIALIZATION SCRIPT IS ALREADY RUNNING!!!\n");
    else
    {
        isInitializing = true;
        juce::Thread::launch([this, initMixerBtn]()
        {
            // Perform Mixer initialization.
            InitErrorType initResult = initializeMixer();
            if (initResult != INIT_SUCCESS)
            {
                juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon,
                                        "Mixer Initialization Error code",
                                        juce::String(initResult),
                                        "OK");
            }

            //test();
            isInitializing = false;

            juce::MessageManager::callAsync([initMixerBtn]()
            {
                initMixerBtn->setEnabled(true); // Re-enable the button
            }); 
        });
    }

    // Check if we're already initializing.
}

// Other things to be aware of in constructor:
// Which Bank to select?
// config file
//