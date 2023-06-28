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
        printf("ERROR - thread already running!");
        exit(1);
    }

    // Clear screen before entering loop.
    write(boardCom, "01u", 3);
    usleep(20000);

    char recvChar = '\0';
    std::string message = "";
    int result;

    // Clear com buffer for starting out
    tcflush(boardCom, TCIOFLUSH);

    printf("Running brain message loop\n");
    // Run the infinite loop.
    while (true)
    {
        result = read(boardCom, &recvChar, 1);

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
                    heartbeatReceived();
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
    printf("BRAIN LOOP EXITED!!\n");
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

