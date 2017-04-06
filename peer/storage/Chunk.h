#ifndef _PEER_CHUNK_H_
#define _PEER_CHUNK_H_

#include "kit/buffer/Buffer.h"
#include <boost/shared_ptr.hpp>
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
        void SetPiece(unsigned int piece_index, boost::shared_ptr<kit::Buffer> buffer);
        boost::shared_ptr<kit::Buffer> GetPiece(unsigned int piece_index);

        unsigned int GetChunkSize();
    private:
        unsigned int chunk_id_;
        std::vector<boost::shared_ptr<kit::Buffer> > pieces_;
    };
}

#endif