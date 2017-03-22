#ifndef _PEER_PROTOCOL_H_
#define _PEER_PROTOCOL_H_

#include <boost/cstdint.hpp>

namespace peer
{
    struct PieceInfo
    {
        boost::uint32_t chunk_id_;
        boost::uint32_t piece_id_;

        bool operator < (const PieceInfo & p) const
        {
            if (chunk_id_ != p.chunk_id_)
            {
                return chunk_id_ < p.chunk_id_;
            }

            return piece_id_ < p.piece_id_;
        }
    };
}

#endif
