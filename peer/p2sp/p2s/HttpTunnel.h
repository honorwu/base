#ifndef _PEER_HTTP_TUNNEL_H_
#define _PEER_HTTP_TUNNEL_H_

#include "kit/network/http/HttpClient.h"

namespace peer
{
    class HttpTunnel
        : public kit::IHttpClientListener
    {
    public:
        void Start();
        void Stop();

        void Resume();
        void Pause();
    private:
        boost::shared_ptr<kit::HttpClient> http_client_;
    };
}

#endif