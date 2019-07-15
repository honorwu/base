#ifndef _KIT_LOGSTREAM_H_
#define _KIT_LOGSTREAM_H_

#include <string>
#include "kit/buffer/Buffer.h"

namespace kit
{
    const unsigned int SmallBufferSize = 1024;

    class LogStream
    {
    public:
        LogStream()
        {
            buffer_ = kit::Buffer::Create(SmallBufferSize);
        }

        LogStream & operator << (bool v);
        LogStream & operator << (short);
        LogStream & operator << (unsigned short);
        LogStream & operator << (int);
        LogStream & operator << (unsigned int);
        LogStream & operator << (long);
        LogStream & operator << (unsigned long);
        LogStream & operator << (long long);
        LogStream & operator << (unsigned long long);
        LogStream & operator << (const void*);
        LogStream & operator << (float);
        LogStream & operator << (double);
        LogStream & operator << (char);
        LogStream & operator << (const char*);
        LogStream & operator << (const std::string &);

        void append(const char* data, int len)
        {
            buffer_->Append(data, len);
        }

        std::shared_ptr<kit::Buffer> buffer()
        {
            return buffer_;
        }

    private:
        template<typename T>
        void formatInteger(T);

        std::shared_ptr<kit::Buffer> buffer_;

        static const int MaxNumericSize = 16;
    };
}

#endif