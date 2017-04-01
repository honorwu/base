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

    void P2PTunnel::OnDataResponsePacket(boost::asio::ip::udp::endpoint & end_point, PieceInfo & piece)
    {
        std::map<boost::asio::ip::udp::endpoint, boost::shared_ptr<PeerTunnel> >::iterator
            iter = peer_tunnel_map_.find(end_point);

        if (iter == peer_tunnel_map_.end())
        {
            return;
        }

        iter->second->OnPieceReceved(piece, 0);
    }
}