#include "HttpTunnel.h"

namespace peer
{
    void HttpTunnel::Start()
    {
        http_client_.reset(new kit::HttpClient());
    }

    void HttpTunnel::Stop()
    {
        http_client_->Close();
        http_client_.reset();
    }

    void HttpTunnel::Resume()
    {

    }

    void HttpTunnel::Pause()
    {

    }
}