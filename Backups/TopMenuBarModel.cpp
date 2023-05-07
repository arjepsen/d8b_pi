/*
  ==============================================================================

    TopMenuBarModel.cpp
    Created: 6 May 2023 8:02:12pm
    Author:  anders

  ==============================================================================
*/

#include "TopMenuBarModel.h"

TopMenuBarModel::TopMenuBarModel() {}
TopMenuBarModel::~TopMenuBarModel() {}

// This method returns a "juce::StringArray" of the names of the menus in the menubar.
// TODO: Maybe do this more generalized? Maybe use some preprocessor constants, instead of direct litterals?
juce::StringArray TopMenuBarModel::getMenuBarNames()
{
    juce::StringArray names{"File", "Edit", "Channel", "Options", "Plugins", "Windows"};
    return names;
}

// This method populates a popupmenu, and returns it.
juce::PopupMenu TopMenuBarModel::getMenuForIndex(int menuIndex, const juce::String &menuName)
{
    juce::PopupMenu menu;

    switch (menuIndex)
    {
        case 0: // "File"
            menu.addItem(1, "New Session...");
            menu.addItem(2, "Open Session...");
            menu.addSeparator();
            menu.addItem(3, "Close Window");
            menu.addSeparator();
            menu.addItem(4, "Save Session");
            menu.addItem(5, "Save Session As...");
            menu.addSeparator();
            menu.addItem(6, "Save As Template");
            menu.addItem(7, "Reset Template");
            menu.addSeparator();
            menu.addItem(8, "Desktop");
            menu.addSeparator();
            menu.addItem(9, "Shutdown");
            break;
        case 1: // "Edit"
            menu.addItem(10, "Undo");
            menu.addItem(11, "Redo");
            menu.addItem(12, "Cut Channels");
            menu.addItem(13, "Copy Channel(s)");
            menu.addItem(14, "Paste Channels");
            menu.addItem(15, "Delete Events");
            menu.addSeparator();
            menu.addItem(16, "Set Default Levels...");
            menu.addItem(17, "Modify Levels...");
            menu.addSeparator();
            menu.addItem(18, "Insert Time");
            menu.addItem(19, "Delete Time");
            menu.addSeparator();
            menu.addItem(20, "Insert Global Time");
            menu.addItem(21, "Delete Global Time");
            menu.addSeparator();
            menu.addItem(22, "Clear Automation...");
            menu.addSeparator();
            menu.addItem(23, "Clear Edit History");
            menu.addSeparator();
            menu.addItem(24, "Copy Mix To Bus");
            menu.addSeparator();
            menu.addItem(25, "Play From Selection");
            menu.addSeparator();
            menu.addItem(26, "Select All");
            break;
        case 2: // "Channel"
            menu.addItem(27, "Channel List");
            menu.addSeparator();
            menu.addItem(28, "Open Channel...");
            menu.addItem(29, "Save Channel As...");
            menu.addSeparator();
            menu.addItem(30, "Group");
            menu.addItem(31, "UnGroup");
            menu.addSeparator();
            menu.addItem(32, "Channel Link...");
            menu.addItem(33, "Unlink");
            menu.addSeparator();
            menu.addItem(34, "Faders to Unity");
            menu.addItem(35, "Faders to OFF");
            menu.addSeparator();
            menu.addItem(36, "Adjust Channel Delay...");
            menu.addItem(37, "Reset Channel Delay...");
            menu.addSeparator();
            menu.addItem(38, "Reset Selected Channels...");
            menu.addItem(39, "Reset Channel Layout");
            menu.addSeparator();
            menu.addItem(40, "Select Fader Bank");
            menu.addItem(41, "Select All Faders");
            menu.addSeparator();
            menu.addItem(42, "Channel Notes");
            break;
        case 3: // "Options"
            menu.addItem(43, "Solo Latch");
            menu.addItem(44, "Link Speakers");
            menu.addSeparator();
            menu.addItem(45, "Automation");
            menu.addItem(46, "Transport");
            menu.addItem(47, "Meters");
            menu.addSeparator();
            menu.addItem(48, "Faders to Tape");
            menu.addSeparator();
            menu.addItem(49, "Use Nine Pin Control");
            menu.addSeparator();
            menu.addItem(50, "HUI Mode");
            break;
        case 4: // "Plugins"
            menu.addItem(51, "Plugins");
            menu.addSeparator();
            menu.addItem(52, "P 1");
            menu.addItem(53, "P 2");
            menu.addItem(54, "P 3");
            menu.addItem(55, "P 4");
            menu.addItem(56, "P 5");
            menu.addItem(57, "P 6");
            menu.addItem(58, "P 7");
            menu.addItem(59, "P 8");
            menu.addItem(60, "P 9");
            menu.addItem(61, "P 10");
            menu.addItem(62, "P 11");
            menu.addItem(63, "P 12");
            menu.addItem(64, "P 13");
            menu.addItem(65, "P 14");
            menu.addItem(66, "P 15");
            menu.addItem(67, "P 16");
            break;
        case 5:
            menu.addItem(68, "About");
            menu.addItem(69, "Help");
            menu.addItem(70, "Hot Keys");
            menu.addSeparator();
            menu.addItem(71, "Close All");
            menu.addItem(72, "Desktop");
            menu.addSeparator();
            menu.addItem(73, "Setup");
            menu.addItem(74, "Snapshot");
            menu.addItem(75, "Surround");
            menu.addItem(76, "Locator");
            menu.addSeparator();
            menu.addItem(77, "Mix Editor");
            menu.addItem(78, "FatChannel");
            menu.addItem(79, "Panning");
            menu.addItem(80, "Faders");
            menu.addItem(81, "Event Track");
            menu.addSeparator();
            menu.addItem(82, "Track Sheet");
            menu.addItem(83, "History List");
            menu.addItem(84, "MIDI Map");
            menu.addSeparator();
            menu.addItem(85, "Upgrade UFX Cards...");
            menu.addItem(86, "Erase UFX Memory...");
            break;
        default:
            break;
    }

    return menu;
}

void TopMenuBarModel::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{
  // Handle menuitem selection
}