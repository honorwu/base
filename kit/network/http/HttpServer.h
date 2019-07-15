#ifndef _KIT_HTTP_SERVER_H_
#define _KIT_HTTP_SERVER_H_

#include "kit/buffer/Buffer.h"
#include <list>
#include "experimental/net"
#include "experimental/buffer"

namespace kit
{
    class HttpServer;

    struct IHttpServerListener
    {
        virtual void HandleRecv(std::shared_ptr<kit::HttpServer> http_server, const std::string & request) = 0;
        virtual ~IHttpServerListener()
        {}
    };

    enum CloseStatus
    {
        Pending = 0,
        WillClose = 1,
        Closed = 2
    };

    class HttpServer
        : public std::enable_shared_from_this<HttpServer>
    {
    public:
        HttpServer(std::experimental::net::ip::tcp::socket * socket,
            std::shared_ptr<IHttpServerListener> listener)
            : socket_(socket)
            , listener_(listener)
            , close_status_(Pending)
        {

        }

        void Recv();
        void Send(std::shared_ptr<kit::Buffer> buffer);
        void Close();
		std::experimental::net::ip::tcp::endpoint GetRemoteEndpint()
        {
            return socket_->remote_endpoint();
        }

    private:
        void HandleRecv(const std::error_code& ec, unsigned int bytes_transferred);
        void HandleSend(const std::error_code& ec, unsigned int bytes_transferred);

    private:
		//std::experimental::net::basic_socket_streambuf request_;
		std::string request_;
		std::experimental::net::ip::tcp::socket * socket_;
        std::list<std::shared_ptr<kit::Buffer>> send_list_;
        std::shared_ptr<IHttpServerListener> listener_;
        unsigned int close_status_;
    };
}

#endif