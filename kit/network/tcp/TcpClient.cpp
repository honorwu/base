#include "kit/network/tcp/TcpClient.h"
#include "kit/buffer/Buffer.h"
#include "kit/log/LoggingModule.h"

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <string>

namespace kit
{
    static const unsigned int BUFFER_SIZE = 1024;

    TcpClient::TcpClient(boost::asio::io_service & io_service, 
        const std::string & server, boost::uint16_t port,
        boost::shared_ptr<ITcpClientListener> listener)
        : socket_(io_service)
        , resolver_(io_service)
        , server_(server)
        , port_(port)
        , listener_(listener)
    {
    }

    void TcpClient::Connect()
    {
        boost::system::error_code ec;
        boost::asio::ip::address_v4 server_address = boost::asio::ip::address_v4::from_string(server_, ec);

        if (ec)
        {
            Resolve();
            return;
        }

        boost::asio::ip::tcp::resolver::iterator resolver_iterator;
        boost::asio::ip::tcp::endpoint endpoint = boost::asio::ip::tcp::endpoint(server_address, port_);

        socket_.async_connect(endpoint, boost::bind(&TcpClient::HandleConnect, this,
                boost::asio::placeholders::error, resolver_iterator));
    }

    void TcpClient::Resolve()
    {
        boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), server_,
            boost::lexical_cast<std::string>(port_));

        resolver_.async_resolve(query, boost::bind(&TcpClient::HandleResolve, this,
                boost::asio::placeholders::error, boost::asio::placeholders::iterator));
    }

    void TcpClient::HandleResolve(const boost::system::error_code & ec,
        boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
    {
        if (ec)
        {
            return;
        }

        boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;

        socket_.async_connect(endpoint, boost::bind(&TcpClient::HandleConnect, this,
            boost::asio::placeholders::error, ++endpoint_iterator));
    }

    void TcpClient::HandleConnect(const boost::system::error_code & ec,
        boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
    {
        if (ec)
        {
            if (endpoint_iterator == boost::asio::ip::tcp::resolver::iterator())
            {
                listener_->HandleConnect(ec);
                return;
            }

            boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;

            socket_.async_connect(endpoint, boost::bind(&TcpClient::HandleConnect, this,
                boost::asio::placeholders::error, ++endpoint_iterator));

            return;
        }

        listener_->HandleConnect(ec);
    }

    void TcpClient::Send(boost::shared_ptr<kit::Buffer> buffer)
    {
        bool is_send_list_empty = send_list_.empty();
        send_list_.push_back(buffer);

        if (is_send_list_empty)
        {
            DoSend(buffer);
        }
    }

    void TcpClient::DoSend(boost::shared_ptr<kit::Buffer> buffer)
    {
        boost::asio::async_write(socket_, boost::asio::buffer(buffer->Data(), buffer->Length()),
                boost::bind(&TcpClient::HandleSend, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

    void TcpClient::HandleSend(const boost::system::error_code& ec, size_t bytes_transferred)
    {
        send_list_.pop_front();

        if (ec)
        {
            return;
        }

        if (!send_list_.empty())
        {
            DoSend(send_list_.front());
        }
    }

    void TcpClient::Recv()
    {
        recv_buffer_ = kit::Buffer::Create(BUFFER_SIZE);

        socket_.async_read_some(boost::asio::buffer(recv_buffer_->Data(), recv_buffer_->RemainSize()), boost::bind(&TcpClient::HandleRecv,
            this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }

    void TcpClient::HandleRecv(const boost::system::error_code& ec, size_t bytes_transferred)
    {
        if (ec)
        {
            return;
        }

        recv_buffer_->SetLength(bytes_transferred);

        listener_->HandleRecv(recv_buffer_);

        Recv();
    }

    void TcpClient::Close()
    {
        boost::system::error_code ec;
        socket_.close(ec);

        listener_.reset();
    }
}