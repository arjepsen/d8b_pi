/**********************************************************************
 * @file MainComponent.h
 * @author Anders R. Jepsen
 * @brief The "Main Component" is run inside the JUCE program window.
 *        This is effectively the starting point of the program.
 *        Here we instantiate various UI objects, and also the
 *        Channelstrip and channel objects, along with the EventBus and
 *        MixerManager.
 * @version 0.1
 * @date 2024-07-26
 **********************************************************************/

#pragma once

#include "ChannelClass.h"
#include "ChannelStripClass.h"
#include "ChannelStripComponent.h"
#include "ChannelStripInterface.h"
#include "EventBusClass.h"
#include "MasterStripComponent.h"
#include "MenuBar.h"
#include "MixerManagerClass.h"
#include "SharedDataStructures.h"
#include <JuceHeader.h>

class MainComponent : public juce::Component
{

  public:
    MainComponent();
    ~MainComponent() override;

    void paint(juce::Graphics &) override;
    void resized() override;

  private:
    // Instantiate the top menubar component.
    MenuBar menuBar;

    // Declare arrays of ChannelStrip objects, both UI and for console.
    // This declaration will instantiate the objects, running their constructors.
    ChannelStripComponent channelStripComponentArray[CHANNEL_STRIP_COUNT];
    ChannelStrip channelStripArray[CHANNEL_STRIP_COUNT];

    // Likewise, declare array of Channel objects.
    Channel channelArray[CHANNEL_COUNT];

    // Instantiate the Master channel (strip) and the master UI strip.
    MasterChannel &masterChannel;
    MasterStripComponent masterStripComponent;

    // Finally, instantiate the EventBus and MixerManager singletons.
    EventBus &eventBus;
    MixerManager &mixerManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
