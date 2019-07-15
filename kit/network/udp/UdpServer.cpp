#include "kit/network/udp/UdpServer.h"
#include <experimental/buffer>

namespace kit
{
    UdpServer::UdpServer(std::shared_ptr<IUdpServerListener> handler,
        std::experimental::net::io_context & io_context)
        : handler_(handler)
        , socket_(io_context)
    {
    }

    bool UdpServer::Listen(unsigned short port)
    {
		std::error_code ec;
        socket_.open(std::experimental::net::ip::udp::v4(), ec);
        if (ec)
        {
            socket_.close(ec);
            return false;
        }
        socket_.bind(std::experimental::net::ip::udp::endpoint(std::experimental::net::ip::udp::v4(), port), ec);
        if (!ec)
        {
            port_ = port;
            return true;
        }
        else
        {
            socket_.close(ec);
            return false;
        }
    }

    void UdpServer::Recv(unsigned int count)
    {
        for (unsigned int i=0; i<20; i++)
        {
            UdpRecvFrom();
        }
    }
    
    void UdpServer::UdpRecvFrom()
    {
        std::shared_ptr<kit::Buffer> recv_buffer = kit::Buffer::Create(2048);
        std::shared_ptr<std::experimental::net::v1::ip::udp::endpoint> endpoint(new std::experimental::net::v1::ip::udp::endpoint());
        recv_buffer->Clear();

		socket_.async_receive_from(
			std::experimental::net::buffer(recv_buffer->Data(), recv_buffer->RemainSize()),
			*endpoint, 
            std::bind(
                &UdpServer::HandleUdpRecvFrom, 
                shared_from_this(),
                std::placeholders::_1,
				std::placeholders::_2,
                recv_buffer,
                endpoint));
    }

    void UdpServer::UdpSendTo(std::shared_ptr<kit::Buffer> buffer, const std::experimental::net::v1::ip::udp::endpoint & endpoint)
    {
		socket_.async_send_to(std::experimental::net::buffer(buffer->Data(), buffer->Length()), endpoint,
			std::bind(&UdpServer::HandleUdpSendTo, shared_from_this(),
				std::placeholders::_1,
				std::placeholders::_2,
				buffer));
    }

    void UdpServer::HandleUdpSendTo(const std::error_code & ec, std::size_t bytes_transferred,
        std::shared_ptr<kit::Buffer> send_buffer)
    {

    }

    void UdpServer::HandleUdpRecvFrom(const std::error_code & ec, std::size_t bytes_transferred,
        std::shared_ptr<kit::Buffer> recv_buffer,
        std::shared_ptr<std::experimental::net::v1::ip::udp::endpoint> endpoint)
    {
        if (!ec)
        {
            recv_buffer->SetLength(bytes_transferred);
            handler_->OnUdpRecv(recv_buffer, endpoint);
        }

        endpoint.reset();
        recv_buffer.reset();

        UdpRecvFrom();
    }

    void UdpServer::Close()
    {
		std::error_code ec;
        socket_.close(ec);
    }
}