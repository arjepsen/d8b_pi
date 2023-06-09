#include "CircularBuffer.h"
#include <cstring>

CircularBuffer::CircularBuffer()
    : head_(0), tail_(0) {}

CircularBuffer::~CircularBuffer() {}

void CircularBuffer::push(const char *message)
{
    std::unique_lock<std::mutex> lock(mutex_);
    const size_t next_head = next_index(head_);
    if (next_head == tail_)
    {
        // Buffer is full. You can decide how to handle this.
        // For now, this implementation will overwrite the oldest element.
        tail_ = next_index(tail_);
    }
    strncpy(buffer_[head_].data(), message, BUFFER_WIDTH - 1);
    buffer_[head_][BUFFER_WIDTH - 1] = '\0'; // Ensure null-termination
    head_ = next_head;

    msgCount++;

    if (maxMsgCount < msgCount)
        maxMsgCount = msgCount;

    lock.unlock();
    condVar_.notify_one(); // Notify reader
}

std::string CircularBuffer::pop()
{
    std::unique_lock<std::mutex> lock(mutex_);
    // If head and tail points to the same, the buffer is empty.
    // Then put the thread to sleep, until signaled. (use while, for spurious wakeups)

    while (head_ == tail_)
    {
        condVar_.wait(lock);
    }

    // Find acutal length of message
    //size_t messageLength = strnlen(buffer_[tail_].data(), BUFFER_WIDTH);

    std::string message(buffer_[tail_].data(), strnlen(buffer_[tail_].data(), BUFFER_WIDTH));
    tail_ = next_index(tail_);

    msgCount--;
    //printf("POP - messages in queue: %d\n", msgCount);


    printf("returning %s, max buffer msgs: %d\n", message.c_str(), maxMsgCount);
    lock.unlock();
    return message;
}

size_t CircularBuffer::next_index(size_t index) const
{
    return (index + 1) % BUFFER_LENGTH;
}
