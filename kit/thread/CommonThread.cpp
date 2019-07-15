#include "CommonThread.h"

namespace kit
{
    CommonThread::CommonThread ()
        : work_guard_(NULL)
        , io_context_(NULL)
        , thread_(NULL)
    {
    }

    void CommonThread::Start()
    {
        if (NULL == work_guard_)
        {
            io_context_ = new std::experimental::net::io_context();
			work_guard_ = &std::experimental::net::make_work_guard(*io_context_);
            thread_ = new std::thread(std::bind(&CommonThread::Run, this));
        }
    }

    void CommonThread::Stop()
    {
        if (NULL != work_guard_)
        {
            delete work_guard_;
			work_guard_ = NULL;
            io_context_->stop();
            thread_->join();
            delete thread_;
            thread_ = NULL;
            delete io_context_;
			io_context_ = NULL;
        }
    }

    void CommonThread::Run()
    {
		io_context_->run();
    }

    void CommonThread::Post(std::function<void()> handler)
    {
        if (work_guard_ != NULL && io_context_ != NULL)
        {
			std::experimental::net::post(io_context_->get_executor(), handler);
        }
    }
}