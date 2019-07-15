#include "kit/network/tcp/TcpServer.h"
#include "kit/buffer/Buffer.h"

namespace kit
{
    static const unsigned int BUFFER_SIZE = 1024;

	TcpServer::TcpServer(std::experimental::net::ip::tcp::socket * socket)
		: socket_(socket)
	{
	}

    void TcpServer::SetHandler(std::shared_ptr<ITcpServerListener> handler)
    {
        handler_ = handler;
    }

	void TcpServer::Recv()
	{
        if (handler_)
        {
            recv_buffer_ = kit::Buffer::Create(BUFFER_SIZE);
            socket_->async_read_some(
				std::experimental::net::buffer(recv_buffer_->Data(), recv_buffer_->RemainSize()),
				std::bind(&TcpServer::HandleRecv, 
                shared_from_this(), std::placeholders::_1, std::placeholders::_2));
        }
	}

    void TcpServer::HandleRecv(const std::error_code& ec, size_t bytes_transferred)
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

	void TcpServer::Send(std::shared_ptr<kit::Buffer> buffer)
	{
		bool is_send_list_empty = send_list_.empty();
		send_list_.push_back(buffer);

		if (is_send_list_empty)
		{
            DoSend(buffer);
		}
	}

    void TcpServer::DoSend(std::shared_ptr<kit::Buffer> buffer)
    {
		socket_->async_send(std::experimental::net::buffer(buffer->Data(), buffer->Length()),
			std::bind(&TcpServer::HandleSend, shared_from_this(),
				std::placeholders::_1,
				std::placeholders::_2));
    }

	void TcpServer::HandleSend(const std::error_code& ec, size_t bytes_transferred)
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

        std::error_code ec;
        socket_->shutdown(std::experimental::net::socket_base::shutdown_both, ec);
        socket_->close(ec);
    }

    unsigned int TcpServer::GetIp() const
    {
        return socket_->local_endpoint().address().to_v4().to_uint();
    }

    unsigned short TcpServer::GetPort() const
    {
        return socket_->local_endpoint().port();
    }

	unsigned int TcpServer::GetRemoteIP() const
	{
		return socket_->remote_endpoint().address().to_v4().to_uint();
	}
}