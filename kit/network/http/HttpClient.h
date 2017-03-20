#ifndef _KIT_HTTP_CLIENT_H_
#define _KIT_HTTP_CLIENT_H_

#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

namespace kit
{
    struct IHttpClientListener
    {
        virtual void OnConnectSucced() = 0;
        virtual ~IHttpClientListener()
        {}
    };

    enum HttpClientStatus
    {
        None = 0,
        Connecting = 1
    };

    class HttpClient
        : public boost::enable_shared_from_this<HttpClient>
    {
    public:
		HttpClient(std::string url);
        void Connect();
        void Close();
    private:
        boost::asio::ip::tcp::socket * socket_;
        boost::shared_ptr<IHttpClientListener> listener_;
        std::string host_;
        std::string port_;
        boost::uint32_t status_;
    };
}

#endif