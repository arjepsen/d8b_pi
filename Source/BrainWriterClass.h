/*
  ==============================================================================

    BrainWriterClass.h
    Created: 25 Jun 2023 10:52:04pm
    Author:  anders

  ==============================================================================
*/

#pragma once

class BrainWriter
{
private:
    BrainWriter();  // Constructor
    ~BrainWriter(); // Destructor

    // Delete copy constructor and assignment operator, to avoid copying the singleton.
    BrainWriter(const BrainWriter &) = delete;
    BrainWriter &operator=(const BrainWriter &) = delete;

public:
    static BrainWriter &getInstance(); // Returns a reference to the instance.
};

// Singleton modifications
inline BrainWriter &BrainWriter::getInstance()
{
    static BrainWriter instance;
    return instance;
}