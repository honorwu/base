#ifndef _PEER_DOWNLOADER_H_
#define _PEER_DOWNLOADER_H_

#include <string>
#include "Protocol.h"
#include "kit/base/Guid.h"
#include "kit/buffer/Buffer.h"

namespace peer
{
    class ProxyConnection;
    class HttpTunnel;
    class P2PTunnel;
    class Resource;

    class Downloader
        : public std::enable_shared_from_this<Downloader>
    {
    public:
        Downloader(std::shared_ptr<ProxyConnection> proxy_connection)
            : proxy_connection_(proxy_connection)
        {

        }
        void Start(std::string & url, kit::Guid & resource_id);
        void Stop();

        PieceInfo & GetPushPosition()
        {
            return push_position;
        }

        void OnRecvPieceBuffer(std::shared_ptr<kit::Buffer> buffer);

    private:
        std::shared_ptr<ProxyConnection> proxy_connection_;
        std::shared_ptr<HttpTunnel> http_tunnel_;
        std::shared_ptr<P2PTunnel> p2p_tunnel_;

        std::shared_ptr<Resource> resource_;

        PieceInfo push_position;
    };
}

#endif
