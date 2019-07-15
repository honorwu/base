#ifndef _KIT_UDPSERVER_H_
#define _KIT_UDPSERVER_H_
#include <experimental/net>
#include <experimental/io_context>
#include "kit/buffer/Buffer.h"

namespace kit
{
    struct IUdpServerListener
    {
        virtual void OnUdpRecv(std::shared_ptr<kit::Buffer> buffer,
            std::shared_ptr<std::experimental::net::ip::udp::endpoint> end_point) = 0;
        virtual ~IUdpServerListener() {}
    };

    class UdpServer
        : public std::enable_shared_from_this<UdpServer>
    {
    public:
        
        static std::shared_ptr<UdpServer> create(std::shared_ptr<IUdpServerListener> handler, std::experimental::net::io_context& io_context)
        {
            return std::shared_ptr<UdpServer>(new UdpServer(handler, io_context));
        }

    public:
        bool Listen(unsigned short port);
        void Recv(unsigned int count);
        void UdpRecvFrom();
        void UdpSendTo(std::shared_ptr<kit::Buffer> buffer, const std::experimental::net::v1::ip::udp::endpoint & endpoint);
        void Close();

        unsigned short GetPort() const
        {
            return port_;
        }

    protected:
        void HandleUdpRecvFrom(const std::error_code & ec, std::size_t bytes_transferred,
            std::shared_ptr<kit::Buffer> recv_buffer,
            std::shared_ptr<std::experimental::net::v1::ip::udp::endpoint> endpoint);

        void HandleUdpSendTo(const std::error_code & ec, std::size_t bytes_transferred,
            std::shared_ptr<kit::Buffer> send_buffer);

    private:
        UdpServer(std::shared_ptr<IUdpServerListener> handler, 
			std::experimental::net::io_context & io_context);

    private:
		std::experimental::net::ip::udp::socket socket_;
        std::shared_ptr<IUdpServerListener> handler_;
        unsigned short port_;
    };
}

#endif