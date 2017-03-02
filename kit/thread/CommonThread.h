#ifndef _KIT_COMMON_THREAD_H_
#define _KIT_COMMON_THREAD_H_

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/function.hpp>

namespace kit
{
    class CommonThread 
        : public boost::noncopyable
    {
    public:
        CommonThread();
        void Start();
        void Stop();

        boost::asio::io_service & get_io_service()
        {
            return * io_service_;
        }

        void Post(boost::function<void()> handler);

    private:
        void Run();

    private:
        boost::asio::io_service * io_service_;
        boost::asio::io_service::work * work_;
        boost::thread * thread_;
    };
}
#endif