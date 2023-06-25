/*
  ==============================================================================

    DspWriterClass.h
    Created: 25 Jun 2023 10:51:32pm
    Author:  anders

  ==============================================================================
*/

#pragma once

class DspWriter
{
private:
    DspWriter();  // Constructor
    ~DspWriter(); // Destructor

    // Delete copy constructor and assignment operator, to avoid copying the singleton.
    DspWriter(const DspWriter &) = delete;
    DspWriter &operator=(const DspWriter &) = delete;

public:
    static DspWriter &getInstance(); // Returns a reference to the instance.
};

// Singleton modifications
inline DspWriter &DspWriter::getInstance()
{
    static DspWriter instance;
    return instance;
}