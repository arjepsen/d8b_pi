/*
  ==============================================================================

    settingsplay.h
    Created: 5 May 2023 6:13:19pm
    Author:  anders

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SettingsWindow : public juce::DocumentWindow
{
public:
    SettingsWindow() : DocumentWindow("Settings", juce::Colours::lightgrey, DocumentWindow::allButtons)
    {
        // Create the settings component and add it to the window
        auto settingsComponent = std::make_unique<Component>();
        auto label = std::make_unique<juce::Label>("Label", "KurtLabel");
        settingsComponent->addAndMakeVisible(label.get());
        settingsComponent->setSize(800, 600);
        setContentOwned(settingsComponent.release(), true);
        //setMenuBar();
    }

    void closeButtonPressed() override
    {
        // Override the closeButtonPressed() function to hide the window instead of closing it
        setVisible(false);
    }
};
