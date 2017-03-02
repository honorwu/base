#ifndef _KIT_TCPACCEPTOR_H_
#define _KIT_TCPACCEPTOR_H_

#include <boost/shared_ptr.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace kit
{
    struct ITcpAcceptorListener
    {
        virtual void HandleAccept(const boost::system::error_code& ec, 
            boost::asio::ip::tcp::socket * socket) = 0;
        virtual ~ITcpAcceptorListener()
        {
        }
    };

    class TcpAcceptor
    {
    public:
        TcpAcceptor(
            boost::asio::io_service & io_service,
            boost::uint16_t port,
            boost::shared_ptr<ITcpAcceptorListener> listener);

    public:
        bool Listen();
        void Accept(boost::asio::ip::tcp::socket * socket);
        void Close();
    public:
        boost::uint16_t GetHttpPort() const
        {
            return port_;
        }

    protected:
        virtual void HandleAccept(const boost::system::error_code& ec,
            boost::asio::ip::tcp::socket * socket);

    private:
        boost::asio::ip::tcp::acceptor acceptor_;
        boost::uint16_t port_;
        boost::shared_ptr<ITcpAcceptorListener> listener_;
    };
}
#endif