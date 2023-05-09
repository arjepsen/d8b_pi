/*
  ==============================================================================

    SettingsClass.cpp
    Created: 21 Apr 2023 7:48:13pm
    Author:  anders

  ==============================================================================
*/

#include "SettingsClass.h"

#include <glob.h>
#include <iostream>
#include <map>
#include <stdio.h>
#include <string>
#include <vector>

Settings::Settings() : brainPort(-1), dspPort(-1)
{
    std::cout << "Settings Constructor" << std::endl;
    findUSBDevices();
    // std::vector<std::string> devices;
    // glob_t glob_result;

    // glob("/dev/ttyUSB*", GLOB_TILDE, nullptr, &glob_result);
    // for (size_t i = 0; i < glob_result.gl_pathc; ++i)
    // {
    //     devices.push_back(std::string(glob_result.gl_pathv[i]));
    // }

    // globfree(&glob_result);

    // for (const auto &device : devices)
    // {
    //     std::cout << "Device: " << device << std::endl;
    //     std::string lsusb_command = "udevadm info -q property --export -p $(udevadm info -q path -n " + device + ") | grep 'ID_MODEL_FROM_DATABASE=' | cut -d'=' -f2";
    //     FILE *lsusb_output = popen(lsusb_command.c_str(), "r");
    //     if (lsusb_output)
    //     {
    //         char buffer[128];
    //         while (fgets(buffer, sizeof(buffer), lsusb_output) != nullptr)
    //         {
    //             std::cout << "Info: " << buffer;
    //         }
    //         pclose(lsusb_output);
    //     }
    // }
}

Settings::~Settings() {}

const std::map<std::string, std::string> &Settings::getUSBDevices() const
{
    return usbDevices;
}

// This method scans the /dev/ttyUSB* ports, and puts the info in the usbDevices map.
void Settings::findUSBDevices()
{
    std::cout << "Scanning ttyUSB* for devices..." << std::endl;
    glob_t glob_result;

    glob("/dev/ttyUSB*", GLOB_TILDE, nullptr, &glob_result);
    for (size_t i = 0; i < glob_result.gl_pathc; ++i)
    {
        std::string port = std::string(glob_result.gl_pathv[i]);
        std::string info;

        std::string lsusb_command = "udevadm info -q property --export -p $(udevadm info -q path -n " + port + ") | grep 'ID_MODEL_FROM_DATABASE=' | cut -d'=' -f2";
        FILE *lsusb_output = popen(lsusb_command.c_str(), "r");
        if (lsusb_output)
        {
            char buffer[128];
            while (fgets(buffer, sizeof(buffer), lsusb_output) != nullptr)
            {
                info += buffer;
            }
            pclose(lsusb_output);
        }
        usbDevices[port] = info;
    }

    globfree(&glob_result);
}

void Settings::printUsbDevices()
{
    //The device infomation can be retreived like this : auto &settings = Settings::getInstance();
    const auto &devices = getUSBDevices();

    for (const auto &device : devices)
    {
        std::cout << "Device: " << device.first << std::endl;
        std::cout << "Info: " << device.second << std::endl;
    }
}