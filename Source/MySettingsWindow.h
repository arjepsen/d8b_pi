/*
  ==============================================================================

    MySettingsWindow.h
    Created: 9 May 2023 9:36:29pm
    Author:  anders

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class MySettingsWindow : public juce::DocumentWindow
{
public:
    MySettingsWindow() : juce::DocumentWindow ("Settings", 
                                                juce::Colours::white, 
                                                juce::DocumentWindow::allButtons)
    {
        setResizable(true, true);
        setContentOwned(new juce::Component(), true);
        centreWithSize(500, 500);
        setVisible(true);
    }
};
