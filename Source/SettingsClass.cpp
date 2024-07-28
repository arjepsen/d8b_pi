/*
  ==============================================================================

    SettingsClass.cpp
    Created: 21 Apr 2023 7:48:13pm
    Author:  anders

  ==============================================================================
*/

#include "SettingsClass.h"

#include "Debug.h"
#include <glob.h>
#include <iostream>
#include <map>
#include <stdio.h>
#include <string>
#include <vector>

Settings::Settings() : brainPort(""), dspPort(""), brainBoostState(false)
{

    DEBUG_MSG("SETTINGS CONSTRUCTOR\n");
    findUSBDevices();
}

Settings::~Settings() {}

const std::map<std::string, std::string> &Settings::getUSBDevices() const
{
    return usbDevicesMap;
}

void Settings::findUSBDevices()
{
    glob_t glob_result;

    // First, empty the map
    usbDevicesMap.clear();

    // Run lsusb to get descriptions of all devices
    std::map<std::string, std::string> lsusb_devices;
    FILE *lsusb_output = popen("lsusb", "r");
    if (lsusb_output)
    {
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), lsusb_output) != nullptr)
        {
            std::string line = buffer;
            auto id_pos = line.find("ID ");
            if (id_pos != std::string::npos)
            {
                std::string id = line.substr(id_pos + 3, 9); // Extract the vendor:product ID
                std::string desc = line.substr(id_pos + 12); // Extract the description
                std::cout << "FOUND - ID: " << id << ", Description: " << desc << std::endl;
                lsusb_devices[id] = desc;
            }
        }
        pclose(lsusb_output);
    }

    glob("/dev/ttyUSB*", GLOB_TILDE, nullptr, &glob_result);
    for (size_t i = 0; i < glob_result.gl_pathc; ++i)
    {
        std::string port = std::string(glob_result.gl_pathv[i]);
        std::string info;

        std::string udev_command = "udevadm info -n " + port + " -q property | grep '^ID_USB_VENDOR_ID\\|^ID_USB_MODEL_ID'";
        FILE *udev_output = popen(udev_command.c_str(), "r");
        if (udev_output)
        {
            char buffer[128];
            while (fgets(buffer, sizeof(buffer), udev_output) != nullptr)
            {
                info += buffer;
            }
            pclose(udev_output);
            std::cout << "info: " << info << std::endl;
        }

        // Extract the vendor and product IDs and combine them to match the format used by lsusb
        auto vendor_pos = info.find("ID_USB_VENDOR_ID=");
        auto model_pos = info.find("ID_USB_MODEL_ID=");
        if (vendor_pos != std::string::npos && model_pos != std::string::npos)
        {
            std::string vendor_id = info.substr(vendor_pos + 17, 4);
            std::string model_id = info.substr(model_pos + 16, 4);
            std::string id = vendor_id + ":" + model_id;

            // If this ID was found in the lsusb output, use the corresponding description
            if (lsusb_devices.count(id))
            {
                usbDevicesMap[port] = lsusb_devices[id];
            }
        }
    }

    globfree(&glob_result);
}

void Settings::printUsbDevices()
{
    // The device infomation can be retreived like this : auto &settings = Settings::getInstance();
    const auto &devices = getUSBDevices();

    for (const auto &device : devices)
    {
        std::cout << "Device: " << device.first << std::endl;
        std::cout << "Info: " << device.second << std::endl;
    }
}

void Settings::setBrainPort(std::string deviceString)
{
    brainPort = deviceString;
}

void Settings::setDspPort(std::string deviceString)
{
    dspPort = deviceString;
}

const std::string Settings::getBrainPort() const
{
    return brainPort;
}

const std::string Settings::getDspPort() const
{
    return dspPort;
}

bool Settings::getBrainBoostState()
{
    return brainBoostState;
}

void Settings::setBrainBoostState(bool canWeGoToEleven)
{
    if (canWeGoToEleven)
        std::cout << "WOOOHOOOOO" << std::endl;
    else
        std::cout << "oh.. ok... turtle speed then." << std::endl;
    brainBoostState = canWeGoToEleven;
}