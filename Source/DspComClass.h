/*
  ==============================================================================

    DspWriterClass.h
    Created: 25 Jun 2023 10:51:32pm
    Author:  anders

  ==============================================================================
*/

#pragma once

#include "ComBaseClass.h"

class DspCom : public ComBase
{
private:
    DspCom();  // Constructor
    ~DspCom(); // Destructor

    // Delete copy constructor and assignment operator, to avoid copying the singleton.
    DspCom(const DspCom &) = delete;
    DspCom &operator=(const DspCom &) = delete;

    virtual void messageReceiver() override;

public:
    static DspCom &getInstance(); // Returns a reference to the instance.
};

// Singleton modifications
inline DspCom &DspCom::getInstance()
{
    static DspCom instance;
    return instance;
}


