#pragma once

#include <JuceHeader.h>
#include "ChannelStripComponent.h"

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

private:
    //==============================================================================

    // Your private member variables go here...

    ChannelStripComponent channelStrip1;
    ChannelStripComponent channelStrip2;
    ChannelStripComponent channelStrip3;
    ChannelStripComponent channelStrip4;
    ChannelStripComponent channelStrip5;
    ChannelStripComponent channelStrip6;
    ChannelStripComponent channelStrip7;
    ChannelStripComponent channelStrip8;
    ChannelStripComponent channelStrip9;
    ChannelStripComponent channelStrip10;
    ChannelStripComponent channelStrip11;
    ChannelStripComponent channelStrip12;
    ChannelStripComponent channelStrip13;
    ChannelStripComponent channelStrip14;
    ChannelStripComponent channelStrip15;
    ChannelStripComponent channelStrip16;
    ChannelStripComponent channelStrip17;
    ChannelStripComponent channelStrip18;
    ChannelStripComponent channelStrip19;
    ChannelStripComponent channelStrip20;
    ChannelStripComponent channelStrip21;
    ChannelStripComponent channelStrip22;
    ChannelStripComponent channelStrip23;
    ChannelStripComponent channelStrip24;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
