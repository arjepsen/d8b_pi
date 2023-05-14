/*
  ==============================================================================

    MixerInitScripts.cpp
    Created: 13 May 2023 11:15:17pm
    Author:  anders

  ==============================================================================
*/

#include "MixerInitScripts.h"
#include <fcntl.h>


void initializeMixer()
{

    // Get access to the mixerManager singleton:
    MixerManager &mixerManager = MixerManager::getInstance();
    
    // Fetch the com ports:
    const char *BRAIN_DEVICE = "/dev/ttyUSB0";

    const int SLOW_BRAIN = open_serial_port(BRAIN_DEVICE, B115200);
}

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