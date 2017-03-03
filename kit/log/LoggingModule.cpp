#include "LoggingModule.h"
#include <time.h>
#include <fstream>

namespace kit
{
    boost::shared_ptr<LogggingModule> LogggingModule::instance_;

    const unsigned int LargeBufferSize = 4096;

    LogggingModule::LogggingModule()
        : written_bytes_(0)
        , fp_(NULL)
        , logger_level_(TRACE)
        , roll_size_(DefaultLogSizeInByte)
    {
        log_buffer_ = kit::Buffer::Create(LargeBufferSize);
    }

    void LogggingModule::Start(const std::string & file_path)
    {
        file_path_ = file_path;

        fp_ = fopen(file_path_.c_str(), "rb+");

        if (!fp_)
        {
            fp_ = fopen(file_path_.c_str(), "wb");
            if (fp_)
            {
                fclose(fp_);
            }
            fp_ = NULL;
            fp_ = fopen(file_path_.c_str(), "rb+");
        }

        if (fp_)
        {
            fread(&written_bytes_, sizeof(boost::uint32_t), 1, fp_);
            if (written_bytes_ < LogHeaderLength)
            {
                written_bytes_ = LogHeaderLength;
            }

            fseek(fp_, 0, SEEK_END);
            boost::uint32_t filesize = ftell(fp_);

            if (written_bytes_ > filesize)
            {
                written_bytes_ = LogHeaderLength;
            }

            fseek(fp_, written_bytes_, SEEK_SET);
        }

        thread_.Start();

        timer_ = boost::shared_ptr<kit::Timer>(new kit::Timer(thread_.get_io_service(), 1000, shared_from_this()));
        timer_->Start();

        UpdateCurrentTimestamp();
    }

    void LogggingModule::Stop()
    {
        timer_->Stop();
        timer_.reset();
        thread_.Stop();
      
        if (fp_)
        {
            fclose(fp_);
            fp_ = NULL;
        }

        instance_.reset();
    }

    void LogggingModule::Append(const std::string & module, const char* logline, int length)
    {
        if (!fp_)
        {
            return;
        }

        std::string log(logline, length);
        thread_.Post(boost::bind(&LogggingModule::AppendInThread, shared_from_this(), module, log));
    }

    void LogggingModule::SetLogLevel(LogLevel level)
    {
        logger_level_ = level;
    }

    void LogggingModule::SetRollSize(size_t size)
    {
        roll_size_ = size;
    }

    void LogggingModule::SetLogModule(const std::string & modules)
    {
        logger_module_.clear();
        Tokenizer(modules, logger_module_);
    }

    void LogggingModule::AppendInThread(std::string & module, const std::string & log)
    {
        std::transform(module.begin(), module.end(), module.begin(), tolower);
        
        if (module.length() > 0 &&
            logger_module_.find(module) == logger_module_.end() &&
            logger_module_.find("*") == logger_module_.end())
        {
            return;
        }

        if (strcmp(current_timestamp_buffer, last_timestamp_buffer) != 0)
        {
            strcpy(last_timestamp_buffer, current_timestamp_buffer);
            const std::string& timestamp_str(last_timestamp_buffer);
            std::string time_module = "";
            AppendInThread(time_module , timestamp_str);
        }

        if (log_buffer_->RemainSize() > log.length())
        {
            log_buffer_->Append(log.c_str(), log.length());
        }
        else
        {
            Write();
            AppendInThread(module, log);
        }
    }

    void LogggingModule::OnTimerElapsed(boost::shared_ptr<Timer> pointer, unsigned int times)
    {
        if (!fp_)
        {
            return;
        }

        UpdateCurrentTimestamp();

        Write();
    }

    void LogggingModule::Write()
    {
        ParseConfigIfExist();

        if (log_buffer_->Length() == 0)
        {
            return;
        }

        if (written_bytes_ + log_buffer_->Length() > roll_size_ - LogHeaderLength)
        {
            boost::int32_t write_size = roll_size_  - LogHeaderLength - written_bytes_;
            if (write_size > 0)
            {
                fwrite(log_buffer_->Data(), write_size, 1, fp_);
                log_buffer_->Consume(write_size);
            }
            
            fseek(fp_, LogHeaderLength, SEEK_SET);
            written_bytes_ = LogHeaderLength;
        }

        if (fwrite(log_buffer_->Data(), log_buffer_->Length(), 1, fp_))
        {
            written_bytes_ += log_buffer_->Length();
            fseek(fp_, 0, SEEK_SET);
            char head_buffer[6] = "0000\n";
            memcpy(head_buffer, &written_bytes_, sizeof(boost::uint32_t));
            
            fwrite(&head_buffer, LogHeaderLength, 1, fp_);
            
            fseek(fp_, written_bytes_, SEEK_SET);
        }

        log_buffer_->Clear();

        fflush(fp_);
    }

    void LogggingModule::UpdateCurrentTimestamp()
    {
        time_t timestamp = time(0);
        strftime(current_timestamp_buffer, sizeof(current_timestamp_buffer), "%Y-%m-%d %X\n", localtime(&timestamp));
    }

    /*
    ParseConfigIfExist可能会存在一些多线程访问的风险，不过通常使用是没有问题的
    所以这里没有把SetRollSize,SetLogLevel以及ClearModule&EnableLogModule这4个函数post到主线程访问
    config文件格式如下:

    level=INFO
    size=204800
    module=httpclient,httpserver

    */
    bool LogggingModule::ParseConfigIfExist()
    {
        std::string config_path = file_path_ + ".config";

        FILE * fp = fopen(config_path.c_str(), "rb+");

        if (fp == NULL)
        {
            return false;
        }

        fclose(fp);
        fp = NULL;

        std::ifstream in(config_path.c_str());
        if (!in.is_open())
        {
            return false;
        }

        std::string line;
        while (getline(in, line))
        {
            size_t pos = line.find("=");
            if (pos == std::string::npos)
            {
                continue;
            }

            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);

            if (key == "level")
            {
                if (value == "TRACE")
                {
                    logger_level_ = TRACE;
                }
                else if (value == "INFO")
                {
                    logger_level_ = INFO;
                }
                else if (value == "FAULT")
                {
                    logger_level_ = FAULT;
                }
            }
            else if (key == "size")
            {
                roll_size_ = atoi(value.c_str());
            }
            else if (key == "module")
            {
                logger_module_.clear();
                Tokenizer(value, logger_module_);
            }
        }
        in.close();

        std::string apply_config_path = config_path + "[applied]";
        rename(config_path.c_str(), apply_config_path.c_str());

        return true;
    }

    void LogggingModule::Tokenizer(const std::string & str, std::set<std::string> & s)
    {
        unsigned int begin_pos = 0;
        unsigned int end_pos = 0;
        for (end_pos = 0; end_pos < str.length(); end_pos++)
        {
            if (str[end_pos] == ',')
            {
                std::string word = str.substr(begin_pos, end_pos - begin_pos);
                std::transform(word.begin(), word.end(), word.begin(), tolower);
                s.insert(word);
                begin_pos = end_pos + 1;
            }
        }

        std::string word = str.substr(begin_pos, end_pos - begin_pos);
        std::transform(word.begin(), word.end(), word.begin(), tolower);
        s.insert(word);
    }
}