#ifndef _KIT_TCP_CLIENT_H_
#define _kit_TCP_CLIENT_H_

#include <string>
#include <deque>
#include "experimental/net"

namespace kit
{
    class Buffer;
}

namespace kit
{
    struct ITcpClientListener
    {
        virtual void HandleConnect(const std::error_code & ec) = 0;
        virtual void HandleRecv(std::shared_ptr<kit::Buffer> buffer) = 0;
        virtual ~ITcpClientListener() 
        {
        }
    };

    class TcpClient
    {
    public:
        TcpClient(std::experimental::net::io_context & io_context,
			const std::string & server,
			unsigned short port,
            std::shared_ptr<ITcpClientListener> listener);
        void Connect();
        void HandleConnect(const std::error_code & ec);

        void Resolve();
        void HandleResolve(const std::error_code & ec, 
			const std::experimental::net::ip::tcp::resolver::results_type & results_type);

        void Send(std::shared_ptr<kit::Buffer> buffer);
        void HandleSend(const std::error_code& ec, size_t bytes_transferred);

        void Recv();
        void HandleRecv(const std::error_code& ec, size_t bytes_transferred);

        void Close();

    private:
        void DoSend(std::shared_ptr<kit::Buffer> buffer);

        std::experimental::net::ip::tcp::socket socket_;
		std::experimental::net::ip::tcp::resolver resolver_;

        std::string server_;
        unsigned short port_;

        std::list<std::shared_ptr<kit::Buffer> > send_list_;
        std::shared_ptr<kit::Buffer> recv_buffer_;        

        std::shared_ptr<ITcpClientListener> listener_;
    };
}

#endif