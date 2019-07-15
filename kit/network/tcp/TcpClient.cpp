#include "kit/network/tcp/TcpClient.h"
#include "kit/buffer/Buffer.h"

#include <string>

namespace kit
{
    static const unsigned int BUFFER_SIZE = 1024;

    TcpClient::TcpClient(std::experimental::net::io_context & io_context,
        const std::string & server,
		unsigned short port,
        std::shared_ptr<ITcpClientListener> listener)
        : socket_(io_context)
        , resolver_(io_context)
        , server_(server)
        , port_(port)
        , listener_(listener)
    {
    }

    void TcpClient::Connect()
    {
        std::error_code ec;
		std::experimental::net::ip::address server_address = std::experimental::net::ip::make_address(server_, ec);

        if (ec)
        {
            Resolve();
            return;
        }

		std::experimental::net::ip::tcp::endpoint endpoint = std::experimental::net::ip::tcp::endpoint(server_address, port_);

        socket_.async_connect(endpoint, std::bind(&TcpClient::HandleConnect, this,
                std::placeholders::_1));
    }

    void TcpClient::Resolve()
    {
		resolver_.async_resolve(server_, "http", std::bind(&TcpClient::HandleResolve, this,
                std::placeholders::_1, std::placeholders::_2));
    }

    void TcpClient::HandleResolve(const std::error_code & ec,
		const std::experimental::net::ip::tcp::resolver::results_type & endpoints)
    {
        if (ec)
        {
            return;
        }

		std::experimental::net::async_connect(socket_, endpoints,
			std::bind(&TcpClient::HandleConnect, this,
				std::placeholders::_1));
    }

    void TcpClient::HandleConnect(const std::error_code & ec)
    {
        if (ec)
        {
			return;
        }

        listener_->HandleConnect(ec);
    }

    void TcpClient::Send(std::shared_ptr<kit::Buffer> buffer)
    {
        bool is_send_list_empty = send_list_.empty();
        send_list_.push_back(buffer);

        if (is_send_list_empty)
        {
            DoSend(buffer);
        }
    }

    void TcpClient::DoSend(std::shared_ptr<kit::Buffer> buffer)
    {
		socket_.async_send(std::experimental::net::buffer(buffer->Data(), buffer->Length()),
			std::bind(&TcpClient::HandleSend, this,
				std::placeholders::_1,
				std::placeholders::_2));
    }

    void TcpClient::HandleSend(const std::error_code& ec, size_t bytes_transferred)
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

        socket_.async_read_some(std::experimental::net::buffer(recv_buffer_->Data(), recv_buffer_->RemainSize()),
			std::bind(&TcpClient::HandleRecv,
            this, std::placeholders::_1, std::placeholders::_2));
    }

    void TcpClient::HandleRecv(const std::error_code& ec, size_t bytes_transferred)
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
        std::error_code ec;
        socket_.close(ec);

        listener_.reset();
    }
}