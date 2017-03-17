#ifndef _PEER_DOWNLOADER_H_
#define _PEER_DOWNLOADER_H_

#include <boost/shared_ptr.hpp>

namespace peer
{
    class HttpTunnel;
    class P2PTunnel;

    class Downloader
    {
    public:
        void Start();
        void Stop();
    private:
        boost::shared_ptr<HttpTunnel> http_tunnel_;
        boost::shared_ptr<P2PTunnel> p2p_tunnel_;
    };
}

#endif
