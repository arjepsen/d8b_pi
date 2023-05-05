#include "MainComponent.h"

//==============================================================================

MainComponent::MainComponent()

{
    //setSize(800,600);
    //addAndMakeVisible(settingsWindow);

    setSize(1920, 1080);
    addAndMakeVisible(channelStrip1);
    addAndMakeVisible(channelStrip2);
    addAndMakeVisible(channelStrip3);
    addAndMakeVisible(channelStrip4);
    addAndMakeVisible(channelStrip5);
    addAndMakeVisible(channelStrip6);
    addAndMakeVisible(channelStrip7);
    addAndMakeVisible(channelStrip8);
    addAndMakeVisible(channelStrip9);
    addAndMakeVisible(channelStrip10);
    addAndMakeVisible(channelStrip11);
    addAndMakeVisible(channelStrip12);
    addAndMakeVisible(channelStrip13);
    addAndMakeVisible(channelStrip14);
    addAndMakeVisible(channelStrip15);
    addAndMakeVisible(channelStrip16);
    addAndMakeVisible(channelStrip17);
    addAndMakeVisible(channelStrip18);
    addAndMakeVisible(channelStrip19);
    addAndMakeVisible(channelStrip20);
    addAndMakeVisible(channelStrip21);
    addAndMakeVisible(channelStrip22);
    addAndMakeVisible(channelStrip23);
    addAndMakeVisible(channelStrip24);

}

MainComponent::~MainComponent()

{
}

//==============================================================================

void MainComponent::paint(juce::Graphics &g)

{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.setFont(juce::Font(16.0f));
    g.setColour(juce::Colours::white);
    g.drawText("Hello World!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()

{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    //settingsWindow.setBounds(10, 10, 300, 120);

    channelStrip1.setBounds(0, 28, 75, 1024);
    channelStrip2.setBounds(76, 28, 75, 1024);
    channelStrip3.setBounds(152, 28, 75, 1024);
    channelStrip4.setBounds(228, 28, 75, 1024);
    channelStrip5.setBounds(304, 28, 75, 1024);
    channelStrip6.setBounds(380, 28, 75, 1024);
    channelStrip7.setBounds(456, 28, 75, 1024);
    channelStrip8.setBounds(532, 28, 75, 1024);
    channelStrip9.setBounds(608, 28, 75, 1024);
    channelStrip10.setBounds(684, 28, 75, 1024);
    channelStrip11.setBounds(760, 28, 75, 1024);
    channelStrip12.setBounds(836, 28, 75, 1024);
    channelStrip13.setBounds(912, 28, 75, 1024);
    channelStrip14.setBounds(988, 28, 75, 1024);
    channelStrip15.setBounds(1064, 28, 75, 1024);
    channelStrip16.setBounds(1140, 28, 75, 1024);
    channelStrip17.setBounds(1216, 28, 75, 1024);
    channelStrip18.setBounds(1292, 28, 75, 1024);
    channelStrip19.setBounds(1368, 28, 75, 1024);
    channelStrip20.setBounds(1444, 28, 75, 1024);
    channelStrip21.setBounds(1520, 28, 75, 1024);
    channelStrip22.setBounds(1596, 28, 75, 1024);
    channelStrip23.setBounds(1064, 28, 75, 1024);
    channelStrip24.setBounds(1064, 28, 75, 1024);

}
