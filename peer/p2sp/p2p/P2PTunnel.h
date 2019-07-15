#ifndef _PEER_P2P_TUNNEL_H_
#define _PEER_P2P_TUNNEL_H_

#include <map>
#include "PeerTunnel.h"
#include "kit/base/Guid.h"
#include "experimental/net"

namespace peer
{
    class P2PTunnel
    {
    public:
        P2PTunnel(kit::Guid & resource_id)
            : resource_id_(resource_id)
        {

        }

        void Start();
        void Stop();

        void Resume();
        void Pause();

        void OnDataResponsePacket(std::experimental::net::ip::udp::endpoint & end_point, PieceInfo & piece);
    private:
        kit::Guid & resource_id_;
        std::map<std::experimental::net::ip::udp::endpoint, std::shared_ptr<PeerTunnel> > peer_tunnel_map_;
    };
}

#endif