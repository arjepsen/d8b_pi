/*
  ==============================================================================

    TestBar.cpp
    Created: 7 May 2023 1:54:29pm
    Author:  anders

  ==============================================================================
*/

#include "MenuBar.h"
#include <JuceHeader.h>

//==============================================================================
MenuBar::MenuBar()
{
    // Create the menubarcomponent, and show it.
    menuBar.reset(new juce::MenuBarComponent(this));
    addAndMakeVisible(menuBar.get());

    // setApplicationCommandManagerToWatch(&commandManager);
    // commandManager.registerAllCommandsForTarget(this);

    // // Let cmdMangr use keypresses.
    // addKeyListener(commandManager.getKeyMappings());

    // Not sure what setSize here would do....
    // setSize(100, 10);
    
}

MenuBar::~MenuBar() {}

void MenuBar::paint(juce::Graphics &g)
{
    // Set background for the MenuBar - draw a rectangle around it.
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1); // draw an outline around the component
}

void MenuBar::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    menuBar->setBounds(getLocalBounds());
}

juce::StringArray MenuBar::getMenuBarNames()
{
    juce::StringArray names{"File", "Edit", "Channel", "Options", "Plugins", "Windows"};
    return names;
}

// This method populates a popupmenu, and returns it.
juce::PopupMenu MenuBar::getMenuForIndex(int menuIndex, const juce::String &menuName)
{
    juce::PopupMenu menu;
    juce::PopupMenu automationSubMenu;
    juce::PopupMenu transportSubMenu;
    juce::PopupMenu metersSubMenu;
    juce::PopupMenu meterTypeSubMenu;
    juce::PopupMenu meterRefPointSubMenu;

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
            menu.addSeparator();
            menu.addItem(87, "Settings");
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

            // Automation submenu
            automationSubMenu.addItem(450, "Bypass");
            automationSubMenu.addSeparator();
            automationSubMenu.addItem(451, "Trim Levels");
            automationSubMenu.addItem(452, "Auto Touch");
            automationSubMenu.addSeparator();
            automationSubMenu.addItem(453, "Faders");
            automationSubMenu.addItem(454, "Mutes");
            automationSubMenu.addItem(455, "Pan");
            automationSubMenu.addItem(456, "All");
            automationSubMenu.addSeparator();
            automationSubMenu.addItem(457, "Write Flyback");
            automationSubMenu.addItem(458, "Write Ready Mode");
            menu.addSubMenu("Automation", automationSubMenu);

            // Transport submenu
            transportSubMenu.addItem(460, "Use PreRoll");
            transportSubMenu.addItem(461, "Record Safe");
            transportSubMenu.addItem(462, "One Button Punch");
            menu.addSubMenu("Transport", transportSubMenu);

            // MeterType (sub)submenu
            meterTypeSubMenu.addItem(470, "Pre Fader");
            meterTypeSubMenu.addItem(471, "Post Fader");
            meterTypeSubMenu.addItem(472, "Post Mute/Fader");

            // Meter Reference Point (sub)submenu
            meterRefPointSubMenu.addItem(473, "-10 dB");
            meterRefPointSubMenu.addItem(474, "-15 dB");
            meterRefPointSubMenu.addItem(475, "-20 dB");

            // Meters submenu
            metersSubMenu.addSubMenu("Meter Type", meterTypeSubMenu);
            metersSubMenu.addSubMenu("Meter Reference Point", meterRefPointSubMenu);
            metersSubMenu.addSeparator();
            metersSubMenu.addItem(476, "Show Meters In Trim Mode");
            metersSubMenu.addItem(477, "Show Peak Holds");
            metersSubMenu.addSeparator();
            metersSubMenu.addItem(478, "Clear Overloads");
            menu.addSubMenu("Meters", metersSubMenu);

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
        case 5: // "Windows"
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

void MenuBar::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{
    // Handle menuitem selection
    if (menuItemID == 87)
    {
        std::cout << "MenuBar::menuItemSelected. (settings chosen)" << std::endl;

        // Check if settings window is already open.
        if (SettingsWindow::currentInstance != nullptr)
        {
            SettingsWindow::currentInstance->toFront(true);
        }
        else
        {
            auto *window = new SettingsWindow("Settings", juce::Colours::lightgrey, juce::DocumentWindow::allButtons);
            window->setVisible(true);
        }
    }
}