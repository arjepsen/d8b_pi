/*
  ==============================================================================

    SettingsClass.cpp
    Created: 21 Apr 2023 7:48:13pm
    Author:  anders

  ==============================================================================
*/

#include "SettingsClass.h"

Settings::Settings() : brainPort(-1), dspPort(-1) {}
Settings::~Settings() {}

Settings& Settings::getInstance()
{
  static Settings instance;
  return instance;
}