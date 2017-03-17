#ifndef _PEER_PROXY_CONNECTION_H_
#define _PEER_PROXY_CONNECTION_H_

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <kit/network/http/HttpServer.h>

namespace peer
{
    class ProxyConnection
        : public boost::enable_shared_from_this<ProxyConnection>
        , public kit::IHttpServerListener
    {
    public:
        ProxyConnection(boost::asio::ip::tcp::socket * socket)
            : socket_(socket)
        {

        }

        void Start();
        void Stop();

        void Recv();
        virtual void HandleRecv(boost::shared_ptr<kit::HttpServer> http_server, const std::string & request);

    private:
        boost::asio::ip::tcp::socket * socket_;
        boost::shared_ptr<kit::HttpServer> http_server_;
    };
}

#endif