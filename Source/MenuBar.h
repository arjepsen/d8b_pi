/*
  ==============================================================================

    TestBar.h
    Created: 7 May 2023 1:54:29pm
    Author:  anders

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
 */
class MenuBar : public juce::Component,
                public juce::MenuBarModel
// public juce::ApplicationCommandTarget

{
public:
    MenuBar();
    ~MenuBar() override;

    void paint(juce::Graphics &) override;
    void resized() override;

    juce::StringArray getMenuBarNames() override; // Returns an array of menu names (File, Edit, etc..)
    juce::PopupMenu getMenuForIndex(int menuIndex, const juce::String &menuName) override;
    void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;

private:
    std::unique_ptr<juce::MenuBarComponent> menuBar;
    // juce::ApplicationCommandManager commandManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MenuBar)
};
