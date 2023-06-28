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

#include "ComBaseClass.h"
// #include <mutex>
// #include <string>
// #include <termios.h>
// #include <thread>
//#include "CircularBuffer.h"

class BrainCom : public ComBase
{
private:
    BrainCom();  // Constructor
    ~BrainCom(); // Destructor

    // Delete copy constructor and assignment operator, to avoid copying the singleton.
    BrainCom(const BrainCom &) = delete;
    BrainCom &operator=(const BrainCom &) = delete;

	bool brainBoostState = false;

    virtual void messageReceiver() override;

    void heartbeatReceived();


	//CircularBuffer &circBuffer;	// Buffer singleton reference


    // std::mutex brainWriteMutex; // Mutex for thread-safe write operations.

    // int openSerialPort(const char *devicePath, speed_t baudRate);

    // std::string brainPort = "";
    

    // int brain; // This is the file descripter

    // std::thread brainReceiverThread;
    // bool brainThreadRunning = false;



public:
    static BrainCom &getInstance(); // Returns a reference to the instance.

    // void settingSetBrainPort(std::string deviceString);
    // void setBrainPort(std::string port);
    // std::string getBrainPort();

    // void flushBuffer();

    // void initializeBrainCom();

    // const std::string &getMessage();
    // void sendCmd(const std::string &command);

    // void startBrainReceiverThread();
};

// Singleton modifications
inline BrainCom &BrainCom::getInstance()
{
    static BrainCom instance;
    return instance;
}
