#pragma once

//#include <array>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <string>

constexpr size_t BUFFER_LENGTH = 30; // The max number of elements in the buffer
constexpr size_t BUFFER_WIDTH = 100; // The max number of characters in each element
// TODO: check the "width" - should max fit the longest mesage that can be received.
// BE AWARE OF LONG STRINGS DURING MIXER BOOT

class CircularBuffer
{
  public:
    // CircularBuffer();
    static CircularBuffer &getInstance(); // Return reference to this instance.

    void push(const char *message, size_t msgLength);
    size_t pop(char *message);	// Returns the length of the message.

  private:
    CircularBuffer();
    ~CircularBuffer();

    // Delete copy constructor & assignment operator, no copying a singleton!
    CircularBuffer(const CircularBuffer &) = delete;
    CircularBuffer &operator=(const CircularBuffer &) = delete;

	// Define the circular buffer array, and a second array to keep the length of each message.
	char buffer_[BUFFER_LENGTH][BUFFER_WIDTH];
	size_t messageLengths_[BUFFER_LENGTH];

    std::atomic<size_t> head_;
    std::atomic<size_t> tail_;
    std::mutex mutex_;
    std::condition_variable condVar_;

    int msgCount = 0;
    int maxMsgCount = 0;

    size_t next_index(size_t index) const;
};

inline CircularBuffer &CircularBuffer::getInstance()
{
    static CircularBuffer instance;
    return instance;
}
