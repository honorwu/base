#ifndef _PEER_PROXY_MODULE_H_
#define _PEER_PROXY_MODULE_H_

#include <kit/network/tcp/TcpAcceptor.h>

namespace peer
{
    class ProxyModule
        : public std::enable_shared_from_this<ProxyModule>
        , public kit::ITcpAcceptorListener
    {
    public:
        static std::shared_ptr<ProxyModule> Instance()
        {
            return instance_;
        }
        void Start(unsigned short port);
        void Stop();
        virtual void HandleAccept(const std::error_code& ec,
			std::experimental::net::ip::tcp::socket * socket);
    private:
		ProxyModule()
			: io_context_(std::experimental::net::io_context())
		{
		}
        void Accept();

        static std::shared_ptr<ProxyModule> instance_;
        std::shared_ptr<kit::TcpAcceptor> tcp_acceptor_;
		std::experimental::net::io_context & io_context_;
    };
}

#endif
