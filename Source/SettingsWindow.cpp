/*
  ==============================================================================

    SettingsWindow.cpp
    Created: 10 May 2023 6:40:36pm
    Author:  anders

  ==============================================================================
*/

#include "SettingsWindow.h"

SettingsWindow *SettingsWindow::currentInstance = nullptr; // Define the static member.

SettingsWindow::SettingsWindow(const juce::String &name, juce::Colour backgroundColour, int requiredButtons, bool addToDesktop)
    : DocumentWindow(name, backgroundColour, requiredButtons, addToDesktop),
      mixerManager(MixerManager::getInstance())
{
    // setUsingNativeTitleBar(true);
    // setResizable(false, false);
    setAlwaysOnTop(true);
    centreWithSize(400, 300);
    currentInstance = this;

    // Add any necessary components to your window here

    // Get a reference to the MixerManager singleton, and pass it to the SettingsComponent.
    auto &MixerManager = MixerManager::getInstance();
    settingsComponent = std::make_unique<SettingsComponent>(mixerManager);
    
    setContentOwned(settingsComponent.get(), true);
}

SettingsWindow::~SettingsWindow() {}

void SettingsWindow::closeButtonPressed()
{
    currentInstance = nullptr; // Reset the current instance when window closed.
    delete this;
}