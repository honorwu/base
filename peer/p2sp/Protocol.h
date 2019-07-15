#ifndef _PEER_PROTOCOL_H_
#define _PEER_PROTOCOL_H_

namespace peer
{
    struct PieceInfo
    {
        unsigned int chunk_id_;
        unsigned int piece_id_;

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
