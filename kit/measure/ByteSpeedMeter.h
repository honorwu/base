#ifndef _KIT_BYTESPEEDMETER_H_
#define _KIT_BYTESPEEDMETER_H_

#include <string.h>
#include "kit/timer/TickCounter.h"

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

        void SubmitBytes(unsigned int bytes);

		unsigned int AverageByteSpeed() const;  // bytes per second

		unsigned int SecondByteSpeed();  // 1 second

		unsigned int CurrentByteSpeed();  // 5 seconds

		unsigned int RecentByteSpeed();  // 20 seconds

		unsigned int RecentMinuteByteSpeed();  // 1 minute

        inline unsigned int GetElapsedTimeInSeconds() const;

		unsigned int TotalBytes() const;

    private:

        static unsigned int GetPositionFromSeconds(unsigned int seconds);

        inline void CheckTickCount();

        inline void UpdateTickCount(unsigned int curr_sec);

        inline unsigned int CalcSpeedInDuration(unsigned int duration);

    private:

        static const unsigned int SECONDS_IN_SECOND = 1;

        static const unsigned int SECONDS_IN_RECENT = 5;

        static const unsigned int SECONDS_IN_RECENT_20SEC = 20;

        static const unsigned int SECONDS_IN_MINUTE = 60;

        static const unsigned int HISTORY_INTERVAL_IN_SEC = SECONDS_IN_MINUTE;

    private:
        kit::TickCounter tick_count_;

		unsigned int total_bytes_;

		unsigned int history_bytes_[HISTORY_INTERVAL_IN_SEC];

		unsigned int current_sec_;

        bool is_running_;
    };

    inline void ByteSpeedMeter::SubmitBytes(unsigned int bytes)
    {
        CheckTickCount();
        total_bytes_ += bytes;
        history_bytes_[GetPositionFromSeconds(current_sec_)] += bytes;
    }

    inline void ByteSpeedMeter::CheckTickCount()
    {
		unsigned int curr_sec = tick_count_.Elapsed() / 1000;
        if (curr_sec == current_sec_)
            return;
        UpdateTickCount(curr_sec);
    }

    inline void ByteSpeedMeter::UpdateTickCount(unsigned int curr_sec)
    {
        if (curr_sec - current_sec_ >= HISTORY_INTERVAL_IN_SEC)
        {
            memset(history_bytes_, 0, sizeof(history_bytes_));
        }
        else
        {
            for (unsigned int i = curr_sec; i > current_sec_; i--)
                history_bytes_[GetPositionFromSeconds(i)] = 0;
        }

        current_sec_ = curr_sec;
    }

    inline unsigned int ByteSpeedMeter::CalcSpeedInDuration(unsigned int duration)
    {
        CheckTickCount();

		unsigned __int64 bytes_in_recent = 0;
		unsigned int last_sec = current_sec_ - 1;
        for (unsigned int i = last_sec; i > last_sec - duration; i--)
            bytes_in_recent += history_bytes_[GetPositionFromSeconds(i)];

		unsigned int elapsed_time = GetElapsedTimeInSeconds();
        if (elapsed_time > duration)
            return static_cast<unsigned int>(bytes_in_recent / duration);
        else
            return static_cast<unsigned int>(bytes_in_recent / elapsed_time);
    }

    inline unsigned int ByteSpeedMeter::GetElapsedTimeInSeconds() const
    {
		unsigned int s = tick_count_.Elapsed() / 1000;
        return s <= 0 ? 1 : s;
    }
}

#endif