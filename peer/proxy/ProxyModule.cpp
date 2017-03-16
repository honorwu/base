#include "ProxyModule.h"
#include "kit/log/LogStream.h"
#include "ProxyConnection.h"
#include <boost/shared_ptr.hpp>

namespace peer
{
    boost::shared_ptr<ProxyModule> ProxyModule::instance_;

    void ProxyModule::Start(boost::uint16_t port)
    {
        tcp_acceptor_.reset(new kit::TcpAcceptor(io_service_, port, shared_from_this()));
        tcp_acceptor_->Listen();
    }

    void ProxyModule::Stop()
    {
        instance_.reset();
    }

    void ProxyModule::Accept()
    {
        boost::asio::ip::tcp::socket * socket = new boost::asio::ip::tcp::socket(io_service_);
        tcp_acceptor_->Accept(socket);
    }

    void ProxyModule::HandleAccept(const boost::system::error_code& ec,
        boost::asio::ip::tcp::socket * socket)
    {
        boost::shared_ptr<ProxyConnection> tcp_connection(new ProxyConnection(socket));
        tcp_connection->Start();
        tcp_connection->Recv();

        Accept();
    }
}