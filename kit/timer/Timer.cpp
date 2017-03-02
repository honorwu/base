#include "Timer.h"

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

        boost::system::error_code ec;
        asio_timer_.cancel(ec);

        handler_.reset();
    }

    void Timer::DoOnceStart()
    {
        asio_timer_.expires_from_now(boost::posix_time::milliseconds(interval_in_milliseconds_));
        asio_timer_.async_wait(
                boost::bind(&Timer::OnTimer, shared_from_this(), boost::asio::placeholders::error));
    }

    void Timer::OnTimer(const boost::system::error_code & ec)
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