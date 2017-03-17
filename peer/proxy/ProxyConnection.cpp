#include "ProxyConnection.h"

namespace peer
{
    void ProxyConnection::Start()
    {
        http_server_.reset(new kit::HttpServer(socket_, shared_from_this()));
    }

    void ProxyConnection::Stop()
    {
        http_server_->Close();
        http_server_.reset();
    }

    void ProxyConnection::Recv()
    {
        http_server_->Recv();
    }

    void ProxyConnection::HandleRecv(boost::shared_ptr<kit::HttpServer> http_server, const std::string & request)
    {
        http_server_->Recv();
    }
}