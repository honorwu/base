#include "HttpServer.h"
#include <string>
#include <boost/bind.hpp>

namespace kit
{
    void HttpServer::Recv()
    {
        std::string delim("\r\n\r\n");
        boost::asio::async_read_until(*socket_, request_, delim, boost::bind(&HttpServer::HandleRecv,
            shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }

    void HttpServer::HandleRecv(const boost::system::error_code& ec, boost::uint32_t bytes_transferred)
    {
        if (ec)
        {
            Close();
            return;
        }

        std::string request_string;
        std::copy(std::istreambuf_iterator<char>(&request_), std::istreambuf_iterator<char>(), std::back_inserter(request_string));

        listener_->HandleRecv(shared_from_this(), request_string);

        Recv();
    }

    void HttpServer::Send(boost::shared_ptr<kit::Buffer> buffer)
    {
        bool is_send_list_empty = send_list_.empty();
        send_list_.push_back(buffer);

        if (is_send_list_empty)
        {
            boost::asio::async_write(
                *socket_,
                boost::asio::buffer(buffer->Data(), buffer->Length()),
                boost::asio::transfer_at_least(buffer->Length()),
                boost::bind(&HttpServer::HandleSend, shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
        }
    }

    void HttpServer::HandleSend(const boost::system::error_code& ec, boost::uint32_t bytes_transferred)
    {
        if (ec)
        {
            return;
        }

        send_list_.pop_front();

        if (!send_list_.empty())
        {
            boost::asio::async_write(*socket_, boost::asio::buffer(send_list_.front()->Data(), send_list_.front()->Length()),
                boost::asio::transfer_at_least(send_list_.front()->Length()), boost::bind(&HttpServer::HandleSend,
                shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
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
            boost::system::error_code ec;
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