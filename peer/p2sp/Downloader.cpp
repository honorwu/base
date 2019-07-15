#include "Downloader.h"
#include "../proxy/ProxyConnection.h"
#include "p2s/HttpTunnel.h"
#include "p2p/P2PTunnel.h"
#include "../storage/StorageModule.h"
#include "../storage/Resource.h"

namespace peer
{
    void Downloader::Start(std::string & url, kit::Guid & resource_id)
    {
        http_tunnel_.reset(new HttpTunnel(url));
        http_tunnel_->Start();

        p2p_tunnel_.reset(new P2PTunnel(resource_id));
        p2p_tunnel_->Start();

        resource_ = StorageModule::Instance()->CreateResource(resource_id);
    }

    void Downloader::Stop()
    {
        http_tunnel_->Stop();
        http_tunnel_.reset();

        p2p_tunnel_->Stop();
        p2p_tunnel_.reset();

        proxy_connection_.reset();

        resource_->DetachDownloader(shared_from_this());
        resource_.reset();
    }

    void Downloader::OnRecvPieceBuffer(std::shared_ptr<kit::Buffer> buffer)
    {
        proxy_connection_->Send(buffer);
    }
}