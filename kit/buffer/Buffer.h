#ifndef _KIT_BUFFER_H_
#define _KIT_BUFFER_H_

#include <string>
#include <memory>

namespace kit
{
    class Buffer
    {
    public:
        static std::shared_ptr<Buffer> Create(unsigned int size)
        {
            return std::shared_ptr<Buffer>(new Buffer(size));
        }
        
        ~Buffer();

        unsigned char * Data() const;
        unsigned int Length() const;
        unsigned int RemainSize() const;
        void SetLength(unsigned int length);

        void Append(const char * buffer, unsigned int buffer_length);
        void Consume(unsigned int bytes);
        void Clear();

    private:
        void Extend(unsigned int addition_bytes);

    private:
        Buffer(unsigned int size);
        Buffer(const Buffer &);
        unsigned int offset_;
        unsigned int length_;
        unsigned int remain_size_;
        unsigned char * buffer_;
    };
}
#endif