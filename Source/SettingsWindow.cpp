/*
  ==============================================================================

    SettingsWindow.cpp
    Created: 10 May 2023 6:40:36pm
    Author:  anders

  ==============================================================================
*/

#include "SettingsWindow.h"





SettingsWindow::SettingsWindow(const juce::String& name, juce::Colour backgroundColour, int requiredButtons, bool addToDesktop)
    : DocumentWindow(name, backgroundColour, requiredButtons, addToDesktop)
{
    //setUsingNativeTitleBar(true);
    //setResizable(false, false);
    centreWithSize(400, 300);

    // Add any necessary components to your window here
    
}

SettingsWindow::~SettingsWindow() {}

void SettingsWindow::closeButtonPressed()
{
    delete this;
}