#ifndef _KIT_UDPSERVER_H_
#define _KIT_UDPSERVER_H_

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "kit/buffer/Buffer.h"

namespace kit
{
    struct IUdpServerListener
    {
        virtual void OnUdpRecv(boost::asio::ip::udp::endpoint & end_point, boost::shared_ptr<kit::Buffer> buffer) = 0;
        virtual boost::asio::io_service & GetIoService() = 0;
        virtual ~IUdpServerListener() {}
    };

    class UdpServer
        : public boost::enable_shared_from_this<UdpServer>
    {
    public:
        
        static boost::shared_ptr<UdpServer> create(boost::shared_ptr<IUdpServerListener> handler)
        {
            boost::asio::io_service * io_service = new boost::asio::io_service();
            boost::asio::io_service::work * work = new boost::asio::io_service::work(*io_service);
            return boost::shared_ptr<UdpServer>(new UdpServer(handler, io_service, work));
        }

    public:
        bool Listen(boost::uint16_t port);
        void Recv(boost::uint32_t count);
        void UdpRecvFrom();
        void UdpSendTo(boost::shared_ptr<kit::Buffer> buffer, const boost::asio::ip::udp::endpoint & endpoint);
        void Close();

        boost::uint16_t GetPort() const
        {
            return port_;
        }

    protected:
        void HandleUdpRecvFrom(const boost::system::error_code & ec, std::size_t bytes_transferred,
            boost::shared_ptr<kit::Buffer> recv_buffer, boost::asio::ip::udp::endpoint * endpoint);

        void HandleUdpSendTo(const boost::system::error_code & ec, std::size_t bytes_transferred,
            boost::shared_ptr<kit::Buffer> send_buffer);

    private:
        UdpServer(boost::shared_ptr<IUdpServerListener> handler, boost::asio::io_service * io_service,
            boost::asio::io_service::work * work);

    private:
        boost::asio::io_service *io_service_;
        boost::asio::io_service::work *work_;
        boost::asio::ip::udp::socket socket_;
        boost::shared_ptr<IUdpServerListener> handler_;
        boost::uint16_t port_;
    };
}

#endif