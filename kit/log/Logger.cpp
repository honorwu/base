#include "Logger.h"
#include "LoggingModule.h"

namespace kit
{
    const char* LogLevelName[NUM_LOG_LEVELS] =
    {
        "TRACE",
        "INFO",
        "FAULT",
    };

    LoggerImpl::LoggerImpl(LogLevel level, const std::string & module, int line)
        : level_(level)
        , module_(module)
        , line_(line)
        , stream_()
    {
        stream_ << '[' << LogLevelName[level];
        stream_ << '|' << module_;
        stream_ << '|' << line_ << "] ";
    }

    Logger::Logger(const std::string & module, int line, LogLevel level)
        : logger_imple_(level, module, line)
    {
    }

    Logger::~Logger()
    {
        logger_imple_.stream_ << '\n';
        kit::LogggingModule::Instance().Append(logger_imple_.module_, (char*)stream().buffer()->Data(), stream().buffer()->Length());
    }
}