#ifndef _KIT_HTTPSERVER_H_
#define _KIT_HTTPSERVER_H_

#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "kit/buffer/Buffer.h"
#include <list>

namespace kit
{
    class HttpServer;

    struct IHttpServerListener
    {
        virtual void HandleRecv(boost::shared_ptr<kit::HttpServer> http_server, const std::string & request) = 0;
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
        : public boost::enable_shared_from_this<HttpServer>
    {
    public:
        HttpServer(boost::asio::ip::tcp::socket * socket,
            boost::shared_ptr<IHttpServerListener> listener)
            : socket_(socket)
            , listener_(listener)
            , close_status_(Pending)
        {

        }

        void Recv();
        void Send(boost::shared_ptr<kit::Buffer> buffer);
        void Close();
        boost::asio::ip::tcp::endpoint GetRemoteEndpint()
        {
            return socket_->remote_endpoint();
        }

    private:
        void HandleRecv(const boost::system::error_code& ec, boost::uint32_t bytes_transferred);
        void HandleSend(const boost::system::error_code& ec, boost::uint32_t bytes_transferred);

    private:
        boost::asio::streambuf request_;
        boost::asio::ip::tcp::socket * socket_;
        std::list<boost::shared_ptr<kit::Buffer> > send_list_;
        boost::shared_ptr<IHttpServerListener> listener_;
        boost::uint32_t close_status_;
    };
}

#endif