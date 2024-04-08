/*
  ==============================================================================

    TapeSlotClass.cpp
    Created: 18 May 2023 9:55:51pm
    Author:  anders

  ==============================================================================
*/

#include "IOSlotClass.h"
#include "CommandStrings.h"
#include <map>
#include <sstream>
#include <unistd.h>
#include <iostream>


// UNCOMMENT TO ENABLE DEBUG MESSAGES.
#define IO_INIT_DEBUG_MESSAGES

#ifdef IO_INIT_DEBUG_MESSAGES
#define DEBUG_MSG(format, ...) printf("IO_INIT_DBG: " format, ##__VA_ARGS__)
#else
#define DEBUG_MSG(format, ...) ((void)0) // do {} while (0)
#endif

// Constructor (initialization list)
IOSlot::IOSlot(int brainDescriptor, IOSlotID slotIDtoQuery) //: ioCard(nullptr)
{
    DEBUG_MSG("\n======== Start IO constructor ========\n");
    // Query the specified slot
    switch (slotIDtoQuery)
    {
        case TAPEIO_SLOT_A:
            write(brainDescriptor, QUERY_TAPE_A, 3);
            break;
        case TAPEIO_SLOT_B:
            write(brainDescriptor, QUERY_TAPE_B, 3);
            break;
        case TAPEIO_SLOT_C:
            write(brainDescriptor, QUERY_TAPE_C, 3);
            break;
        case TAPEIO_SLOT_ALTIO:
            write(brainDescriptor, QUERY_ALTIO, 3);
            break;
        case DIGITAL_IO_SLOT:
            write(brainDescriptor, QUERY_DIGI_SLOT, 3);
            break;
        case CLOCK_IO_SLOT:
            write(brainDescriptor, QUERY_CLOCK, 3);
            break;
        default:
            perror("that tape slot does not exist.");
            exit(1);
    }

    // Get the response (This will be a ascii hex string with a p or o delimiter)
    std::string ioSlotResponse = getSlotResponse(brainDescriptor);

    // Convert to regular text, and save member.
    ioCardString = decodeString(ioSlotResponse);
    DEBUG_MSG("IO card found:\n  ---> %s\n", ioCardString.c_str());

    // Set up short Card-ID
    if (ioCardString == "")
        ioCardID = EMPTY_IOSLOT_ID_STRING;
    else
    {
        // Map structure used in for loop.
        std::map<std::string, std::string> cardIdMap
        {
            {"Analog", AIO8_ID_STRING},         // (C) 1997 Mackie Designs {Mackie 8-channel 20-bit Analog interface}
            {"Opt-8", OPT8_ID_STRING},          // (C) 2000 Mackie Designs {Mackie Opt-8 24-Bit ADAT Light Pipe I/O}
            {"AES", AES_ID_STRING},             // (C) 1997 Mackie Designs {Mackie Stereo AES/EBU interface}
            {"Clock", MACKIE_CLOCK_ID_STRING},  // (C) 1997 Mackie Designs {Mackie Clock Card}
            {"Apogee", APOGEE_CLOCK_ID_STRING} // (C) 1997 Mackie Designs {Apogee Low Jitter Sync Card}
        };

        // Iterate over the IO slot reply string, use map to set ID.
        for (const auto card : cardIdMap)
        {
            if (ioCardString.find(card.first) != std::string::npos)
            {
                ioCardID = card.second;
                break;
            }
        }
    }
    DEBUG_MSG("IO card ID: %s\n", ioCardID.c_str());
    DEBUG_MSG("\n      === End of constructor ===\n\n");
}

// ####################################################################
// This is a localized method for getting a message from the Brain.
// The "result" is 1 if one char is read, 0, if end of file, and -1 for error.
// End of message would be either EOF, or receving an "l" or a "k"
// ####################################################################
std::string IOSlot::getSlotResponse(int brainDescriptor)
{
    DEBUG_MSG("-- Gettings slot response....  \n");
    char response = '\0';
    std::stringstream brainReplyStream;

    while (true)
    {
        int result = read(brainDescriptor, &response, 1);

        // If "l" or "k" (heartbeat) it's end of message.
        if (response == 'l' || response == 'k')
        {
            DEBUG_MSG("Got heartbeat - end of message.\n");
            break;
        }

        // Check if we received a response char
        if (result == 1)
            brainReplyStream.put(response);
        else if (result < 0)
        {
            printf("ERROR querying tape slot: Read returned: %d\n", result);
            exit(1);
        }
        else if (result == 0)
        {
            DEBUG_MSG("\nEOF\n");
            break;
        }
    }

    return brainReplyStream.str();
}

// ################################################################################
// Method for decoding the ascii hex outputs
// ################################################################################
std::string IOSlot::decodeString(const std::string &input_string)
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

std::string IOSlot::getCardString()
{
    return ioCardString;
}

std::string IOSlot::getCardID()
{
    return ioCardID;
}