#include "P2PTunnel.h"

namespace peer
{
    void P2PTunnel::Start()
    {

    }

    void P2PTunnel::Stop()
    {

    }

    void P2PTunnel::Resume()
    {

    }

    void P2PTunnel::Pause()
    {

    }

    void P2PTunnel::OnDataResponsePacket(std::experimental::net::ip::udp::endpoint & end_point, PieceInfo & piece)
    {
        std::map<std::experimental::net::ip::udp::endpoint, std::shared_ptr<PeerTunnel> >::iterator
            iter = peer_tunnel_map_.find(end_point);

        if (iter == peer_tunnel_map_.end())
        {
            return;
        }

        iter->second->OnPieceReceved(piece, 0);
    }
}