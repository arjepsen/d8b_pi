/*
  ==============================================================================

    SettingsClass.h
    Created: 21 Apr 2023 7:48:13pm
    Author:  anders

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Settings
{
  public:
    static Settings& getInstance();

    // Store USB port settings:
    int brainPort;
    int dspPort;

  private:
  Settings();
  ~Settings();

  Settings(const Settings&) = delete;
  Settings& operator=(const Settings&) = delete;
};