#ifndef _PEER_P2P_TUNNEL_H_
#define _PEER_P2P_TUNNEL_H_

#include <string>
#include <map>
#include <boost/asio.hpp>
#include "PeerTunnel.h"

namespace peer
{
    class P2PTunnel
    {
    public:
        void Start();
        void Stop();

        void Resume();
        void Pause();

        void OnDataResponsePacket(boost::asio::ip::udp::endpoint & end_point, PieceInfo & piece);
    private:
        std::string url_;
        std::map<boost::asio::ip::udp::endpoint, boost::shared_ptr<PeerTunnel> > peer_tunnel_map_;
    };
}

#endif