#include "kit/network/tcp/TcpAcceptor.h"

#include <boost/bind.hpp>
#include <boost/asio/placeholders.hpp>

namespace kit
{
    TcpAcceptor::TcpAcceptor(boost::asio::io_service & io_service,
        boost::uint16_t port,
        boost::shared_ptr<ITcpAcceptorListener> listener)
        : acceptor_(io_service)
        , port_(port)
        , listener_(listener)
    {
    }

    bool TcpAcceptor::Listen()
    {
        boost::system::error_code ec;
        boost::asio::ip::address localhost = boost::asio::ip::address::from_string("0.0.0.0", ec);

        if (ec)
        {
            return false;
        }

        boost::asio::ip::tcp::endpoint endpoint(localhost, port_);

        acceptor_.open(endpoint.protocol(), ec);

        if (ec)
        {
            return false;
        }

        boost::asio::socket_base::reuse_address option(true);
        acceptor_.set_option(option, ec);

        if (ec)
        {
            Close();
            return false;
        }

        acceptor_.bind(endpoint, ec);

        if (ec)
        {
            return false;
        }

        acceptor_.listen(0, ec);

        if (ec)
        {
            return false;
        }

        return true;
    }

    void TcpAcceptor::Accept(boost::asio::ip::tcp::socket * socket)
    {
        acceptor_.async_accept(*socket, boost::bind(&TcpAcceptor::HandleAccept,
            this, boost::asio::placeholders::error, socket));
    }

    void TcpAcceptor::Close()
    {
        boost::system::error_code ec;
        acceptor_.cancel(ec);
        acceptor_.close(ec);
    }

    void TcpAcceptor::HandleAccept(const boost::system::error_code& ec,
        boost::asio::ip::tcp::socket * socket)
    {
        listener_->HandleAccept(ec, socket);
    }
}