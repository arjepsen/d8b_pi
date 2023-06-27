/*
  ==============================================================================

    BrainWriterClass.cpp
    Created: 25 Jun 2023 10:52:04pm
    Author:  anders

  ==============================================================================
*/

#include "BrainComClass.h"
#include <fcntl.h>
#include <stdexcept>
#include <unistd.h>

BrainCom::BrainCom() : circBuffer(CircularBuffer::getInstance()) {}
BrainCom::~BrainCom() {}

// ############################################################################
// This method is used to set up and open the communication port with the Brain
// ############################################################################
int BrainCom::openSerialPort(const char *devicePath, speed_t baudRate)
{
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

// ###################################
// Setter and Getter for the brainPort
// ###################################
void BrainCom::setBrainPort(std::string port)
{
    brainPort = port;
}

std::string BrainCom::getBrainPort()
{
    return brainPort;
}

// ###############################################
// Method for sending a command to the brain board
// ###############################################
void BrainCom::sendCmd(const std::string &command)
{
    std::lock_guard<std::mutex> lock(brainWriteMutex); // Automatically locks the mutex for current scope.
    write(brain, command.c_str(), command.size());
}

// #######################################
// Method for flushing the receiver buffer
// #######################################
void BrainCom::flushBuffer()
{
    std::lock_guard<std::mutex> lock(brainWriteMutex); // Automatically locks the mutex for current scope.
    tcflush(brain, TCIOFLUSH);
}

void startBrainReceiverThread()
{
    brainReceiverThread = std::thread(&brainCom::messageReceiver, this);
}

// This method is run in a thread. It is responsible for reading messages from the Brain board, and
// Putting them in the circular buffer singleton.
void BrainCom::brainMessageReceiver()
{
    // Clear screen before entering loop.
    // write(brainDescriptor, "01u", 3);
    brain.sendCmd("01u");
    usleep(20000);

    char recvChar = '\0';
    std::string message = "";
    int result;

    // Clear com buffer for starting out
    // tcflush(brainDescriptor, TCIOFLUSH);
    brain.flushBuffer();

    DEBUG_MSG("running brain message loop\n");
    // Run the infinite loop.
    while (true)
    {
        result = read(brainDescriptor, &recvChar, 1);

        if (result == 1) // One char was recevied.
        {
            message += recvChar;

            // A lower case letter means message complete.
            if (recvChar >= 'a' && recvChar <= 'z')
            {
                // printf("brain message: %s\n", message.c_str());

                if (recvChar == 'l' || recvChar == 'k')
                {
                    printf("hearbeat: %c\n", recvChar);
                    heartBeatReceived();
                }
                else
                    circBuffer.push(message.c_str()); // Push message to the circular buffer.
                message = "";                         // Reset message string.
            }
        }

        else if (result < 0)
        {
            perror("Error reading from file descriptor");
            exit(1);
        }
        else if (result == 0) // 0 chars recevied - EOF
        {
            // Should we add any special functionality here??
        }
    }

    // We probably shouldn't get here... but just in case.
    printf("\n\n########################## BRAIN LOOP EXITED!!!!!!! ##############################\n");
}