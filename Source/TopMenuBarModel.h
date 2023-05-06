/*
  ==============================================================================

    TopMenuBarModel.h
    Created: 6 May 2023 8:02:12pm
    Author:  anders

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class TopMenuBarModel : public juce::MenuBarModel
{
public:
    TopMenuBarModel();
    ~TopMenuBarModel() override;

    juce::StringArray getMenuBarNames() override; // Returns an array of menu names (File, Edit, etc..)
    juce::PopupMenu getMenuForIndex(int menuIndex, const juce::String &menuName) override;
    void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;
};