#include "Buffer.h"
#include <string.h>
#include <assert.h> 

namespace kit
{
    Buffer::Buffer(unsigned int size)
        : length_(0)
        , offset_(0)
        , remain_size_(size == 0 ? 1 : size)
    {
        assert(size > 0);
        buffer_ = new unsigned char[remain_size_];
    }

    Buffer::~Buffer()
    {
        delete[] buffer_;
    }

    unsigned char * Buffer::Data() const
    {
        return buffer_ + offset_;
    }

    unsigned int Buffer::Length() const
    {
        return length_;
    }

    unsigned int Buffer::RemainSize() const
    {
        return remain_size_;
    }

    void Buffer::SetLength(unsigned int length)
    {
        assert(offset_ + length_ + remain_size_ >= length);
        remain_size_ = offset_ + length_ + remain_size_ - length;
        length_ = length;
    }

    void Buffer::Extend(unsigned int addition_bytes)
    {
        unsigned int capacity = offset_ + length_ + remain_size_;

        unsigned char * temp_buffer = new unsigned char[capacity + addition_bytes];
        memcpy(temp_buffer, Data(), length_);
        delete[] buffer_;
        buffer_ = temp_buffer;

        remain_size_ = capacity + addition_bytes - length_;
        offset_ = 0;
    }

    void Buffer::Append(const char * buffer, unsigned int buffer_length)
    {
        while (buffer_length > remain_size_)
        {
            Extend(buffer_length - remain_size_);
        }

        memcpy(buffer_ + offset_ + length_, buffer, buffer_length);
        length_ += buffer_length;
        remain_size_ -= buffer_length;
    }


    void Buffer::Clear()
    {
        remain_size_ = remain_size_ + offset_ + length_;
        length_ = 0;
        offset_ = 0;
    }

    void Buffer::Consume(unsigned int bytes)
    {
        assert(length_ >= bytes);
        offset_ += bytes;
        length_ -= bytes;
    }
}