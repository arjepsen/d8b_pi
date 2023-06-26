/*
  ==============================================================================

    BrainWriterClass.h
    Created: 25 Jun 2023 10:52:04pm
    Author:  anders

	This Class is used for sending commands to the Brain board, AFTER the mixer
	has been initialized.

  ==============================================================================
*/

#pragma once

#include <termios.h>

class BrainWriter
{
private:
    BrainWriter();  // Constructor
    ~BrainWriter(); // Destructor

    // Delete copy constructor and assignment operator, to avoid copying the singleton.
    BrainWriter(const BrainWriter &) = delete;
    BrainWriter &operator=(const BrainWriter &) = delete;

	int BrainWriter::openSerialPort(const char *devicePath, speed_t baudRate);

	//std::string brainPort;
	int brain;	// This is the file descripter

public:
    static BrainWriter &getInstance(); // Returns a reference to the instance.

	void initializeWriter()

	void read(std::string message);
	void write(std::string command);

};

// Singleton modifications
inline BrainWriter &BrainWriter::getInstance()
{
    static BrainWriter instance;
    return instance;
}