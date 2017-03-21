#ifndef _KIT_HTTP_CLIENT_H_
#define _KIT_HTTP_CLIENT_H_

#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include "kit/buffer/Buffer.h"
#include "kit/network/http/HttpResponse.h"
#include "kit/network/http/UrlParser.h"

namespace kit
{
    struct IHttpClientListener
    {
        virtual void HandleConnect(const boost::system::error_code & ec) = 0;
        virtual void HandleRecvHttpResponse(boost::shared_ptr<kit::HttpResponse> http_response) = 0;
        virtual void HandleRecvHttpData(const boost::system::error_code & ec,
            boost::shared_ptr<kit::Buffer> buffer) = 0;
        virtual ~IHttpClientListener()
        {
        }
    };

    class HttpClient
        : public boost::enable_shared_from_this<HttpClient>
    {
    public:
        static boost::shared_ptr<HttpClient> Create(boost::asio::io_service & io_service,
            const std::string & url, boost::shared_ptr<IHttpClientListener> listener)
        {
            return boost::shared_ptr<HttpClient>(new HttpClient(io_service, url, listener));
        }

        void Connect();
        void HandleConnect(const boost::system::error_code & ec, boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

        void Resolve();
        void HandleResolve(const boost::system::error_code & ec, boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

        void SendHttpRequest(unsigned int range_begin = 0, unsigned int range_end = 0);
        void HandleSendHttpRequest(const boost::system::error_code& ec, size_t bytes_transferred);

        void RecvHttpHeader();
        void HandleRecvHttpHeader(const boost::system::error_code& ec, size_t bytes_transferred);

        void RecvHttpData(unsigned int piece_size);
        void HandleRecvHttpData(const boost::system::error_code& ec, size_t bytes_transferred);

        void Close();

    private:
        explicit HttpClient(boost::asio::io_service & io_service, const std::string & url,
            boost::shared_ptr<IHttpClientListener> listener)
            : socket_(io_service)
            , resolver_(io_service)
            , listener_(listener)
            , url_parser_(url)
        {

        }

        kit::UrlParser url_parser_;

        boost::asio::ip::tcp::socket socket_;
        boost::asio::ip::tcp::resolver resolver_;

        boost::asio::streambuf response_;

        boost::shared_ptr<kit::Buffer> recv_buffer_;

        boost::shared_ptr<IHttpClientListener> listener_;
    };
}

#endif