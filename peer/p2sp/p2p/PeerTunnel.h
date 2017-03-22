#ifndef _PEER_PEER_TUNNEL_H_
#define _PEER_PEER_TUNNEL_H_

#include <string>
#include "peer/p2sp/Protocol.h"
#include <set>

namespace peer
{
    class PeerTunnel
    {
    public:
        void Start();
        void Stop();

        void AssignPiece(const PieceInfo & piece);

        void RequestAllPiece();

        void OnPieceReceved(const PieceInfo & piece);
        void OnPieceTimeout(const PieceInfo & piece);

    private:
        std::set<PieceInfo> task_set_;
        unsigned int window_size_;
        unsigned int requesting_count_;
    };
}

#endif
