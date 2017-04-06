#ifndef _PEER_DOWNLOADER_H_
#define _PEER_DOWNLOADER_H_

#include <boost/shared_ptr.hpp>
#include <string>
#include "Protocol.h"
#include "kit/base/Guid.h"
#include "kit/buffer/Buffer.h"
#include <boost/enable_shared_from_this.hpp>

namespace peer
{
    class ProxyConnection;
    class HttpTunnel;
    class P2PTunnel;
    class Resource;

    class Downloader
        : public boost::enable_shared_from_this<Downloader>
    {
    public:
        Downloader(boost::shared_ptr<ProxyConnection> proxy_connection)
            : proxy_connection_(proxy_connection)
        {

        }
        void Start(std::string & url, kit::Guid & resource_id);
        void Stop();

        PieceInfo & GetPushPosition()
        {
            return push_position;
        }

        void OnRecvPieceBuffer(boost::shared_ptr<kit::Buffer> buffer);

    private:
        boost::shared_ptr<ProxyConnection> proxy_connection_;
        boost::shared_ptr<HttpTunnel> http_tunnel_;
        boost::shared_ptr<P2PTunnel> p2p_tunnel_;

        boost::shared_ptr<Resource> resource_;

        PieceInfo push_position;
    };
}

#endif
