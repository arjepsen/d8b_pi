/*
  ==============================================================================

    SettingsWindow.h
    Created: 10 May 2023 6:40:36pm
    Author:  anders

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SettingsWindow : public juce::DocumentWindow
{
public:
    SettingsWindow(const juce::String& name, juce::Colour backgroundColour, int requiredButtons, bool addToDesktop = true);

    ~SettingsWindow() override;

    void closeButtonPressed() override;

private:
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsWindow)
};