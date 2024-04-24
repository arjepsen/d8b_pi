/*
  ==============================================================================

    DspWriterClass.cpp
    Created: 25 Jun 2023 10:51:32pm
    Author:  anders

  ==============================================================================
*/

#include "DspComClass.h"
#include <unistd.h>

DspCom::DspCom() {}
DspCom::~DspCom() {}


// ################################################################################################
// This method is run in a thread. It is responsible for reading messages from the DSP board, and
// Putting them in the circular buffer singleton.
// ################################################################################################
void DspCom::messageReceiver()
{
    // Set up the file descriptor for the DSP port.
    // const int DSP = openSerialPort(getDspPort().c_str(), B115200);

    char recvChar = '\0';
    //std::string message = "";
    char message[BUFFER_WIDTH];
    size_t msgIndex = 0;
    int result;

    // Clear com buffer for starting out
    tcflush(boardCom, TCIOFLUSH);

    printf("Running dsp message loop\n");
    while (true)
    {
        result = read(boardCom, &recvChar, 1);

        if (result == 1)
        {
            //message += recvChar;
            message[msgIndex] = recvChar;
            msgIndex++;


            if (recvChar >= 'a' && recvChar <= 'z')
            {
                
                // Lower case letter received, message complete. push to buffer.
                message[msgIndex] = '\0';
                circBuffer.push(message, msgIndex);
                printf("DSP MESSAGE RECEIVED: %s\n", message);

                // Reset
                msgIndex = 0;
            }
        }

        else if (result < 0)
        {
            perror("Error reading from file descriptor");
            exit(1);
        }
        else if (result == 0)
        {
            // EOF
            // should any handling happen here?
        }
    }

    // This should not happen
    printf("DSP READER THREAD EXITED!!!!\n");
}