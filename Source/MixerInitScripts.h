/*
  ==============================================================================

    MixerInitScripts.h
    Created: 13 May 2023 11:15:17pm
    Author:  anders

  ==============================================================================
*/

#pragma once

#include "MixerManagerClass.h"
#include <termios.h>

#define BRAINWARE_FAST_FILE "../DataFiles/controlfast.asc"


int open_serial_port(const char *device_path, speed_t baud_rate);