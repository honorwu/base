#include "Downloader.h"
#include "p2s/HttpTunnel.h"
#include "p2p/P2PTunnel.h"

namespace peer
{
    void Downloader::Start()
    {
        http_tunnel_.reset(new HttpTunnel());
        http_tunnel_->Start();

        p2p_tunnel_.reset(new P2PTunnel());
        p2p_tunnel_->Start();
    }

    void Downloader::Stop()
    {
        http_tunnel_->Stop();
        http_tunnel_.reset();

        p2p_tunnel_->Stop();
        p2p_tunnel_.reset();
    }
}