/*
  ==============================================================================

    SettingsClass.h
    Created: 21 Apr 2023 7:48:13pm
    Author:  anders

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <map>
#include <string>

class Settings
{
public:
    static Settings &getInstance();

    // Public method to retreive the usb device list.
    const std::map<std::string, std::string> &getUSBDevices() const;
    void printUsbDevices();
    void settingSetBrainPort(std::string deviceString);
    void settingSetDspPort(std::string deviceString);
    std::string settingsGetBrainPort();
    std::string settingsGetDspPort();
    void findUSBDevices();

private:
    Settings();
    ~Settings();

    Settings(const Settings &) = delete;
    Settings &operator=(const Settings &) = delete;

    // Settings members:
    std::string brainPort;
    std::string dspPort;

    std::map<std::string, std::string> usbDevicesMap; // Map of connectd USB devices.
    
};

// "Singleton" - ensure that only one instance of this class is created.
inline Settings &Settings::getInstance()
{
    static Settings instance;
    return instance;
}

// The device infomation can be retreived like this:
// auto& settings = Settings::getInstance();
// const auto& devices = settings.getUSBDevices();

// for (const auto& device : devices) {
//     std::cout << "Device: " << device.first << std::endl;
//     std::cout << "Info: " << device.second << std::endl;
// }
