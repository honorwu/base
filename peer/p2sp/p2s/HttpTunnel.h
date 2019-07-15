#ifndef _PEER_HTTP_TUNNEL_H_
#define _PEER_HTTP_TUNNEL_H_

#include "kit/network/http/HttpClient.h"

namespace peer
{
    class HttpTunnel
        : public kit::IHttpClientListener
        , public std::enable_shared_from_this<HttpTunnel>
    {
    public:
        HttpTunnel(std::string & url)
            : url_(url)
        {

        }

        void Start();
        void Stop();

        void Resume();
        void Pause();

        virtual void HandleConnect(const std::error_code & ec);
        virtual void HandleRecvHttpResponse(std::shared_ptr<kit::HttpResponse> http_response);
        virtual void HandleRecvHttpData(const std::error_code & ec,
            std::shared_ptr<kit::Buffer> buffer);
    private:
        std::shared_ptr<kit::HttpClient> http_client_;
        std::string url_;
    };
}

#endif