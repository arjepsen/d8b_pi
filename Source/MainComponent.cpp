#include "MainComponent.h"

//==============================================================================

MainComponent::MainComponent()
    : eventBus(EventBus::getInstance()),
      mixerManager(MixerManager::getInstance()),
      masterChannel(MasterChannel::getInstance())
{
    // Main window size.
    setSize(1920, 1080);

    // This would print out the devices
    // settings.printUsbDevices();

    // Show channelStrips.
    for (auto &strip : channelStripComponetArray)
    {
        addAndMakeVisible(strip);
    }

    // Give the mixermanager a pointer to the channelstrip array.
    // mixerManager.setChannelStripComponentArray(chStrips);

    // Set up the pointers to the channelstrips in the eventbus
    eventBus.setChannelStripComponentArray(channelStripComponetArray, &masterStripComponent);
    eventBus.setChannelArray(channelArray);
    eventBus.setChannelStripArray(channelStripArray, &masterChannel);


    // Set the fader event handlers
    


    // Set the initial associations ()
    // This will hand a pointer to a channel to each channelstrip.
    // The channelstrip will then read info from the channel, and update it's settings (leds, fader)
    for (int i = 0; i < CHANNEL_STRIP_COUNT; i++)
    {
        ChStripID chStripID = channelStripArray[i].getChannelStripID();
        //ChStripID chStripID = channelStrip.getChannelStripID();

        // The channelstrips controls one channel per bank. Set their initial pointers.
        for (Bank bank = LINE_BANK; bank < NUMBER_OF_BANKS; bank = (Bank)(bank + 1))
        {
            int channelIndex = chStripID + (bank * CHANNEL_STRIP_COUNT);
            //eventBus.channelStripEventSubscribe(channelIndex, chStripID, bank);

            channelStripArray[i].setChannelPointer(bank, &channelArray[channelIndex]);

            eventBus.channelStripEventSubscribe(channelIndex, chStripID, bank);
        }
    }


    // Show MasterStrip
    addAndMakeVisible(masterStripComponent);

    // Show MenuBar.
    addAndMakeVisible(menuBar);

    // TODO: how about the display.

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
    for (auto &strip : channelStripComponetArray)
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
    masterStripComponent.setBounds(1800, 28, 120, 1024);

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
