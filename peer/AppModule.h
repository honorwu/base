#ifndef _PEER_APP_MODULE_H_
#define _PEER_APP_MODULE_H_

#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "kit/network/udp/UdpServer.h"

namespace peer
{
    class AppModule
        : public kit::IUdpServerListener
        , public boost::enable_shared_from_this<AppModule>
    {
    public:
        static boost::shared_ptr<AppModule> Instance()
        {
            if (!instance_)
            {
                instance_.reset(new AppModule());
            }
            return instance_;
        }
        void Start(boost::uint16_t proxy_port, boost::uint16_t udp_port);
        void Stop();

        virtual void OnUdpRecv(boost::shared_ptr<kit::Buffer> buffer,
            boost::shared_ptr<boost::asio::ip::udp::endpoint> end_point);
        void SendUdpBuffer(boost::asio::ip::udp::endpoint & end_point,
            boost::shared_ptr<kit::Buffer> buffer);

    private:
        AppModule();
        static boost::shared_ptr<AppModule> instance_;
        boost::shared_ptr<kit::UdpServer> udp_server_;
    };
}

#endif