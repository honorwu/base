#ifndef _PEER_STORAGE_MODULE_H_
#define _PEER_STORAGE_MODULE_H_

#include <boost/shared_ptr.hpp>
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
        static boost::shared_ptr<StorageModule> Instance()
        {
            if (!instance_)
            {
                instance_.reset(new StorageModule());
            }
            return instance_;
        }
        void Start();
        void Stop();

        boost::shared_ptr<Resource> CreateResource(kit::Guid & resource_id);

        void AddPiece(const kit::Guid & guid, const PieceInfo & piece, boost::shared_ptr<kit::Buffer> buffer);
    private:
        StorageModule();
        static boost::shared_ptr<StorageModule> instance_;
        std::map<kit::Guid, boost::shared_ptr<Resource> > resource_map_;
    };
}

#endif