#include "AppModule.h"
#include "proxy/ProxyModule.h"
#include "storage/StorageModule.h"

namespace peer
{
    std::shared_ptr<AppModule> AppModule::instance_(new AppModule());

    void AppModule::Start(unsigned short proxy_port, unsigned short udp_port)
    {
        ProxyModule::Instance()->Start(proxy_port);
        StorageModule::Instance()->Start();

        udp_server_ = kit::UdpServer::create(shared_from_this(), std::experimental::net::io_context());
        udp_server_->Listen(udp_port);
    }

    void AppModule::Stop()
    {
        ProxyModule::Instance()->Stop();
        StorageModule::Instance()->Stop();
        instance_.reset();
    }

    void AppModule::OnUdpRecv(std::shared_ptr<kit::Buffer> buffer,
        std::shared_ptr<std::experimental::net::ip::udp::endpoint> end_point)
    {

    }

    void AppModule::SendUdpBuffer(std::experimental::net::ip::udp::endpoint & end_point,
        std::shared_ptr<kit::Buffer> buffer)
    {
        udp_server_->UdpSendTo(buffer, end_point);
    }
}