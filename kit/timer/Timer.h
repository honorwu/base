#ifndef _KIT_TIMER_H_
#define _KIT_TIMER_H_

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace kit
{
    class Timer;

    struct ITimerListener
    {
        virtual void OnTimerElapsed(boost::shared_ptr<Timer> pointer, unsigned int times) = 0;
        virtual ~ITimerListener() {}
    };

    class Timer
        : public boost::noncopyable
        , public boost::enable_shared_from_this<Timer>
    {
    public:
        Timer(boost::asio::io_service & io_service, unsigned int interval_in_milliseconds,
            boost::shared_ptr<ITimerListener> handler)
            : interval_in_milliseconds_(interval_in_milliseconds)
            , handler_(handler)
            , asio_timer_(io_service)
            , times_(0)
        { }

        void SetInterval(unsigned int interval_in_milliseconds);

        boost::uint32_t GetInterval() const
        {
            return interval_in_milliseconds_;
        }

        void Start();
        void Stop();

    private:
        void DoOnceStart();
        void DoOnTimer();

        void OnTimer(const boost::system::error_code & ec);

    protected:
        unsigned int interval_in_milliseconds_;
        boost::shared_ptr<ITimerListener> handler_;
        boost::asio::deadline_timer asio_timer_;
        unsigned int times_;
    };
}

#endif

