/*
  ==============================================================================

    MixerInitScripts.cpp
    Created: 13 May 2023 11:15:17pm
    Author:  anders

  ==============================================================================
*/

#include "MixerInitScripts.h"
#include "CommandStrings.h"
#include <fcntl.h>
#include <unistd.h>

void initializeMixer()
{

    // ================== SET UP COM PORTS =============================

    // Get access to the mixerManager singleton:
    MixerManager &mixerManager = MixerManager::getInstance();

    // Fetch the com ports:
    const char *BRAIN_DEVICE = mixerManager.getBrainPort().c_str();
    const char *DSP_DEVICE = mixerManager.getDspPort().c_str();

    std::cout << "Brainy is: " << BRAIN_DEVICE << std::endl;
    std::cout << "Dispy is: " << DSP_DEVICE << std::endl;

    // Make a file descriptor for the 115200 baud rate brain connection.
    int BRAIN = openSerialPort(BRAIN_DEVICE, B115200);  // Not const, since it can change to boost later.
    const int DSP = openSerialPort(DSP_DEVICE, B115200);

    std::cout << "Ports should now be inited" << std::endl;

    usleep(100000); // Sleep for 100ms

    // ================= INITIALIZE COMMUNICATION ========================
    // Send an 'R' (reset), and wait to get an 'R' back

    char replyChar = '\0'; // Initial null terminator.

    // Reset DSP and Brain:
    //write(DSP, "R", 1);
    write(BRAIN, "R", 1);

    // Check Brain reply
    while (replyChar != 'R')
    {
        if (read(BRAIN, &replyChar, 1) == 1 && replyChar == 'R')
        {
            printf("Brain connected.\n");
            std::cout << "replychar is: " << replyChar << std::endl;
        }
        printf("napping....\n");
        usleep(200000); // Sleep 200ms
    }

    printf("Brainy Happy - now write to dsp\n");
    // Same for DSP
    write(DSP, "R", 1);
    replyChar = '\0'; // Reset for DSP round.
    printf("Writting - checking response\n");
    while (replyChar != 'R')
    {
        if (read(DSP, &replyChar, 1) == 1 && replyChar == 'R')
            printf("DSP connected.\n");
        usleep(200000);
    }

    // ================== DISPLAY WELCOME MESSAGE ============================

    write(BRAIN, DISPLAY_LOW, strlen(DISPLAY_LOW)); // Display intensity = low (preserve display)

    // Send "01u" which clears the display.
    write(BRAIN, "01u", 3);
    usleep(20000); // Sleep for 20ms - needed for clear display to finish.

    // Send "0Cu" - unsure what this does... not even sure it's needed... but it is sent in the original.
    write(BRAIN, "0Cu", 3);
    usleep(20000); // Sleep for 20ms

    write(BRAIN, WELCOME_STRING, strlen(WELCOME_STRING));
    // usleep(20000); // Sleep for 20ms
    sleep(4);

    // ====================== SEND FIRMWARES =======================================
    // Clear Screen
    // NOTE - seems like "01u" only works BEFORE brainware is uploaded.
    write(BRAIN, "01u", 3); 
    usleep(20000);

    printf("sending brain \n");

    if (mixerManager.getBrainBoostState())
    {
        // This one goes to 11!
        write(BRAIN, BOOST_MESSAGE, strlen(BOOST_MESSAGE));
        sendFirmwareFile(BRAINWARE_FAST_FILE, BRAIN);
        close(BRAIN);
        BRAIN = openSerialPort(BRAIN_DEVICE, B230400);
    }
    else
    {
        //write(BRAIN, UPLOAD_FIRMWARE_MESSAGE, strlen(UPLOAD_FIRMWARE_MESSAGE));
        write(BRAIN, UPLOADING_BRAIN_MESSAGE, strlen(UPLOADING_BRAIN_MESSAGE));
        sendFirmwareFile(BRAINWARE_FILE, BRAIN);
    }

    // Brainware uploaded - now upload DSP firmware.
    printf("Now sending dspware..\n");

    // Clear Screen


    exit(1);


    printf("had a nap... cleared the screen... now write new display message\n");

    write(BRAIN, UPLOADING_DSP_MESSAGE, strlen(UPLOADING_DSP_MESSAGE));

    printf("dsp display message should be written.. now sending dsp firmware\n");

    sendFirmwareFile(DSP_MASTER_FIRMWARE_FILE, DSP);

    // =================== FIRMWARES ARE ULOADED ================================
    // Check response
    // !!!!!!!!!!!!!!!!!!!WE STILL DONT KNOW WHAT THIS DATA MEANS!!!!!!!!!!!!!!!!!

    std::cout << "Brain replied: " << getBrainResponse(BRAIN) << std::endl;

    // ====================== I/O CARD LIST ====================================
    // This section prints out a list of detected I/O cards.

    // Print the tape list in the display
    write(BRAIN, TAPE_LIST, strlen(TAPE_LIST));

    // Poke IO card slots, and write their identifier in display:
    std::string cardID = identifyIOCard(TAPE_A, BRAIN);
    cardID.insert(0, TAPE_A_LOC);
    write(BRAIN, cardID.c_str(), cardID.length());

    cardID = identifyIOCard(TAPE_B, BRAIN);
    cardID.insert(0, TAPE_B_LOC);
    write(BRAIN, cardID.c_str(), cardID.length());

    cardID = identifyIOCard(TAPE_C, BRAIN);
    cardID.insert(0, TAPE_C_LOC);
    write(BRAIN, cardID.c_str(), cardID.length());

    cardID = identifyIOCard(ALTIO, BRAIN);
    cardID.insert(0, ALTIO_LOC);
    write(BRAIN, cardID.c_str(), cardID.length());

    sleep(2);

    // =================== LIST DIGITAL IO & CLOCK ================================
    // Clear screen
    write(BRAIN, "01u", 3);
    usleep(20000);

    write(BRAIN, DIGITAL_IO_STRING, strlen(DIGITAL_IO_STRING));
    write(BRAIN, CLOCK_STRING, strlen(CLOCK_STRING));

    // Clear the buffer of "klk's"
    tcflush(BRAIN, TCIOFLUSH);

    cardID = identifyIOCard(DIGI_IO, BRAIN);
    cardID.insert(0, DIGICARD_LOC);
    write(BRAIN, cardID.c_str(), cardID.length());

    cardID = identifyIOCard(CLOCK, BRAIN);
    cardID.insert(0, CLOCK_LOC);
    write(BRAIN, cardID.c_str(), cardID.length());

    // Next, send an "s".... dunno why!
    write(BRAIN, "s", 1);

    // Then check response.
    std::cout << "s Response: " << getBrainResponse(BRAIN) << std::endl;

    // ============== CHECK FIRMWARE RESPONSE FROM DSP =====================

    std::cout << "DSP RESPONSE: " << getDspResponse(DSP) << std::endl;

    // ===================== SEND SLAVE PART OF DSP FIRMWARE ==================
    std::cout << "Sending DSP Slave part" << std::endl;

    FILE *dspSlaveWare = fopen(DSP_SLAVE_FILE, "r");
    if (dspSlaveWare == NULL)
    {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        exit(1);
    }

    char buf[BUF_SIZE];
    size_t dspBytesRead = 0;
    while ((dspBytesRead = fread(buf, 1, BUF_SIZE, dspSlaveWare)) > 0)
    {
        ssize_t dspBytesWritten = write(DSP, buf, dspBytesRead);
        if (dspBytesWritten < 0)
        {
            perror("write");
            exit(1);
        }
    }

    fclose(dspSlaveWare);

    // Clear buffer
    memset(buf, 0, sizeof(buf));

    // A moment of silence.
    sleep(2);

    // =================== DSP PLUGINS (Config) ===========================
    // Display "Loading DSP plugins..."
    write(BRAIN, LOADING_DSP, strlen(LOADING_DSP));

    // Now send the config file to DSP ...twice... for some reason..
    FILE *dspConfig = fopen(DSP_CONFIG_FILE, "r");
    if (dspConfig == NULL)
    {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        exit(1);
    }

    // Reusing the buffer and variable from above
    for (int i = 0; i < 2; ++i)
    {
        dspBytesRead = 0;
        while ((dspBytesRead = fread(buf, 1, BUF_SIZE, dspConfig)) > 0)
        {
            ssize_t dspBytesWritten = write(DSP, buf, dspBytesRead);
            if (dspBytesWritten < 0)
            {
                perror("write");
                exit(1);
            }
        }
    }

    fclose(dspConfig);

    printf("Config sent twice.\n");

    // =========================== INIT FX CARDS =============================

    // Display the FX card list:
    write(BRAIN, FX_CARD_LIST, strlen(FX_CARD_LIST));

    // Create the FX card slot object pointers.
    FXSlot *slotA = new FXSlot(BRAIN, FX_SLOT_A);
    FXSlot *slotB = new FXSlot(BRAIN, FX_SLOT_B);
    FXSlot *slotC = new FXSlot(BRAIN, FX_SLOT_C);
    FXSlot *slotD = new FXSlot(BRAIN, FX_SLOT_D);

    // Pass the pointers to the mixermanager
    mixerManager.initFXSlot(slotA, FX_SLOT_A);
    mixerManager.initFXSlot(slotB, FX_SLOT_B);
    mixerManager.initFXSlot(slotC, FX_SLOT_C);
    mixerManager.initFXSlot(slotD, FX_SLOT_D);


    // ==================== Send some 7x2$... ================================
    // Some further configuration.....
    // Unsure what this does - but it does it for all the channels.
    // Command is similar to the filter for each channel, but slightly different.

    // Open the file for reading
    printf("Sending 7x2$....\n");
    FILE *dspCmd1 = fopen(DSP_CMD1_FILE, "r");
    if (dspCmd1 == NULL)
    {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        exit(1);
    }

    // Send the file
    dspBytesRead = 0;
    while ((dspBytesRead = fread(buf, 1, BUF_SIZE, dspCmd1)) > 0)
    {
        ssize_t bytesWritten = write(DSP, buf, dspBytesRead);
        if (bytesWritten < 0)
        {
            perror("write");
            exit(1);
        }
    }

    fclose(dspCmd1);

    // Should there be a reponse?
    std::cout << getDspResponse(DSP) << std::endl;


    // AFTER THIS IT THE MIXER IS PROBABLY UP AND RUNNING, ALTHOUGH NO CHRISTMAS LIGHTS YET....
    // MAYBE HERE WE SEND THE SAVED CONFIGURATION, AND THEN HAND OVER THINGS TO THE 
    // CIRCULAR BUFFER MERRY GOAROUND....?anders
    

}

//
//
//
// ######################## OTHER HELPER FUNCTIONS #################################

// ###################################################################################
// Function that opens a serial port for communication. It returns a file descriptor.
// ###################################################################################
int openSerialPort(const char *devicePath, speed_t baudRate)
{
    std::cout << "opening serial port" << std::endl;
    struct termios options;
    int fd = open(devicePath, O_RDWR | O_NOCTTY);
    if (fd < 0)
    {
        perror("Error opening serial device");
        exit(1);
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

    std::cout << "returning" << std::endl;
    return fd;
}

// ################################################################################
// Function for sending the firmware to the dsp processors.
// Takes a filename (incl. path) and a file descriptor of the com port to send to.
// ################################################################################
void sendFirmwareFile(const char *filename, int comPortDescriptor)
{
    printf("sendFirmwareFile function - filename: %s\n", filename);

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
    printf("exiting sendFirmwareFile\n");
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
            printf("got an l or k - end of message.\n");
            break;
        }

        if (result == 1)
            // brain_reply_stream << response;
            brainReplyStream.put(response);
        else if (result < 0)
        {
            perror("Error reading from file descriptor");
            exit(1);
        }
        else if (result == 0)
        {
            std::cout << "END OF FIIIIIILE!" << std::endl;
            break;
        }
        // usleep(10000); // Sleep for 10ms
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
            if (response == 'd')
                break;
            else
                dspReplyStream.put(response);
        }
        else if (result == 0)
        {
            printf("DSP SENT END OF FILE... or maybe just nothing...");
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
// Function for identifying the card in the specified card slot
// Takes card slot object, and a Brain COM port file descriptor
// ################################################################################
std::string identifyIOCard(ioCardType tapeSlot, int brainDescriptor)
{
    // Send the query-string for the supplied tape slot.
    switch (tapeSlot)
    {
        case TAPE_A:
            write(brainDescriptor, QUERY_TAPE_A, 3);
            break;
        case TAPE_B:
            write(brainDescriptor, QUERY_TAPE_B, 3);
            break;
        case TAPE_C:
            write(brainDescriptor, QUERY_TAPE_C, 3);
            break;
        case ALTIO:
            write(brainDescriptor, QUERY_ALTIO, 3);
            break;
        case DIGI_IO:
            write(brainDescriptor, QUERY_DIGI_SLOT, 3);
            break;
        case CLOCK:
            write(brainDescriptor, QUERY_CLOCK, 3);
            break;
        default:
            perror("that tape slot does not exist.");
            exit(1);
    }

    // Check the repsonse
    std::string iocardID = getBrainResponse(brainDescriptor);
    // std::cout << "Card hex: " << iocard_id << std::endl;

    std::cout << "Slot " << tapeSlot << ": " << decodeString(iocardID) << std::endl;

    // std::string returnString;
    if (iocardID == AIO8_REPLY)
        return std::string(AIO8_STRING);
    else if (iocardID == OPT8_REPLY)
        return std::string(OPT8_STRING);
    else if (iocardID == AES_REPLY)
        return std::string(AES_STRING);
    else if (iocardID == MACKIE_CLOCK_REPLY)
        return std::string(MACKIE_CLOCK_STRING);
    else if (iocardID == APOGEE_CLOCK_REPLY)
        return std::string(APOGEE_CLOCK_STRING);
    else if (iocardID.length() == 0)
        return std::string(EMPTY_SLOT_STRING);
    else
    {
        perror("Response on tape query didnt match any known card");
        exit(1);
    }
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