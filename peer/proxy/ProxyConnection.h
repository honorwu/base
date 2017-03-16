#ifndef _PEER_PROXY_CONNECTION_H_
#define _PEER_PROXY_CONNECTION_H_

#include <boost/asio.hpp>

namespace peer
{
    class ProxyConnection
    {
    public:
        ProxyConnection(boost::asio::ip::tcp::socket * socket)
            : socket_(socket)
        {

        }

        void Start();
        void Stop();

        void Recv();
    private:
        boost::asio::ip::tcp::socket * socket_;
    };
}

#endif