#ifndef _KIT_COMMON_THREAD_H_
#define _KIT_COMMON_THREAD_H_

#include "experimental/net"
#include "experimental/executor"

namespace kit
{
    class CommonThread 
    {
    public:
        CommonThread();
        void Start();
        void Stop();

        std::experimental::net::io_context & get_io_context()
        {
            return * io_context_;
        }

        void Post(std::function<void()> handler);

    private:
        void Run();

    private:
		std::experimental::net::io_context * io_context_;
		std::experimental::net::executor_work_guard<std::experimental::net::io_context::executor_type > * work_guard_;
        std::thread * thread_;
    };
}
#endif