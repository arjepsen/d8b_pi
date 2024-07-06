#pragma once

#include "ChannelStripComponent.h"
#include "ChannelClass.h"
#include "ChannelStripClass.h"
#include "ChannelStripInterface.h"
#include "MenuBar.h"
#include "MixerManagerClass.h"
#include "MasterStripComponent.h"
#include "EventBusClass.h"
#include <JuceHeader.h>
#include "SharedDataStructures.h"

//#include "SettingsWindow.h"
//#include "settingsplay.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

class MainComponent : public juce::Component

{

public:
    //==============================================================================

    MainComponent();
    ~MainComponent() override;

    //==============================================================================

    void paint(juce::Graphics &) override;
    void resized() override;

    // My stuff
    //void faderUiMoveCallback(const std::string channelStripComponentID, float newValue);
    //void masterFaderMoveCallback(float newValue);


private:
    //==============================================================================

    MenuBar menuBar;

    // Declare array of UI channelstrips.
    ChannelStripComponent channelStripComponetArray[CHANNEL_STRIP_COUNT];

    // Same for the Channel and Channelstrip objects.
    Channel channelArray[CHANNEL_COUNT]; 
    ChannelStrip channelStripArray[CHANNEL_STRIP_COUNT];    // +1 for the master strip.
    MasterChannel &masterChannel;

    MasterStripComponent masterStripComponent;

    EventBus &eventBus;
    MixerManager &mixerManager;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
