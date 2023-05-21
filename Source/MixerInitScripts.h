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
#include "IOSlotClass.h"

// For the final program, these files should be in a "Datafiles" folder within the folder that the program file is in.
#define BRAINWARE_FILE "../DataFiles/control.asc"
#define BRAINWARE_FAST_FILE "../DataFiles/controlfast.asc"
#define DSP_MASTER_FIRMWARE_FILE "../DataFiles/master.asc"
#define DSP_SLAVE_FIRMWARE_FILE "../DataFiles/slave.asc"
#define DSP_CONFIG_FILE "../DataFiles/Config.asc"
#define DSP_CMD1_FILE "../DataFiles/dspCmd1.asc"
#define BUF_SIZE 1024

// These commands are used to write the card ID under the specific TAPE in list.
// Locations:
#define TAPE_A_DISPLAY_LOCATION "C2u"
#define TAPE_B_DISPLAY_LOCATION "CCu"
#define TAPE_C_DISPLAY_LOCATION "D6u"
#define ALTIO_DISPLAY_LOCATION "E0u"
#define DIGICARD_DISPLAY_LOCATION "92u"
#define CLOCK_DISPLAY_LOCATION "D2u"

// I/O Card names.
#define AIO8_DISPLAY_CODE "41v49v4Fv2Dv38v"            // "AIO-8"
#define OPT8_DISPLAY_CODE "4Fv50v54v2Dv38v"            // "OPT-8"
#define AES_DISPLAY_CODE "41v45v53v2Fv45v42v55v"       // "AES/EBU"
#define MACKIE_CLOCK_DISPLAY_CODE "4Dv41v43v4Bv49v45v" // "MACKIE"
#define APOGEE_CLOCK_DISPLAY_CODE "41v50v4Fv47v45v45v" // "APOGEE"
#define EMPTY_SLOT_DISPLAY_CODE "65v6Dv70v74v79v"      // "empty"

// IO Card List view
#define TAPE_LIST "81u54v41v50v45v86u41v8Bu54v41v50v45v90u42v95u54v41v50v45v9Au43vA0u41v4Cv54v49v4FvC4u2DvCEu2DvD8u2DvE2u2Dv"

// DIGITAL I/O CARD view
#define DIGITAL_IO_STRING "80u44v49v47v49v54v41v4Cv20v49v2Fv4Fv20v43v41v52v44v3Av20v"

// CLOCK CARD view
#define CLOCK_STRING "C6u43v4Cv4Fv43v4Bv20v43v41v52v44v3Av"

// Response from Brain on "s" command
#define BRAIN_S_RESPONSE "000001E65288c"


enum IoCardSlotType
{
    TAPE_A,
    TAPE_B,
    TAPE_C,
    ALTIO,
    DIGI_IO,
    CLOCK
};

enum InitErrorType
{
    INIT_SUCCESS,
    PORT_OPEN_FAILED,
    RESET_BRAIN_TIMEOUT,
    RESET_DSP_TIMEOUT,
    UPLOAD_BRAIN_FAILED,
    UPLOAD_DSP_FAILED,
    S_RESPONSE_FAILED
};

int openSerialPort(const char *device_path, speed_t baud_rate);
void sendFirmwareFile(const char* filename, int comPortDescriptor);
std::string identifyIOCard(IoCardSlotType tapeSlot, int brainDescriptor);
std::string decodeString(const std::string &input_string);
std::string getBrainResponse(int brainDescriptor);
std::string getDspResponse(int dspDescriptor);
void writeCardIdDisplayCode(int brainDesriptor, IOSlot* ioSlot, std::string locationCode);

InitErrorType initializeMixer();

void test();