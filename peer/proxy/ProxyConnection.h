#ifndef _PEER_PROXY_CONNECTION_H_
#define _PEER_PROXY_CONNECTION_H_

#include <kit/network/http/HttpServer.h>

namespace peer
{
    class Downloader;

    class ProxyConnection
        : public std::enable_shared_from_this<ProxyConnection>
        , public kit::IHttpServerListener
    {
    public:
        ProxyConnection(std::experimental::net::ip::tcp::socket * socket)
            : socket_(socket)
        {

        }

        void Start();
        void Stop();

        void Recv();
        virtual void HandleRecv(std::shared_ptr<kit::HttpServer> http_server, const std::string & request);
        void Send(std::shared_ptr<kit::Buffer> buffer);

    private:
		std::experimental::net::ip::tcp::socket * socket_;
        std::shared_ptr<kit::HttpServer> http_server_;
        std::shared_ptr<Downloader> downloader_;
    };
}

#endif