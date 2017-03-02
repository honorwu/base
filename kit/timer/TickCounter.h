#ifndef _KIT_TICK_COUNTER_H_
#define _KIT_TICK_COUNTER_H_

#include <boost/noncopyable.hpp>
#include <time.h>

namespace kit
{
    class TickCounter
        : public boost::noncopyable
    {
    public:
        TickCounter()
        {
            start_time_ = time(NULL);
        }

        void Restart()
        {
            start_time_ = time(NULL);
        }
        
        unsigned long Elapsed() const
        {
            return (unsigned long)(time(NULL) - start_time_);
        }

    private:
        time_t start_time_;
    };
}

#endif