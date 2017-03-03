#ifndef _KIT_LOGGER_H_
#define _KIT_LOGGER_H_

#include "LogStream.h"

namespace kit
{
    enum LogLevel
    {
        TRACE,
        INFO,
        FAULT,
        NUM_LOG_LEVELS,
    };

    class LoggerImpl
    {
    public:
        LoggerImpl(LogLevel level, const std::string & module, int line);

        LogLevel level_;
        std::string module_;
        int line_;

        LogStream stream_;
    };

    class Logger
    {
    public:
        Logger(const std::string & module, int line, LogLevel level);

        ~Logger();

        LogStream& stream()
        {
            return logger_imple_.stream_;
        }
    private:
        LoggerImpl logger_imple_;
    };
}

#endif