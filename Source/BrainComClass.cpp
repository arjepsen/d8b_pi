/*
  ==============================================================================

    BrainWriterClass.cpp
    Created: 25 Jun 2023 10:52:04pm
    Author:  anders

  ==============================================================================
*/

#include "BrainComClass.h"
#include "Debug.h"
#include <stdexcept>
#include <unistd.h>

// Define action codes
#define ACTION_NONE 0
#define ACTION_TERMINATOR 1
#define ACTION_HEARTBEAT 2
#define ACTION_APOGEE_HEARTBEAT 3

BrainCom::BrainCom()
{
    // Populate the action table with actions
    actionTable['f'] = ACTION_TERMINATOR; // End of fader message
    actionTable['v'] = ACTION_TERMINATOR; // End of vPot message
    actionTable['u'] = ACTION_TERMINATOR; // end of button down message
    actionTable['s'] = ACTION_TERMINATOR; // End of button up message
    actionTable['o'] = ACTION_TERMINATOR; // End of apogee heartbeat ("F8o")
    actionTable['K'] = ACTION_TERMINATOR; // End of "system" message? (Used by fader calibrations.)

    actionTable['l'] = ACTION_HEARTBEAT;
    actionTable['k'] = ACTION_HEARTBEAT;
    // TODO: others?
}

BrainCom::~BrainCom() {}

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

            // Use lookup table for determining action, instead of checking every possibility
            uint8_t action = actionTable[(unsigned char)recvChar];
            switch (action)
            {
                case ACTION_TERMINATOR:
                    message[msgIndex] = '\0';       // Terminate string.
                    circBuffer.push(message, msgIndex); // Push to buffer.
                    printf("BRAIN MESSAGE RECEIVED: %s\n", message);
                    msgIndex = 0;                       // Reset index for the next message
                    break;

                case ACTION_HEARTBEAT:
                    // TODO: Set up heartbeat handling
                    msgIndex --;    // Decrement to overwrite heartbeat character.
                    heartbeatReceived();
                    break;

                default:
                    // Char was part of message, and is already written. 
                    break;

            }

            // // TODO: maybe we can optimize this? Might not need everything from a to z?
            // // A lower case letter means message complete. Push, and reset.
            // if (recvChar >= 'a' && recvChar <= 'z')
            // {
            //     // We might receive heartbeat in the middle of a message.
            //     // In that case, don't increment index, or reset.
            //     if (recvChar == 'l' || recvChar == 'k')
            //     {
            //         // DEBUG_MSG("hearbeat: %c\n", recvChar);
            //         msgIndex--; // Decrement, to erase from messages.
            //         heartbeatReceived();
            //     }
            //     else
            //     {
            //         // Full message received. Add null terminator.
            //         message[msgIndex] = '\0';
            //         circBuffer.push(message, msgIndex); // Push message to the circular buffer.

            //         // Reset index for next message
            //         msgIndex = 0;
            //     }
            // }
        }

        // TODO:
        // else if (result == 0) // 0 chars recevied - EOF
        // {
        //     // Should we add any special functionality here??
        // }

        else if (result < 0)
        {
            perror("Error reading from file descriptor");
            exit(1);
        }
    }

    // We probably shouldn't get here... but just in case.
    DEBUG_MSG("BRAIN LOOP EXITED!!\n");
}
