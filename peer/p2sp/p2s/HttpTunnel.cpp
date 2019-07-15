#include "HttpTunnel.h"

namespace peer
{
    void HttpTunnel::Start()
    {
        http_client_ = kit::HttpClient::Create(std::experimental::net::io_context(), url_, shared_from_this());
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

    void HttpTunnel::HandleConnect(const std::error_code & ec)
    {

    }

    void HttpTunnel::HandleRecvHttpResponse(std::shared_ptr<kit::HttpResponse> http_response)
    {
        switch (http_response->GetStatusCode())
        {
        case 200:
        case 206:
            break;
        case 404:
            break;
        default:
            break;
        }
    }

    void HttpTunnel::HandleRecvHttpData(const std::error_code & ec,
        std::shared_ptr<kit::Buffer> buffer)
    {

    }
}