#include <array>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <string>

constexpr size_t BUFFER_LENGTH = 30; // The max number of elements in the buffer
constexpr size_t BUFFER_WIDTH = 256; // The max number of characters in each element

class CircularBuffer
{
public:
    //CircularBuffer();
    static CircularBuffer &getInstance();   // Return reference to this instance.

    void push(const char* message);
    std::string pop();

private:
    CircularBuffer();
    ~CircularBuffer();

    // Delete copy constructor & assignment operator, no copying a singleton!
    CircularBuffer(const CircularBuffer &) = delete;
    CircularBuffer &operator = (const CircularBuffer&) = delete;

    std::array<std::array<char, BUFFER_WIDTH>, BUFFER_LENGTH> buffer_;
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
