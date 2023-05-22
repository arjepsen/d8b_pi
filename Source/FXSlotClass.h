#pragma once

//#include "FXCardClass.h"
#include <string>

enum FXSlotID
{
    FX_SLOT_A,
    FX_SLOT_B,
    FX_SLOT_C,
    FX_SLOT_D
};

enum FXCardID
{
    FX_EMPTY,
    MFX_CARD,
    UFX_CARD
};

// FX slot class declaration.
class FXSlot
{
private:
    // name?   (A, B, C, D) Maybe the enumeration?
    // card-object

    //FXCard *fxcard;
    //std::string fxCardID;
    FXCardID fxCardID;


    std::string getBrainResponse(int brainDescriptor);

public:
    // Constructor
    FXSlot(int brainDescriptor, FXSlotID slotID);
    //~FXSlot();

    // getter for the card object? or just the name?
    // init, which queries the slot, creates a card object, and puts it in private storage.
    //std::string get_card();
    FXCardID getCardID();
};