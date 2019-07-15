#include "Timer.h"
#include <functional>

namespace kit
{
    void Timer::SetInterval(unsigned int interval_in_milliseconds)
    {
        interval_in_milliseconds_ = interval_in_milliseconds;
    }

    void Timer::Start()
    {
        DoOnceStart();
    }

    void Timer::Stop()
    {
        if (!handler_)
        {
            return;
        }

        timer_.cancel();
        handler_.reset();
    }

    void Timer::DoOnceStart()
    {
        timer_.expires_after(std::chrono::duration<int, std::milli>(interval_in_milliseconds_));
        timer_.async_wait(
                std::bind(&Timer::OnTimer, shared_from_this(), std::placeholders::_1));
    }

    void Timer::OnTimer(const std::error_code & ec)
    {    
        if (ec)
        {
            return;
        }

        DoOnTimer();
    }

    void Timer::DoOnTimer()
    {
        times_++;
        assert(handler_);
        
        handler_->OnTimerElapsed(shared_from_this(), times_);
        DoOnceStart();
    }
}