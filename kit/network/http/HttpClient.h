#ifndef _KIT_HTTP_CLIENT_H_
#define _KIT_HTTP_CLIENT_H_

#include "kit/buffer/Buffer.h"
#include "kit/network/http/HttpResponse.h"
#include "kit/network/http/UrlParser.h"
#include "experimental/net"

namespace kit
{
    struct IHttpClientListener
    {
        virtual void HandleConnect(const std::error_code & ec) = 0;
        virtual void HandleRecvHttpResponse(std::shared_ptr<kit::HttpResponse> http_response) = 0;
        virtual void HandleRecvHttpData(const std::error_code & ec,
            std::shared_ptr<kit::Buffer> buffer) = 0;
        virtual ~IHttpClientListener()
        {
        }
    };

    class HttpClient
        : public std::enable_shared_from_this<HttpClient>
    {
    public:
        static std::shared_ptr<HttpClient> Create(std::experimental::net::io_context & io_context,
            const std::string & url, std::shared_ptr<IHttpClientListener> listener)
        {
            return std::shared_ptr<HttpClient>(new HttpClient(io_context, url, listener));
        }

        void Connect();
        void HandleConnect(const std::error_code & ec);

        void Resolve();
        void HandleResolve(const std::error_code & ec,
			const std::experimental::net::ip::tcp::resolver::results_type& endpoints);

        void SendHttpRequest(unsigned int range_begin = 0, unsigned int range_end = 0);
        void HandleSendHttpRequest(const std::error_code& ec, size_t bytes_transferred);

        void RecvHttpHeader();
        void HandleRecvHttpHeader(const std::error_code& ec, size_t bytes_transferred);

        void RecvHttpData(unsigned int piece_size);
        void HandleRecvHttpData(const std::error_code& ec, size_t bytes_transferred);

        void Close();

    private:
        explicit HttpClient(std::experimental::net::io_context & io_context, const std::string & url,
            std::shared_ptr<IHttpClientListener> listener)
            : socket_(io_context)
            , resolver_(io_context)
            , listener_(listener)
            , url_parser_(url)
        {

        }

        kit::UrlParser url_parser_;

		std::experimental::net::ip::tcp::socket socket_;
		std::experimental::net::ip::tcp::resolver resolver_;

        std::string response_;

        std::shared_ptr<kit::Buffer> recv_buffer_;

        std::shared_ptr<IHttpClientListener> listener_;
    };
}

#endif