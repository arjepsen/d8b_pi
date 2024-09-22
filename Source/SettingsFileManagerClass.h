/*
  ==============================================================================

    SettingsFileManagerClass.h
    Created: 29 Aug 2024 10:46:18pm
    Author:  anders

    We need to be able to save settings. This class is meant to handle this.
    The idea is to keep the file open consistently, to avoid having to open/close
    at every write - this will optimize, since we are aiming to write changes
    to settings as they happen.
    TODO: Maybe the class should occationally write to a backup file?

  ==============================================================================
*/

THIS IS AIMING AT HANDLING ALL SETTINGS, WRITING ALL EVERY TIME.
INEFFECIENT.
LETS MAKE IT SO WE CAN EFFECIENTLY CHANGE A SINGLE SETTING.
BUT HOW OFTEN DO WE WRITE?
DO WE REALLY WANT TO WRITE ON EVERYTHING, OR INSTEAD ON MAIN CHANGES?
NO - THERE IS A SAVE OPTION - SO WE ONLY SAVE TO FILE WHEN THAT IT USED.
SO WE MUST KEEP A "STANDARD SETTINGS" FILE....

from manual:
New Session (Ctrl+N)
• Press Ctrl+N or click the File menu, drag down to
New Session, and release.
• Select this to—wait, let me guess—start a new
session.
• The OS will ask you to save changes to the
current session, if necessary.
• Next, you get the chance to name the new session,
select which folder it ends up in, create a new
folder to put it in, or cancel.
• The session can be saved to the internal hard
drive or to a 3.5" floppy disk.
• A session consists of all snapshots, automation
data, and—if you choose—custom EQ, effects,
and dynamics patches.
• A new session uses the Template as its initial
status"


autosave feature.....

a new session is using a template, but we can also save as template to update the ... tadaaa... template.

the following are stored in the Template:
• Channel Name
• Noise Filter setting (in Channel List)
• Channel Layout (rearrangement)
• MDS Network settings
• Stereo/Surround setting
• Current Tempo (default to 120 bpm)
• Time View (SMPTE/BBT/Milliseconds)
• Mix Editor – Grid Setting (resolution—called
Snap Grid in the pull-down), Enable Snapping,
Snap to Cues, Snap to Grid

#pragma once

#include <cstdio>
#include <mutex>

class SettingsFileManager
{
  private:
    FILE *file;
    std::mutex fileMutex;
    const char *filename;

  public:
    SettingsFileManager(const char *filename) : filename(filename), file(nullptr)
    {
        openFile();
    }

    ~SettingsFileManager()
    {
        closeFile();
    }

    void openFile()
    {
        std::lock_guard<std::mutex> lock(fileMutex);
        if (!file)
        {
            file = fopen(filename, "r+b"); // Open for reading and writing in binary mode
            if (!file)
            { // File does not exist, create it
                file = fopen(filename, "w+b");
            }
        }
    }

    void closeFile()
    {
        std::lock_guard<std::mutex> lock(fileMutex);
        if (file)
        {
            fclose(file);
            file = nullptr;
        }
    }

    bool writeSettings(const void *settings, size_t size)
    {
        std::lock_guard<std::mutex> lock(fileMutex);
        if (file && fseek(file, 0, SEEK_SET) == 0)
        {
            size_t written = fwrite(settings, size, 1, file);
            fflush(file); // Flush the file buffer to ensure data is written to disk
            return written == 1;
        }
        return false;
    }

    bool readSettings(void *settings, size_t size)
    {
        std::lock_guard<std::mutex> lock(fileMutex);
        if (file && fseek(file, 0, SEEK_SET) == 0)
        {
            size_t read = fread(settings, size, 1, file);
            return read == 1;
        }
        return false;
    }
};
