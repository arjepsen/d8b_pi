#pragma once
#include "FXCardClass.h"

enum FXSlotID
{
    FX_SLOT_A,
    FX_SLOT_B,
    FX_SLOT_C,
    FX_SLOT_D
};

// FX slot class declaration.
class FXSlot
{
private:
    // name?   (A, B, C, D) Maybe the enumeration?
    // card-object

    FXCard *fxcard;

    std::string getBrainResponse(int brainDescriptor);

public:
    // Constructor
    FXSlot(int brainDescriptor, FXSlotID slotID);
    ~FXSlot();

    // getter for the card object? or just the name?
    // init, which queries the slot, creates a card object, and puts it in private storage.
    std::string get_card();
};