/*
  ==============================================================================

    LookupTables.h
    Created: 15 Apr 2023 9:32:42pm
    Author:  anders

  ==============================================================================
*/

#pragma once
#include <functional>
#include <string>
#include <unordered_map>


// Create a structure to hold info for the different Brain Messages.
struct MessageInfo
{
    uint8_t number;
    std::string buttonFunction;
    std::function<void(const uint8_t &, const std::string &)> handler;
};

// // Handler Function Declarations
// void handleChannelStripMessage(const uint8_t channelStrip, const std::string &buttonFunction);
// void handleBankSelectMessage([[maybe_unused]] uint8_t number, const std::string &buttonFunction);
// void handleVpotAssignMessage([[maybe_unused]] uint8_t number, const std::string &buttonFunction);
// void handleSetupMessage([[maybe_unused]] uint8_t number, const std::string &buttonFunction);
// void handleTransportMessage([[maybe_unused]] uint8_t number, const std::string &buttonFunction);
// void handleControlRoomMessage([[maybe_unused]] uint8_t number, const std::string &buttonFunction);
// void handleAutomationMessage([[maybe_unused]] uint8_t number, const std::string &buttonFunction);
// void handleAssignmentMessage([[maybe_unused]] uint8_t number, const std::string &buttonFunction);
// void handleClipboardMessage([[maybe_unused]] uint8_t number, const std::string &buttonFunction);
// void handleMasterLRShortcuts([[maybe_unused]] uint8_t number, const std::string &buttonFunction);
// void handleSoloStudioMessage([[maybe_unused]] uint8_t number, const std::string &buttonFunction);
// void handlePhonesCueMixMessage([[maybe_unused]] uint8_t number, const std::string &buttonFunction);
// void handleFatChannelMessage([[maybe_unused]] uint8_t number, const std::string &buttonFunction);

// void handleOtherMessage([[maybe_unused]] uint8_t number, const std::string &buttonFunction);

void processBrainMessage(std::string message);

extern std::unordered_map<std::string, MessageInfo> brainMessageLookupTable;


