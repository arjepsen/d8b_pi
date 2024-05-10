/*
  ==============================================================================

    ButtonLookupTable.h
    Created: 12 Apr 2024 8:46:26pm
    Author:  anders

  ==============================================================================
*/

#pragma once
#include <functional>
#include <unordered_map>
#include <string>

enum ButtonType
{
    // ChannelStrip buttons
    MUTE_BTN,
    SOLO_BTN,
    SELECT_BTN,
    WRITE_BTN,
    ASSIGN_BTN,
    REC_RDY_BTN,
    MASTER_SECTION_BTN
};

enum ButtonAction : char
{
    BTN_PRESS = 's',
    BTN_RELEASE = 'u'
};


// Enum to use for indexing the event bus' array of button callbacks.
// The numbers assigned are listed in hex, since this is the values
// that the console sends as chars.
enum ChStripButtonID
{
    MUTE_BTN1 = 0x051, MUTE_BTN2 = 0x059, MUTE_BTN3 = 0x041, MUTE_BTN4 = 0x049, 
    MUTE_BTN5 = 0x031, MUTE_BTN6 = 0x039, MUTE_BTN7 = 0x021, MUTE_BTN8 = 0x029,
    MUTE_BTN9 = 0x011, MUTE_BTN0 = 0x019, MUTE_BTN11 = 0x001, MUTE_BTN12 = 0x009,
    MUTE_BTN13 = 0x179, MUTE_BTN14 = 0x161, MUTE_BTN15 = 0x169, MUTE_BTN16 = 0x151,
    MUTE_BTN17 = 0x159, MUTE_BTN18 = 0x141, MUTE_BTN19 = 0x149, MUTE_BTN20 = 0x131,
    MUTE_BTN21 = 0x139
};


int channelStripButtonBaseIndex[]
{
    // Base button index of channelstrips
    0x050, 0x058, 0x040, 0x048, 0x030, 0x038,   // 1 - 6
    0x020, 0x028, 0x010, 0x018, 0x000, 0x008,   // 7 - 12
    0x178, 0x160, 0x168, 0x150, 0x158, 0x140,   // 13 - 18
    0x148, 0x130, 0x138, 0x120, 0x129, 0x110   // 19 - 24
};



// std::unordered_map<int, ButtonCallbackFunction> buttonCallbacks{

//     {0x001, {11, "mute", handleChannelStripMessage}}, // ChannelStrip 11
//     {0x002, {11, "solo", handleChannelStripMessage}},
//     {0x003, {11, "select", handleChannelStripMessage}},
//     {0x004, {11, "write", handleChannelStripMessage}},
//     {0x005, {11, "assign", handleChannelStripMessage}},
//     {0x006, {11, "recrdy", handleChannelStripMessage}},
//     //{"007", {"NONE", -1}},
//     //{"008", {"NONE", -1}},
//     {0x009, {12, "mute", handleChannelStripMessage}}, // ChannelStrip 12
//     {0x00A, {12, "solo", handleChannelStripMessage}},
//     {0x00B, {12, "select", handleChannelStripMessage}},
//     {0x00C, {12, "write", handleChannelStripMessage}},
//     {0x00D, {12, "assign", handleChannelStripMessage}},
//     {0x00E, {12, "recrdy", handleChannelStripMessage}},
//     //{"00F", {"NONE", -1}},
//     //{"010", {"NONE", -1}},
//     {0x011, {9, "mute", handleChannelStripMessage}}, // ChannelStrip 9
//     {0x012, {9, "solo", handleChannelStripMessage}},
//     {0x013, {9, "select", handleChannelStripMessage}},
//     {0x014, {9, "write", handleChannelStripMessage}},
//     {0x015, {9, "assign", handleChannelStripMessage}},
//     {0x016, {9, "recrdy", handleChannelStripMessage}},
//     //{"017", {"NONE", -1}},
//     //{"018", {"NONE", -1}},
//     {0x019, {10, "mute", handleChannelStripMessage}}, // ChannelStrip 10
//     {0x01A, {10, "solo", handleChannelStripMessage}},
//     {0x01B, {10, "select", handleChannelStripMessage}},
//     {0x01C, {10, "write", handleChannelStripMessage}},
//     {0x01D, {10, "assign", handleChannelStripMessage}},
//     {0x01E, {10, "recrdy", handleChannelStripMessage}},
//     //{"01F", {"NONE", -1}},
//     //{"020", {"NONE", -1}},
//     {0x021, {7, "mute", handleChannelStripMessage}}, // ChannelStrip 7
//     {0x022, {7, "solo", handleChannelStripMessage}},
//     {0x023, {7, "select", handleChannelStripMessage}},
//     {0x024, {7, "write", handleChannelStripMessage}},
//     {0x025, {7, "assign", handleChannelStripMessage}},
//     {0x026, {7, "recrdy", handleChannelStripMessage}},
//     //{"027", {"NONE", -1}},
//     //{"028", {"NONE", -1}},
//     {0x029, {8, "mute", handleChannelStripMessage}}, // ChannelStrip 8
//     {0x02A, {8, "solo", handleChannelStripMessage}},
//     {0x02B, {8, "select", handleChannelStripMessage}},
//     {0x02C, {8, "write", handleChannelStripMessage}},
//     {0x02D, {8, "assign", handleChannelStripMessage}},
//     {0x02E, {8, "recrdy", handleChannelStripMessage}},
//     //{"02F", {"NONE", -1}},
//     //{"030", {"NONE", -1}},
//     {0x031, {5, "mute", handleChannelStripMessage}}, // ChannelStrip 5
//     {0x032, {5, "solo", handleChannelStripMessage}},
//     {0x033, {5, "select", handleChannelStripMessage}},
//     {0x034, {5, "write", handleChannelStripMessage}},
//     {0x035, {5, "assign", handleChannelStripMessage}},
//     {0x036, {5, "recrdy", handleChannelStripMessage}},
//     //{"037", {"NONE", -1}},
//     //{"038", {"NONE", -1}},
//     {0x039, {6, "mute", handleChannelStripMessage}}, // ChannelStrip 6
//     {0x03A, {6, "solo", handleChannelStripMessage}},
//     {0x03B, {6, "select", handleChannelStripMessage}},
//     {0x03C, {6, "write", handleChannelStripMessage}},
//     {0x03D, {6, "assign", handleChannelStripMessage}},
//     {0x03E, {6, "recrdy", handleChannelStripMessage}},
//     //{"03F", {"NONE", -1}},
//     //{"040", {"NONE", -1}},
//     {0x041, {3, "mute", handleChannelStripMessage}}, // ChannelStrip 3
//     {0x042, {3, "solo", handleChannelStripMessage}},
//     {0x043, {3, "select", handleChannelStripMessage}},
//     {0x044, {3, "write", handleChannelStripMessage}},
//     {0x045, {3, "assign", handleChannelStripMessage}},
//     {0x046, {3, "recrdy", handleChannelStripMessage}},
//     //{"047", {"NONE", -1}},
//     //{"048", {"NONE", -1}},
//     {0x049, {4, "mute", handleChannelStripMessage}}, // ChannelStrip 4
//     {0x04A, {4, "solo", handleChannelStripMessage}},
//     {0x04B, {4, "select", handleChannelStripMessage}},
//     {0x04C, {4, "write", handleChannelStripMessage}},
//     {0x04D, {4, "assign", handleChannelStripMessage}},
//     {0x04E, {4, "recrdy", handleChannelStripMessage}},
//     //{"04F", {"NONE", -1}},
//     //{"050", {"NONE", -1}},
//     {0x051, {1, "mute", handleChannelStripMessage}}, // ChannelStrip 1
//     {0x052, {1, "solo", handleChannelStripMessage}},
//     {0x053, {1, "select", handleChannelStripMessage}},
//     {0x054, {1, "write", handleChannelStripMessage}},
//     {0x055, {1, "assign", handleChannelStripMessage}},
//     {0x056, {1, "recrdy", handleChannelStripMessage}},
//     //{"057", {"NONE", -1}},
//     //{"058", {"NONE", -1}},
//     {0x059, {2, "mute", handleChannelStripMessage}}, // ChannelStrip 2
//     {0x05A, {2, "solo", handleChannelStripMessage}},
//     {0x05B, {2, "select", handleChannelStripMessage}},
//     {0x05C, {2, "write", handleChannelStripMessage}},
//     {0x05D, {2, "assign", handleChannelStripMessage}},
//     {0x05E, {2, "recrdy", handleChannelStripMessage}},
//     //{"05F", {"NONE", -1}},
//     //{"060", {"NONE", -1}},

//     // "060" - "0FF" NONE

//     {0x100, {0, "DigitalTrim", handleVpotAssignMessage}}, // Master Vpot Assigns
//     {0x101, {0, "Aux2", handleVpotAssignMessage}},
//     {0x102, {0, "Aux4", handleVpotAssignMessage}},
//     {0x103, {0, "Aux6", handleVpotAssignMessage}},
//     {0x104, {0, "Aux8", handleVpotAssignMessage}},
//     {0x105, {0, "Aux11", handleVpotAssignMessage}}, // 11-12
//     {0x106, {0, "AuxPan11", handleVpotAssignMessage}},
//     {0x107, {0, "PanVpots", handleVpotAssignMessage}},
//     {0x108, {0, "Masters", handleBankSelectMessage}},
//     {0x109, {0, "MasterSolo", handleVpotAssignMessage}},
//     {0x10A, {0, "SHIFT", handleOtherMessage}},
//     {0x10B, {0, "MICLINE", handleBankSelectMessage}},
//     {0x10C, {0, "TAPEIN", handleBankSelectMessage}},
//     {0x10D, {0, "EFFECTS", handleBankSelectMessage}},
//     // {"10E", {"NONE"}, -1},
//     // {"10F", {"NONE", -1}},
//     // {"110", {"NONE", -1}
//     {0x111, {24, "mute", handleChannelStripMessage}}, // ChannelStrip 24
//     {0x112, {24, "solo", handleChannelStripMessage}},
//     {0x113, {24, "select", handleChannelStripMessage}},
//     {0x114, {24, "write", handleChannelStripMessage}},
//     {0x115, {24, "assign", handleChannelStripMessage}},
//     {0x116, {24, "recrdy", handleChannelStripMessage}},
//     //{"117", {"NONE", -1}},
//     //{"118", {"NONE", -1}},
//     {0x119, {0, "LevelToTape", handleVpotAssignMessage}}, // Master Vpot Assigns
//     {0x11A, {0, "AUX1", handleVpotAssignMessage}},
//     {0x11B, {0, "AUX3", handleVpotAssignMessage}},
//     {0x11C, {0, "AUX5", handleVpotAssignMessage}},
//     {0x11D, {0, "AUX7", handleVpotAssignMessage}},
//     {0x11E, {0, "AUX910", handleVpotAssignMessage}}, // 9-10
//     {0x11F, {0, "AUXPAN910", handleVpotAssignMessage}},
//     //{"120", {"NONE", -1}},
//     {0x121, {22, "mute", handleChannelStripMessage}}, // ChannelStrip 22
//     {0x122, {22, "solo", handleChannelStripMessage}},
//     {0x123, {22, "select", handleChannelStripMessage}},
//     {0x124, {22, "write", handleChannelStripMessage}},
//     {0x125, {22, "assign", handleChannelStripMessage}},
//     {0x126, {22, "recrdy", handleChannelStripMessage}},
//     //{"127", {"NONE", -1}},
//     //{"128", {"NONE", -1}},
//     {0x129, {23, "mute", handleChannelStripMessage}}, // ChannelStrip 23
//     {0x12A, {23, "solo", handleChannelStripMessage}},
//     {0x12B, {23, "select", handleChannelStripMessage}},
//     {0x12C, {23, "write", handleChannelStripMessage}},
//     {0x12D, {23, "assign", handleChannelStripMessage}},
//     {0x12E, {23, "recrdy", handleChannelStripMessage}},
//     //{"12F", {"NONE", -1}},
//     //{"130", {"NONE", -1}},
//     {0x131, {20, "mute", handleChannelStripMessage}}, // ChannelStrip 20
//     {0x132, {20, "solo", handleChannelStripMessage}},
//     {0x133, {20, "select", handleChannelStripMessage}},
//     {0x134, {20, "write", handleChannelStripMessage}},
//     {0x135, {20, "assign", handleChannelStripMessage}},
//     {0x136, {20, "recrdy", handleChannelStripMessage}},
//     //{"137", {"NONE", -1}},
//     //{"138", {"NONE", -1}},
//     {0x139, {21, "mute", handleChannelStripMessage}}, // ChannelStrip 21
//     {0x13A, {21, "solo", handleChannelStripMessage}},
//     {0x13B, {21, "select", handleChannelStripMessage}},
//     {0x13C, {21, "write", handleChannelStripMessage}},
//     {0x13D, {21, "assign", handleChannelStripMessage}},
//     {0x13E, {21, "recrdy", handleChannelStripMessage}},
//     //{"13F", {"NONE", -1}},
//     //{"140", {"NONE", -1}},
//     {0x141, {18, "mute", handleChannelStripMessage}}, // ChannelStrip 18
//     {0x142, {18, "solo", handleChannelStripMessage}},
//     {0x143, {18, "select", handleChannelStripMessage}},
//     {0x144, {18, "write", handleChannelStripMessage}},
//     {0x145, {18, "assign", handleChannelStripMessage}},
//     {0x146, {18, "recrdy", handleChannelStripMessage}},
//     //{"147", {"NONE", -1}},
//     //{"148", {"NONE", -1}},
//     {0x149, {19, "mute", handleChannelStripMessage}}, // ChannelStrip 19
//     {0x14A, {19, "solo", handleChannelStripMessage}},
//     {0x14B, {19, "select", handleChannelStripMessage}},
//     {0x14C, {19, "write", handleChannelStripMessage}},
//     {0x14D, {19, "assign", handleChannelStripMessage}},
//     {0x14E, {19, "recrdy", handleChannelStripMessage}},
//     //{"14F", {"NONE", -1}},
//     //{"150", {"NONE", -1}},
//     {0x151, {16, "mute", handleChannelStripMessage}}, // ChannelStrip 16
//     {0x152, {16, "solo", handleChannelStripMessage}},
//     {0x153, {16, "select", handleChannelStripMessage}},
//     {0x154, {16, "write", handleChannelStripMessage}},
//     {0x155, {16, "assign", handleChannelStripMessage}},
//     {0x156, {16, "recrdy", handleChannelStripMessage}},
//     //{"157", {"NONE", -1}},
//     //{"158", {"NONE", -1}},
//     {0x159, {17, "mute", handleChannelStripMessage}}, // ChannelStrip 17
//     {0x15A, {17, "solo", handleChannelStripMessage}},
//     {0x15B, {17, "select", handleChannelStripMessage}},
//     {0x15C, {17, "write", handleChannelStripMessage}},
//     {0x15D, {17, "assign", handleChannelStripMessage}},
//     {0x15E, {17, "recrdy", handleChannelStripMessage}},
//     //{"15F", {"NONE", -1}},
//     //{"160", {"NONE", -1}},
//     {0x161, {14, "mute", handleChannelStripMessage}}, // ChannelStrip 14
//     {0x162, {14, "solo", handleChannelStripMessage}},
//     {0x163, {14, "select", handleChannelStripMessage}},
//     {0x164, {14, "write", handleChannelStripMessage}},
//     {0x165, {14, "assign", handleChannelStripMessage}},
//     {0x166, {14, "recrdy", handleChannelStripMessage}},
//     //{"167", {"NONE", -1}},
//     //{"168", {"NONE", -1}},
//     {0x169, {15, "mute", handleChannelStripMessage}}, // ChannelStrip 15
//     {0x16A, {15, "solo", handleChannelStripMessage}},
//     {0x16B, {15, "select", handleChannelStripMessage}},
//     {0x16C, {15, "write", handleChannelStripMessage}},
//     {0x16D, {15, "assign", handleChannelStripMessage}},
//     {0x16E, {15, "recrdy", handleChannelStripMessage}},
//     //{"16F", {"NONE", -1}},
//     //{"170", {"NONE", -1}},

//     // "170" - "178" = NONE

//     {0x179, {13, "mute", handleChannelStripMessage}}, // ChannelStrip 13
//     {0x17A, {13, "solo", handleChannelStripMessage}},
//     {0x17B, {13, "select", handleChannelStripMessage}},
//     {0x17C, {13, "write", handleChannelStripMessage}},
//     {0x17D, {13, "assign", handleChannelStripMessage}},
//     {0x17E, {13, "recrdy", handleChannelStripMessage}},
//     //{"17F", {"NONE", -1}},
//     //{"180", {"NONE", -1}},

//     // "181" - "1FF" = NONE

//     {0x200, {0, "Load", handleSetupMessage}}, // Setups
//     {0x201, {0, "Plugins", handleSetupMessage}},
//     {0x202, {0, "DigitalIO", handleSetupMessage}},
//     {0x203, {4, "Number", handleTransportMessage}}, // Transport Section
//     {0x204, {5, "Number", handleTransportMessage}},
//     {0x205, {9, "Number", handleTransportMessage}},
//     {0x206, {0, "Number", handleTransportMessage}},
//     {0x207, {0, "Locator", handleTransportMessage}},
//     {0x208, {0, "Snapshot", handleTransportMessage}},
//     {0x209, {0, "Play", handleTransportMessage}},
//     {0x20A, {0, "Record", handleTransportMessage}},
//     {0x20B, {0, "JogShuttle", handleTransportMessage}},
//     {0x20C, {0, "Mono", handleControlRoomMessage}},
//     {0x20D, {0, "SetTime", handleTransportMessage}},
//     // {"20E", {"NONE"}},
//     // {"20F", {"NONE"}},
//     {0x210, {0, "SaveAs", handleSetupMessage}},
//     {0x211, {0, "Group", handleSetupMessage}},
//     {0x212, {0, "General", handleSetupMessage}},
//     {0x213, {2, "Number", handleTransportMessage}},
//     {0x214, {3, "Number", handleTransportMessage}},
//     {0x215, {7, "Number", handleTransportMessage}},
//     {0x216, {8, "Number", handleTransportMessage}},
//     {0x217, {0, "Loop", handleTransportMessage}},
//     {0x218, {0, "Store", handleTransportMessage}},
//     {0x219, {0, "FastFwd", handleTransportMessage}},
//     {0x21A, {0, "Stop", handleTransportMessage}},
//     {0x21B, {0, "Mutes", handleAutomationMessage}},
//     {0x21C, {0, "AutoTouch", handleAutomationMessage}},
//     {0x21D, {0, "All", handleAutomationMessage}},
//     {0x21E, {0, "TrimLevels", handleAutomationMessage}},
//     {0x21F, {0, "New", handleSetupMessage}},
//     {0x220, {5, "Bus", handleAssignmentMessage}},
//     {0x221, {6, "Bus", handleAssignmentMessage}},
//     {0x222, {7, "Bus", handleAssignmentMessage}},
//     {0x223, {8, "Bus", handleAssignmentMessage}},
//     {0x224, {9, "LR", handleAssignmentMessage}},
//     {0x225, {10, "RouteToTape", handleAssignmentMessage}},
//     {0x226, {0, "SmpteView", handleTransportMessage}},
//     {0x227, {1, "Number", handleTransportMessage}},
//     {0x228, {6, "Number", handleTransportMessage}},
//     {0x229, {0, "Enter", handleTransportMessage}},
//     {0x22A, {0, "Rewind", handleTransportMessage}},
//     {0x22B, {0, "Bypass", handleAutomationMessage}},
//     {0x22C, {0, "Faders", handleAutomationMessage}},
//     {0x22D, {0, "FaderMotorsOff", handleAutomationMessage}},
//     {0x22E, {0, "Pan", handleAutomationMessage}},
//     {0x22F, {0, "Save", handleSetupMessage}},
//     {0x230, {0, "NearField", handleControlRoomMessage}},
//     {0x231, {0, "Main", handleControlRoomMessage}},
//     {0x232, {0, "Dim", handleControlRoomMessage}},
//     {0x233, {0, "TalkBack", handleControlRoomMessage}},
//     {0x234, {0, "CutZeroSet", handleClipboardMessage}},
//     {0x235, {0, "Copy", handleClipboardMessage}},
//     {0x236, {0, "Paste", handleClipboardMessage}},
//     {0x237, {0, "Undo", handleClipboardMessage}},
//     {0x238, {0, "Select", handleMasterLRShortcuts}},
//     {0x239, {0, "Control", handleMasterLRShortcuts}},
//     {0x23A, {0, "Write", handleMasterLRShortcuts}},
//     {0x23B, {0, "Alt", handleMasterLRShortcuts}},
//     {0x23C, {1, "Bus", handleAssignmentMessage}},
//     {0x23D, {2, "Bus", handleAssignmentMessage}},
//     {0x23E, {3, "Bus", handleAssignmentMessage}},
//     {0x23F, {4, "Bus", handleAssignmentMessage}},
//     {0x240, {0, "StudioLevel", handleSoloStudioMessage}},
//     {0x241, {0, "TalkbackLevel", handleSoloStudioMessage}},
//     {0x242, {1, "Aux910", handlePhonesCueMixMessage}},
//     {0x243, {1, "Aux1112", handlePhonesCueMixMessage}},
//     {0x244, {1, "CopyToCue", handlePhonesCueMixMessage}},
//     {0x245, {1, "ControlRoom", handlePhonesCueMixMessage}},
//     {0x246, {2, "Aux910", handlePhonesCueMixMessage}},
//     {0x247, {2, "Aux1112", handlePhonesCueMixMessage}},
//     {0x248, {2, "CopyToCue", handlePhonesCueMixMessage}},
//     {0x249, {2, "ControlRoom", handlePhonesCueMixMessage}},
//     {0x24A, 2 Track A},
//     {0x24B, 2 Track B},
//     {0x24C, 2 Track C},
//     {0x24D, Digital In 1},
//     {0x24E, Digital In 2},
//     {0x24F, Master L-R},
//     {0x250, {0, "Setup", handleFatChannelMessage}},
//     {0x251, {0, "MemoryA", handleFatChannelMessage}},
//     {0x252, {0, "MemoryB", handleFatChannelMessage}},
//     {0x253, {0, "Next", handleFatChannelMessage}},
//     {0x254, {0, "EQ", handleFatChannelMessage}},
//     {0x255, {0, "Gate", handleFatChannelMessage}},
//     {0x256, {0, "Compressor", handleFatChannelMessage}},
//     {0x257, {0, "Plugins", handleFatChannelMessage}},
//     {0x258, {0, "LoadPatch", handleFatChannelMessage}},
//     {0x259, {0, "SavePatch", handleFatChannelMessage}},
//     {0x25A, {0, "MixdownSolo", handleSoloStudioMessage}},
//     {0x25B, {0, "PflSolo", handleSoloStudioMessage}},
//     {0x25C, {0, "AflSolo", handleSoloStudioMessage}},
//     {0x25D, {0, "TalkbackStudio", handleSoloStudioMessage}},
//     {0x25E, {0, "ClearSolo", handleSoloStudioMessage}},
//     {0x25F, {0, "SoloLevel", handleSoloStudioMessage}},

//     // 260 - 268 = NONE

//     {0x269, {0, "Select1", handleFatChannelMessage}},
//     {0x26A, {0, "Select2", handleFatChannelMessage}},
//     {0x26B, {0, "Select3", handleFatChannelMessage}},
//     {0x26C, {0, "Select4", handleFatChannelMessage}},
//     {0x26D, {0, "Help", handleFatChannelMessage}},
//     {0x26E, {0, "Previous", handleFatChannelMessage}},
//     {0x26F, {0, "On", handleFatChannelMessage}}

// };