// THIS SEEMS TO WORK

#include "CommandStrings.h"
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <termios.h>
#include <unistd.h>
#include "FXSlotClass.h"
#include <thread>
#include "CircularBuffer.h"

#include "test.h"

#define BUF_SIZE 1024
#define BRAIN_DEVICE "/dev/ttyUSB0"
#define DSP_DEVICE "/dev/ttyUSB1"
#define BRAINWARE_FILE "../controlfast.asc"
#define DSP_MASTER_FILE "../master.asc"
#define DSP_SLAVE_FILE "../slave.asc"
#define DSP_CONFIG_FILE "../Config.asc"
#define DSP_CMD1_FILE "../dspCmd1.asc"

const int bufferSize = 100;
    


// #define WELCOME_STRING "90u44v38v42v94u35v2Ev31vCDu4Dv41v43v4Bv49v45vD4u44v49v47v49v54v41v4Cv"

// ############### PROTOTYPES #########################
int open_serial_port(const char *device_path, speed_t baud_rate);
std::string decode_string(const std::string &input_string);
std::string get_brainresponse(int brain_descriptor);
std::string identify_io_card(tape tape_slot, int brain_descriptor);
std::string get_dspresponse(int dsp_descriptor);
void send_firmware_file(const char* filename, int com_descriptor);
void send_cmd(const char * cmd, int com_descriptor);
void reader_thread(CircularBuffer& buffer);
void brainToBuffer(CircularBuffer& buffer);
void dspToBuffer(CircularBuffer& buffer);

int main()
{
    

    printf("584: %c\n", fdata[584]);
    printf("631: %c\n", fdata[631]);
    printf("56731: %c\n", fdata[56731]);
    printf("56732: %c\n", fdata[56732]);
    printf("56733: %c\n", fdata[56733]);


    // ########### SET UP COM PORTS #####################

    const int SLOW_BRAIN = open_serial_port(BRAIN_DEVICE, B115200);
    //const int FASTBRAIN = open_serial_port(BRAIN_DEVICE, B230400);
    const int DSP = open_serial_port(DSP_DEVICE, B115200);

    usleep(100000); // Sleep for 100ms

    // ################ INITIALIZE COMUNICATION #####################
    // NOTE: mixer should be powered on first!
    // Initialize each board, by sending an "R", which resets, and wait for
    // the board to send an "R" back.

    char reply_char = '\0';

    // Initialize DSP and Brain:
    write(DSP, "R", 1);
    write(SLOW_BRAIN, "R", 1);

    // Check reply
    while (reply_char != 'R')
    {
        if (read(SLOW_BRAIN, &reply_char, 1) == 1 && reply_char == 'R')
            printf("Brain connected.\n");
        usleep(200000);
    }

    reply_char = '\0'; // Reset for DSP round.
    while (reply_char != 'R')
    {
        if (read(DSP, &reply_char, 1) == 1 && reply_char == 'R')
            printf("DSP connected.\n");
        usleep(200000);
    }

    // ############ WELCOME SCREEN ####################
    std::cout << "Writing welcome string to display" << std::endl;

    // Set display intensity / Brightness to low. (Helps preserve display)
    write(SLOW_BRAIN, DISPLAY_LOW, strlen(DISPLAY_LOW));

    // Send "01u" which clears the display.
    write(SLOW_BRAIN, "01u", 3);
    usleep(20000); // Sleep for 20ms - needed for clear display to finish.

    // Send "0Cu" - unsure what this does.
    write(SLOW_BRAIN, "0Cu", 3);
    usleep(20000); // Sleep for 20ms

    write(SLOW_BRAIN, WELCOME_STRING, strlen(WELCOME_STRING));
    usleep(20000); // Sleep for 20ms


    // ################ SEND FIRMWARES ############################
    std::cout << "Sending firmwares" << std::endl;

    // printf("Opening mr. DSP\n");
    // FILE *dspWare = fopen(DSP_MASTER_FILE, "r");
    // if (dspWare == NULL)
    // {
    //     fprintf(stderr, "Error opening file: %s\n", strerror(errno));
    //     exit(1);
    // }

    // // Send the dsp firmware
    // char buf[BUF_SIZE];
    // size_t bytes_read = 0;
    // while ((bytes_read = fread(buf, 1, BUF_SIZE, dspWare)) > 0)
    // {
    //     ssize_t bytes_written = write(DSP, buf, bytes_read);
    //     if (bytes_written < 0)
    //     {
    //         perror("write");
    //         exit(1);
    //     }
    // }

    // fclose(dspWare);

    // // Open the file for reading
    // printf("Opening mr. Brain\n");
    // FILE *brainWare = fopen(BRAINWARE_FILE, "r");
    // if (brainWare == NULL || dspWare == NULL)
    // {
    //     fprintf(stderr, "Error opening file: %s\n", strerror(errno));
    //     exit(1);
    // }

    // // Send the brain firmware
    // bytes_read = 0;
    // while ((bytes_read = fread(buf, 1, BUF_SIZE, brainWare)) > 0)
    // {
    //     ssize_t bytes_written = write(BRAIN, buf, bytes_read);
    //     if (bytes_written < 0)
    //     {
    //         perror("write");
    //         exit(1);
    //     }
    // }

    // fclose(brainWare);

    send_firmware_file(DSP_MASTER_FILE, DSP);
    send_firmware_file(BRAINWARE_FILE, SLOW_BRAIN);

    close(SLOW_BRAIN);

    // going to turbo mode - this one goes to 11!!!!
    const int BRAIN = open_serial_port(BRAIN_DEVICE, B230400);

    // ###################### FIRMWARES ARE DONE ###############
    // CHECK response

    // WE STILL DONT KNOW WHAT THIS DATA MEANS!!!!!!!!!!!!!!!!!

    std::cout << "Brain replied: " << get_brainresponse(BRAIN) << std::endl;

    // ################ I/O CARD LIST ##########################
    printf("Printing list of I/O Cards\n");

    // Print the tape list in the display
    write(BRAIN, TAPE_LIST, strlen(TAPE_LIST));

    // Poke IO card slots, and write their identifier in display:
    std::string card_id = identify_io_card(TAPE_A, BRAIN);
    card_id.insert(0, TAPE_A_LOC);
    write(BRAIN, card_id.c_str(), card_id.length());

    card_id = identify_io_card(TAPE_B, BRAIN);
    card_id.insert(0, TAPE_B_LOC);
    write(BRAIN, card_id.c_str(), card_id.length());

    card_id = identify_io_card(TAPE_C, BRAIN);
    card_id.insert(0, TAPE_C_LOC);
    write(BRAIN, card_id.c_str(), card_id.length());

    card_id = identify_io_card(ALTIO, BRAIN);
    card_id.insert(0, ALTIO_LOC);
    write(BRAIN, card_id.c_str(), card_id.length());

    sleep(2);

    // ################### LIST DIGITAL IO & CLOCK #######################
    write(BRAIN, "01u", 3);
    usleep(20000);

    write(BRAIN, DIGITAL_IO_STRING, strlen(DIGITAL_IO_STRING));
    write(BRAIN, CLOCK_STRING, strlen(CLOCK_STRING));

    // Clear the buffer of "klk's"
    tcflush(BRAIN, TCIOFLUSH);

    card_id = identify_io_card(DIGI_IO, BRAIN);
    card_id.insert(0, DIGICARD_LOC);
    write(BRAIN, card_id.c_str(), card_id.length());

    card_id = identify_io_card(CLOCK, BRAIN);
    card_id.insert(0, CLOCK_LOC);
    write(BRAIN, card_id.c_str(), card_id.length());

    // Next, send an "s".... dunno why!
    write(BRAIN, "s", 1);

    // Then check response.
    std::cout << "s Response: " << get_brainresponse(BRAIN) << std::endl;

    // Sleep for a couple of seconds to let the people admire the beauty.
    // sleep(3);

    // ################# CHECK FIRMWARE RESPONSE FROM DSP #############

    std::cout << "DSP RESPONSE: " << get_dspresponse(DSP) << std::endl;

    // ############# SEND SLAVE PART OF DSP FIRMWARE ##################
    std::cout << "Sending DSP Slave part" << std::endl;

    FILE *dspSlaveWare = fopen(DSP_SLAVE_FILE, "r");
    if (dspSlaveWare == NULL)
    {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        exit(1);
    }

    char buf[BUF_SIZE];
    size_t bytes_read = 0;
    //bytes_read = 0;
    while ((bytes_read = fread(buf, 1, BUF_SIZE, dspSlaveWare)) > 0)
    {
        ssize_t bytes_written = write(DSP, buf, bytes_read);
        if (bytes_written < 0)
        {
            perror("write");
            exit(1);
        }
    }

    fclose(dspSlaveWare);

    //printf("done sending slave\n");
    // Let's take another moments silence, to admire the display
    sleep(2);

    // #################### DSP PLUGINS (Config) ############################
    // Write "Loading DSP plugins..."
    write(BRAIN, LOADING_DSP, strlen(LOADING_DSP));

    printf("done writing dsp loading...\n");
    // Now send the config file to DSP ... twice, for some reason..

    FILE *dspConfig = fopen(DSP_CONFIG_FILE, "r");
    if (dspSlaveWare == NULL)
    {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        exit(1);
    }

    // char buf[BUF_SIZE];
    for (int i = 0; i < 2; ++i)
    {
        bytes_read = 0;
        while ((bytes_read = fread(buf, 1, BUF_SIZE, dspConfig)) > 0)
        {
            ssize_t bytes_written = write(DSP, buf, bytes_read);
            if (bytes_written < 0)
            {
                perror("write");
                exit(1);
            }
        }
    }

    fclose(dspConfig);

    printf("Config sent twice.\n");


    // ############### FX CARDS ##########################
    
    // Display the FX card list:
    write(BRAIN, FX_CARD_LIST, strlen(FX_CARD_LIST));

    // Create the FX card slot objects:
    FXSlot slot_a(BRAIN,FX_SLOT_A);
    FXSlot slot_b(BRAIN, FX_SLOT_B);
    FXSlot slot_c(BRAIN, FX_SLOT_C);
    FXSlot slot_d(BRAIN, FX_SLOT_D);
    printf("Now fx cards should be listed\n");


    // ############### Send some 7x2$... ####################

    // Open the file for reading
    printf("Sending 7x2$....\n");
    FILE *dspCmd1 = fopen(DSP_CMD1_FILE, "r");
    if (dspCmd1 == NULL)
    {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        exit(1);
    }

    // Send the file
    bytes_read = 0;
    while ((bytes_read = fread(buf, 1, BUF_SIZE, dspCmd1)) > 0)
    {
        ssize_t bytes_written = write(DSP, buf, bytes_read);
        if (bytes_written < 0)
        {
            perror("write");
            exit(1);
        }
    }

    fclose(dspCmd1);

    // Should there be a reponse?
    std::cout << get_dspresponse(DSP) << std::endl;
    std::cout << get_dspresponse(DSP) << std::endl;
    std::cout << get_dspresponse(DSP) << std::endl;
    std::cout << get_dspresponse(DSP) << std::endl;


    // ############## Send brain-last 1 and 2 ########################
    printf("Sending\n");
    // These commands turn on leds, and pulls faders down and such.
    // Just some general initial set up. Maybe we should do this 
    // it separate parts (faders, leds, ...??) (or maybe we save memory, 
    // by having a long command from a file, so it will be unloaded again....)


    // ################## Open a pair of channels ###################################

    #define PAN9L "0AdFEFFX0OFDFFXP"

    // pan 9 Left
    send_cmd("0AdFEFFX0OFDFFXP", DSP);

    // pan 10 right
    send_cmd("22dFEFFXFEOFDFFXP", DSP);

    // Set both channels to unity
    send_cmd("0AcXC1Q", DSP);
    send_cmd("22cXC1Q", DSP);

    // Turn up master
    send_cmd("4Cc9X70QAX70Q", DSP);


    // After this there should be sound out on Masters.
    // lets activate some control room outs.
    // Activate Master L-R (deactivate digital... I think...)
    send_cmd("67d66e", BRAIN);
    // Activate Control room Mains:
    send_cmd("5Fd", BRAIN);
    // Set speaker level
    send_cmd("BC0w", BRAIN);


    // ######################################################################

    close(BRAIN);
    close(DSP);

    CircularBuffer circBuffer;

    std::thread brainThread(brainToBuffer, std::ref(circBuffer));
    std::thread dspThread(dspToBuffer, std::ref(circBuffer));
    std::thread popperThread(reader_thread, std::ref(circBuffer));



    brainThread.join();
    popperThread.join();






    // Set up non-blocking I/O for reading from serial
    fcntl(BRAIN, F_SETFL, O_NONBLOCK);

    // Set up non-blocking I/O for reading from stdin
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

    // Loop for reading input and output
    char input_buf[BUF_SIZE];
    char serial_buf[BUF_SIZE];
    size_t serial_len = 0;

    while (1)
    {
        // Read from serial port
        ssize_t bytes_read2 = read(BRAIN, serial_buf + serial_len, BUF_SIZE - serial_len);
        if (bytes_read2 > 0)
        {
            serial_len += bytes_read2;
        }

        // Print any received bytes
        if (serial_len > 0)
        {
            printf("%.*s", (int)serial_len, serial_buf);
            serial_len = 0;
        }

        // Read from stdin
        bytes_read2 = read(STDIN_FILENO, input_buf, BUF_SIZE);
        if (bytes_read2 > 0)
        {
            // Send any input to the serial port
            ssize_t bytes_written = write(BRAIN, input_buf, bytes_read2);
            if (bytes_written < 0)
            {
                perror("write");
                exit(1);
            }
        }

        usleep(20000); // Sleep for 20ms
    }

    // Close serial port
    close(BRAIN);
    close(DSP);

    return 0;
}

// ########### Function Implementations ##########################

int open_serial_port(const char *device_path, speed_t baud_rate)
{
    struct termios options;
    int fd = open(device_path, O_RDWR | O_NOCTTY);
    if (fd < 0)
    {
        perror("Error opening serial device");
        exit(1);
    }

    // Get current options
    tcgetattr(fd, &options);

    // Set baud rate
    cfsetispeed(&options, baud_rate);
    cfsetospeed(&options, baud_rate);

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

    return fd;
}

std::string decode_string(const std::string &input_string)
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

// TODO: Not sure this function would work with the DSP... it doesn not do "lk"
std::string get_brainresponse(int brain_descriptor)
{
    char response = '\0';
    std::stringstream brain_reply_stream;

    while (true)
    {
        int result = read(brain_descriptor, &response, 1);
        if (response == 'l' || response == 'k')
        {
            printf("got an l or k - end of message.\n");
            break;
        }

        if (result == 1)
            // brain_reply_stream << response;
            brain_reply_stream.put(response);
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

    return brain_reply_stream.str();
}

std::string get_dspresponse(int dsp_descriptor)
{
    char response = '\0';
    std::stringstream dsp_reply_stream;

    while (true)
    {
        int result = read(dsp_descriptor, &response, 1);
        if (result == 1)
        {
            if (response == 'd')
                break;
            else
                dsp_reply_stream.put(response);
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

    return dsp_reply_stream.str();
}

std::string identify_io_card(tape tape_slot, int brain_descriptor)
{
    switch (tape_slot)
    {
        case TAPE_A:
            write(brain_descriptor, QUERY_TAPE_A, 3);
            break;
        case TAPE_B:
            write(brain_descriptor, QUERY_TAPE_B, 3);
            break;
        case TAPE_C:
            write(brain_descriptor, QUERY_TAPE_C, 3);
            break;
        case ALTIO:
            write(brain_descriptor, QUERY_ALTIO, 3);
            break;
        case DIGI_IO:
            write(brain_descriptor, QUERY_DIGI_SLOT, 3);
            break;
        case CLOCK:
            write(brain_descriptor, QUERY_CLOCK, 3);
            break;
        default:
            perror("that tape slot does not exist.");
            exit(1);
    }

    std::string iocard_id = get_brainresponse(brain_descriptor);
    // std::cout << "Card hex: " << iocard_id << std::endl;

    std::cout << "Slot " << tape_slot << ": " << decode_string(iocard_id) << std::endl;

    std::string return_string;
    if (iocard_id == AIO8_REPLY)
        return std::string(AIO8_STRING);
    else if (iocard_id == OPT8_REPLY)
        return std::string(OPT8_STRING);
    else if (iocard_id == AES_REPLY)
        return std::string(AES_STRING);
    else if (iocard_id == MACKIE_CLOCK_REPLY)
        return std::string(MACKIE_CLOCK_STRING);
    else if (iocard_id == APOGEE_CLOCK_REPLY)
        return std::string(APOGEE_CLOCK_STRING);
    else if (iocard_id.length() == 0)
        return std::string(EMPTY_SLOT_STRING);
    else
    {
        perror("Response on tape query didnt match any known card");
        exit(1);
    }

}

// std::string identify_fx_card(const std::string query, int fd)
// {
//     // Query the slot for MFX:
//     write(fd, fx_query[query].c_str(), fx_query[query].length());

    
// }




void send_firmware_file(const char* filename, int com_descriptor)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        exit(1);
    }

    // Determine the size of the file
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    // Allocate memory for the file contents
    char* file_contents = new char[file_size];

    // Read the file into memory
    if (fread(file_contents, 1, file_size, file) != file_size)
    {
        perror("read");
        exit(1);
    }

    // Send the file to DSP
    ssize_t bytes_written = write(com_descriptor, file_contents, file_size);
    if (bytes_written < 0)
    {
        perror("write");
        exit(1);
    }

    // Free the memory used for the file contents
    delete[] file_contents;
    fclose(file);
}

void send_cmd(const char * cmd, int com_descriptor)
{
    int length = strlen(cmd);
    write(com_descriptor, cmd, length);
}

void brainToBuffer(CircularBuffer& buffer)
{
    const int brainDescriptor = open_serial_port(BRAIN_DEVICE, B230400);

    char recvChar = '\0';
    std::string message = "";
    int result;

    while(true)
    {
        result = read(brainDescriptor, &recvChar, 1);

        if (result == 1)
        {
            message += recvChar;
            if (recvChar >= 'a' && recvChar <= 'z')
            {
                // Lower case letter received, message complete. push to buffer.
                buffer.push(message.c_str());
                message = "";
            }
        }

        else if (result < 0)
        {
            perror("Error reading from file descriptor");
            exit(1);
        }
        else if (result == 0)
        {
            std::cout << "END OF FIIIIIILE!" << std::endl;
            //break;
        }
    }

    close(brainDescriptor);
}

void dspToBuffer(CircularBuffer& buffer)
{
    const int dspDescriptor = open_serial_port(DSP_DEVICE, B115200);

    char recvChar = '\0';
    std::string message = "";
    int result;

    while(true)
    {
        result = read(dspDescriptor, &recvChar, 1);

        if (result == 1)
        {
            message += recvChar;
            if (recvChar >= 'a' && recvChar <= 'z')
            {
                // Lower case letter received, message complete. push to buffer.
                buffer.push(message.c_str());
                message = "";
            }
        }

        else if (result < 0)
        {
            perror("Error reading from file descriptor");
            exit(1);
        }
        else if (result == 0)
        {
            std::cout << "END OF FIIIIIILE!" << std::endl;
            //break;
        }
    }

    close(dspDescriptor);
}

void reader_thread(CircularBuffer& buffer)
{
    while (true)
    {
        std::string message = buffer.pop();
        std::cout << "reader thread: " << message << std::endl;
    }

}
