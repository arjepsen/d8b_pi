/*
  ==============================================================================

    Debug.h
    Created: 11 May 2024 11:16:01am
    Author:  anders

  ==============================================================================
*/

#pragma once
//#include <cstdio>  // For printf

// Uncomment this to enable debug messages.
#define DEBUG_MESSAGES

// Base debug message definition
#ifdef DEBUG_MESSAGES
#define DEBUG_MSG_BASE(file, line, format, ...) printf("DEBUG: %s:%d: " format, file, line, ##__VA_ARGS__)
#else
#define DEBUG_MSG_BASE(file, line, format, ...) ((void)0)  // No operation
#endif

// Convenience macro to automatically fill in file and line
#define DEBUG_MSG(format, ...) DEBUG_MSG_BASE(__FILE__, __LINE__, format, ##__VA_ARGS__)
