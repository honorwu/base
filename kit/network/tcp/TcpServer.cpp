#include "kit/network/tcp/TcpServer.h"
#include "kit/buffer/Buffer.h"
#include "kit/log/LoggingModule.h"

#include <boost/bind.hpp>

namespace kit
{
    static const unsigned int BUFFER_SIZE = 1024;

	TcpServer::TcpServer(boost::asio::ip::tcp::socket * socket)
		: socket_(socket)
	{
	}

    void TcpServer::SetHandler(boost::shared_ptr<ITcpServerListener> handler)
    {
        handler_ = handler;
    }

	void TcpServer::Recv()
	{
        if (handler_)
        {
            recv_buffer_ = kit::Buffer::Create(BUFFER_SIZE);
            socket_->async_read_some(boost::asio::buffer(recv_buffer_->Data(), recv_buffer_->RemainSize()), boost::bind(&TcpServer::HandleRecv, 
                shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        }
	}

    void TcpServer::HandleRecv(const boost::system::error_code& ec, size_t bytes_transferred)
    {
        if (ec)
        {
            if (handler_)
            {
                handler_->OnClose(ec);
            }

			return;
		}

        recv_buffer_->SetLength(bytes_transferred);

        handler_->HandleRecv(recv_buffer_);

		Recv();
	}

	void TcpServer::Send(boost::shared_ptr<kit::Buffer> buffer)
	{
		bool is_send_list_empty = send_list_.empty();
		send_list_.push_back(buffer);

		if (is_send_list_empty)
		{
            DoSend(buffer);
		}
	}

    void TcpServer::DoSend(boost::shared_ptr<kit::Buffer> buffer)
    {
        boost::asio::async_write(*socket_, boost::asio::buffer(buffer->Data(), buffer->Length()),
                boost::bind(&TcpServer::HandleSend, shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
    }

	void TcpServer::HandleSend(const boost::system::error_code& ec, size_t bytes_transferred)
	{
		if (ec)
		{
			return;
		}

		send_list_.pop_front();

		if (!send_list_.empty())
		{
			DoSend(send_list_.front());
		}
	}

    void TcpServer::Close()
    {
        handler_.reset();

        boost::system::error_code ec;
        socket_->shutdown(boost::asio::socket_base::shutdown_both, ec);
        socket_->close(ec);
    }

    boost::uint32_t TcpServer::GetIp() const
    {
        return socket_->local_endpoint().address().to_v4().to_ulong();
    }

    boost::uint16_t TcpServer::GetPort() const
    {
        return socket_->local_endpoint().port();
    }

	boost::uint32_t TcpServer::GetRemoteIP() const
	{
		return socket_->remote_endpoint().address().to_v4().to_ulong();
	}
}