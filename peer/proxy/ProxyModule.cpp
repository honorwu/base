#include "ProxyModule.h"
#include "kit/log/LogStream.h"
#include "ProxyConnection.h"

namespace peer
{
	std::shared_ptr<ProxyModule> ProxyModule::instance_(new ProxyModule());

    void ProxyModule::Start(unsigned short port)
    {
        tcp_acceptor_.reset(new kit::TcpAcceptor(io_context_, port, shared_from_this()));
        tcp_acceptor_->Listen();
    }

    void ProxyModule::Stop()
    {
        instance_.reset();
    }

    void ProxyModule::Accept()
    {
		std::experimental::net::ip::tcp::socket * socket = new std::experimental::net::ip::tcp::socket(io_context_);
        tcp_acceptor_->Accept(socket);
    }

    void ProxyModule::HandleAccept(const std::error_code& ec,
		std::experimental::net::ip::tcp::socket * socket)
    {
        std::shared_ptr<ProxyConnection> tcp_connection(new ProxyConnection(socket));
        tcp_connection->Start();
        tcp_connection->Recv();

        Accept();
    }
}