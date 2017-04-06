#include "StorageModule.h"
#include "Resource.h"

namespace peer
{
    boost::shared_ptr<StorageModule> StorageModule::instance_;

    void StorageModule::Start()
    {
    }

    void StorageModule::Stop()
    {
        instance_.reset();
    }

    boost::shared_ptr<Resource> StorageModule::CreateResource(kit::Guid & resource_id)
    {
        std::map<kit::Guid, boost::shared_ptr<Resource> >::iterator iter = resource_map_.find(resource_id);
        if (iter != resource_map_.end())
        {
            return iter->second;
        }

        boost::shared_ptr<Resource> resource(new Resource(resource_id));
        resource_map_[resource_id] = resource;
        return resource;
    }

    void StorageModule::AddPiece(const kit::Guid & guid, const PieceInfo & piece,
        boost::shared_ptr<kit::Buffer> buffer)
    {
        if (resource_map_.find(guid) == resource_map_.end())
        {
            return;
        }

        resource_map_[guid]->AddPiece(piece, buffer);
    }
}