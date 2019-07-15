#ifndef _PEER_RESOURCE_H_
#define _PEER_RESOURCE_H_

#include "peer/p2sp/Protocol.h"
#include "kit/buffer/Buffer.h"
#include "kit/base/Guid.h"
#include <set>
#include <vector>

namespace peer
{
    class Downloader;
    class Chunk;

    class Resource
    {
    public:
        Resource(kit::Guid & resource_id)
            : resource_id_(resource_id)
        {

        }

        void AttachDownloader(std::shared_ptr<Downloader> downloader);
        void DetachDownloader(std::shared_ptr<Downloader> downloader);

        void AddPiece(const PieceInfo & piece, std::shared_ptr<kit::Buffer> buffer);
        void PushHttpResponseData();
    private:
        kit::Guid & resource_id_;
        std::vector<std::shared_ptr<Chunk> > chunks_;
        std::set<std::shared_ptr<Downloader> > downloaders_;
    };
}

#endif
