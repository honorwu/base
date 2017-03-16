#include "AppModule.h"
#include "proxy/ProxyModule.h"
#include "storage/StorageModule.h"

namespace peer
{
    boost::shared_ptr<AppModule> AppModule::instance_;

    void AppModule::Start(boost::uint16_t proxy_port)
    {
        ProxyModule::Instance()->Start(proxy_port);
        StorageModule::Instance()->Start();
    }

    void AppModule::Stop()
    {
        ProxyModule::Instance()->Stop();
        StorageModule::Instance()->Stop();
        instance_.reset();
    }
}