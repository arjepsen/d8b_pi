/*
  ==============================================================================

    TapeSlotClass.h
    Created: 18 May 2023 9:55:51pm
    Author:  anders

  ==============================================================================
*/

#pragma once
#include "FXCardClass.h"

enum IOSlotID
{
    TAPEIO_SLOT_A,
    TAPEIO_SLOT_B,
    TAPEIO_SLOT_C,
    ALTIO_SLOT,
    DIGITAL_IO_SLOT,
    CLOCK_IO_SLOT
};

// IO slot class declaration.
class IOSlot
{
private:
    // name?   (A, B, C, D) Maybe the enumeration?
    // card-object

    IOCard *iocard;

    std::string getBrainResponse(int brainDescriptor);

public:
    // Constructor
    IOSlot(int brainDescriptor, IOSlotID slotID);
    ~IOSlot();

    // getter for the card object? or just the name?
    // init, which queries the slot, creates a card object, and puts it in private storage.
    std::string get_card();
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