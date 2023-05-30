/*
  ==============================================================================

    BankMastersMessageHandlerClass.cpp
    Created: 29 May 2023 7:35:18pm
    Author:  anders

  ==============================================================================
*/

#include "BankMastersMessageHandlerClass.h"

void MastersBankMessageHandler::handleMessage(const std::string &message)
{

}

void MastersBankMessageHandler::setComDescriptors(int brainDescriptor, int dspDescriptor)
{
	brain = brainDescriptor;
	dsp = dspDescriptor;
}