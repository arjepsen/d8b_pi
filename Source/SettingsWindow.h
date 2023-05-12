/*
  ==============================================================================

    SettingsWindow.h
    Created: 10 May 2023 6:40:36pm
    Author:  anders

  ==============================================================================
*/

#pragma once

#include "MixerManagerClass.h"
#include "SettingsComponent.h"
#include <JuceHeader.h>

class SettingsWindow : public juce::DocumentWindow
{
public:
    SettingsWindow(const juce::String &name, juce::Colour backgroundColour, int requiredButtons, bool addToDesktop = true);
    ~SettingsWindow() override;

    static SettingsWindow *currentInstance; // Pointer to the current instance.

    void closeButtonPressed() override;

private:
    std::unique_ptr<SettingsComponent> settingsComponent;
    MixerManager &mixerManager; // Pointer to the MixerManager Singleton Instance

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsWindow)
};