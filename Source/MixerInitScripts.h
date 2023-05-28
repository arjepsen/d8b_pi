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

// Display intensity commands.
#define DISPLAY_LOW "30u2v"
#define DISPLAY_MID "30u1v"
#define DISPLAY_HIGH "30u0v"

// Various messages
#define WELCOME_STRING "90u44v38v42v94u36v2Ev30v20v28v61v6Cv70v68v61v29vCDu4Dv41v43v4Bv49v45vD4u44v49v47v49v54v41v4Cv"
#define BOOST_MESSAGE1 "80u42v75v63v6Bv6Cv65v20v55v70v20v44v6Fv72v6Fv74v68v79v2Cv20v27v63v61v75v73v65v20v4Bv61v6Ev73v61v73v2Ev2Ev2Ev"
#define BOOST_MESSAGE2 "C6u69v73v20v67v6Fv69v6Ev67v20v42v79v65v2Dv42v79v65v21vDCu28v62v6Fv6Fv73v74v20v6Dv6Fv64v65v29v"

// Firmware messages
#define UPLOAD_FIRMWARE_MESSAGE "80u55v70v6Cv6Fv61v64v69v6Ev67v20v46v69v72v6Dv77v61v72v65v73v2Ev2Ev2Ev"
#define UPLOAD_FIRMWARE_MESSAGE1 "80u55v70v6Cv6Fv61v64v69v6Ev67v20v42v72v61v69v6E57v61v72v65v"
#define UPLOAD_FIRMWARE_MESSAGE2 "C0u57v41v52v4Ev49V4Ev47v3Av20v 4Dv61v79v20v63v61v75v73v65v20v74v65v6Dv70v6Fv72v61v72v79v20v64v69v73v63v6Fv20v66v65v76v65v72v"

#define UPLOADING_BRAIN_MESSAGE "80u55v70v6Cv6Fv61v64v69v6Ev67v20v42v72v61v69v6Ev57v61v72v65v2Ev2Ev2EvC6u28v62v72v61v61v61v69v69v6Ev6Ev7Av7Av2Ev2Ev2Ev29v"
#define UPLOADING_DSP_FIRMWARE_MESSAGE "80u55v70v6Cv6Fv61v64v69v6Ev67v20v44v53v50v20v46v69v72v6Dv77v61v72v65v20v28v4Dv61v73v74v65v72v29v"
#define UPLOADING_DSP_SLAVE_MESSAGE "80u55v70v6Cv6Fv61v64v69v6Ev67v20v44v53v50v20v46v69v72v6Dv77v61v72v65v20v28v53v6Cv61v76v65v29v"
#define LOADING_DSP_PLUGINS "80u4Cv6Fv61v64v69v6Ev67v20v44v53v50v20v50v6Cv75v67v69v6Ev73v2Ev2Ev2Ev"
#define CONFIG_FILE_INFO "C5u28v43v6Fv6Ev66v69v67v2Ev61v73v63v20v66v69v6Cv65v29v"
#define MORE_CONFIG_MESSAGE "80u53v65v6Ev64v69v6Ev67v20v6Dv6Fv72v65v20v63v6Fv6Ev66v69v67v20v73v75v66v66v2Ev2Ev2Ev"

// Location commands for lists.
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

// List views
#define TAPE_LIST "81u54v41v50v45v86u41v8Bu54v41v50v45v90u42v95u54v41v50v45v9Au43vA0u41v4Cv54v49v4FvC4u2DvCEu2DvD8u2DvE2u2Dv"
#define DIGITAL_IO_STRING "80u44v49v47v49v54v41v4Cv20v49v2Fv4Fv20v43v41v52v44v3Av20v"
#define CLOCK_STRING "C6u43v4Cv4Fv43v4Bv20v43v41v52v44v3Av"
#define FX_CARD_LIST "80u20v46v58v43v61v72v64v88u41v20v20v46v58v43v61v72v64v20v42v20v20v46v58v43v61v72v64v9Cu43v9Fu46v58v43v61v72v64vA6u44vC4u2DvCEu2DvD8u2DvE2u2Dv"

// Response from Brain on "s" command
#define BRAIN_S_RESPONSE "000001E65288c"

// FX card display command bases.
#define MFX_DISPLAY_BASE "u4Dv46v58v"
#define UFX_DISPLAY_BASE "u55v46v58v"
#define EMPTY_DISPLAY_BASE "u65v6Dv70v74v79v"

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
void writeIOCardIdDisplayCode(int brainDescriptor, IOSlot* ioSlot, std::string locationCode);
void writeFXCardIdDisplayCode(int brainDescriptor, FXSlot* fxSlot, FXSlotID slotID);

InitErrorType initializeMixer();

void test();