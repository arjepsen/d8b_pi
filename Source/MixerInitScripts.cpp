/*
  ==============================================================================

    MixerInitScripts.cpp
    Created: 13 May 2023 11:15:17pm
    Author:  anders

  ==============================================================================
*/

#include "MixerInitScripts.h"
#include <fcntl.h>
#include <unistd.h>
#include "Debug.h"


// Trying a new initialization script
InitErrorType initializeMixerNew()
{
    const int MAX_RETRIES = 5;

    // ===========================================================================
    // ================== 1. SET UP COM PORTS ====================================
    // ===========================================================================
    MixerManager &mixerManager = MixerManager::getInstance();   // Fetch singleton.

    // Set up file descriptors. Brain not const, since we can boost it
    int BRAIN = openSerialPort(mixerManager.getBrainPort().c_str(), B115200);
    const int DSP = openSerialPort(mixerManager.getDspPort().c_str(), B115200);

    // Check for error.
    if (BRAIN == -1 || DSP == -1)
    {
        DEBUG_MSG("PORT INITIALIZATION FAILED!\n");
        return PORT_OPEN_FAILED;
    }

    // Flush both for good measure.
    tcflush(BRAIN, TCIOFLUSH);
    tcflush(DSP, TCIOFLUSH);

    DEBUG_MSG("Ports initialized.\n");
    usleep(50000); // Sleep.


    // ===========================================================================
    // ====================== 2. INITIALIZE COMMUNICATION ========================
    // ===========================================================================
    char replyChar = '\0'; // Initial null terminator.
    int retries = 0;

    // Send 'R' to both boards, and check reply.
    write(BRAIN, "R", 1);
    while (replyChar != 'R')
    {
        if (read(BRAIN, &replyChar, 1) == 1 && replyChar == 'R')
        {
            DEBUG_MSG("Brain connected.\n");
            break;
        }

        retries++;
        usleep(200000); // Sleep 200ms
        if (retries >= MAX_RETRIES)
            return RESET_BRAIN_TIMEOUT; // Maybe something wrong with brain connection
    }

    // Reset for DSP round.
    replyChar = '\0'; 
    retries = 0;
    write(DSP, "R", 1);
    while (replyChar != 'R')
    {
        if (read(DSP, &replyChar, 1) == 1 && replyChar == 'R')
        {
            DEBUG_MSG("DSP connected.\n");
            break;
        }
        retries++;
        usleep(200000);
        if (retries >= MAX_RETRIES)
            return RESET_DSP_TIMEOUT; // Maybe something wrong with DSP connection
    }


    // ===========================================================================
    // ====================== 3. DISPLAY WELCOME MESSAGE =========================
    // ===========================================================================
    write(BRAIN, DISPLAY_LOW, strlen(DISPLAY_LOW)); // Display intensity = low (preserve display)

    // Send "01u" which clears the display.
    write(BRAIN, "01u", 3);
    usleep(20000); // Sleep for 20ms - needed for clear display to finish.

    // Send "0Cu" - unsure what this does... not even sure it's needed... but it is sent in the original.
    write(BRAIN, "0Cu", 3);
    usleep(20000); // Sleep for 20ms

    // Display the welcome message.
    write(BRAIN, WELCOME_STRING, strlen(WELCOME_STRING));
    sleep(2); // Let it stay there a fews secs for admirations sake.


    // ===========================================================================
    // ====================== 4. SEND DSP FIRMWARE (Master) ======================
    // ===========================================================================
    // Clear Screen
    write(BRAIN, "01u", 3);
    usleep(20000);

    // Display DSP firmware upload message
    write(BRAIN, UPLOADING_DSP_FIRMWARE_MESSAGE, strlen(UPLOADING_DSP_FIRMWARE_MESSAGE));

    // Send the DSP firmware
    sendFirmwareFile(DSP_MASTER_FIRMWARE_FILE, DSP);

    // DSP Should respond with an "R" to the master firmware.
    retries = 0;

    while (true)
    {
        std::string dspResponse = getDspResponse(DSP);
        DEBUG_MSG("\n\nDSP RESPONSE IS: %s\n\n", dspResponse.c_str());

        if (dspResponse == "R")
        {
            break;
        }
        else
        {
            retries++;
            DEBUG_MSG("Number of retries for DSP 'R' are now: %d\n", retries);
        }

        if (retries > MAX_RETRIES)
        {
            DEBUG_MSG("NOT R RESPONSE\n");
            return UPLOAD_DSP_FAILED;
        }
    }


    // ===========================================================================
    // ========================= 5. SEND BRAIN FIRMWARE ==========================
    // ===========================================================================
    // Clear Screen
    write(BRAIN, "01u", 3);
    usleep(20000);

    // Upload firmware (check for boost)
    if (mixerManager.getBrainBoostState())
    {
        // Display fast firmware message
        write(BRAIN, BOOST_MESSAGE1, strlen(BOOST_MESSAGE1));
        sleep(1);
        write(BRAIN, BOOST_MESSAGE2, strlen(BOOST_MESSAGE2));

        // Upload fast firmware.
        sendFirmwareFile(BRAINWARE_FAST_FILE, BRAIN);

        // Close the old slow filedescriptor of yestoreyore:
        close(BRAIN);

        // Initialize new connection speed:
        BRAIN = openSerialPort(mixerManager.getBrainPort().c_str(), B230400);
    }
    else
    {
        // Guess we will stay with old slow speed then.
        write(BRAIN, UPLOAD_FIRMWARE_MESSAGE1, strlen(UPLOAD_FIRMWARE_MESSAGE1));
        write(BRAIN, UPLOAD_FIRMWARE_MESSAGE2, strlen(UPLOAD_FIRMWARE_MESSAGE2));
        sendFirmwareFile(BRAINWARE_FILE, BRAIN);
    }

    // Now check for a response from the brain. 
    // This will be an R, followed a report of some fader and vpot positions.
    // After this, the heartbeat "l-k-l-k" will start being sent by the brain.
    // TODO: does this impact the calibration string below?

    // Check that we are getting the right response.
    retries = 0;
    while (true)
    {
        std::string brainResponse = getBrainResponse(BRAIN);
        if (brainResponse.substr(0, 3) == "R00")
        {
            DEBUG_MSG("Brain reply to firmware: %s\n", brainResponse.c_str());
            break;
        }

        retries++;
        if (retries > MAX_RETRIES)
        {
            DEBUG_MSG("Brain seems unresponsive to receiving firmware");
            return UPLOAD_BRAIN_FAILED;
        }       
    }


    // ===========================================================================
    // ======================= 6. SEND FADER CALIBRATIONS ========================
    // ===========================================================================

    // Faders gets calibrated by sending commands to the brain, and reacting on 
    // the responses. The commands has a format like this: 163AF840L16K, with this
    // being a command for fader 16 (hex value 0x16 = 22 = channel strip 23) 
    // So, in that command, the value is "3AF840". During boot, we will send a 
    // value where we add 0x100 to that value, so on boot we would send the 
    // command: "163AF940L"

    // TODO:
    // What we need to do here is to implement calibration, and let it save to 
    // a file. Then we need to open that here, and use the saved calibration
    // command.
    // For now, just use this:
    std::string calibString = "0034F640L013AF780L0238F6C0L0336F600L0432F480L053AF800L0636F640L0738F740L0832F6C0L093AF8C0L0A38F840L0B3AF880L0C3AF900L0D36F800L0E3EFA80L0F38F840L103AF900L1136F800L1236F800L1336F840L1438F940L1536F840L163AF940L1738F8C0L183AFA00L";
    write(BRAIN, calibString.c_str(), calibString.length());


    // ===========================================================================
    // ========================== 7. SET UP I/O CARDS ============================
    // ===========================================================================
    // Detection of I/O cards are done by sending queries to the brain.
    // First, clear display, then display:
    // TAPE A   TAPE B   TAPE C   ALTIO
    //   -        -        -        -

    write(BRAIN, "01u", 3);
    usleep(20000);
    write(BRAIN, TAPE_LIST, strlen(TAPE_LIST));

    // Create the IO card slot object pointers. (Constructor initializes card).
    // Then use function to query and display ID code.
    IOSlot *tapeA = new IOSlot(BRAIN, TAPEIO_SLOT_A);
    writeIOCardIdDisplayCode(BRAIN, tapeA, TAPE_A_DISPLAY_LOCATION);

    IOSlot *tapeB = new IOSlot(BRAIN, TAPEIO_SLOT_B);
    writeIOCardIdDisplayCode(BRAIN, tapeB, TAPE_B_DISPLAY_LOCATION);

    IOSlot *tapeC = new IOSlot(BRAIN, TAPEIO_SLOT_C);
    writeIOCardIdDisplayCode(BRAIN, tapeC, TAPE_C_DISPLAY_LOCATION);

    IOSlot *altIO = new IOSlot(BRAIN, TAPEIO_SLOT_ALTIO);
    writeIOCardIdDisplayCode(BRAIN, altIO, ALTIO_DISPLAY_LOCATION);

    // Admire the beauty of it.
    sleep(1);


    // ===========================================================================
    // ===================== 8. SET UP DIGITAL I/O & CLOCK =======================
    // ===========================================================================
    // Clear screen
    write(BRAIN, "01u", 3);
    usleep(20000);

    // Display "Digital IO card" and "Clock card:"
    write(BRAIN, DIGITAL_IO_STRING, strlen(DIGITAL_IO_STRING));
    write(BRAIN, CLOCK_STRING, strlen(CLOCK_STRING));

    // Clear the buffer of "klk's" otherwise card detect fails.
    tcflush(BRAIN, TCIOFLUSH);

    // Identify Digital card and Clock.
    IOSlot *digiIO = new IOSlot(BRAIN, DIGITAL_IO_SLOT);
    writeIOCardIdDisplayCode(BRAIN, digiIO, DIGICARD_DISPLAY_LOCATION);

    IOSlot *clockIO = new IOSlot(BRAIN, CLOCK_IO_SLOT);
    writeIOCardIdDisplayCode(BRAIN, clockIO, CLOCK_DISPLAY_LOCATION);

    // More admiration.
    sleep(1);


    // ===========================================================================
    // ==================== 9. QUERY MR. BRAIN'S ESN NUMBER ======================
    // ===========================================================================
    tcflush(BRAIN, TCIOFLUSH); // First clear buffer, otherwise no worky
    write(BRAIN, "s", 1);

    // Then check response.
    std::string brainResponse = getBrainResponse(BRAIN);
    DEBUG_MSG("My ESN: %s\n", brainResponse.c_str());
   
    // TODO: Implement some error checking here.

    usleep(20000);


    // ===========================================================================
    // ==================== 10. UPLOAD DSP FIRMWARE (slave) ======================
    // ===========================================================================
    // Clear Screen
    write(BRAIN, "01u", 3);
    usleep(20000);

    write(BRAIN, UPLOADING_DSP_SLAVE_MESSAGE, strlen(UPLOADING_DSP_SLAVE_MESSAGE));
    sendFirmwareFile(DSP_SLAVE_FIRMWARE_FILE, DSP);

    sleep(1);



    // ===========================================================================
    // ==================== 11. UPLOAD Config.asc file twice =====================
    // ===========================================================================
    DEBUG_MSG("Uploading 'Config.asc' twice\n");

    // For some reason, the config file is sent twice... not sure why.
    sendFirmwareFile(DSP_CONFIG_FILE, DSP);
    sendFirmwareFile(DSP_CONFIG_FILE, DSP);

    sleep(1);


    // ===========================================================================
    // ======================== 12. INITIALIZE FX CARDS ==========================
    // ===========================================================================
    // Clear Screen
    DEBUG_MSG("Initializing FX cards\n");

    // Clear Screen
    write(BRAIN, "01u", 3);
    usleep(20000);

    // Display the FX card list:
    write(BRAIN, FX_CARD_LIST, strlen(FX_CARD_LIST));
    tcflush(BRAIN, TCIOFLUSH);

    // Create the FX card slot object pointers.
    FXSlot *fxSlotA = new FXSlot(BRAIN, FX_SLOT_A);
    writeFXCardIdDisplayCode(BRAIN, fxSlotA, FX_SLOT_A);

    FXSlot *fxSlotB = new FXSlot(BRAIN, FX_SLOT_B);
    writeFXCardIdDisplayCode(BRAIN, fxSlotB, FX_SLOT_B);

    FXSlot *fxSlotC = new FXSlot(BRAIN, FX_SLOT_C);
    writeFXCardIdDisplayCode(BRAIN, fxSlotC, FX_SLOT_C);

    FXSlot *fxSlotD = new FXSlot(BRAIN, FX_SLOT_D);
    writeFXCardIdDisplayCode(BRAIN, fxSlotD, FX_SLOT_D);

    // Pass the pointers to the mixermanager
    mixerManager.initFXSlot(fxSlotA, FX_SLOT_A);
    mixerManager.initFXSlot(fxSlotB, FX_SLOT_B);
    mixerManager.initFXSlot(fxSlotC, FX_SLOT_C);
    mixerManager.initFXSlot(fxSlotD, FX_SLOT_D);

    sleep(1);


    // ===========================================================================
    // ========================= 13. SEND 7X2$.. CODES ===========================
    // ===========================================================================
    // Not sure what these commands do exactly, but it seems to be sent for all
    // channel strips. The command is similar to the one that turns filter on/off
    // for a channel, but has a different number after X.
    DEBUG_MSG("Sending further configs '7x2$...'\n");

    // Clear Screen
    write(BRAIN, "01u", 3);
    usleep(20000);

    // Display more config stuff message
    write(BRAIN, MORE_CONFIG_MESSAGE, strlen(MORE_CONFIG_MESSAGE));

    // Send file. Slight difference in the end, if Apogee clock card is used.
    if (clockIO->getCardID() != APOGEE_CLOCK_ID_STRING)
    {
        sendFirmwareFile(DSP_CMD1_FILE, DSP);
    }
    else
    {
        sendFirmwareFile(DSP_CMD1_APOGEE_FILE, DSP);
    }


    // #######################################################################
    // THERE SEEMS TO BE A WAIT OF ABOUT 10 SECONDS AT THIS POINT OF THE BOOT
    // PROCESS - NOTHING HAPPENS ON EITHER COM, EXCEPT FOR THE HEARTBEAT.
    // #######################################################################
    // TODO: Try lowering, or removing....
    sleep(10);


    // ===========================================================================
    // ===================== 14. LOAD SAVED STARTUP SETTINGS =====================
    // ===========================================================================

    // At this point there are sent stuff simultaneously on both coms.
    // The DSP part is sending commands for setting the BUS volume level to unity,
    // and setting up the parameters of the Compressor and Gate for each channel.
    // At the same time, the Brain gets commands for turning on various LED's 
    // and displaying the initial EQ text in the display.
    // These settings should be loaded from saved file.
    // TODO: Implement file saving/loading of settings here.

    // Next, all faders are sent to 0, and the green LINE bank led is lit.
    // Again, this might be a saved thing....
    // Interesting thing is that here we start seeing a ton of 
    //                  "66e"
    // Still not quite sure what it does exactly - seems maybe turning off
    // digital output?

    // Next part of the saved settings are the settings for the phones etc.
    // Again - here we go through all saved settings...
    // BUT THIS IS FOR THE STARTUP FILE... but again - we could have changd
    // the startup file.... 

    // NExt, again as part of saved stuff, we update the channel number 
    // 7-seg indicator, and the 7segs in the transport section.

    // BUT then something very interesting happens.


    // ===========================================================================
    // ============ 15. SEND WEIRD CODE TO BRAIN - CHECK DSP REPLY ===============
    // ===========================================================================
    // So now this string is sent TO THE BRAIN!. The interesting thing is, that 
    // this has an impact on the DSP board, that until now has had a blinking
    // green led. So sending this string to the Brain board seems to sort of
    // "finish up" the DSP boards boot process, upon which it will respond
    // something similar to "3515d".


    std::string dspFinish1 = "0o8o0o8o0o8o0o8o0o8o0o8o0o8o0o8o10o0o8o0o8o0o8o0o8o0o8o0o8o0o8o20o28o10o";
    write(BRAIN, dspFinish1.c_str(), dspFinish1.length());

    // Check DSP response
    retries = 0;
    while (true)
    {
        std::string dspResponse = getDspResponse(DSP);
        DEBUG_MSG("\n\nDSP RESPONSE IS: %s\n\n", dspResponse.c_str());
        if (dspResponse.substr(0, 2) == "35" && dspResponse.substr(4, 1) == "d")
        {
            // Break out on a "35xxd" response.
            break;
        }
        else
        {
            retries++;
            DEBUG_MSG("Number of retries are now: %d\n", retries);
        }

        if (retries > MAX_RETRIES)
        {
            DEBUG_MSG("NOT R35xxd RESPONSE\n");
            return DSP_R35xxd_RESPONSE_FAILED;
        }
    }

    printf("\n\n==================================================\n");
    printf("Got the 35xxd - now checking if more stuf has arrived\n\n");

    while (true)
    {
        std::string dspResponse = getDspResponse(DSP);
        DEBUG_MSG("\n\nDSP RESPONSE IS: %s\n\n", dspResponse.c_str());

        if (dspResponse != "8000d")
        {
            printf("\n\n\n\n NOOOOOOOOT 8000d.... \n\n\n");
            sleep(1);
        }
        else
        {
            break;
        }
    }


    // ===========================================================================
    // ================= 16. SEND MORE WEIRD CODE TO BRAIN =======================
    // ===========================================================================
    // Some of this - the 4p-stuff are some configuration bits.... so this is still
    // part of saved settings. But we still dont quite know what the 0o8o does....

    std::string moreWeird = "4pCp24p2Cp4pCp24p2Cp4pCp4pCp4pCp4pCp4pCp4pCp4pCp4pCp4pCp4pCp24p2Cp4pCp14p0o8o0o8o0o8o0o8o0o8o0o8o0o8o0o8o10o0o8o0o8o0o8o0o8o0o8o0o8o0o8o20o28o10o";
    write(BRAIN, moreWeird.c_str(), moreWeird.length());



    // After this, more "66e"'s are sent, and then, if we have an Apogee, the
    // apogee heartbeat: "C0o - C8o" starts being sent back and forth between console
    // and brain.


    // Now, set up 


    // ===========================================================================
    // ================= 17. FINISH CONSOLE BOOTING =======================
    // ===========================================================================
    close(BRAIN);
    close(DSP);

    return INIT_SUCCESS;
}




///////////////////// OLD /////////////////////////////////////////

InitErrorType initializeMixer()
{
    const int MAX_RETRIES = 5; // Used for defining how many retries various attempts will have.

    // ================== SET UP COM PORTS =============================

    // Get access to the mixerManager singleton:
    MixerManager &mixerManager = MixerManager::getInstance();

    // Fetch the com ports:
    // const char *BRAIN_DEVICE = mixerManager.getBrainPort().c_str();
    // const char *DSP_DEVICE = mixerManager.getDspPort().c_str();


    // Make a file descriptor for the 115200 baud rate brain connection.
    //int BRAIN = openSerialPort(BRAIN_DEVICE, B115200); // Not const, since it can change to boost later.
    int BRAIN = openSerialPort(mixerManager.getBrainPort().c_str(), B115200);
    const int DSP = openSerialPort(mixerManager.getDspPort().c_str(), B115200);

    if (BRAIN == -1 || DSP == -1)
    {
        printf("PORT INITIALIZATION FAILED!\n");
        return PORT_OPEN_FAILED;
    }

    DEBUG_MSG("Ports initialized.\n");
    usleep(100000); // Sleep for 100ms

    // ================= INITIALIZE COMMUNICATION ========================
    // Sending an "R" resets the processor, which will then return an "R".

    char replyChar = '\0'; // Initial null terminator.
    int retries = 0;

    // Reset Brain:
    write(BRAIN, "R", 1);

    // Check Brain reply
    while (replyChar != 'R')
    {
        if (read(BRAIN, &replyChar, 1) == 1 && replyChar == 'R')
        {
            DEBUG_MSG("Brain connected.\n");
        }

        retries++;
        usleep(200000); // Sleep 200ms
        if (retries >= MAX_RETRIES)
            return RESET_BRAIN_TIMEOUT; // Maybe something wrong with brain connection
    }

    // Same for DSP
    write(DSP, "R", 1);
    replyChar = '\0'; // Reset for DSP round.
    retries = 0;
    while (replyChar != 'R')
    {
        retries++;
        if (read(DSP, &replyChar, 1) == 1 && replyChar == 'R')
            DEBUG_MSG("DSP connected.\n");
        usleep(200000);
        if (retries >= MAX_RETRIES)
            return RESET_DSP_TIMEOUT; // Maybe something wrong with DSP connection
    }

    // ================== DISPLAY WELCOME MESSAGE ============================

    write(BRAIN, DISPLAY_LOW, strlen(DISPLAY_LOW)); // Display intensity = low (preserve display)

    // Send "01u" which clears the display.
    write(BRAIN, "01u", 3);
    usleep(20000); // Sleep for 20ms - needed for clear display to finish.

    // Send "0Cu" - unsure what this does... not even sure it's needed... but it is sent in the original.
    write(BRAIN, "0Cu", 3);
    usleep(20000); // Sleep for 20ms

    // Display the welcome message.
    write(BRAIN, WELCOME_STRING, strlen(WELCOME_STRING));
    sleep(2); // Let it stay there a fews secs for admirations sake.

    // ====================== SEND BRAIN FIRMWARE =======================================

    // Clear Screen
    write(BRAIN, "01u", 3);
    usleep(20000);

    // Upload firmware (check for boost)
    if (mixerManager.getBrainBoostState())
    {
        // Display fast firmware message
        write(BRAIN, BOOST_MESSAGE1, strlen(BOOST_MESSAGE1));
        sleep(1);
        write(BRAIN, BOOST_MESSAGE2, strlen(BOOST_MESSAGE2));

        // Upload fast firmware.
        sendFirmwareFile(BRAINWARE_FAST_FILE, BRAIN);

        // Close the old slow filedescriptor of yestoreyore:
        close(BRAIN);

        // Initialize new connection speed:
        BRAIN = openSerialPort(mixerManager.getBrainPort().c_str(), B230400);
    }
    else
    {
        // Guess we will stay with old slow speed then.
        write(BRAIN, UPLOAD_FIRMWARE_MESSAGE1, strlen(UPLOAD_FIRMWARE_MESSAGE1));
        write(BRAIN, UPLOAD_FIRMWARE_MESSAGE2, strlen(UPLOAD_FIRMWARE_MESSAGE2));
        sendFirmwareFile(BRAINWARE_FILE, BRAIN);
    }

    DEBUG_MSG("sending brain \n");

    // Check response from Brain it should look something like:
    // R0027v0129v0202v0326v042Fv053Dv0623v073Ev083Cv0918v0A3Fv0B3Cv0C3Ev0D0Cv0E1Av0F2Cv1024v1127v1205v131Fv1419v1524v163Bv1705v1804v1A31v1B2Av1C39v1D37v1E2Dv1F03v2014v
    // So the first R might be a reset, or something?
    // But the rest follows the vpot messages, so it is likely a vpot report for all the desks vpots.
    // Could be a position. Seems if faders are up at boot, they will also be reported.

    retries = 0;
    while (true)
    {
        std::string brainResponse = getBrainResponse(BRAIN);
        if (brainResponse.substr(0, 3) == "R00")
        {
            DEBUG_MSG("Brain reply to firmware: %s\n", brainResponse.c_str());
            break;
        }

        retries++;
        if (retries > MAX_RETRIES)
        {
            DEBUG_MSG("Brain seems unresponsive to receiving firmware");
            return UPLOAD_BRAIN_FAILED;
        }
            
    }

    // ====================== I/O CARD LIST ====================================
    // This section detects and prints out a list of I/O cards.

    DEBUG_MSG("\nPrinting Card List\n");

    // Clear screen
    write(BRAIN, "01u", 3);
    usleep(20000);

    // // Print the tape list in the display
    write(BRAIN, TAPE_LIST, strlen(TAPE_LIST));

    // Create the IO card slot object pointers. (Constructor initializes card).
    // Then use function to display ID code.
    IOSlot *tapeA = new IOSlot(BRAIN, TAPEIO_SLOT_A);
    writeIOCardIdDisplayCode(BRAIN, tapeA, TAPE_A_DISPLAY_LOCATION);

    IOSlot *tapeB = new IOSlot(BRAIN, TAPEIO_SLOT_B);
    writeIOCardIdDisplayCode(BRAIN, tapeB, TAPE_B_DISPLAY_LOCATION);

    IOSlot *tapeC = new IOSlot(BRAIN, TAPEIO_SLOT_C);
    writeIOCardIdDisplayCode(BRAIN, tapeC, TAPE_C_DISPLAY_LOCATION);

    IOSlot *altIO = new IOSlot(BRAIN, TAPEIO_SLOT_ALTIO);
    writeIOCardIdDisplayCode(BRAIN, altIO, ALTIO_DISPLAY_LOCATION);

    // Admire the beauty of it.
    sleep(1);

    // =================== LIST DIGITAL IO & CLOCK ================================

    DEBUG_MSG("Displaying Digital IO & Clock\n");

    // Clear screen
    write(BRAIN, "01u", 3);
    usleep(20000);

    // Display "Digital IO card" and "Clock card:"
    write(BRAIN, DIGITAL_IO_STRING, strlen(DIGITAL_IO_STRING));
    write(BRAIN, CLOCK_STRING, strlen(CLOCK_STRING));

    // Clear the buffer of "klk's" otherwise card detect fails.
    tcflush(BRAIN, TCIOFLUSH);

    // Identify Digital card and Clock.
    IOSlot *digiIO = new IOSlot(BRAIN, DIGITAL_IO_SLOT);
    writeIOCardIdDisplayCode(BRAIN, digiIO, DIGICARD_DISPLAY_LOCATION);

    IOSlot *clockIO = new IOSlot(BRAIN, CLOCK_IO_SLOT);
    writeIOCardIdDisplayCode(BRAIN, clockIO, CLOCK_DISPLAY_LOCATION);

    // More admiration.
    sleep(1);

    // ================== "S" ================================

    // Next, send an "s".... This queries the ESN number of the brain.
    tcflush(BRAIN, TCIOFLUSH); // First clear buffer.
    write(BRAIN, "s", 1);

    // Then check response.
    retries = 0;
    while (true)
    {
        std::string brainResponse = getBrainResponse(BRAIN);
        
        // if (brainResponse == BRAIN_S_RESPONSE)
        // {
        //     break;
        // }
        // retries++;
        // if (retries > MAX_RETRIES)
        // {
        //     return S_RESPONSE_FAILED;
        // }
            
    }

    usleep(20000);

    // ============== UPLOAD DSP FIRMWARE =====================

    DEBUG_MSG("Uploading DSP firmware.\n");

    // Clear Screen
    write(BRAIN, "01u", 3);
    usleep(20000);

    // Display DSP firmware upload message
    write(BRAIN, UPLOADING_DSP_FIRMWARE_MESSAGE, strlen(UPLOADING_DSP_FIRMWARE_MESSAGE));

    // Send the DSP firmware
    sendFirmwareFile(DSP_MASTER_FIRMWARE_FILE, DSP);

    // Here the DSP replies "R350D" - check it. Sometimes its 351Dd
    // Actually, it seems the R is sent during the firmware upload, and the remaining 351Dd can
    // be sent much later, after the "7X2$...."
    DEBUG_MSG("Checking DSP reply to firmware upload\n");
    retries = 0;
    while (true)
    {
        std::string dspResponse = getDspResponse(DSP);
        DEBUG_MSG("\n\nDSP RESPONSE IS: %s\n\n", dspResponse.c_str());
        if (dspResponse.substr(0, 3) == "R35")
        {
            // Maybe this is only for Mackie clock card?
            DEBUG_MSG("DSP Replied: %s\n", dspResponse.c_str());
            break;
        }
        else
        {
            retries++;
            DEBUG_MSG("Number of retries are now: %d\n", retries);
        }

        if (retries > MAX_RETRIES)
        {
            DEBUG_MSG("NOT R35 RESPONSE\n");
            //return UPLOAD_DSP_FAILED;
        }
        // if (clockIO->getCardID() == APOGEE_CLOCK_ID_STRING)
        // {
        //     printf("Clock card ID'ed as an APOGEE card. Send something extra before slave firmware\n");
        //     write(DSP, APOGEE_FW_THING, strlen(APOGEE_FW_THING));
        //     dspResponse = getDspResponse(DSP);
        //     printf("\n\nDSP APOGEE RESPONSE IS: %s\n\n", dspResponse.c_str());
        // }

        break;
        // return UPLOAD_DSP_FAILED;
    }

    // Something extra is sent to the dsp board, if we are using an apogee card.
    // TODO: Should this be within the above loop or not???
    if (clockIO->getCardID() == APOGEE_CLOCK_ID_STRING)
    {
        printf("Clock card ID'ed as an APOGEE card. Send something extra before slave firmware\n");
        write(DSP, APOGEE_FW_THING, strlen(APOGEE_FW_THING));
        std::string dspResponse = getDspResponse(DSP);
        printf("\n\nDSP APOGEE RESPONSE IS: %s\n\n", dspResponse.c_str());
    }

    // ===================== SEND SLAVE PART OF DSP FIRMWARE ==================

    DEBUG_MSG("Uploading 'slave' dsp firmware\n");

    // Clear Screen
    write(BRAIN, "01u", 3);
    usleep(20000);

    write(BRAIN, UPLOADING_DSP_SLAVE_MESSAGE, strlen(UPLOADING_DSP_SLAVE_MESSAGE));
    sendFirmwareFile(DSP_SLAVE_FIRMWARE_FILE, DSP);

    sleep(1);

    // =================== DSP PLUGINS (Config.asc file) ===========================

    DEBUG_MSG("Uploading 'Config.asc' twice\n");

    // Clear Screen
    write(BRAIN, "01u", 3);
    usleep(20000);

    // Display "Loading DSP plugins..."
    write(BRAIN, LOADING_DSP_PLUGINS, strlen(LOADING_DSP_PLUGINS));
    write(BRAIN, CONFIG_FILE_INFO, strlen(CONFIG_FILE_INFO));

    // For some reason, the config file is sent twice... not sure why.
    sendFirmwareFile(DSP_CONFIG_FILE, DSP);
    sendFirmwareFile(DSP_CONFIG_FILE, DSP);

    sleep(1);

    // =========================== INIT FX CARDS =============================

    DEBUG_MSG("Initializing FX cards\n");

    // Clear Screen
    write(BRAIN, "01u", 3);
    usleep(20000);

    // Display the FX card list:
    write(BRAIN, FX_CARD_LIST, strlen(FX_CARD_LIST));
    tcflush(BRAIN, TCIOFLUSH);

    // Create the FX card slot object pointers.
    FXSlot *fxSlotA = new FXSlot(BRAIN, FX_SLOT_A);
    writeFXCardIdDisplayCode(BRAIN, fxSlotA, FX_SLOT_A);

    FXSlot *fxSlotB = new FXSlot(BRAIN, FX_SLOT_B);
    writeFXCardIdDisplayCode(BRAIN, fxSlotB, FX_SLOT_B);

    FXSlot *fxSlotC = new FXSlot(BRAIN, FX_SLOT_C);
    writeFXCardIdDisplayCode(BRAIN, fxSlotC, FX_SLOT_C);

    FXSlot *fxSlotD = new FXSlot(BRAIN, FX_SLOT_D);
    writeFXCardIdDisplayCode(BRAIN, fxSlotD, FX_SLOT_D);

    // Pass the pointers to the mixermanager
    mixerManager.initFXSlot(fxSlotA, FX_SLOT_A);
    mixerManager.initFXSlot(fxSlotB, FX_SLOT_B);
    mixerManager.initFXSlot(fxSlotC, FX_SLOT_C);
    mixerManager.initFXSlot(fxSlotD, FX_SLOT_D);

    sleep(1);

    // ==================== SEND 7x2$... CODES ================================
    // Some further configuration.....
    // Unsure what this does - but it does it for all the channels.
    // Command is similar to the filter for each channel, but slightly different.

    DEBUG_MSG("Sending further configs '7x2$...'\n");

    // Clear Screen
    write(BRAIN, "01u", 3);
    usleep(20000);

    // Display more config stuff message
    write(BRAIN, MORE_CONFIG_MESSAGE, strlen(MORE_CONFIG_MESSAGE));

    // Send file.
    if (clockIO->getCardID() != APOGEE_CLOCK_ID_STRING)
    {
        sendFirmwareFile(DSP_CMD1_FILE, DSP);
    }
    else
    {
        sendFirmwareFile(DSP_CMD1_APOGEE_FILE, DSP);
        getDspResponse(DSP);
        sendFirmwareFile(DSP_CMD2_APOGEE_FILE, DSP);
    }

    // sleep(1);

    // // Seems like there is an "8000" response from the DSP, when things are fine....
    // std::string dspReply = "";
    // do
    // {
    //     dspReply = getDspResponse(DSP);
    //     DEBUG_MSG("DSP reply: %s\n", dspReply.c_str());
    // } while (dspReply != "8000");

    sleep(1);
    // Sometimes it seems DSP is sending numbers decreasing to 0000.. wait till it stops sending:
    getDspResponse(DSP);
    getDspResponse(DSP);
    getDspResponse(DSP);
    getDspResponse(DSP);

    std::string dspReply = "";
    do
    {
        dspReply = getDspResponse(DSP);
        DEBUG_MSG("DSP reply: %s\n", dspReply.c_str());
    } while (dspReply != "");

    sleep(1);

    // // ==================== Load Standard config.. or some user choice =================

    // // For now, for testing, just open a pair of channels.

    // pan 9 Left
    //write(DSP, "0AdFEFFX0OFDFFXP", strlen("0AdFEFFX0OFDFFXP"));

    // pan 10 left
    //write(DSP, "22dFEFFX0OFDFFXP", strlen("22dFEFFX0OFDFFXP"));

    // pan 11 right
    //write(DSP, "0BdFEFFXFEOFDFFXP", strlen("0BdFEFFXFEOFDFFXP"));

    // Set both channels to unity
   // write(DSP, "0BcXC1Q", strlen("0BcXC1Q"));
    //write(DSP, "22cXC1Q", strlen("22cXC1Q"));

    // Turn up master
    //write(DSP, "4Cc9X70QAX70Q", strlen("4Cc9X70QAX70Q"));

    // After this there should be sound out on Masters.
    // lets activate some control room outs.
    // Activate Master L-R (deactivate digital... I think...)

    write(BRAIN, "67d66e", strlen("67d66e"));

    // Set speaker level
    write(BRAIN, "BC0w", 4);

    // Activate Control room Mains:
    write(BRAIN, "5Fd", 3);



    // sleep(5);

    ///////////////////////////////////////////////////////////

    // AFTER THIS IT THE MIXER IS PROBABLY UP AND RUNNING, ALTHOUGH NO CHRISTMAS LIGHTS YET....
    // MAYBE HERE WE SEND THE SAVED CONFIGURATION, AND THEN HAND OVER THINGS TO THE
    // CIRCULAR BUFFER MERRY GOAROUND....?

    // Actually... here we should probably send a final config file....
    // The initial startup settings....

    // If everything is ok, close file descriptors and return true

    close(BRAIN);
    close(DSP);

    return INIT_SUCCESS;
}

// ######################## OTHER HELPER FUNCTIONS #################################

// ###################################################################################
// Function that opens a serial port for communication. It returns a file descriptor.
// ###################################################################################
int openSerialPort(const char *devicePath, speed_t baudRate)
{
    DEBUG_MSG("Opening Serial port\n");
    struct termios options;
    int fd = open(devicePath, O_RDWR | O_NOCTTY);
    if (fd < 0)
    {
        perror("Error opening serial device");
        // exit(1);
        return -1;
    }

    // Get current options
    tcgetattr(fd, &options);

    // Set baud rate
    cfsetispeed(&options, baudRate);
    cfsetospeed(&options, baudRate);

    // Set terminal mode
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    options.c_oflag &= ~(OPOST | ONLCR);

    // Set data format
    options.c_cflag &= ~(PARENB | PARODD | CMSPAR | CSTOPB | CSIZE);
    options.c_cflag |= CS8;

    // Set input control options
    options.c_iflag &= ~(INPCK | IXOFF | IUCLC | IXANY | IMAXBEL | IUTF8);
    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 5;

    // Apply the new settings
    tcflush(fd, TCIOFLUSH);
    tcsetattr(fd, TCSANOW, &options);

    // Return the file descriptor for the port.
    return fd;
}

// ################################################################################
// Function for sending the firmware to the dsp processors.
// Takes a filename (incl. path) and a file descriptor of the com port to send to.
// ################################################################################
void sendFirmwareFile(const char *filename, int comPortDescriptor)
{
    DEBUG_MSG("Opening and uploading: %s\n", filename);

    // Set up a file handle (file descriptor)
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error in function 'sendFirmwareFile', opening file: %s\n", strerror(errno));
        exit(1);
    }

    // Determine the size of the file
    fseek(file, 0, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    // Allocate memory for the file contents
    char *fileContents = new char[fileSize];

    // Read the file into memory
    if (fread(fileContents, 1, fileSize, file) != fileSize)
    {
        perror("read");
        exit(1);
    }

    // Send the file to DSP
    ssize_t bytes_written = write(comPortDescriptor, fileContents, fileSize);
    if (bytes_written < 0)
    {
        perror("write");
        exit(1);
    }

    // Free the memory used for the file contents
    delete[] fileContents;
    fclose(file);
}

// ################################################################################
// Function for receiving a response from the Brain
// It will receive untill it gets an "l" or a "k", which seems to be some sort of
// "heartbeat" signal that the console Brain sends.
// ################################################################################
std::string getBrainResponse(int brainDescriptor)
{
    char response = '\0';
    std::stringstream brainReplyStream;
    while (true)
    {
        int result = read(brainDescriptor, &response, 1);

        if (response == 'l' || response == 'k')
        {
            DEBUG_MSG("Got an l or k - end of message.\n");
            break;
        }

        if (result == 1)
            brainReplyStream.put(response);
        else if (result < 0)
        {
            perror("Error reading from file descriptor");
            exit(1);
        }
        else if (result == 0)
        {
            DEBUG_MSG("EOF\n");
            break;
        }
    }
    return brainReplyStream.str();
}

// ################################################################################
// Function for receiving a response from the DSP board.
// ################################################################################
std::string getDspResponse(int dspDescriptor)
{
    char response = '\0';
    std::stringstream dspReplyStream;
    while (true)
    {
        int result = read(dspDescriptor, &response, 1);
        if (result == 1)
        {
            dspReplyStream.put(response);

            // If we receive a 'd', then message is finished.
            if (response == 'd')
                break;
        }
        else if (result == 0)
        {
            DEBUG_MSG("Dsp EOF\n");
            break;
        }
        else
        {
            perror("Error reading from DSP");
            exit(1);
        }
    }
    return dspReplyStream.str();
}

// ################################################################################
// Function for checking the heartbeat of the Brain.
// Returns 1, if the brain sends an l or a k.
// Returns 0 , if end of file (empty).
// Exits, if reading error.
// ################################################################################
int getHeartbeat(int brainDescriptor)
{
    char response = '\0';

    while (true)
    {
        int result = read(brainDescriptor, &response, 1);

        if (result == 1)
        {
            if (response == 'l' || response == 'k')
                return 1;
        }
        else if (result < 0)
        {
            perror("Error reading from file descriptor");
            exit(1);
        }
        else if (result == 0) // "EOF"
        {
            return 0;
        }
    }
}

// ###############################################################################
// Helper function for retrieving the display string command for a given ioCard ID
// ###############################################################################
void writeIOCardIdDisplayCode(int brainDescriptor, IOSlot *ioSlot, std::string locationCode)
{
    DEBUG_MSG("Getting code for ID: %s\n", ioSlot->getCardID().c_str());

    // Correlation map for ID's and display codes.
    std::map<std::string, std::string> cardIdStrings{
        {AIO8_ID_STRING, AIO8_DISPLAY_CODE},
        {OPT8_ID_STRING, OPT8_DISPLAY_CODE},
        {AES_ID_STRING, AES_DISPLAY_CODE},
        {MACKIE_CLOCK_ID_STRING, MACKIE_CLOCK_DISPLAY_CODE},
        {APOGEE_CLOCK_ID_STRING, APOGEE_CLOCK_DISPLAY_CODE},
        {EMPTY_IOSLOT_ID_STRING, EMPTY_SLOT_DISPLAY_CODE}};

    // Get the ID strings for each slot, and write in the display
    std::string cardIdDisplayCode = "";
    cardIdDisplayCode.append(locationCode);
    cardIdDisplayCode.append(cardIdStrings[ioSlot->getCardID()]);

    // Write to display
    write(brainDescriptor, cardIdDisplayCode.c_str(), cardIdDisplayCode.length());
}

// ###############################################################################
// Helper function for retrieving the display string command for a given ioCard ID
// ###############################################################################
void writeFXCardIdDisplayCode(int brainDescriptor, FXSlot *fxSlot, FXSlotID slotID)
{
    // Check card ID, put command together, write to display.
    std::string displayCommand = "";

    if (fxSlot->getCardID() == FX_EMPTY)
    {
        std::map<FXSlotID, std::string> emptyFxLookupTable{
            {FX_SLOT_A, "C2"},
            {FX_SLOT_B, "CC"},
            {FX_SLOT_C, "D6"},
            {FX_SLOT_D, "E0"}};
        displayCommand = emptyFxLookupTable[slotID] + EMPTY_DISPLAY_BASE;
    }
    else
    {
        std::map<FXSlotID, std::string> loadedFxLookupTable{
            {FX_SLOT_A, "C3"},
            {FX_SLOT_B, "CD"},
            {FX_SLOT_C, "D7"},
            {FX_SLOT_D, "E1"}};
        if (fxSlot->getCardID() == MFX_CARD)
        {
            displayCommand = loadedFxLookupTable[slotID] + MFX_DISPLAY_BASE;
        }
        else
        {
            displayCommand = loadedFxLookupTable[slotID] + UFX_DISPLAY_BASE;
        }
    }
    write(brainDescriptor, displayCommand.c_str(), displayCommand.length());
}

// ################################################################################
// Function for decoding the ascii hex outputs
// ################################################################################
std::string decodeString(const std::string &input_string)
{
    std::string decoded_str = "";
    std::string hex_val = "";

    for (char character : input_string)
    {
        if (character == 'p' || character == 'o')
        {
            decoded_str += static_cast<char>(std::stoi(hex_val, nullptr, 16));
            hex_val.clear();
        }
        else
        {
            hex_val += character;
        }
    }

    return decoded_str;
}