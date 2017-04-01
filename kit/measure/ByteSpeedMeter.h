#ifndef _KIT_BYTESPEEDMETER_H_
#define _KIT_BYTESPEEDMETER_H_

#include <boost/cstdint.hpp>
#include <boost/timer.hpp>
#include <string.h>

namespace kit
{
    class ByteSpeedMeter
    {
    public:
        ByteSpeedMeter();

        void Start();

        void Stop();

        bool IsRunning() const;

        void Clear();

    public:

        void SubmitBytes(boost::uint32_t bytes);

        boost::uint32_t AverageByteSpeed() const;  // bytes per second

        boost::uint32_t SecondByteSpeed();  // 1 second

        boost::uint32_t CurrentByteSpeed();  // 5 seconds

        boost::uint32_t RecentByteSpeed();  // 20 seconds

        boost::uint32_t RecentMinuteByteSpeed();  // 1 minute

        inline boost::uint32_t GetElapsedTimeInSeconds() const;

        boost::uint32_t TotalBytes() const;

    private:

        static boost::uint32_t GetPositionFromSeconds(boost::uint32_t seconds);

        inline void CheckTickCount();

        inline void UpdateTickCount(boost::uint32_t curr_sec);

        inline boost::uint32_t CalcSpeedInDuration(boost::uint32_t duration);

    private:

        static const boost::uint32_t SECONDS_IN_SECOND = 1;

        static const boost::uint32_t SECONDS_IN_RECENT = 5;

        static const boost::uint32_t SECONDS_IN_RECENT_20SEC = 20;

        static const boost::uint32_t SECONDS_IN_MINUTE = 60;

        static const boost::uint32_t HISTORY_INTERVAL_IN_SEC = SECONDS_IN_MINUTE;

    private:
        boost::timer tick_count_;

        boost::uint32_t total_bytes_;

        boost::uint32_t history_bytes_[HISTORY_INTERVAL_IN_SEC];

        boost::uint32_t current_sec_;

        bool is_running_;
    };

    inline void ByteSpeedMeter::SubmitBytes(boost::uint32_t bytes)
    {
        CheckTickCount();
        total_bytes_ += bytes;
        history_bytes_[GetPositionFromSeconds(current_sec_)] += bytes;
    }

    inline void ByteSpeedMeter::CheckTickCount()
    {
        boost::uint32_t curr_sec = tick_count_.elapsed() / 1000;
        if (curr_sec == current_sec_)
            return;
        UpdateTickCount(curr_sec);
    }

    inline void ByteSpeedMeter::UpdateTickCount(boost::uint32_t curr_sec)
    {
        if (curr_sec - current_sec_ >= HISTORY_INTERVAL_IN_SEC)
        {
            memset(history_bytes_, 0, sizeof(history_bytes_));
        }
        else
        {
            for (boost::uint32_t i = curr_sec; i > current_sec_; i--)
                history_bytes_[GetPositionFromSeconds(i)] = 0;
        }

        current_sec_ = curr_sec;
    }

    inline boost::uint32_t ByteSpeedMeter::CalcSpeedInDuration(boost::uint32_t duration)
    {
        CheckTickCount();

        boost::int64_t bytes_in_recent = 0;
        boost::uint32_t last_sec = current_sec_ - 1;
        for (boost::uint32_t i = last_sec; i > last_sec - duration; i--)
            bytes_in_recent += history_bytes_[GetPositionFromSeconds(i)];

        boost::uint32_t elapsed_time = GetElapsedTimeInSeconds();
        if (elapsed_time > duration)
            return static_cast<boost::uint32_t>(bytes_in_recent / duration);
        else
            return static_cast<boost::uint32_t>(bytes_in_recent / elapsed_time);
    }

    inline boost::uint32_t ByteSpeedMeter::GetElapsedTimeInSeconds() const
    {
        boost::uint32_t s = tick_count_.elapsed() / 1000;
        return s <= 0 ? 1 : s;
    }
}

#endif