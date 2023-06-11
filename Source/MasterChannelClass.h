/*
  ==============================================================================

    MasterChannel.h
    Created: 11 Jun 2023 12:14:52pm
    Author:  anders

  ==============================================================================
*/

#pragma once

#include <string>

class MasterChannel
{
private:
	// Private constructor, since this is a singleton
	MasterChannel();
	//~MasterChannel();

	// Delete copy constructor and assignment operator, to avoid copying the singleton.
    MasterChannel(const MasterChannel &) = delete;
    MasterChannel &operator=(const MasterChannel &) = delete;


	std::string masterVolume;


public:
	static MasterChannel &getInstance(); // Returns a reference to the instance.
	void setMasterVolume(std::string volumeString, int dspDescriptor);
};

// Singleton modifications
inline MasterChannel &MasterChannel::getInstance()
{
    static MasterChannel instance;
    return instance;
}