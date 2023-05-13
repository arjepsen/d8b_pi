/*
  ==============================================================================

    MixerInitScripts.h
    Created: 13 May 2023 11:15:17pm
    Author:  anders

  ==============================================================================
*/

#pragma once

#include "MixerManagerClass.h"

int open_serial_port(const char *device_path, speed_t baud_rate);