#ifndef _PEER_STORAGE_MODULE_H_
#define _PEER_STORAGE_MODULE_H_

#include "peer/p2sp/Protocol.h"
#include "kit/buffer/Buffer.h"
#include "kit/base/Guid.h"
#include <map>

namespace peer
{
    class Resource;

    class StorageModule
    {
    public:
        static std::shared_ptr<StorageModule> Instance()
        {
            return instance_;
        }
        void Start();
        void Stop();

        std::shared_ptr<Resource> CreateResource(kit::Guid & resource_id);

        void AddPiece(const kit::Guid & guid, const PieceInfo & piece, std::shared_ptr<kit::Buffer> buffer);
    private:
		StorageModule() {}
        static std::shared_ptr<StorageModule> instance_;
        std::map<kit::Guid, std::shared_ptr<Resource> > resource_map_;
    };
}

#endif