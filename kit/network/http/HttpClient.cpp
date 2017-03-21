#include "HttpClient.h"
#include "kit/network/http/UrlParser.h"
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

namespace kit
{
    void HttpClient::Connect()
    {
        boost::system::error_code ec;
        boost::asio::ip::address_v4 server_address = boost::asio::ip::address_v4::from_string(url_parser_.GetHost(), ec);

        if (ec)
        {
            Resolve();
            return;
        }

        boost::asio::ip::tcp::resolver::iterator resolver_iterator;
        boost::asio::ip::tcp::endpoint endpoint = boost::asio::ip::tcp::endpoint(server_address, atoi(url_parser_.GetPort().c_str()));

        socket_.async_connect(endpoint, boost::bind(&HttpClient::HandleConnect, shared_from_this(),
            boost::asio::placeholders::error, resolver_iterator));
    }

    void HttpClient::HandleConnect(const boost::system::error_code & ec, boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
    {
        if (ec)
        {
            if (endpoint_iterator == boost::asio::ip::tcp::resolver::iterator())
            {
                listener_->HandleConnect(ec);
                return;
            }

            boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;

            socket_.async_connect(endpoint, boost::bind(&HttpClient::HandleConnect, shared_from_this(),
                boost::asio::placeholders::error, ++endpoint_iterator));

            return;
        }

        listener_->HandleConnect(ec);
    }

    void HttpClient::Resolve()
    {
        boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), url_parser_.GetHost(), url_parser_.GetPort());

        resolver_.async_resolve(query, boost::bind(&HttpClient::HandleResolve, shared_from_this(),
            boost::asio::placeholders::error, boost::asio::placeholders::iterator));
    }

    void HttpClient::HandleResolve(const boost::system::error_code & ec, boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
    {
        if (ec)
        {
            return;
        }

        boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;

        socket_.async_connect(endpoint, boost::bind(&HttpClient::HandleConnect, shared_from_this(),
            boost::asio::placeholders::error, ++endpoint_iterator));
    }

    void HttpClient::SendHttpRequest(unsigned int range_begin, unsigned int range_end)
    {
        std::stringstream sstr;
        sstr << "GET " << url_parser_.GetRequest() << " HTTP/1.0\r\n";

        if (range_end != 0)
        {
            char t[256] = { 0 };
            sprintf(t, "%d-%d", range_begin, range_end);

            sstr << "Range:bytes=" << std::string(t) << "\r\n";
        }

        sstr << "Host: " << url_parser_.GetHost() << "\r\n";
        sstr << "Connection: Close\r\n\r\n";

        std::string request_string = sstr.str();

        boost::shared_ptr<kit::Buffer> buffer = kit::Buffer::Create(request_string.length());
        buffer->Append(request_string.c_str(), request_string.length());

        boost::asio::async_write(socket_, boost::asio::buffer(buffer->Data(), buffer->Length()),
            boost::bind(&HttpClient::HandleSendHttpRequest, shared_from_this(),
                boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }

    void HttpClient::HandleSendHttpRequest(const boost::system::error_code& ec, size_t bytes_transferred)
    {
        RecvHttpHeader();
    }

    void HttpClient::RecvHttpHeader()
    {
        std::string delim("\r\n\r\n");
        boost::asio::async_read_until(socket_, response_,
            delim, boost::bind(&HttpClient::HandleRecvHttpHeader, shared_from_this(),
                boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }

    void HttpClient::HandleRecvHttpHeader(const boost::system::error_code& ec, size_t bytes_transferred)
    {
        assert(bytes_transferred <= response_.size());
        std::string response_string;
        std::istream is(&response_);
        response_string.resize(bytes_transferred);
        is.read((char*)&response_string[0], bytes_transferred);

        boost::shared_ptr<kit::HttpResponse> http_response =
            kit::HttpResponse::ParseFromBuffer(response_string);

        listener_->HandleRecvHttpResponse(http_response);
    }

    void HttpClient::RecvHttpData(unsigned int piece_size)
    {
        recv_buffer_ = kit::Buffer::Create(piece_size);

        unsigned int offset = 0;
        if (response_.size() > piece_size)
        {
            std::istream is(&response_);
            is.read((char*)recv_buffer_->Data(), piece_size);
            HandleRecvHttpData(boost::system::error_code(), piece_size);
        }
        else if (response_.size() > 0)
        {
            std::istream is(&response_);
            offset = response_.size();
            is.read((char*)recv_buffer_->Data(), response_.size());
            recv_buffer_->SetLength(offset);
        }

        boost::asio::async_read(socket_, boost::asio::buffer(recv_buffer_->Data() + offset, recv_buffer_->RemainSize() - offset),
            boost::asio::transfer_at_least(recv_buffer_->RemainSize() - offset),
            boost::bind(&HttpClient::HandleRecvHttpData, shared_from_this(),
                boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }

    void HttpClient::HandleRecvHttpData(const boost::system::error_code& ec, size_t bytes_transferred)
    {
        recv_buffer_->SetLength(recv_buffer_->Length() + bytes_transferred);
        listener_->HandleRecvHttpData(ec, recv_buffer_);
    }

    void HttpClient::Close()
    {
        boost::system::error_code ec;
        socket_.shutdown(boost::asio::socket_base::shutdown_both, ec);
        socket_.close(ec);

        listener_.reset();
    }
}