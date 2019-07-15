#ifndef _PEER_CHUNK_H_
#define _PEER_CHUNK_H_

#include "kit/buffer/Buffer.h"
#include <vector>

namespace peer
{
    class Chunk
    {
    public:
        Chunk(unsigned int chunk_id)
            : chunk_id_(chunk_id)
        {

        }
        void SetPiece(unsigned int piece_index, std::shared_ptr<kit::Buffer> buffer);
        std::shared_ptr<kit::Buffer> GetPiece(unsigned int piece_index);

        unsigned int GetChunkSize();
    private:
        unsigned int chunk_id_;
        std::vector<std::shared_ptr<kit::Buffer> > pieces_;
    };
}

#endif