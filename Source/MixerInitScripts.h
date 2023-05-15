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
#include "FXSlotClass.h"

// For the final program, these files should be in a "Datafiles" folder within the folder that the program file is in.
#define BRAINWARE_FILE ".Datafiles/control.asc"
#define BRAINWARE_FAST_FILE "./DataFiles/controlfast.asc"
#define DSP_MASTER_FIRMWARE_FILE "./Datafiles/"
#define DSP_SLAVE_FILE "./Datafiles/slave.asc"
#define DSP_CONFIG_FILE "./Datafiles/Config.asc"
#define BUF_SIZE 1024

enum ioCardType
{
    TAPE_A,
    TAPE_B,
    TAPE_C,
    ALTIO,
    DIGI_IO,
    CLOCK
};


int openSerialPort(const char *device_path, speed_t baud_rate);
void sendFirmwareFile(const char* filename, int comPortDescriptor);
std::string identifyIOCard(ioCardType tapeSlot, int brainDescriptor);
std::string decodeString(const std::string &input_string);
std::string getBrainResponse(int brainDescriptor);
std::string getDspResponse(int dspDescriptor);