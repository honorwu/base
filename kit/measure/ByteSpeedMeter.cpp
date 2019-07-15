#include "ByteSpeedMeter.h"

namespace kit
{
    ByteSpeedMeter::ByteSpeedMeter()
        : is_running_(false)
    {
        Clear();
    }

    void ByteSpeedMeter::Clear()
    {
        total_bytes_ = 0;
        current_sec_ = 0;
        memset(history_bytes_, 0, sizeof(history_bytes_));
    }

    void ByteSpeedMeter::Start()
    {
        if (is_running_ == true)
            return;

        Clear();
        CheckTickCount();
        is_running_ = true;
    }

    void ByteSpeedMeter::Stop()
    {
        if (is_running_ == false)
            return;

        Clear();

        is_running_ = false;
    }

    bool ByteSpeedMeter::IsRunning() const
    {
        return is_running_;
    }

    unsigned int ByteSpeedMeter::TotalBytes() const
    {
        return total_bytes_;
    }

    unsigned int ByteSpeedMeter::GetPositionFromSeconds(unsigned int seconds)
    {
        return seconds % HISTORY_INTERVAL_IN_SEC;
    }

	unsigned int ByteSpeedMeter::AverageByteSpeed() const
    {
        if (is_running_ == false)
            return 0;

        return (unsigned __int64)total_bytes_ / GetElapsedTimeInSeconds();
    }

	unsigned int ByteSpeedMeter::SecondByteSpeed()  // 2 second
    {
        return CalcSpeedInDuration(SECONDS_IN_SECOND);
    }

	unsigned int ByteSpeedMeter::CurrentByteSpeed()  // 5 seconds
    {
        return CalcSpeedInDuration(SECONDS_IN_RECENT);
    }

	unsigned int ByteSpeedMeter::RecentByteSpeed()  // 20 seconds
    {
        return CalcSpeedInDuration(SECONDS_IN_RECENT_20SEC);
    }

	unsigned int ByteSpeedMeter::RecentMinuteByteSpeed()  // 1 minute
    {
        return CalcSpeedInDuration(HISTORY_INTERVAL_IN_SEC);
    }
}