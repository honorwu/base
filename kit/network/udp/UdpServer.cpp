#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include "kit/network/udp/UdpServer.h"

namespace kit
{
    UdpServer::UdpServer(boost::shared_ptr<IUdpServerListener> handler, boost::asio::io_service * io_service,
        boost::asio::io_service::work * work)
        : handler_(handler)
        , port_(0)
        , io_service_(io_service)
        , work_(work)
        , socket_(*io_service)
    {
        for (size_t i=0; i<4; i++)
        {
            boost::shared_ptr<boost::thread> thread(new boost::thread(
                boost::bind(&boost::asio::io_service::run, &*io_service_)));
        }
    }

    bool UdpServer::Listen(boost::uint16_t port)
    {
        boost::system::error_code error;
        socket_.open(boost::asio::ip::udp::v4(), error);
        if (error)
        {
            socket_.close(error);
            return false;
        }
        socket_.bind(boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port), error);
        if (!error)
        {
            port_ = port;
            return true;
        }
        else
        {
            socket_.close(error);
            return false;
        }
    }

    void UdpServer::Recv(boost::uint32_t count)
    {
        for (boost::uint32_t i=0; i<20; i++)
        {
            UdpRecvFrom();
        }
    }
    
    void UdpServer::UdpRecvFrom()
    {
        boost::shared_ptr<kit::Buffer> recv_buffer = kit::Buffer::Create(2048);
        boost::asio::ip::udp::endpoint * endpoint = new boost::asio::ip::udp::endpoint();

        socket_.async_receive_from(
            boost::asio::buffer(recv_buffer->Data(), recv_buffer->RemainSize()),
            *endpoint,
            boost::bind(
                &UdpServer::HandleUdpRecvFrom, 
                shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred,
                recv_buffer,
                endpoint));
    }

    void UdpServer::UdpSendTo(boost::shared_ptr<kit::Buffer> buffer, const boost::asio::ip::udp::endpoint & endpoint)
    {
        boost::system::error_code ec;

        socket_.async_send_to(boost::asio::buffer(buffer->Data(), buffer->Length()), endpoint,
            boost::bind(&UdpServer::HandleUdpSendTo, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred,
            buffer));
    }

    void UdpServer::HandleUdpSendTo(const boost::system::error_code & ec, std::size_t bytes_transferred,
        boost::shared_ptr<kit::Buffer> send_buffer)
    {

    }

    void UdpServer::HandleUdpRecvFrom(const boost::system::error_code & ec, std::size_t bytes_transferred,
        boost::shared_ptr<kit::Buffer> recv_buffer, boost::asio::ip::udp::endpoint * endpoint)
    {
        if (!ec)
        {
            recv_buffer->SetLength(bytes_transferred);
            handler_->GetIoService().post(boost::bind(&kit::IUdpServerListener::OnUdpRecv, handler_, *endpoint, recv_buffer));
        }

        delete endpoint;

        UdpRecvFrom();
    }

    void UdpServer::Close()
    {
        boost::system::error_code ec;
        socket_.close(ec);

        delete work_;
        work_ = NULL;
        io_service_->stop();

        delete io_service_;
        io_service_ = NULL;
    }
}