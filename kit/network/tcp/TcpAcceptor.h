#ifndef _KIT_TCPACCEPTOR_H_
#define _KIT_TCPACCEPTOR_H_

#include "experimental/net"
#include "experimental/io_context"

namespace kit
{
    struct ITcpAcceptorListener
    {
        virtual void HandleAccept(const std::error_code& ec, 
			std::experimental::net::ip::tcp::socket * socket) = 0;
        virtual ~ITcpAcceptorListener()
        {
        }
    };

    class TcpAcceptor
    {
    public:
        TcpAcceptor(
			std::experimental::net::io_context & io_context,
            unsigned short port,
            std::shared_ptr<ITcpAcceptorListener> listener);

    public:
        bool Listen();
        void Accept(std::experimental::net::ip::tcp::socket * socket);
        void Close();
    public:
        unsigned short GetHttpPort() const
        {
            return port_;
        }

    protected:
        virtual void HandleAccept(const std::error_code& ec,
			std::experimental::net::ip::tcp::socket * socket);

    private:
        std::experimental::net::ip::tcp::acceptor acceptor_;
        unsigned short port_;
        std::shared_ptr<ITcpAcceptorListener> listener_;
    };
}
#endif