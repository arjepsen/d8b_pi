/*
  ==============================================================================

    FileManager.h
    Created: 26 Jul 2024 1:13:50pm
    Author:  anders

  ==============================================================================
*/

#pragma once

#include <string>

class FileManager
{
  public:
    FileManager(const std::string &filename);
    bool save(const void *data, size_t dataSize);
    bool load(void *data, size_t dataSize);

  private:
    std::string filename;
    bool writeToFile(const void *data, size_t dataSize, const std::string &filepath);
    bool readFromFile(void *data, size_t dataSize, const std::string &filepath);
};

// WE SHOULD KEEP ONE GENERAL FILE THAT WILL ALWAYS BE LOADED ON STARTUP.
// IF THE USER MAKES A NEW FILE, THIS WILL THEN OVERWRITE THE GENERAL
// SO THE GENERAL FILE SHOULD ALWAYS ONLY BE A COPY OF THE CURRENTLY
// CHOSEN FILE.