#ifndef _KIT_TIMER_H_
#define _KIT_TIMER_H_

#include "experimental/__net_ts/steady_timer.hpp"
#include "experimental/__net_ts/io_context.hpp"

namespace kit
{
    class Timer;

    struct ITimerListener
    {
        virtual void OnTimerElapsed(std::shared_ptr<Timer> pointer, unsigned int times) = 0;
        virtual ~ITimerListener() {}
    };

    class Timer
        : public std::enable_shared_from_this<Timer>
    {
    public:
        Timer(std::experimental::net::io_context & io_context, unsigned int interval_in_milliseconds,
            std::shared_ptr<ITimerListener> handler)
            : interval_in_milliseconds_(interval_in_milliseconds)
            , handler_(handler)
            , timer_(io_context)
            , times_(0)
        { }

        void SetInterval(unsigned int interval_in_milliseconds);

        unsigned int GetInterval() const
        {
            return interval_in_milliseconds_;
        }

        void Start();
        void Stop();

    private:
        void DoOnceStart();
        void DoOnTimer();

        void OnTimer(const std::error_code & ec);

    protected:
        unsigned int interval_in_milliseconds_;
        std::shared_ptr<ITimerListener> handler_;
		std::experimental::net::steady_timer timer_;
        unsigned int times_;
    };
}

#endif

