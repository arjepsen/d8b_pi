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

#include <mutex>
#include <string>
#include <termios.h>
#include <thread>
#include "CircularBuffer.h"

class BrainCom
{
private:
    BrainCom();  // Constructor
    ~BrainCom(); // Destructor

    // Delete copy constructor and assignment operator, to avoid copying the singleton.
    BrainCom(const BrainCom
                 &) = delete;
    BrainCom &operator=(const BrainCom
                            &) = delete;

    std::mutex brainWriteMutex; // Mutex for thread-safe write operations.

    int BrainCom ::openSerialPort(const char *devicePath, speed_t baudRate);

    std::string brainPort;
    int brain; // This is the file descripter

    std::thread brainReceiverThread;

    void brainMessageReceiver();

    CircularBuffer &circBuffer;

public:
    static BrainCom &getInstance(); // Returns a reference to the instance.

    void settingSetBrainPort(std::string deviceString);
    void setBrainPort(std::string port);
    std::string getBrainPort();

    void flushBuffer();

    void initializeWriter();

    const std::string &read();
    void sendCmd(const std::string &command);
};

// Singleton modifications
inline BrainCom &BrainCom::getInstance()
{
    static BrainCom
        instance;
    return instance;
}
