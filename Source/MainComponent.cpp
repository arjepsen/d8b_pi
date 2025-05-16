#include "MainComponent.h"

/*******************************************************************
 * @brief Constructor for MainComponent.
 *        Here we do some initial setups, like handing pointers of
 *        the channelstrips and channels to the EventBus, and a few
 *        UI things.
 ******************************************************************/
MainComponent::MainComponent()
    : eventBus(EventBus::getInstance()),
      mixerManager(MixerManager::getInstance()),
      masterChannel(MasterChannel::getInstance())
{
    // Main window size.
    setSize(1920, 1080);

    // Show channelStrips.
    for (auto &strip : channelStripComponentArray)
    {
        addAndMakeVisible(strip);
    }

    // Use eventBus methods to hand it some pointers to the channel
    // and channelstrip objects.
    eventBus.setChannelStripComponentArray(channelStripComponentArray, &masterStripComponent);
    eventBus.setChannelArray(channelArray);
    eventBus.setChannelStripArray(channelStripArray, &masterChannel);

    // Set the initial associations ()
    // This will hand a pointer to a channel to each channelstrip.
    // The channelstrip will then read info from the channel, and update it's settings (leds, fader)
    for (int i = 0; i < CHANNEL_STRIP_COUNT; i++)
    {
        ChStripID chStripID = channelStripArray[i].getChannelStripID();

        // The channelstrips controls one channel per bank. Set their initial pointers.
        for (Bank bank = LINE_BANK; bank < NUMBER_OF_BANKS; bank = (Bank)(bank + 1))
        {
            int channelIndex = chStripID + (bank * CHANNEL_STRIP_COUNT);
            channelStripArray[i].setChannelPointer(bank, &channelArray[channelIndex]);
            eventBus.channelStripEventSubscribe(channelIndex, chStripID, bank);
        }
    }

    // Show MasterStrip
    addAndMakeVisible(masterStripComponent);

    // Show MenuBar.
    addAndMakeVisible(menuBar);

    // TODO: how about the console display?
}

MainComponent::~MainComponent()
{
}

void MainComponent::paint(juce::Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

/******************************************************************
 * @brief This is called when the MainComponent is resized.
 *        When adding child components, this is where the positions
 *        gets updated.
 ******************************************************************/
void MainComponent::resized()
{
    menuBar.setBounds(0, 0, getWidth(), 30);

    // Channelstrip placements.
    int strip_x = 0;
    for (auto &strip : channelStripComponentArray)
    {
        strip.setBounds(strip_x, 28, 75, 1024);
        strip_x += 75;
    }

    // Master strip placement
    masterStripComponent.setBounds(1800, 28, 120, 1024);
}
