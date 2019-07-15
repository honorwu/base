#include "kit/network/tcp/TcpAcceptor.h"

namespace kit
{
    TcpAcceptor::TcpAcceptor(std::experimental::net::io_context & io_context,
        unsigned short port,
        std::shared_ptr<ITcpAcceptorListener> listener)
        : acceptor_(io_context)
        , port_(port)
        , listener_(listener)
    {
    }

    bool TcpAcceptor::Listen()
    {
        std::error_code ec;
		std::experimental::net::ip::address localhost = std::experimental::net::ip::make_address("0.0.0.0", ec);

        if (ec)
        {
            return false;
        }

		std::experimental::net::ip::tcp::endpoint endpoint(localhost, port_);

        acceptor_.open(endpoint.protocol(), ec);

        if (ec)
        {
            return false;
        }

		std::experimental::net::socket_base::reuse_address option(true);
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

    void TcpAcceptor::Accept(std::experimental::net::ip::tcp::socket * socket)
    {
        acceptor_.async_accept(std::bind(&TcpAcceptor::HandleAccept,
            this, std::placeholders::_1, socket));
    }

    void TcpAcceptor::Close()
    {
        std::error_code ec;
        acceptor_.cancel(ec);
        acceptor_.close(ec);
    }

    void TcpAcceptor::HandleAccept(const std::error_code& ec,
		std::experimental::net::ip::tcp::socket * socket)
    {
        listener_->HandleAccept(ec, socket);
    }
}