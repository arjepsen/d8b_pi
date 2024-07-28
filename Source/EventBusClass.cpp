/*
  ==============================================================================

    EventBusClass.cpp
    Created: 11 Jun 2023 8:59:37pm
    Author:  anders

  ==============================================================================
*/

#include "EventBusClass.h"
#include <stdexcept>
#include "Debug.h"


/*****************************************************************************
 * @brief Constructor for the EventBus.
 *        "MainComponent" is the first class to instantiate this singleton,
 *        so this constructor is run on program startup, after the channel
 *        and UI/console channelstrip objects have been instantiated in 
 *        arrays. Then after that, the MainComponent constructor will
 *        call methods here in EventBus, to hand over pointers to the objects.
 *****************************************************************************/
EventBus::EventBus() : channelStripButtonBase{
                           // Base button index of channelstrips
                           0x051, 0x059, 0x041, 0x049, 0x031, 0x039, // 1 - 6
                           0x021, 0x029, 0x011, 0x019, 0x001, 0x009, // 7 - 12
                           0x179, 0x161, 0x169, 0x151, 0x159, 0x141, // 13 - 18
                           0x149, 0x131, 0x139, 0x121, 0x129, 0x111  // 19 - 24
                       }
{
    //Set the initial bank to Line Bank
    currentBank = LINE_BANK;

    // Register master strip in the association bitmap (The regular strips are handled with "chStripEventSubscribe")
    channelAssociationBitmaps[CH_STRIP_MASTER][LINE_BANK] |= 1 << CH_STRIP_MASTER;
    channelAssociationBitmaps[CH_STRIP_MASTER][TAPE_BANK] |= 1 << CH_STRIP_MASTER;
    channelAssociationBitmaps[CH_STRIP_MASTER][EFFECTS_BANK] |= 1 << CH_STRIP_MASTER;
    channelAssociationBitmaps[CH_STRIP_MASTER][MASTERS_BANK] |= 1 << CH_STRIP_MASTER;

    // TODO: give a good comment for this.
    initializeButtonCallbackMaps();

}

// Destructor
EventBus::~EventBus() {}



// TODO: BUT - A SUBSCRIPTION FOR A STRIP IS FOR ALL THREE CONTROLS....
// YES - BUT THEN WHAT ABOUT NON-CHANNELSTRIP?
// OK, ONE SUBSCRIBE METHOD FOR CHANNELS. ANOTHER FOR THE MASTER SECTION.


// NEW WAY (JUNE'24) USING BITMAPS FOR TRACKING SUBSCRIPTION
void EventBus::channelStripEventSubscribe(int chArrayIndex, ChStripID channelStripID, Bank associationBank)
{
    // Set the bit in the map
    channelAssociationBitmaps[channelStripID][associationBank] |= 1 << chArrayIndex;

    // Call the channelStrip to set it's stuff.
    channelStripArray[channelStripID]->setChannelAssociation(currentBank, associationBank, &channelArray[chArrayIndex]);
}


// ##########################################################################################
// ############################### POSTERS ##################################################
// ##########################################################################################

void EventBus::postFaderEvent(ChStripID channelStripID, char (&eventValue)[2], EventSource source)
{
    // First, let the channelstrip call the channel for sending DSP command.
    //(channelStripArray[channelStripID]->*faderEventHandlers[channelStripID])(currentBank, eventValue);

    channelStripArray[channelStripID]->updateChannelVolume(currentBank, eventValue);

    // Prepare bitmaps for indexing all associated strips, excluding the "caller".
    uint32_t fullBitmap = channelAssociationBitmaps[channelStripID][currentBank];
    uint32_t reducedBitmap = fullBitmap & ~(1 << channelStripID);

    // Next, we must move all associated fader, including the identical strip on the "opposite" (UI/Console).
    // Use the fact that source is an enum (Console=0, UI=1), to set up bitmaps. (Avoids using conditionals.)
    // So the idea is: "source" is either 0 or 1, so all it's bits will be 0, except the last, which can be 1 or 0.
    // We want to create two bitmaps of all 1's or all 0's.
    // For mask1, we first invert, then add 1. If source was 0, then mask1 is all 0's, if source was 1, it's now all 1's.
    // For mask2, we simply invert mask1.

    uint32_t mask1 = ~source + 1;
    uint32_t mask2 = ~mask1; //(~source ^ 1) + 1;

    // Use them to set the correct bitmasks, so the "caller" is not moved.
    uint32_t consoleBitmap = (fullBitmap & mask1) | (reducedBitmap & mask2);
    uint32_t uiBitmap = (fullBitmap & mask2) | (reducedBitmap & mask1);

    // Now iterate using __builtin_ctz to count trailing zeros.
    while (consoleBitmap)
    {
        int stripID = __builtin_ctz(consoleBitmap);  // Get lowest set bit.
        channelStripArray[stripID]->updateFaderPosition(currentBank);
        consoleBitmap &= consoleBitmap - 1; // Clear lowest set bit.
    }

    // Do the same for the UI faders
    while (uiBitmap)
    {
        int stripID = __builtin_ctz(uiBitmap);
        channelStripComponentArray[stripID]->faderMoveEventCallback(eventValue);
        uiBitmap &= uiBitmap - 1;
    }
}

void EventBus::postVpotEvent(ChStripID channelStripID, int eventValue, EventSource source)
{
    // First let the channelStrip call the channel to update DSP.
    // We must supply the currently selected vPot functionality.
    // Since console sends a change and UI a specific value, this returns the specific.
    int newValue = channelStripArray[channelStripID]->vPotEventHandler(currentBank, eventValue, currentVpotFunction, source);

    // Update LED's for all associated pots.
    uint32_t associationBitmap = channelAssociationBitmaps[channelStripID][currentBank];
    while (associationBitmap)
    {
        int stripID = __builtin_ctz(associationBitmap);  // Get lowest set bit for indexing array.

        // Call update methods for console and UI
        channelStripArray[stripID]->updateVpotLeds(currentBank, currentVpotFunction);
        channelStripComponentArray[stripID]->vPotTurnEventCallback(newValue, currentVpotFunction);

        associationBitmap &= associationBitmap - 1;
    }
}



// ##################################################################################
// ###################################### OTHERS ####################################
// ##################################################################################
void EventBus::setCurrentBank(Bank bank)
{
    // Set the currentBank member
    currentBank = bank;

    // // Update the callback map pointer.
    // switch (currentBank)
    // {
    //     case LINE_BANK:
    //         currentBankCallbacks = &lineBankCallbacks;
    //         break;
    //     case TAPE_BANK:
    //         currentBankCallbacks = &tapeBankCallbacks;
    //         break;
    //     case EFFECTS_BANK:
    //         currentBankCallbacks = &effectsBankCallbacks;
    //         break;
    //     case MASTERS_BANK:
    //         currentBankCallbacks = &mastersBankCallbacks;
    //         break;
    //     default:;
    //         // Handle invalid bank input.
    // }
}

Bank EventBus::getCurrentBank()
{
    return currentBank;
}

void EventBus::initializeButtonCallbackMaps()
{
    // Initialize the buttonCallback maps with button ID's.
    // clang-format off
    buttonCallbackMap[LINE_BANK] = {
        // ================== CHANNELSTRIP 1-12 BUTTONS =============================
        // mute, solo, select, write, assing, recRdy
        {0x001, {}}, {0x002, {}}, {0x003, {}}, {0x004, {}}, {0x005, {}}, {0x006, {}},
        // ChannelStrip 12
        {0x009, {}}, {0x00A, {}}, {0x00B, {}}, {0x00C, {}}, {0x00D, {}}, {0x00E, {}},
        // ChannelStrip 9
        {0x011, {}}, {0x012, {}}, {0x013, {}}, {0x014, {}}, {0x015, {}}, {0x016, {}},
        // ChannelStrip 10
        {0x019, {}}, {0x01A, {}}, {0x01B, {}}, {0x01C, {}}, {0x01D, {}}, {0x01E, {}},
        // ChannelStrip 7
        {0x021, {}}, {0x022, {}}, {0x023, {}}, {0x024, {}}, {0x025, {}}, {0x026, {}},
        // ChannelStrip 8
        {0x029, {}}, {0x02A, {}}, {0x02B, {}}, {0x02C, {}}, {0x02D, {}}, {0x02E, {}},
        // ChannelStrip 5
        {0x031, {}}, {0x032, {}}, {0x033, {}}, {0x034, {}}, {0x035, {}}, {0x036, {}},
        // ChannelStrip 6
        {0x039, {}}, {0x03A, {}}, {0x03B, {}}, {0x03C, {}}, {0x03D, {}}, {0x03E, {}},
        // ChannelStrip 3
        {0x041, {}}, {0x042, {}}, {0x043, {}}, {0x044, {}}, {0x045, {}}, {0x046, {}},
        // ChannelStrip 4
        {0x049, {}}, {0x04A, {}}, {0x04B, {}}, {0x04C, {}}, {0x04D, {}}, {0x04E, {}},
        // ChannelStrip 1
        {0x051, {}}, {0x052, {}}, {0x053, {}}, {0x054, {}}, {0x055, {}}, {0x056, {}},
        // ChannelStrip 2
        {0x059, {}}, {0x05A, {}}, {0x05B, {}}, {0x05C, {}}, {0x05D, {}}, {0x05E, {}},

        // ===================== MASTER VPOT SELECT & BANK SELECT BUTTONS ========================
        // Digital Trim, Aux2, Aux4, Aux6, Aux8, Aux11, AuxPan11-12
        {0x100, {}}, {0x101, {}}, {0x102, {}}, {0x103, {}}, {0x104, {}}, {0x105, {}}, {0x106, {}}, 
        // PanVpots, MASTER, MasterSolo, SHIFT MIC/LINE, TAPE IN, EFFECTS
        {0x107, {}}, {0x108, {}}, {0x109, {}}, {0x10A, {}}, {0x10B, {}}, {0x10C, {}}, {0x10D, {}},

        // ================== CHANNELSTRIP 24 BUTTONS =============================
        {0x111, {}}, {0x112, {}}, {0x113, {}}, {0x114, {}}, {0x115, {}}, {0x116, {}},

        // ======================== MASTER VPOT SELECT BUTTONS ===================================
        // Level To Tape, Aux1, Aux3, Aux5, Aux7, Aux9-10, AuxPan9-10
        {0x119, {}}, {0x11A, {}}, {0x11B, {}}, {0x11C, {}}, {0x11D, {}}, {0x11E, {}}, {0x11F, {}}, 

        // ================== CHANNELSTRIP 13-23 BUTTONS ============================
        // ChannelStrip 22
        {0x121, {}}, {0x122, {}}, {0x123, {}}, {0x124, {}}, {0x125, {}}, {0x126, {}},
        // ChannelStrip 23
        {0x129, {}}, {0x12A, {}}, {0x12B, {}}, {0x12C, {}}, {0x12D, {}}, {0x12E, {}},
        // ChannelStrip 20
        {0x131, {}}, {0x132, {}}, {0x133, {}}, {0x134, {}}, {0x135, {}}, {0x136, {}},
        // ChannelStrip 21
        {0x139, {}}, {0x13A, {}}, {0x13B, {}}, {0x13C, {}}, {0x13D, {}}, {0x13E, {}},
        // ChannelStrip 18
        {0x141, {}}, {0x142, {}}, {0x143, {}}, {0x144, {}}, {0x145, {}}, {0x146, {}},
        // ChannelStrip 19
        {0x149, {}}, {0x14A, {}}, {0x14B, {}}, {0x14C, {}}, {0x14D, {}}, {0x14E, {}},
        // ChannelStrip 16
        {0x151, {}}, {0x152, {}}, {0x153, {}}, {0x154, {}}, {0x155, {}}, {0x156, {}},
        // ChannelStrip 17
        {0x159, {}}, {0x15A, {}}, {0x15B, {}}, {0x15C, {}}, {0x15D, {}}, {0x15E, {}},
        // ChannelStrip 14
        {0x161, {}}, {0x162, {}}, {0x163, {}}, {0x164, {}}, {0x165, {}}, {0x166, {}},
        // ChannelStrip 15
        {0x169, {}}, {0x16A, {}}, {0x16B, {}}, {0x16C, {}}, {0x16D, {}}, {0x16E, {}},
        // ChannelStrip 13
        {0x179, {}}, {0x17A, {}}, {0x17B, {}}, {0x17C, {}}, {0x17D, {}}, {0x17E, {}},


        // ============================= MASTER SECTION BUTTONS ==================================
        // Load, Plugins, DigialIO, '4', '5', '9', '0', Locator
        {0x200, {}}, {0x201, {}}, {0x202, {}}, {0x203, {}}, {0x204, {}}, {0x205, {}}, {0x206, {}}, 
        // Locator, Snapshot, Play, Record, JogShuttle, Mono, Settime.
        {0x207, {}}, {0x208, {}}, {0x209, {}}, {0x20A, {}}, {0x20B, {}}, {0x20C, {}}, {0x20D, {}},
        // Save As, Group, General, '2', '3', '7', '8', Loop
        {0x210, {}}, {0x211, {}}, {0x212, {}}, {0x213, {}}, {0x214, {}}, {0x215, {}}, {0x216, {}}, {0x217, {}},
        // Store, FastFwd, Stop, Mutes, AutoTouch, All, TrimLevels, New
        {0x218, {}}, {0x219, {}}, {0x21A, {}}, {0x21B, {}}, {0x21C, {}}, {0x21D, {}}, {0x21E, {}}, {0x21F, {}},
        // Bus 5, Bus 6, Bus 7, Bus 8, L/R, Route To Tape, SMPTE View, '1' 
        {0x220, {}}, {0x221, {}}, {0x222, {}}, {0x223, {}}, {0x224, {}}, {0x225, {}}, {0x226, {}}, {0x227, {}},
        // '6', Enter, Rewind, Bypass, Faders, Fader Motors Off, Pan, Save
        {0x228, {}}, {0x229, {}}, {0x22A, {}}, {0x22B, {}}, {0x22C, {}}, {0x22D, {}}, {0x22E, {}}, {0x22F, {}},
        // NearField, Main, Dim, TalkBack, Cut / Zero Set, Copy, Paste, Undo
        {0x230, {}}, {0x231, {}}, {0x232, {}}, {0x233, {}}, {0x234, {}}, {0x235, {}}, {0x236, {}}, {0x237, {}},
        // Select, Control, Write, Alt, Bus 1, Bus 2, Bus 3, Bus 4
        {0x238, {}}, {0x239, {}}, {0x23A, {}}, {0x23B, {}}, {0x23C, {}}, {0x23D, {}}, {0x23E, {}}, {0x23F, {}},
        // Studio Level, Talkback Level, phone1: aux9-10, aux11-12, CopyToCue, CtrlRoom, phone2: aux9-10, aux11-12
        {0x240, {}}, {0x241, {}}, {0x242, {}}, {0x243, {}}, {0x244, {}}, {0x245, {}}, {0x246, {}}, {0x247, {}},
        // phone2: CopyToCue, CtrlRoom. | 2TrackA, 2TrackB, 2TrackC, DigitalIn1, DigitalIn2, Master L-R
        {0x248, {}}, {0x249, {}}, {0x24A, {}}, {0x24B, {}}, {0x24C, {}}, {0x24D, {}}, {0x24E, {}}, {0x24F, {}},
        // Setup, Memory A, Memory B, Next, EQ, Gate Compressor, Plugins
        {0x250, {}}, {0x251, {}}, {0x252, {}}, {0x253, {}}, {0x254, {}}, {0x255, {}}, {0x256, {}}, {0x257, {}},
        // LoadPatch, SavePatch, MixDownSolo, PflSolo, AflSolo, TalkBackStudio, Clear Solo, Solo Level.
        {0x258, {}}, {0x259, {}}, {0x25A, {}}, {0x25B, {}}, {0x25C, {}}, {0x25D, {}}, {0x25E, {}}, {0x25F, {}},
        // Select1, Select2, Select3, Select4, Help, Previous, On
        {0x268, {}}, {0x269, {}}, {0x26A, {}}, {0x26B, {}}, {0x26C, {}}, {0x26D, {}}, {0x26E, {}}, {0x26F, {}}

    };
    // clang-format on

    // Copy map to other banks:
    buttonCallbackMap[TAPE_BANK] = buttonCallbackMap[LINE_BANK];
    buttonCallbackMap[EFFECTS_BANK] = buttonCallbackMap[LINE_BANK];
    buttonCallbackMap[MASTERS_BANK] = buttonCallbackMap[LINE_BANK];
}

void EventBus::initializeChannels()
{
    for (int i = 0; i < CHANNEL_COUNT; i++)
    {
        channelArray[i].initializeChannel(currentVpotFunction);
    }
}


void EventBus::initializeChannelStrips()
{
    for (int i = 0; i < CHANNEL_STRIP_COUNT + 1; i++)
    {
        channelStripArray[i]->updateChStrip(currentBank);
    }
}

void EventBus::initializeUiStrips()
{
    // Set all UI faders to an initial position.
    // TODO: eventually initialposition should be read from a file.
    const char initialFaderPos[2] = {'0', '0'};
    for (int i = 0; i < CHANNEL_STRIP_COUNT + 1; i++)   // Includes master strip.
    {
        channelStripComponentArray[i]->faderMoveEventCallback(initialFaderPos);

        // Get vPot value
        int vPotValue = channelStripArray[i]->getChannelStripVpotValue(currentBank, currentVpotFunction);
        
        channelStripComponentArray[i]->vPotTurnEventCallback(vPotValue, currentVpotFunction);
    }


    //TODO: Also set the pot position, and buttons....
    // TODO: maybe this should be done with a more regular method?
}

void EventBus::setChannelStripComponentArray(ChannelStripComponent * chStripCompArray, MasterStripComponent * masterComponentPtr)
{
    for (int i = 0; i < CHANNEL_STRIP_COUNT; i++)
    {
        channelStripComponentArray[i] = &chStripCompArray[i];
    }

    channelStripComponentArray[CHANNEL_STRIP_COUNT] = masterComponentPtr;
}

// Set up our array of pointers to channelstrip objects, including the master strip.
void EventBus::setChannelStripArray(ChannelStrip * chStripArray, MasterChannel * masterChannelPtr)
{
    //channelStripArray = chStripArray;
    for (int i = 0; i < CHANNEL_STRIP_COUNT; i++) 
    {
        channelStripArray[i] = &chStripArray[i]; // Point to each ChannelStrip object
    }
    channelStripArray[CHANNEL_STRIP_COUNT] = masterChannelPtr; // Point to the MasterChannelStrip object

}

void EventBus::setChannelArray(Channel * chArray)
{
    channelArray = chArray;
}

void EventBus::enableUiListeners()
{
    for (int i = 0; i < CHANNEL_STRIP_COUNT + 1; i++)   // include the master strip.
    {
        channelStripComponentArray[i]->activateEventListeners();
    }
}


void EventBus::saveSettings() 
{

}

void EventBus::loadSettings()
{
    // Load settings into a struct.
    // Then write the various parts into their thingimabobs.

    // for now:
    initializeChannels();   // Maybe this method should write in the settings?
    initializeUiStrips();
    initializeChannelStrips();

    // TODO: Also handle the Master Section LED's
}