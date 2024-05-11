#include "CircularBuffer.h"
#include <cstring>
#include "Debug.h"

CircularBuffer::CircularBuffer()
    : head_(0), tail_(0) 
    {
        DEBUG_MSG("CIRCBUF CONSTRUCTOR\n");
    }

CircularBuffer::~CircularBuffer() {}

/***********************************************************
 * @brief Method used for pushing a message on to the queue.
 *      // ENSURE MESSAGE LENGTH IS BELOW "WIDTH"
 *
 * @param message
 **********************************************************/
void CircularBuffer::push(const char *message, size_t msgLength)
{
    // Lock mutex to avoid race conditions.
    std::unique_lock<std::mutex> lock(mutex_);

    const size_t next_head = next_index(head_);
    if (next_head == tail_)
    {
        // Buffer is full.
        // TODO: You can decide how to handle this.
        // For now, this implementation will overwrite the oldest element.
        tail_ = next_index(tail_);
    }

    // Copy message to buffer.
    // strncpy(buffer_[head_].data(), message, BUFFER_WIDTH - 1);
    // buffer_[head_][BUFFER_WIDTH - 1] = '\0'; // Ensure null-termination
    strcpy(buffer_[head_], message);
    head_ = next_head;

    // Write the message length (index of last char) to the message length array.
    messageLengths_[head_] = msgLength;

    // Increment buffer-message-count.
    // TODO: Do we want to use this for anything? Else remove.
    msgCount++;

    if (maxMsgCount < msgCount) maxMsgCount = msgCount;

    // Release mutex, and notify.
    lock.unlock();
    condVar_.notify_one(); // Notify reader
}

// std::string CircularBuffer::pop()
// {
//     // Lock mutex.
//     std::unique_lock<std::mutex> lock(mutex_);

//     // If head and tail points to the same, the buffer is empty.
//     // Then put the thread to sleep, until signaled. (use while, for spurious wakeups)
//     while (head_ == tail_)
//     {
//         condVar_.wait(lock);
//     }

//     // Ok, we're asked for a message, and there's something in the queue.
//     // Find acutal length of message
//     // size_t messageLength = strnlen(buffer_[tail_].data(), BUFFER_WIDTH);

//     std::string message(buffer_[tail_].data(), strnlen(buffer_[tail_].data(), BUFFER_WIDTH));
//     tail_ = next_index(tail_);

//     msgCount--;
//     // printf("POP - messages in queue: %d\n", msgCount);

//     printf("returning %s, max buffer msgs: %d\n", message.c_str(), maxMsgCount);
//     lock.unlock();
//     return message;
// }

/****************************************************************
 * @brief Method for popping messages.
 *          MAKE SURE TO USE THE HEADER's WIDTH DEFINITION
 *          FOR THE SIZE OF THE CHAR ARRAY ARGUMENT
 *
 * @return Returns the length of the message (index of last char)
 ****************************************************************/
// std::string CircularBuffer::pop()
size_t CircularBuffer::pop(char *messageBuffer)
{
    // Lock mutex.
    std::unique_lock<std::mutex> lock(mutex_);

    // If head and tail points to the same, the buffer is empty.
    // Then put the thread to sleep, until signaled. (use while, for spurious wakeups)
    while (head_ == tail_)
    {
        condVar_.wait(lock);
    }

    // Ok, we're asked for a message, and there's something in the queue.
    // Find acutal length of message
    // size_t messageLength = strnlen(buffer_[tail_].data(), BUFFER_WIDTH);

    // std::string message(buffer_[tail_].data(), strnlen(buffer_[tail_].data(), BUFFER_WIDTH));

    //strncpy(message, buffer_[tail_].data(), BUFFER_WIDTH - 1);
    strcpy(messageBuffer, buffer_[tail_]);

    // Get message length
    size_t messageLength = messageLengths_[tail_];

    // Move tail to next index
    tail_ = next_index(tail_);

    msgCount--;

    lock.unlock();

    return messageLength;
}

size_t CircularBuffer::next_index(size_t index) const
{
    return (index + 1) % BUFFER_LENGTH;
}
