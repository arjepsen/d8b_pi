/*
  ==============================================================================

    BrainLookupTable.cpp
    Created: 16 Apr 2023 8:49:27pm
    Author:  anders

  ==============================================================================
*/

#include "BrainLookupTable.h"
#include "BrainMessageHandlers.h"

void processBrainMessage(std::string message)
{
    // Check whether the message is a button, fader or vpot
    char messageType = message.back();
    message.pop_back(); // Remove the extra char so we have the clean message.

    // If messageType is v or f, handle that.
    // else, it must be a button.
    // run the lookup procedure:
    MessageInfo messageInfo = brainMessageLookupTable[message];
    messageInfo.handler(messageInfo.number, messageInfo.buttonFunction);
}

std::unordered_map<std::string, MessageInfo> brainMessageLookupTable
{
    // ###### CHANNELSTRIP BUTTONS ##########

    {"001", {11, "mute", handleChannelStripMessage}}, // ChannelStrip 11
    {"002", {11, "solo", handleChannelStripMessage}},
    {"003", {11, "select", handleChannelStripMessage}},
    {"004", {11, "write", handleChannelStripMessage}},
    {"005", {11, "assign", handleChannelStripMessage}},
    {"006", {11, "recrdy", handleChannelStripMessage}},
    //{"007", {"NONE", -1}},
    //{"008", {"NONE", -1}},
    {"009", {12, "mute", handleChannelStripMessage}}, // ChannelStrip 12
    {"00A", {12, "solo", handleChannelStripMessage}},
    {"00B", {12, "select", handleChannelStripMessage}},
    {"00C", {12, "write", handleChannelStripMessage}},
    {"00D", {12, "assign", handleChannelStripMessage}},
    {"00E", {12, "recrdy", handleChannelStripMessage}},
    //{"00F", {"NONE", -1}},
    //{"010", {"NONE", -1}},
    {"011", {9, "mute", handleChannelStripMessage}}, // ChannelStrip 9
    {"012", {9, "solo", handleChannelStripMessage}},
    {"013", {9, "select", handleChannelStripMessage}},
    {"014", {9, "write", handleChannelStripMessage}},
    {"015", {9, "assign", handleChannelStripMessage}},
    {"016", {9, "recrdy", handleChannelStripMessage}},
    //{"017", {"NONE", -1}},
    //{"018", {"NONE", -1}},
    {"019", {10, "mute", handleChannelStripMessage}}, // ChannelStrip 10
    {"01A", {10, "solo", handleChannelStripMessage}},
    {"01B", {10, "select", handleChannelStripMessage}},
    {"01C", {10, "write", handleChannelStripMessage}},
    {"01D", {10, "assign", handleChannelStripMessage}},
    {"01E", {10, "recrdy", handleChannelStripMessage}},
    //{"01F", {"NONE", -1}},
    //{"020", {"NONE", -1}},
    {"021", {7, "mute", handleChannelStripMessage}}, // ChannelStrip 7
    {"022", {7, "solo", handleChannelStripMessage}},
    {"023", {7, "select", handleChannelStripMessage}},
    {"024", {7, "write", handleChannelStripMessage}},
    {"025", {7, "assign", handleChannelStripMessage}},
    {"026", {7, "recrdy", handleChannelStripMessage}},
    //{"027", {"NONE", -1}},
    //{"028", {"NONE", -1}},
    {"029", {8, "mute", handleChannelStripMessage}}, // ChannelStrip 8
    {"02A", {8, "solo", handleChannelStripMessage}},
    {"02B", {8, "select", handleChannelStripMessage}},
    {"02C", {8, "write", handleChannelStripMessage}},
    {"02D", {8, "assign", handleChannelStripMessage}},
    {"02E", {8, "recrdy", handleChannelStripMessage}},
    //{"02F", {"NONE", -1}},
    //{"030", {"NONE", -1}},
    {"031", {5, "mute", handleChannelStripMessage}}, // ChannelStrip 5
    {"032", {5, "solo", handleChannelStripMessage}},
    {"033", {5, "select", handleChannelStripMessage}},
    {"034", {5, "write", handleChannelStripMessage}},
    {"035", {5, "assign", handleChannelStripMessage}},
    {"036", {5, "recrdy", handleChannelStripMessage}},
    //{"037", {"NONE", -1}},
    //{"038", {"NONE", -1}},
    {"039", {6, "mute", handleChannelStripMessage}}, // ChannelStrip 6
    {"03A", {6, "solo", handleChannelStripMessage}},
    {"03B", {6, "select", handleChannelStripMessage}},
    {"03C", {6, "write", handleChannelStripMessage}},
    {"03D", {6, "assign", handleChannelStripMessage}},
    {"03E", {6, "recrdy", handleChannelStripMessage}},
    //{"03F", {"NONE", -1}},
    //{"040", {"NONE", -1}},
    {"041", {3, "mute", handleChannelStripMessage}}, // ChannelStrip 3
    {"042", {3, "solo", handleChannelStripMessage}},
    {"043", {3, "select", handleChannelStripMessage}},
    {"044", {3, "write", handleChannelStripMessage}},
    {"045", {3, "assign", handleChannelStripMessage}},
    {"046", {3, "recrdy", handleChannelStripMessage}},
    //{"047", {"NONE", -1}},
    //{"048", {"NONE", -1}},
    {"049", {4, "mute", handleChannelStripMessage}}, // ChannelStrip 4
    {"04A", {4, "solo", handleChannelStripMessage}},
    {"04B", {4, "select", handleChannelStripMessage}},
    {"04C", {4, "write", handleChannelStripMessage}},
    {"04D", {4, "assign", handleChannelStripMessage}},
    {"04E", {4, "recrdy", handleChannelStripMessage}},
    //{"04F", {"NONE", -1}},
    //{"050", {"NONE", -1}},
    {"051", {1, "mute", handleChannelStripMessage}}, // ChannelStrip 1
    {"052", {1, "solo", handleChannelStripMessage}},
    {"053", {1, "select", handleChannelStripMessage}},
    {"054", {1, "write", handleChannelStripMessage}},
    {"055", {1, "assign", handleChannelStripMessage}},
    {"056", {1, "recrdy", handleChannelStripMessage}},
    //{"057", {"NONE", -1}},
    //{"058", {"NONE", -1}},
    {"059", {2, "mute", handleChannelStripMessage}}, // ChannelStrip 2
    {"05A", {2, "solo", handleChannelStripMessage}},
    {"05B", {2, "select", handleChannelStripMessage}},
    {"05C", {2, "write", handleChannelStripMessage}},
    {"05D", {2, "assign", handleChannelStripMessage}},
    {"05E", {2, "recrdy", handleChannelStripMessage}},
    //{"05F", {"NONE", -1}},
    //{"060", {"NONE", -1}},

    // "060" - "0FF" NONE

    {"100", {0, "DigitalTrim", handleVpotAssignMessage}}, // Master Vpot Assigns
    {"101", {0, "Aux2", handleVpotAssignMessage}},
    {"102", {0, "Aux4", handleVpotAssignMessage}},
    {"103", {0, "Aux6", handleVpotAssignMessage}},
    {"104", {0, "Aux8", handleVpotAssignMessage}},
    {"105", {0, "Aux11", handleVpotAssignMessage}}, // 11-12
    {"106", {0, "AuxPan11", handleVpotAssignMessage}},
    {"107", {0, "PanVpots", handleVpotAssignMessage}},
    {"108", {0, "Masters", handleBankSelectMessage}},
    {"109", {0, "MasterSolo", handleVpotAssignMessage}},
    {"10A", {0, "SHIFT", handleOtherMessage}},
    {"10B", {0, "MICLINE", handleBankSelectMessage}},
    {"10C", {0, "TAPEIN", handleBankSelectMessage}},
    {"10D", {0, "EFFECTS", handleBankSelectMessage}},
    // {"10E", {"NONE"}, -1},
    // {"10F", {"NONE", -1}},
    // {"110", {"NONE", -1}
    {"111", {24, "mute", handleChannelStripMessage}}, // ChannelStrip 24
    {"112", {24, "solo", handleChannelStripMessage}},
    {"113", {24, "select", handleChannelStripMessage}},
    {"114", {24, "write", handleChannelStripMessage}},
    {"115", {24, "assign", handleChannelStripMessage}},
    {"116", {24, "recrdy", handleChannelStripMessage}},
    //{"117", {"NONE", -1}},
    //{"118", {"NONE", -1}},
    {"119", {0, "LevelToTape", handleVpotAssignMessage}}, // Master Vpot Assigns
    {"11A", {0, "AUX1", handleVpotAssignMessage}},
    {"11B", {0, "AUX3", handleVpotAssignMessage}},
    {"11C", {0, "AUX5", handleVpotAssignMessage}},
    {"11D", {0, "AUX7", handleVpotAssignMessage}},
    {"11E", {0, "AUX910", handleVpotAssignMessage}}, // 9-10
    {"11F", {0, "AUXPAN910", handleVpotAssignMessage}},
    //{"120", {"NONE", -1}},
    {"121", {22, "mute", handleChannelStripMessage}}, // ChannelStrip 22
    {"122", {22, "solo", handleChannelStripMessage}},
    {"123", {22, "select", handleChannelStripMessage}},
    {"124", {22, "write", handleChannelStripMessage}},
    {"125", {22, "assign", handleChannelStripMessage}},
    {"126", {22, "recrdy", handleChannelStripMessage}},
    //{"127", {"NONE", -1}},
    //{"128", {"NONE", -1}},
    {"129", {23, "mute", handleChannelStripMessage}}, // ChannelStrip 23
    {"12A", {23, "solo", handleChannelStripMessage}},
    {"12B", {23, "select", handleChannelStripMessage}},
    {"12C", {23, "write", handleChannelStripMessage}},
    {"12D", {23, "assign", handleChannelStripMessage}},
    {"12E", {23, "recrdy", handleChannelStripMessage}},
    //{"12F", {"NONE", -1}},
    //{"130", {"NONE", -1}},
    {"131", {20, "mute", handleChannelStripMessage}}, // ChannelStrip 20
    {"132", {20, "solo", handleChannelStripMessage}},
    {"133", {20, "select", handleChannelStripMessage}},
    {"134", {20, "write", handleChannelStripMessage}},
    {"135", {20, "assign", handleChannelStripMessage}},
    {"136", {20, "recrdy", handleChannelStripMessage}},
    //{"137", {"NONE", -1}},
    //{"138", {"NONE", -1}},
    {"139", {21, "mute", handleChannelStripMessage}}, // ChannelStrip 21
    {"13A", {21, "solo", handleChannelStripMessage}},
    {"13B", {21, "select", handleChannelStripMessage}},
    {"13C", {21, "write", handleChannelStripMessage}},
    {"13D", {21, "assign", handleChannelStripMessage}},
    {"13E", {21, "recrdy", handleChannelStripMessage}},
    //{"13F", {"NONE", -1}},
    //{"140", {"NONE", -1}},
    {"141", {18, "mute", handleChannelStripMessage}}, // ChannelStrip 18
    {"142", {18, "solo", handleChannelStripMessage}},
    {"143", {18, "select", handleChannelStripMessage}},
    {"144", {18, "write", handleChannelStripMessage}},
    {"145", {18, "assign", handleChannelStripMessage}},
    {"146", {18, "recrdy", handleChannelStripMessage}},
    //{"147", {"NONE", -1}},
    //{"148", {"NONE", -1}},
    {"149", {19, "mute", handleChannelStripMessage}}, // ChannelStrip 19
    {"14A", {19, "solo", handleChannelStripMessage}},
    {"14B", {19, "select", handleChannelStripMessage}},
    {"14C", {19, "write", handleChannelStripMessage}},
    {"14D", {19, "assign", handleChannelStripMessage}},
    {"14E", {19, "recrdy", handleChannelStripMessage}},
    //{"14F", {"NONE", -1}},
    //{"150", {"NONE", -1}},
    {"151", {16, "mute", handleChannelStripMessage}}, // ChannelStrip 16
    {"152", {16, "solo", handleChannelStripMessage}},
    {"153", {16, "select", handleChannelStripMessage}},
    {"154", {16, "write", handleChannelStripMessage}},
    {"155", {16, "assign", handleChannelStripMessage}},
    {"156", {16, "recrdy", handleChannelStripMessage}},
    //{"157", {"NONE", -1}},
    //{"158", {"NONE", -1}},
    {"159", {17, "mute", handleChannelStripMessage}}, // ChannelStrip 17
    {"15A", {17, "solo", handleChannelStripMessage}},
    {"15B", {17, "select", handleChannelStripMessage}},
    {"15C", {17, "write", handleChannelStripMessage}},
    {"15D", {17, "assign", handleChannelStripMessage}},
    {"15E", {17, "recrdy", handleChannelStripMessage}},
    //{"15F", {"NONE", -1}},
    //{"160", {"NONE", -1}},
    {"161", {14, "mute", handleChannelStripMessage}}, // ChannelStrip 14
    {"162", {14, "solo", handleChannelStripMessage}},
    {"163", {14, "select", handleChannelStripMessage}},
    {"164", {14, "write", handleChannelStripMessage}},
    {"165", {14, "assign", handleChannelStripMessage}},
    {"166", {14, "recrdy", handleChannelStripMessage}},
    //{"167", {"NONE", -1}},
    //{"168", {"NONE", -1}},
    {"169", {15, "mute", handleChannelStripMessage}}, // ChannelStrip 15
    {"16A", {15, "solo", handleChannelStripMessage}},
    {"16B", {15, "select", handleChannelStripMessage}},
    {"16C", {15, "write", handleChannelStripMessage}},
    {"16D", {15, "assign", handleChannelStripMessage}},
    {"16E", {15, "recrdy", handleChannelStripMessage}},
    //{"16F", {"NONE", -1}},
    //{"170", {"NONE", -1}},

    // "170" - "178" = NONE

    {"179", {13, "mute", handleChannelStripMessage}}, // ChannelStrip 13
    {"17A", {13, "solo", handleChannelStripMessage}},
    {"17B", {13, "select", handleChannelStripMessage}},
    {"17C", {13, "write", handleChannelStripMessage}},
    {"17D", {13, "assign", handleChannelStripMessage}},
    {"17E", {13, "recrdy", handleChannelStripMessage}},
    //{"17F", {"NONE", -1}},
    //{"180", {"NONE", -1}},

    // "181" - "1FF" = NONE

    {"200", {0, "Load", handleSetupMessage}}, // Setups
    {"201", {0, "Plugins", handleSetupMessage}},
    {"202", {0, "DigitalIO", handleSetupMessage}},
    {"203", {4, "Number", handleTransportMessage}}, // Transport Section
    {"204", {5, "Number", handleTransportMessage}},
    {"205", {9, "Number", handleTransportMessage}},
    {"206", {0, "Number", handleTransportMessage}},
    {"207", {0, "Locator", handleTransportMessage}},
    {"208", {0, "Snapshot", handleTransportMessage}},
    {"209", {0, "Play", handleTransportMessage}},
    {"20A", {0, "Record", handleTransportMessage}},
    {"20B", {0, "JogShuttle", handleTransportMessage}},
    {"20C", {0, "Mono", handleControlRoomMessage}},
    {"20D", {0, "SetTime", handleTransportMessage}},
    // {"20E", {"NONE"}},
    // {"20F", {"NONE"}},
    {"210", {0, "SaveAs", handleSetupMessage}},
    {"211", {0, "Group", handleSetupMessage}},
    {"212", {0, "General", handleSetupMessage}},
    {"213", {2, "Number", handleTransportMessage}},
    {"214", {3, "Number", handleTransportMessage}},
    {"215", {7, "Number", handleTransportMessage}},
    {"216", {8, "Number", handleTransportMessage}},
    {"217", {0, "Loop", handleTransportMessage}},
    {"218", {0, "Store", handleTransportMessage}},
    {"219", {0, "FastFwd", handleTransportMessage}},
    {"21A", {0, "Stop", handleTransportMessage}},
    {"21B", {0, "Mutes", handleAutomationMessage}},
    {"21C", {0, "AutoTouch", handleAutomationMessage}},
    {"21E", {0, "TrimLevels", handleAutomationMessage}},
    {"21F", {0, "New", handleSetupMessage}},
    {"220", {5, "Bus", handleAssignmentMessage}},
    {"221", {6, "Bus", handleAssignmentMessage}},
    {"222", {7, "Bus", handleAssignmentMessage}},
    {"223", {8, "Bus", handleAssignmentMessage}},
    {"224", {9, "LR", handleAssignmentMessage}},
    {"225", {10, "RouteToTape", handleAssignmentMessage}},
    {"226", {0, "SmpteView", handleTransportMessage}},
    {"227", {1, "Number", handleTransportMessage}},
    {"228", {6, "Number", handleTransportMessage}},
    {"229", {0, "Enter", handleTransportMessage}},
    {"22A", {0, "Rewind", handleTransportMessage}},
    {"22B", {0, "Bypass", handleAutomationMessage}},
    {"22C", {0, "Faders", handleAutomationMessage}},
    {"22D", {0, "FaderMotorsOff", handleAutomationMessage}},
    {"22E", {0, "Pan", handleAutomationMessage}},
    {"22F", {0, "Save", handleSetupMessage}},
    {"230", {0, "NearField", handleControlRoomMessage}},
    {"231", {0, "Main", handleControlRoomMessage}},
    {"232", {0, "Dim", handleControlRoomMessage}},
    {"233", {0, "TalkBack", handleControlRoomMessage}},
    {"234", {0, "CutZeroSet", handleClipboardMessage}},
    {"235", {0, "Copy", handleClipboardMessage}},
    {"236", {0, "Paste", handleClipboardMessage}},
    {"237", {0, "Undo", handleClipboardMessage}},
    {"238", {0, "Select", handleMasterLRShortcuts}},
    {"239", {0, "Control", handleMasterLRShortcuts}},
    {"23A", {0, "Write", handleMasterLRShortcuts}},
    {"23B", {0, "Alt", handleMasterLRShortcuts}},
    {"23C", {1, "Bus", handleAssignmentMessage}},
    {"23D", {2, "Bus", handleAssignmentMessage}},
    {"23E", {3, "Bus", handleAssignmentMessage}},
    {"23F", {4, "Bus", handleAssignmentMessage}},
    {"240", {0, "StudioLevel", handleSoloStudioMessage}},
    {"241", {0, "TalkbackLevel", handleSoloStudioMessage}},
    {"242", {1, "Aux910", handlePhonesCueMixMessage}},
    {"243", {1, "Aux1112", handlePhonesCueMixMessage}},
    {"244", {1, "CopyToCue", handlePhonesCueMixMessage}},
    {"245", {1, "ControlRoom", handlePhonesCueMixMessage}},
    {"246", {2, "Aux910", handlePhonesCueMixMessage}},
    {"247", {2, "Aux1112", handlePhonesCueMixMessage}},
    {"248", {2, "CopyToCue", handlePhonesCueMixMessage}},
    {"249", {2, "ControlRoom", handlePhonesCueMixMessage}},
    {"250", {0, "Setup", handleFatChannelMessage}},
    {"251", {0, "MemoryA", handleFatChannelMessage}},
    {"252", {0, "MemoryB", handleFatChannelMessage}},
    {"253", {0, "Next", handleFatChannelMessage}},
    {"254", {0, "EQ", handleFatChannelMessage}},
    {"255", {0, "Gate", handleFatChannelMessage}},
    {"256", {0, "Compressor", handleFatChannelMessage}},
    {"257", {0, "Plugins", handleFatChannelMessage}},
    {"258", {0, "LoadPatch", handleFatChannelMessage}},
    {"259", {0, "SavePatch", handleFatChannelMessage}},
    {"25A", {0, "MixdownSolo", handleSoloStudioMessage}},
    {"25B", {0, "PflSolo", handleSoloStudioMessage}},
    {"25C", {0, "AflSolo", handleSoloStudioMessage}},
    {"25D", {0, "TalkbackStudio", handleSoloStudioMessage}},
    {"25E", {0, "ClearSolo", handleSoloStudioMessage}},
    {"25F", {0, "SoloLevel", handleSoloStudioMessage}},

    // 260 - 268 = NONE

    {"269", {0, "Select1", handleFatChannelMessage}},
    {"26A", {0, "Select2", handleFatChannelMessage}},
    {"26B", {0, "Select3", handleFatChannelMessage}},
    {"26C", {0, "Select4", handleFatChannelMessage}},
    {"26D", {0, "Help", handleFatChannelMessage}},
    {"26E", {0, "Previous", handleFatChannelMessage}},
    {"26F", {0, "On", handleFatChannelMessage}}

};