#ifndef _PEER_PROXY_MODULE_H_
#define _PEER_PROXY_MODULE_H_

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <kit/network/tcp/TcpAcceptor.h>

namespace peer
{
    class ProxyModule
        : public boost::enable_shared_from_this<ProxyModule>
        , public kit::ITcpAcceptorListener
    {
    public:
        static boost::shared_ptr<ProxyModule> Instance()
        {
            if (!instance_)
            {
                instance_.reset(new ProxyModule());
            }
            return instance_;
        }
        void Start(boost::uint16_t port);
        void Stop();
        virtual void HandleAccept(const boost::system::error_code& ec,
            boost::asio::ip::tcp::socket * socket);
    private:
        ProxyModule();
        void Accept();

        static boost::shared_ptr<ProxyModule> instance_;
        boost::shared_ptr<kit::TcpAcceptor> tcp_acceptor_;
        boost::asio::io_service & io_service_;
    };
}

#endif
