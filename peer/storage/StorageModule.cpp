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
}