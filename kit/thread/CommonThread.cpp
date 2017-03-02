#include "CommonThread.h"
#include <boost/bind.hpp>

namespace kit
{
    CommonThread::CommonThread ()
        : work_(NULL)
        , io_service_(NULL)
        , thread_(NULL)
    {
    }

    void CommonThread::Start()
    {
        if (NULL == work_)
        {
            io_service_ = new boost::asio::io_service();
            work_ = new boost::asio::io_service::work(* io_service_);
            thread_ = new boost::thread(boost::bind(&CommonThread::Run, this));
        }
    }

    void CommonThread::Stop()
    {
        if (NULL != work_)
        {
            delete work_;
            work_ = NULL;
            io_service_->stop();
            thread_->join();
            delete thread_;
            thread_ = NULL;
            delete io_service_;
            io_service_ = NULL;
        }
    }

    void CommonThread::Run()
    {
        io_service_->run();
    }

    void CommonThread::Post(boost::function<void()> handler)
    {
        if (work_ != NULL && io_service_ != NULL)
        {
            io_service_->post(handler);
        }
    }
}