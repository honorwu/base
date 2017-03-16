#ifndef _PEER_APP_MODULE_H_
#define _PEER_APP_MODULE_H_

#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>

namespace peer
{
    class AppModule
    {
    public:
        static boost::shared_ptr<AppModule> Instance()
        {
            if (!instance_)
            {
                instance_.reset(new AppModule());
            }
            return instance_;
        }
        void Start(boost::uint16_t proxy_port);
        void Stop();
    private:
        AppModule();
        static boost::shared_ptr<AppModule> instance_;
    };
}

#endif