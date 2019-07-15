#ifndef _KIT_TCP_SERVER_H_
#define _KIT_TCP_SERVER_H_

#include <list>
#include <memory>
#include "experimental/net"

namespace kit
{
    class Buffer;
}

namespace kit
{
    struct ITcpServerListener
    {
        virtual void HandleRecv(std::shared_ptr<kit::Buffer>) = 0;
        virtual void OnClose(const std::error_code& ec) = 0;
        virtual ~ITcpServerListener() 
        {
        }
    };

	class TcpServer
        : public std::enable_shared_from_this<TcpServer>
	{
	public:
		TcpServer(std::experimental::net::ip::tcp::socket * socket);

        void SetHandler(std::shared_ptr<ITcpServerListener> handler);

		void Recv();
		void HandleRecv(const std::error_code& ec, size_t bytes_transferred);

        void Send(std::shared_ptr<kit::Buffer> buffer);
		void HandleSend(const std::error_code& ec, size_t bytes_transferred);

        void Close();

        unsigned int GetIp() const;
        unsigned short GetPort() const;

		unsigned int GetRemoteIP() const;
	private:

        void DoSend(std::shared_ptr<kit::Buffer> buffer);

		std::experimental::net::ip::tcp::socket * socket_;
        std::shared_ptr<kit::Buffer> recv_buffer_;
        std::list<std::shared_ptr<kit::Buffer> > send_list_;
        std::shared_ptr<ITcpServerListener> handler_;
	};
}

#endif