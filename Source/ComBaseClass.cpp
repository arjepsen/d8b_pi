/*
  ==============================================================================

    ComBaseClass.cpp
    Created: 27 Jun 2023 9:10:44pm
    Author:  anders

  ==============================================================================
*/

#include "ComBaseClass.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "Debug.h"

#include <iostream>
#include <string>



// Constructor with initialization list for the circular buffer
ComBase::ComBase() : circBuffer(CircularBuffer::getInstance()) 
{
    DEBUG_MSG("BASE COM CONSTRUCTOR\n");
}

ComBase::~ComBase() {}





/************************************************************************
 * @brief Method for setting up the com port
 * 
 * @param comPortPath Something like /dev/ttyUSB0
 * @param baudRate Likely 115200
 * @return int Returns a file descriptor.
 ************************************************************************/
int ComBase::openSerialPort(const char *comPortPath, speed_t baudRate)
{
    struct termios options;
    int fd = open(comPortPath, O_RDWR | O_NOCTTY);
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

/***********************************************
 * @brief Setter for the com port
 * 
 * @param comPortPath 
 **********************************************/
void ComBase::setPort(std::string comPortPath)
{
    port = comPortPath;
}

/***********************************************
 * @brief Getter for the com port
 * 
 * @return std::string 
 ************************************************/
std::string ComBase::getPort()
{
    return port;
}

void ComBase::setBaudRate(speed_t baud)
{
    baudRate = baud;
}

speed_t ComBase::getBaudRate()
{
    return baudRate;
}


/*************************************************************************
 * @brief This method is for winding up the thread that reads messages,
 *          and puts them in the circular buffer
 ************************************************************************/
void ComBase::startReceiverThread()
{
    // So this would be called from mixermanager
    // TODO: Maybe "openserialport" should be private (protected), and here we first check if port/baudrate is set.
    // Then also check if initialization is done. (maybe by checkinng the filedescriptor?)
    // TODO: Better error handling.
    //////////////////

    // First check if Port is set:
    if (port == "")
    {
        printf("No port was set!\n");
        return;
    }

    // Ok, port was written. Do we have a baud rate set?
    if (baudRate == B0)
    {
        printf("No Baud Rate set!\n");
        return;
    }

    // Both port and baudrate was set - set up the port:
    boardCom = openSerialPort(port.c_str(), baudRate);

    // Check valid port:
    if (boardCom < 0)
    {
        printf("Something wrong with the file descriptor\n");
        return;
    }

    // We should be good now. Run the receiver thread
    receiverThread = std::thread(&ComBase::messageReceiver, this);
}


/**************************************************************************
 * @brief This is the pulbic method that others will use to get stuff sent
 * 
 * @param command 
 ***************************************************************************/
void ComBase::send(const char *command)
{
    std::lock_guard<std::mutex> lock(writeMutex); // Automatically locks the mutex for current scope.
    write(boardCom, command, strlen(command));  // Note, strlen does not include the null terminator.
}

void ComBase::send(const char *command, size_t commandLength)
{
    std::lock_guard<std::mutex> lock(writeMutex); // Automatically locks the mutex for current scope.
    write(boardCom, command, commandLength);
}

// TODO seems we need some buffer clearing - when program starts, something is sent...