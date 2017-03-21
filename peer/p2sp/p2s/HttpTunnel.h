#ifndef _PEER_HTTP_TUNNEL_H_
#define _PEER_HTTP_TUNNEL_H_

#include "kit/network/http/HttpClient.h"
#include <boost/enable_shared_from_this.hpp>

namespace peer
{
    class HttpTunnel
        : public kit::IHttpClientListener
        , public boost::enable_shared_from_this<HttpTunnel>
    {
    public:
        void Start();
        void Stop();

        void Resume();
        void Pause();

        virtual void HandleConnect(const boost::system::error_code & ec);
        virtual void HandleRecvHttpResponse(boost::shared_ptr<kit::HttpResponse> http_response);
        virtual void HandleRecvHttpData(const boost::system::error_code & ec,
            boost::shared_ptr<kit::Buffer> buffer);
    private:
        boost::shared_ptr<kit::HttpClient> http_client_;
        std::string url_;
    };
}

#endif