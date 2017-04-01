#include "AppModule.h"
#include "proxy/ProxyModule.h"
#include "storage/StorageModule.h"

namespace peer
{
    boost::shared_ptr<AppModule> AppModule::instance_;

    void AppModule::Start(boost::uint16_t proxy_port, boost::uint16_t udp_port)
    {
        ProxyModule::Instance()->Start(proxy_port);
        StorageModule::Instance()->Start();

        udp_server_ = kit::UdpServer::create(shared_from_this(), boost::asio::io_service());
        udp_server_->Listen(udp_port);
    }

    void AppModule::Stop()
    {
        ProxyModule::Instance()->Stop();
        StorageModule::Instance()->Stop();
        instance_.reset();
    }

    void AppModule::OnUdpRecv(boost::shared_ptr<kit::Buffer> buffer,
        boost::shared_ptr<boost::asio::ip::udp::endpoint> end_point)
    {

    }

    void AppModule::SendUdpBuffer(boost::asio::ip::udp::endpoint & end_point,
        boost::shared_ptr<kit::Buffer> buffer)
    {
        udp_server_->UdpSendTo(buffer, end_point);
    }
}