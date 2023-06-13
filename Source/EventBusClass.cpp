/*
  ==============================================================================

    EventBusClass.cpp
    Created: 11 Jun 2023 8:59:37pm
    Author:  anders

  ==============================================================================
*/

#include "EventBusClass.h"


EventBus::EventBus() {}
EventBus::~EventBus() {}


void EventBus::subscribe() // CHECK NECESSARY PARAMETERS - something like EventType, ChannelID, Callback...??
{

}	

void EventBus::lineBankChStripPost(EventType eventType, std::string ChannelStripID, std::string eventValue) {}
void EventBus::tapeBankChStripPost(EventType eventType, std::string ChannelStripID, std::string eventValue) {}
void EventBus::effectsBankChStripPost(EventType eventType, std::string ChannelStripID, std::string eventValue) {}
void EventBus::mastersBankChStripPost(EventType eventType, std::string ChannelStripID, std::string eventValue) {}