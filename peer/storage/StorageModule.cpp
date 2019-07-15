#include "StorageModule.h"
#include "Resource.h"

namespace peer
{
    std::shared_ptr<StorageModule> StorageModule::instance_(new StorageModule());

    void StorageModule::Start()
    {
    }

    void StorageModule::Stop()
    {
        instance_.reset();
    }

    std::shared_ptr<Resource> StorageModule::CreateResource(kit::Guid & resource_id)
    {
        std::map<kit::Guid, std::shared_ptr<Resource> >::iterator iter = resource_map_.find(resource_id);
        if (iter != resource_map_.end())
        {
            return iter->second;
        }

        std::shared_ptr<Resource> resource(new Resource(resource_id));
        resource_map_[resource_id] = resource;
        return resource;
    }

    void StorageModule::AddPiece(const kit::Guid & guid, const PieceInfo & piece,
        std::shared_ptr<kit::Buffer> buffer)
    {
        if (resource_map_.find(guid) == resource_map_.end())
        {
            return;
        }

        resource_map_[guid]->AddPiece(piece, buffer);
    }
}