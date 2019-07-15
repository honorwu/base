#ifndef _PEER_PEER_TUNNEL_H_
#define _PEER_PEER_TUNNEL_H_

#include <string>
#include "peer/p2sp/Protocol.h"
#include <set>
#include <map>
#include "kit/measure/ByteSpeedMeter.h"

namespace peer
{
    class PeerTunnel
    {
    public:
        void Start();
        void Stop();

        void AssignPiece(const PieceInfo & piece);

        void RequestAllPiece();

        void OnP2PTimer(unsigned int times);

        void OnPieceReceved(const PieceInfo & piece, unsigned int piece_size_in_byte);
        void OnPieceTimeout(const PieceInfo & piece);

    private:
        void UpdateRTT(const PieceInfo & piece);
        void CheckPieceTimeout();

        std::set<PieceInfo> task_set_;
        unsigned int requesting_count_;
        std::map<PieceInfo, kit::TickCounter> piece_timing_map_;

        unsigned int window_size_;

        unsigned int max_rtt_;
        unsigned int average_rtt_;

        kit::ByteSpeedMeter byte_speed_meter_;

    };
}

#endif