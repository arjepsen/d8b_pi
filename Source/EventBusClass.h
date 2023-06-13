/*
  ==============================================================================

    EventBusClass.h
    Created: 11 Jun 2023 8:59:37pm
    Author:  anders

  ==============================================================================
*/

#pragma once

//#include <any>
#include <functional>
#include <string>
#include <unordered_map>
#include <array>


enum EventType
{
	FADER_EVENT,
	VPOT_EVENT,
	BUTTON_EVENT
};

enum BankEventType
{
	LINE_EVENT,
	TAPE_EVENT,
	EFFECTS_EVENT,
	MASTERS_EVENT
};

struct EventData
{
    //std::unordered_map<std::string, std::any> data;
};

// void exampleMethod()
// {
//     //////// Example
//     EventData eventData;

//     eventData.data["channel"] = (std::string) "00";
//     eventData.data["channelChar"] = "00";
//     eventData.data["faderValue"] = "C1";
//     eventBus.post(FADER_MOVE, eventData);
// }

// ////// And the subscriber
// eventBus.subscribe(FADER_MOVED, [](const EventData &data)
// 	{
// 		int channel = std::any_cast<int>(data.data.at("channel"));
// 		float volume = std::any_cast<float>(data.data.at("volume"));
// 		// Handle the event here...
// 	}
// );

class EventBus
{
private:
    // Map of callbackfunctions for various events...I think...
    // std::unordered_map<EventType, std::vector<std::function<void(const EventData &)>>> handlers;
	// static constexpr int CHANNELS_COUNT = 48;
	// static constexpr int BANKS_COUNT = 4;

    // std::array<std::array<std::function<void(const EventData &)>, CHANNELS_COUNT>, BANKS_COUNT> handlers;
    // Bank currentBank;

    EventBus();  // Constructor
    ~EventBus(); // Destructor

    // Delete copy constructor and assignment operator, to avoid copying the singleton.
    EventBus(const EventBus &) = delete;
    EventBus &operator=(const EventBus &) = delete;


	std::array<std::unordered_map<std::string, std::function<void(const std::string&)>>, 3> lineBankCallbacks;
	std::array<std::unordered_map<std::string, std::function<void(const std::string&)>>, 3> tapeBankCallbacks;
	std::array<std::unordered_map<std::string, std::function<void(const std::string&)>>, 3> effectsBankCallbacks;
	std::array<std::unordered_map<std::string, std::function<void(const std::string&)>>, 3> mastersBankCallbacks;


public:

	static EventBus &getInstance(); // Returns a reference to the instance.

	void subscribe();	// CHECK NECESSARY PARAMETERS - something like EventType, ChannelID, Callback...??

	void lineBankChStripPost(EventType eventType, std::string ChannelStripID, std::string eventValue);
	void tapeBankChStripPost(EventType eventType, std::string ChannelStripID, std::string eventValue);
	void effectsBankChStripPost(EventType eventType, std::string ChannelStripID, std::string eventValue);
	void mastersBankChStripPost(EventType eventType, std::string ChannelStripID, std::string eventValue);

};

// Singleton modifications
inline EventBus &EventBus::getInstance()
{
    static EventBus instance;
    return instance;
}