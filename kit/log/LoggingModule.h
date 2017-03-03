#ifndef _KIT_LOGGING_MODULE_H_
#define _KIT_LOGGING_MODULE_H_

#include <string>
#include <set>
#include "kit/thread/CommonThread.h"
#include "kit/timer/Timer.h"
#include "kit/buffer/Buffer.h"
#include "LogStream.h"
#include "Logger.h"

#include <boost/shared_ptr.hpp>

#define LOG_DEBUG(module, message) \
    if (kit::LogggingModule::Instance().GetLogLevel() <= kit::TRACE) \
        kit::Logger(module, __LINE__, kit::TRACE).stream() << message
#define LOG_INFO(module, message) \
    if (kit::LogggingModule::Instance().GetLogLevel() <= kit::INFO) \
        kit::Logger(module, __LINE__, kit::INFO).stream() << message
#define LOG_FAULT(module, message) \
    if (kit::LogggingModule::Instance().GetLogLevel() <= kit::FAULT) \
        kit::Logger(module, __LINE__, kit::FAULT).stream() << message

namespace kit
{
    class LogggingModule
        : public boost::enable_shared_from_this<LogggingModule>
        , public kit::ITimerListener
    {
    public:
        const static unsigned int LogHeaderLength = 5;
        const static unsigned int WriteLogIntervalInSecond = 5;
        const static unsigned int DefaultLogSizeInByte = 50000 * 1024;

        static LogggingModule & Instance()
        {
            if (!instance_)
            {
                instance_.reset(new LogggingModule());
            }

            return *instance_;
        }

        void Start(const std::string & filename);

        void Stop();

        void SetLogLevel(LogLevel level);

        inline LogLevel GetLogLevel()
        {
            return logger_level_;
        }

        void SetRollSize(size_t size);

        void SetLogModule(const std::string & modules);

        void Append(const std::string & module, const char* logline, int length);

        bool ParseConfigIfExist();

        virtual void OnTimerElapsed(boost::shared_ptr<Timer> pointer, unsigned int times);

    private:
        void UpdateCurrentTimestamp();

        void AppendInThread(std::string & module, const std::string & log);
        
        void Write();

        void Tokenizer(const std::string & str, std::set<std::string> & s);

        LogggingModule();
        std::string file_path_;
        size_t roll_size_;

        static boost::shared_ptr<LogggingModule> instance_;

        kit::CommonThread thread_;

        boost::shared_ptr<kit::Timer> timer_;

        boost::shared_ptr<kit::Buffer> log_buffer_;

        size_t written_bytes_;

        FILE * fp_;

        LogLevel logger_level_;

        std::set<std::string> logger_module_;

        char current_timestamp_buffer[32];
        char last_timestamp_buffer[32];
    };
}

#endif