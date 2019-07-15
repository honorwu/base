#include "HttpClient.h"
#include "kit/network/http/UrlParser.h"
#include "experimental/net"

namespace kit
{
    void HttpClient::Connect()
    {
		std::error_code ec;
		std::experimental::net::ip::address server_address = std::experimental::net::ip::make_address(url_parser_.GetHost(), ec);

		if (ec)
		{
			Resolve();
			return;
		}

		std::experimental::net::ip::tcp::endpoint endpoint = std::experimental::net::ip::tcp::endpoint(server_address, 
			atoi(url_parser_.GetPort().c_str()));

		socket_.async_connect(endpoint, std::bind(&HttpClient::HandleConnect, this,
			std::placeholders::_1));
    }

    void HttpClient::HandleConnect(const std::error_code & ec)
    {
        if (ec)
        {
            return;
        }

        listener_->HandleConnect(ec);
    }

    void HttpClient::Resolve()
    {
		resolver_.async_resolve(url_parser_.GetHost(), "http", std::bind(&HttpClient::HandleResolve, this,
			std::placeholders::_1, std::placeholders::_2));
    }

    void HttpClient::HandleResolve(const std::error_code & ec,
		const std::experimental::net::ip::tcp::resolver::results_type& endpoints)
    {
		if (ec)
		{
			return;
		}

		std::experimental::net::async_connect(socket_, endpoints,
			std::bind(&HttpClient::HandleConnect, this,
				std::placeholders::_1));
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

        std::shared_ptr<kit::Buffer> buffer = kit::Buffer::Create(request_string.length());
        buffer->Append(request_string.c_str(), request_string.length());

		std::experimental::net::async_write(socket_, std::experimental::net::buffer(buffer->Data(), buffer->Length()),
            std::bind(&HttpClient::HandleSendHttpRequest, shared_from_this(),
                std::placeholders::_1, std::placeholders::_2));
    }

    void HttpClient::HandleSendHttpRequest(const std::error_code& ec, size_t bytes_transferred)
    {
        RecvHttpHeader();
    }

    void HttpClient::RecvHttpHeader()
    {
        std::string delim("\r\n\r\n");
		std::experimental::net::async_read_until(socket_, 
			std::experimental::net::dynamic_buffer(response_),
            delim, std::bind(&HttpClient::HandleRecvHttpHeader, shared_from_this(),
                std::placeholders::_1, std::placeholders::_2));
    }

    void HttpClient::HandleRecvHttpHeader(const std::error_code& ec, size_t bytes_transferred)
    {
        assert(bytes_transferred <= response_.size());

        std::shared_ptr<kit::HttpResponse> http_response =
            kit::HttpResponse::ParseFromBuffer(response_);

        listener_->HandleRecvHttpResponse(http_response);
    }

    void HttpClient::RecvHttpData(unsigned int piece_size)
    {
        recv_buffer_ = kit::Buffer::Create(piece_size);

        unsigned int offset = 0;
        if (response_.size() > piece_size)
        {
			memcpy(recv_buffer_->Data(), response_.c_str(), piece_size);
			response_.erase(0, piece_size);

            HandleRecvHttpData(std::error_code(), piece_size);
        }
        else if (response_.size() > 0)
        {
            offset = response_.size();

			memcpy(recv_buffer_->Data(), response_.c_str(), offset);
            recv_buffer_->SetLength(offset);

			response_.erase(0, piece_size);
        }

		std::experimental::net::async_read(socket_, std::experimental::net::buffer(recv_buffer_->Data() + offset, recv_buffer_->RemainSize() - offset),
			std::experimental::net::transfer_at_least(recv_buffer_->RemainSize() - offset),
            std::bind(&HttpClient::HandleRecvHttpData, shared_from_this(),
                std::placeholders::_1, std::placeholders::_2));
    }

    void HttpClient::HandleRecvHttpData(const std::error_code& ec, size_t bytes_transferred)
    {
        recv_buffer_->SetLength(recv_buffer_->Length() + bytes_transferred);
        listener_->HandleRecvHttpData(ec, recv_buffer_);
    }

    void HttpClient::Close()
    {
        std::error_code ec;
        socket_.shutdown(std::experimental::net::socket_base::shutdown_both, ec);
        socket_.close(ec);

        listener_.reset();
    }
}