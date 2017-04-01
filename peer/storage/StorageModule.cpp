#include "StorageModule.h"

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