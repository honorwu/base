#ifndef _PEER_STORAGE_MODULE_H_
#define _PEER_STORAGE_MODULE_H_

#include <boost/shared_ptr.hpp>

namespace peer
{
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
    private:
        StorageModule();
        static boost::shared_ptr<StorageModule> instance_;
    };
}

#endif