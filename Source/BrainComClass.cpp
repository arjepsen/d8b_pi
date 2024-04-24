/*
  ==============================================================================

    BrainWriterClass.cpp
    Created: 25 Jun 2023 10:52:04pm
    Author:  anders

  ==============================================================================
*/

#include "BrainComClass.h"
//#include <fcntl.h>
#include <stdexcept>
#include <unistd.h>

BrainCom::BrainCom(){}
BrainCom::~BrainCom(){}


// UNCOMMENT TO ENABLE DEBUG MESSAGES.
#define BRAINCOM_DEBUG_MESSAGES

#ifdef BRAINCOM_DEBUG_MESSAGES
#define DEBUG_MSG(format, ...) printf("BRAINCOM_DBG: " format, ##__VA_ARGS__)
#else
#define DEBUG_MSG(format, ...) ((void)0) // do {} while (0)
#endif

// // #######################################
// // Method for flushing the receiver buffer
// // #######################################
// void BrainCom::flushBuffer()
// {
//     std::lock_guard<std::mutex> lock(brainWriteMutex); // Automatically locks the mutex for current scope.
//     tcflush(brain, TCIOFLUSH);
// }


// ################################################################################################
// This method is run in a thread. It is responsible for reading messages from the Brain board, and
// Putting them in the circular buffer singleton.
// ################################################################################################
void BrainCom::messageReceiver()
{
    // Make sure the thread hasn't already been started - only one thread allowed!
    if (receiverThreadRunning)
    {
        DEBUG_MSG("ERROR - thread already running!");
        exit(1);
    }

    // Clear screen before entering loop.
    write(boardCom, "01u", 3);
    usleep(20000);

    // Set up a single char receiver variable (we receive one byte at a time)
    char recvChar = '\0';

    // Set up the message string buffer - use width defined in circular buffer.
    //std::string message = "";
    char message[BUFFER_WIDTH];

    // Set up a variable for which index to write next char to in the message array.
    size_t msgIndex = 0;

    // Variable for result of reading/receiving operation.
    int result;

    // Clear com buffer for starting out
    tcflush(boardCom, TCIOFLUSH);

    DEBUG_MSG("Running brain message loop\n");
    // Run the infinite loop.
    while (true)
    {
        result = read(boardCom, &recvChar, 1);

        if (result == 1) // One char was recevied.
        {
            // Write the received char at the given index.
            message[msgIndex] = recvChar;

            // Increment index
            msgIndex++;

            // TODO: maybe we can optimize this? Might not need everything from a to z?
            // A lower case letter means message complete. Push, and reset.
            if (recvChar >= 'a' && recvChar <= 'z')
            {
                // We might receive heartbeat in the middle of a message.
                // In that case, don't increment index, or reset.
                if (recvChar == 'l' || recvChar == 'k')
                {
                    DEBUG_MSG("hearbeat: %c\n", recvChar);
                    msgIndex--; // Decrement, to erase from messages.
                    heartbeatReceived();
                }
                else
                {
                    // Full message received. Add null terminator.
                    message[msgIndex] = '\0';
                    circBuffer.push(message, msgIndex); // Push message to the circular buffer.

                    // Reset index for next message
                    msgIndex = 0;
                }

                // Message was handled, reset for next message.
                //message = "";
            }
        }

        else if (result == 0) // 0 chars recevied - EOF
        {
            // Should we add any special functionality here??
        }

        else if (result < 0)
        {
            perror("Error reading from file descriptor");
            exit(1);
        }
    }

    // We probably shouldn't get here... but just in case.
    DEBUG_MSG("BRAIN LOOP EXITED!!\n");
}

// ##################################################################################
// This method is responsible for what happens when a heart beat (l or k) is recevied
// ##################################################################################
void BrainCom::heartbeatReceived()
{
    // empty for now - maybe send the L / 0???
}

// void BrainCom::initializeBrainCom()
// {
// 	// Check that port was set.
// 	if (brainPort == "")
// 	{
// 		printf("PORT NOT SET!");
// 		exit(1);
// 	}

// 	brain = openSerialPort(brainPort.c_str(), brainBoostState ? B230400 : B115200);
// }

