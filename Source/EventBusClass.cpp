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


/**
 * Constructor for the event bus.
 * TODO: give it a thorough description
 */
EventBus::EventBus() : channelStripButtonBase{
                           // Base button index of channelstrips
                           0x051, 0x059, 0x041, 0x049, 0x031, 0x039, // 1 - 6
                           0x021, 0x029, 0x011, 0x019, 0x001, 0x009, // 7 - 12
                           0x179, 0x161, 0x169, 0x151, 0x159, 0x141, // 13 - 18
                           0x149, 0x131, 0x139, 0x121, 0x129, 0x111  // 19 - 24
                       }
{
    DEBUG_MSG("EVENTBUS CONSTRUCTOR\n");

    //Set the initial bank to Line Bank
    currentBank = LINE_BANK;
    // currentBankCallbacks = &lineBankCallbacks;
    // setBankPoster(LINE_BANK);

    initializeButtonCallbackMaps();

    // Write empty unsubscription callbacks into the unsubscriptions array
    // to avoid errors first time it is called (since it's called before
    // the new callbacks are written ...)
    UnSubscribeCallback emptyCallback = [](Bank, ChStripID) {};
    for (auto& bank : unSubScribeCallbackArray)
    {
        for (auto& unSubScribeCallback : bank)
        {
            unSubScribeCallback = emptyCallback;
        }
    }
    DEBUG_MSG("END OF EVENTBUS CONSTRUCTOR\n");
}

// Destructor
EventBus::~EventBus() {}

//////////////////////////////////////////////////////////////////

/*******************************************************************************
 * @brief This method is used by the channel objects, to "subscribe" to events,
 *        i.e. to write in their callback methods.
 *        First it calls the previously entered "unsubscribe" callback, which
 *        lets the channel update it's channelstrip associations.
 * 
 * @param bank 
 * @param chStripID 
 * @param callbacks This is a structure of all the callbacks for a channelstrip.
 *******************************************************************************/
void EventBus::channelStripEventSubscribe(Bank bank, ChStripID chStripID, ChannelStripCallbacks& callbacks) 
{
    // First, call the unsubscribe callback, to let the channel update
    // it's channelstrip association bitmap.
    unSubScribeCallbackArray[bank][chStripID](bank, chStripID);


    // Next, write in the new callbacks.
    faderCallbackArray[bank][chStripID] = callbacks.faderCallback;
    vPotCallbackArray[bank][chStripID] = callbacks.vPotCallback;

    // Get the button base id, for looking up the button ID's
    int buttonBase = channelStripButtonBase[chStripID];

    // Write in the button callbacks, using the button enumeration for map key.
    buttonCallbackMap[bank][buttonBase + MUTE_BTN] = callbacks.muteBtnCallback;
    buttonCallbackMap[bank][buttonBase + SOLO_BTN] = callbacks.soloBtnCallback;
    buttonCallbackMap[bank][buttonBase + SELECT_BTN] = callbacks.selectBtnCallback;
    buttonCallbackMap[bank][buttonBase + WRITE_BTN] = callbacks.writeBtnCallback;
    buttonCallbackMap[bank][buttonBase + ASSIGN_BTN] = callbacks.assignBtnCallback;
    buttonCallbackMap[bank][buttonBase + REC_RDY_BTN] = callbacks.recRdyBtnCallback;
}

void EventBus::channelStripComponentsubscribe(Bank bank, ChStripID stripID, UiStripCallbacks callbacks)
{
    associateUiFaderCallbackArray[bank][stripID] = callbacks.uiFaderCallback;
    associateUiVpotCallbackArray[bank][stripID] = callbacks.uiVpotCallback;
}




/*****************************************************************************
 * @brief This method is for the master channel object to subscribe to master
 *        fader events. No unsubscribe, since this should not change.
 *        (We don't want to move the master fader to another strip, do we?)
 * 
 * @param masterFaderCallback 
 *****************************************************************************/
void EventBus::masterFaderEventSubscribe(FaderCallback masterFaderCallback)
{
    // The master fader is called on all banks.
    faderCallbackArray[LINE_BANK][MASTER_CH_STRIP] = masterFaderCallback;
    faderCallbackArray[TAPE_BANK][MASTER_CH_STRIP] = masterFaderCallback;
    faderCallbackArray[EFFECTS_BANK][MASTER_CH_STRIP] = masterFaderCallback;
    faderCallbackArray[MASTERS_BANK][MASTER_CH_STRIP] = masterFaderCallback;
}


void EventBus::masterUiFaderSubscribe(MasterUiFaderCallback faderCallback)
{
    masterUiFaderCallback = faderCallback;
}

// BUT - A SUBSCRIPTION FOR A STRIP IS FOR ALL THREE CONTROLS....
// YES - BUT THEN WHAT ABOUT NON-CHANNELSTRIP?
// OK, ONE SUBSCRIBE METHOD FOR CHANNELS. ANOTHER FOR THE MASTER SECTION.


/******************************************************************************
 * @brief This is an event post method, used to call the callbacks for updating
 *      the UI. So this is only cosmetical - nothing sent to the console.
 *
 * @param chStripBitMask An integer. This bitmap is used as an index of which
 *                      channelStrips are associated with a given channel.
 *                      We iterate over this bitmap to index an array of
 *                      callbacks for the UI Channelstrip Component, which then
 *                      updates the faders in the UI.
 *
 * @param faderValue    The value the fader should go to in the UI.
 *****************************************************************************/
void EventBus::associateUiStripFaderEventPost(int chStripBitMask, const char (&faderValue)[2])
{
    // Iterate through the bits in the mask, use them for indexing the callbacks.
    while (chStripBitMask)
    {
        // Get the index of the lowest set bit.
        int stripID = __builtin_ctz(chStripBitMask);

        // Use the index to look up the callback in the array.
        // (So this will update the relevant faders, on the currently chosen bank.)
        associateUiFaderCallbackArray[currentBank][stripID](faderValue);

        // Clear lowest set bit
        chStripBitMask &= chStripBitMask - 1;
    }
}

void EventBus::associateUiStripVpotEventPost(int chStripBitMask, int vPotValue)
{
    // Since this is an associate event, all we do here is iterate
    // Over the ui strips, and update the pot.
    // We don't handle difference in vpot functionality here.
    // So otherwise same procedure as for faders.

    while (chStripBitMask)
    {

        int stripID = __builtin_ctz(chStripBitMask);
        associateUiVpotCallbackArray[currentBank][stripID](vPotValue);
        chStripBitMask &= chStripBitMask - 1;
    }
}

void EventBus::updateUiMasterFaderEventPost(int faderValue)
{
    masterUiFaderCallback(faderValue);
}

// ##################################################################################
// This method is used to change poster method, depending on which bank is selected.
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
    // Initialize the buttonCallback maps with button ID's
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

