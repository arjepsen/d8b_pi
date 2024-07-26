/*
  ==============================================================================

    FileManager.cpp
    Created: 26 Jul 2024 1:13:50pm
    Author:  anders

  ==============================================================================
*/

#include "FileManager.h"
#include <cstdio>
#include <unistd.h>

FileManager::FileManager(const std::string &filename)
    : filename(filename) {}

bool FileManager::save(const void *data, size_t dataSize)
{
    std::string tempFile = filename + ".tmp";
    if (!writeToFile(data, dataSize, tempFile))
    {
        return false;
    }

    if (rename(tempFile.c_str(), filename.c_str()) != 0)
    {
        perror("Error renaming temporary file");
        return false;
    }
    return true;
}

bool FileManager::load(void *data, size_t dataSize)
{
    return readFromFile(data, dataSize, filename);
}

bool FileManager::writeToFile(const void *data, size_t dataSize, const std::string &filepath)
{
    FILE *file = fopen(filepath.c_str(), "wb");
    if (file == NULL)
    {
        perror("Failed to open file");
        return false;
    }

    if (fwrite(data, dataSize, 1, file) != 1)
    {
        perror("Failed to write data");
        fclose(file);
        return false;
    }

    fflush(file);
    fsync(fileno(file));
    fclose(file);
    return true;
}

bool FileManager::readFromFile(void *data, size_t dataSize, const std::string &filepath)
{
    FILE *file = fopen(filepath.c_str(), "rb");
    if (file == NULL)
    {
        perror("Failed to open file");
        return false;
    }

    if (fread(data, dataSize, 1, file) != 1)
    {
        perror("Failed to read data");
        fclose(file);
        return false;
    }

    fclose(file);
    return true;
}
