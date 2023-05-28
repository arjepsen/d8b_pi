/*
  ==============================================================================

    TapeSlotClass.h
    Created: 18 May 2023 9:55:51pm
    Author:  anders

  ==============================================================================
*/

// NOTE:
// The "tpdif" card is not implemented. I do not have such a card, 
// so I do not know what strings it would return.
// If someone has a card and can check this out, it would make it easy to implement.

#pragma once
#include "FXCardClass.h"

// Tape slot queries:
#define QUERY_TAPE_A "80p"
#define QUERY_TAPE_B "81p"
#define QUERY_TAPE_C "82p"
#define QUERY_ALTIO "83p"
#define QUERY_DIGI_SLOT "84p"
#define QUERY_CLOCK "80o"

// Card Identification Strings
#define AIO8_ID_STRING "AIO-8"
#define OPT8_ID_STRING "OPT-8"
#define AES_ID_STRING "AES/EBU"
#define MACKIE_CLOCK_ID_STRING "MACKIE"
#define APOGEE_CLOCK_ID_STRING "APOGEE"
#define EMPTY_IOSLOT_ID_STRING "empty"


enum IOSlotID
{
    TAPEIO_SLOT_A,
    TAPEIO_SLOT_B,
    TAPEIO_SLOT_C,
    TAPEIO_SLOT_ALTIO,
    CLOCK_IO_SLOT,
    DIGITAL_IO_SLOT
};


// IO slot class declaration.
class IOSlot
{
private:
    // TODO:
    // Should we use a secondary class as the io card itself?
    // Or simply host whatever functions are needed as member?
    // For now, using members - expand to class later if it makes sense.

    //IOCard *ioCard;
    
    std::string ioCardString;   // Long descriptsion - (C) 1997 Mackie Designs {Mackie Stereo AES/EBU interface}
    std::string ioCardID;   // Short description - OPT-8, AIO-8, etc..

    std::string getSlotResponse(int brainDescriptor);
    std::string decodeString(const std::string &input_string);

public:
    // Constructor. Provide filedescriptor for the Brain, and the slotID to query
    IOSlot(int brainDescriptor, IOSlotID slotIDtoQuery);
    ~IOSlot();

    std::string getCardString();
    std::string getCardID();
};


// IO Card class declaration
class IOCard
{
private:
    std::string type;

public:
    // Constructor
    IOCard(std::string card_type) : type{card_type} {}

    std::string get_type()
    {
        return type;
    }
};