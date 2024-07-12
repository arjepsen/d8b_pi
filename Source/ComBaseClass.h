/*
  ==============================================================================

    ComBaseClass.h
    Created: 27 Jun 2023 9:10:44pm
    Author:  anders

    This is a base class for the BrainCom and DspCom class singletons.
    It takes care of the shared functionality, and the BrainCom and DspCom
    will then mainly implement their own different mesageReceiver method, that
    will be run in their own theread.

    So in mixermanager we will instantiate BrainCom and DspCom, call the setters
    for the port and baudRate, and finally use "startReceiverThread" to wind up
    the communication.
  ==============================================================================
*/

#pragma once

#include "CircularBuffer.h"
#include <mutex>
#include <string>
#include <termios.h>
#include <thread>

class ComBase
{
  protected:
    ComBase();
    ~ComBase();

    // ========= Common Members =========
    int boardCom = -1; // File descriptor
    std::mutex writeMutex;
    std::thread receiverThread;
    bool receiverThreadRunning = false;
    std::string port = "";
    speed_t baudRate = B0;      // Inital
    CircularBuffer &circBuffer; // Buffer singleton reference

    // ========= Common Methods ==========
    int openSerialPort(const char *comPortPath, speed_t baudRate);
    // void flushComBuffer();
    virtual void messageReceiver() = 0; // Pure Virtual Method.

  public:
    void setPort(std::string comPortPath);
    std::string getPort();
    void setBaudRate(speed_t baud);
    speed_t getBaudRate();
    void startReceiverThread();
    // void send(const std::string &command);
    void send(const char *command);
    void send(const char *command, size_t commandLength);
    
};
