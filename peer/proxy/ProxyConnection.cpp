#include "ProxyConnection.h"
#include "kit/base/Guid.h"
#include "../p2sp/Downloader.h"

namespace peer
{
    void ProxyConnection::Start()
    {
        http_server_.reset(new kit::HttpServer(socket_, shared_from_this()));
    }

    void ProxyConnection::Stop()
    {
        downloader_->Stop();
        downloader_.reset();

        http_server_->Close();
        http_server_.reset();
    }

    void ProxyConnection::Recv()
    {
        http_server_->Recv();
    }

    void ProxyConnection::HandleRecv(boost::shared_ptr<kit::HttpServer> http_server, const std::string & request)
    {
        std::string url;
        kit::Guid resource_id;
        downloader_.reset(new Downloader(shared_from_this()));
        downloader_->Start(url, resource_id);

        http_server_->Recv();
    }

    void ProxyConnection::Send(boost::shared_ptr<kit::Buffer> buffer)
    {
        http_server_->Send(buffer);
    }
}