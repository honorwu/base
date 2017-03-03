#include "LogStream.h"
#include <stdio.h>
#include <stdint.h>
#include <algorithm>
#include <string.h>

namespace kit
{
    const char digits[] = "9876543210123456789";
    const char* zero = digits + 9;

    const char digitsHex[] = "0123456789ABCDEF";

    // Efficient Integer to String Conversions, by Matthew Wilson.
    template<typename T>
    size_t convert(char buf[], T value)
    {
        T i = value;
        char* p = buf;

        do
        {
            int lsd = static_cast<int>(i % 10);
            i /= 10;
            *p++ = zero[lsd];
        } while (i != 0);

        if (value < 0)
        {
            *p++ = '-';
        }
        *p = '\0';
        std::reverse(buf, p);

        return p - buf;
    }

    size_t convertHex(char buf[], uintptr_t value)
    {
        uintptr_t i = value;
        char* p = buf;

        do
        {
            int lsd = i % 16;
            i /= 16;
            *p++ = digitsHex[lsd];
        } while (i != 0);

        *p = '\0';
        std::reverse(buf, p);

        return p - buf;
    }

    template<typename T>
    void LogStream::formatInteger(T v)
    {
        if (buffer_->RemainSize() >= MaxNumericSize)
        {
            size_t len = convert((char*)buffer_->Data() + buffer_->Length(), v);
            buffer_->SetLength(buffer_->Length() + len);
        }
    }

    LogStream & LogStream::operator << (bool v)
    {
        buffer_->Append(v ? "1" : "0", 1);
        return *this;
    }

    LogStream & LogStream::operator << (short v)
    {
        *this << static_cast<int>(v);
        return *this;
    }

    LogStream & LogStream::operator << (unsigned short v)
    {
        *this << static_cast<unsigned int>(v);
        return *this;
    }

    LogStream & LogStream::operator << (int v)
    {
        formatInteger(v);
        return *this;
    }

    LogStream & LogStream::operator << (unsigned int v)
    {
        formatInteger(v);
        return *this;
    }

    LogStream & LogStream::operator << (long v)
    {
        formatInteger(v);
        return *this;
    }

    LogStream & LogStream::operator << (unsigned long v)
    {
        formatInteger(v);
        return *this;
    }

    LogStream & LogStream::operator << (long long v)
    {
        formatInteger(v);
        return *this;
    }

    LogStream & LogStream::operator << (unsigned long long v)
    {
        formatInteger(v);
        return *this;
    }

    LogStream & LogStream::operator << (const void* p)
    {
        uintptr_t v = reinterpret_cast<uintptr_t>(p);
        if (buffer_->RemainSize() >= MaxNumericSize)
        {
            char* buf = (char*)buffer_->Data() + buffer_->Length();
            buf[0] = '0';
            buf[1] = 'x';
            size_t len = convertHex(buf + 2, v);
            buffer_->SetLength(buffer_->Length() + len + 2);
        }
        return *this;
    }

    LogStream & LogStream::operator << (float v)
    {
        *this << static_cast<double>(v);
        return *this;
    }

    LogStream & LogStream::operator << (double v)
    {
        if (buffer_->RemainSize() >= MaxNumericSize)
        {
            int len = sprintf((char*)buffer_->Data() + buffer_->Length(), "%.12g", v);
            buffer_->SetLength(len);
        }

        return *this;
    }

    LogStream & LogStream::operator << (char v)
    {
        buffer_->Append(&v, 1);
        return *this;
    }

    LogStream & LogStream::operator << (const char* v)
    {
        buffer_->Append(v, strlen(v));
        return *this;
    }

    LogStream & LogStream::operator << (const std::string & v)
    {
        buffer_->Append(v.c_str(), v.size());
        return *this;
    }
}
