#ifndef _PEER_RESOURCE_H_
#define _PEER_RESOURCE_H_

#include "peer/p2sp/Protocol.h"
#include <boost/shared_ptr.hpp>
#include "kit/buffer/Buffer.h"

namespace peer
{
    class Resource
    {
    public:
        void AddPiece(const PieceInfo & piece, boost::shared_ptr<kit::Buffer> buffer);
    private:

    };
}

#endif
