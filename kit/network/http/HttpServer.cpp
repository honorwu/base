#include "HttpServer.h"
#include <string>

namespace kit
{
    void HttpServer::Recv()
    {
        std::string delim("\r\n\r\n");
		std::experimental::net::async_read_until(*socket_, 
			std::experimental::net::dynamic_buffer(request_), delim,
			std::bind(&HttpServer::HandleRecv,
            shared_from_this(), std::placeholders::_1, std::placeholders::_2));
    }

    void HttpServer::HandleRecv(const std::error_code& ec, unsigned int bytes_transferred)
    {
        if (ec)
        {
            Close();
            return;
        }

        listener_->HandleRecv(shared_from_this(), request_);

        Recv();
    }

    void HttpServer::Send(std::shared_ptr<kit::Buffer> buffer)
    {
        bool is_send_list_empty = send_list_.empty();
        send_list_.push_back(buffer);

        if (is_send_list_empty)
        {
			socket_->async_send(std::experimental::net::buffer(buffer->Data(), buffer->Length()),
				std::bind(&HttpServer::HandleSend, shared_from_this(),
					std::placeholders::_1,
					std::placeholders::_2));
        }
    }

    void HttpServer::HandleSend(const std::error_code& ec, unsigned int bytes_transferred)
    {
        if (ec)
        {
            return;
        }

        send_list_.pop_front();

        if (!send_list_.empty())
        {
			std::experimental::net::async_write(*socket_, 
				std::experimental::net::buffer(send_list_.front()->Data(), send_list_.front()->Length()),
				std::experimental::net::transfer_at_least(send_list_.front()->Length()),
				std::bind(&HttpServer::HandleSend,
                shared_from_this(), std::placeholders::_1, std::placeholders::_2));
        }
        else if (close_status_ == WillClose)
        {
            Close();
        }
    }

    void HttpServer::Close()
    {
        if (close_status_ == Closed)
        {
            return;
        }

        if (send_list_.empty())
        {
            std::error_code ec;
            socket_->close(ec);
            listener_.reset();
            close_status_ = Closed;
        }
        else
        {
            close_status_ = WillClose;
        }
    }
}