#ifndef _PEER_APP_MODULE_H_
#define _PEER_APP_MODULE_H_

#include "kit/network/udp/UdpServer.h"

namespace peer
{
    class AppModule
        : public kit::IUdpServerListener
        , public std::enable_shared_from_this<AppModule>
    {
    public:
        static std::shared_ptr<AppModule> Instance()
        {
            return instance_;
        }
        void Start(unsigned short proxy_port, unsigned short udp_port);
        void Stop();

        virtual void OnUdpRecv(std::shared_ptr<kit::Buffer> buffer,
            std::shared_ptr<std::experimental::net::ip::udp::endpoint> end_point);
        void SendUdpBuffer(std::experimental::net::ip::udp::endpoint & end_point,
            std::shared_ptr<kit::Buffer> buffer);

    private:
		AppModule() {}
        static std::shared_ptr<AppModule> instance_;
        std::shared_ptr<kit::UdpServer> udp_server_;
    };
}

#endif