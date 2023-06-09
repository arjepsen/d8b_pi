#include "MainComponent.h"

//==============================================================================

MainComponent::MainComponent()
    : eventBus(EventBus::getInstance()),
      mixerManager(MixerManager::getInstance())
{
    std::cout << "MainComponent Constructor" << std::endl;
    // Main window size.
    setSize(1920, 1080);

    // This would print out the devices
    // settings.printUsbDevices();

    // Show channelStrips.
    for (auto &strip : chStrips)
    {
        addAndMakeVisible(strip);
    }

    // Give the mixermanager a pointer to the channelstrip array.
    mixerManager.setChannelStripComponentArray(chStrips);

    // addAndMakeVisible(channelStrip1);
    // addAndMakeVisible(channelStrip2);
    // addAndMakeVisible(channelStrip3);
    // addAndMakeVisible(channelStrip4);
    // addAndMakeVisible(channelStrip5);
    // addAndMakeVisible(channelStrip6);
    // addAndMakeVisible(channelStrip7);
    // addAndMakeVisible(channelStrip8);
    // addAndMakeVisible(channelStrip9);
    // addAndMakeVisible(channelStrip10);
    // addAndMakeVisible(channelStrip11);
    // addAndMakeVisible(channelStrip12);
    // addAndMakeVisible(channelStrip13);
    // addAndMakeVisible(channelStrip14);
    // addAndMakeVisible(channelStrip15);
    // addAndMakeVisible(channelStrip16);
    // addAndMakeVisible(channelStrip17);
    // addAndMakeVisible(channelStrip18);
    // addAndMakeVisible(channelStrip19);
    // addAndMakeVisible(channelStrip20);
    // addAndMakeVisible(channelStrip21);
    // addAndMakeVisible(channelStrip22);
    // addAndMakeVisible(channelStrip23);
    // addAndMakeVisible(channelStrip24);

    // Show MasterStrip
    addAndMakeVisible(masterStrip);

    // Show MenuBar.
    addAndMakeVisible(menuBar);

    // Set up an array of pointers to the channelStripcomponents
    // ChannelStripComponent chStripComponentArray[24];
}

MainComponent::~MainComponent()
{
}

//==============================================================================

void MainComponent::paint(juce::Graphics &g)

{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    // g.setFont(juce::Font(16.0f));
    // g.setColour(juce::Colours::white);
    // g.drawText("Hello World!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()

{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    menuBar.setBounds(0, 0, getWidth(), 30);

    // Channelstrip placements.
    int strip_x = 0;
    for (auto &strip : chStrips)
    {
        strip.setBounds(strip_x, 28, 75, 1024);
        strip_x += 75;

        // Since we're handling each strip anyway, hand over the callbacks:
        // strip.setFaderUiMoveCallbackFunction
        // (
        //     [this](const std::string channelStripID, float newFaderValue)
        //     {
        //         this->faderUiMoveCallback(channelStripID, newFaderValue);
        //     }
        // );

        // NOPE -SET IT UP THROUGH EVENT BUS INSTEAD

    }

    // channelStrip1.setBounds(0, 28, 75, 1024);
    // channelStrip2.setBounds(75, 28, 75, 1024);
    // channelStrip3.setBounds(150, 28, 75, 1024);
    // channelStrip4.setBounds(225, 28, 75, 1024);
    // channelStrip5.setBounds(300, 28, 75, 1024);
    // channelStrip6.setBounds(375, 28, 75, 1024);
    // channelStrip7.setBounds(450, 28, 75, 1024);
    // channelStrip8.setBounds(525, 28, 75, 1024);
    // channelStrip9.setBounds(600, 28, 75, 1024);
    // channelStrip10.setBounds(675, 28, 75, 1024);
    // channelStrip11.setBounds(750, 28, 75, 1024);
    // channelStrip12.setBounds(825, 28, 75, 1024);
    // channelStrip13.setBounds(900, 28, 75, 1024);
    // channelStrip14.setBounds(975, 28, 75, 1024);
    // channelStrip15.setBounds(1050, 28, 75, 1024);
    // channelStrip16.setBounds(1125, 28, 75, 1024);
    // channelStrip17.setBounds(1200, 28, 75, 1024);
    // channelStrip18.setBounds(1275, 28, 75, 1024);
    // channelStrip19.setBounds(1350, 28, 75, 1024);
    // channelStrip20.setBounds(1425, 28, 75, 1024);
    // channelStrip21.setBounds(1500, 28, 75, 1024);
    // channelStrip22.setBounds(1575, 28, 75, 1024);
    // channelStrip23.setBounds(1650, 28, 75, 1024);
    // channelStrip24.setBounds(1725, 28, 75, 1024);

    // Master strip placement
    masterStrip.setBounds(1800, 28, 120, 1024);

    // // Master strip fader callback
    // masterStrip.setMasterFaderMoveCallbackFunction
    // (
    //     [this](float newFaderValue)
    //     {
    //         this->masterFaderMoveCallback(newFaderValue);
    //     }
    // );
}

// void MainComponent::faderMoveCallback(const std::string channelStripID, float newValue)
// {
//     mixerManager.handleUiFaderMove(channelStripID, newValue);

//     // This method should now call a method in MixerManager, to have the value converted, and put together to a DSP board command.
//     // And send it .... of course....
// }

// void MainComponent::masterFaderMoveCallback(float newValue)
// {
//     mixerManager.handleUiMasterFaderMove(newValue);
// }
