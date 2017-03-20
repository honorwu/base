#include "HttpClient.h"
#include "kit/network/http/UrlParser.h"

namespace kit
{
	HttpClient::HttpClient(std::string url)
	{
		kit::UrlParser url_parser(url);
		host_ = url_parser.GetHost();
		port_ = url_parser.GetPort();
	}

    void HttpClient::Connect()
    {
        assert(status_ == None);
        status_ = Connecting;

        boost::system::error_code ec;
        boost::asio::ip::address_v4 addr = boost::asio::ip::address_v4::from_string(host_, ec);
        if (ec)
        {

        }
        else
        {

        }
    }

    void HttpClient::Close()
    {
        listener_.reset();
    }
}