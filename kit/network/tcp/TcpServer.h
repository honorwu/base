#ifndef _KIT_TCP_SERVER_H_
#define _KIT_TCP_SERVER_H_

#include <boost/asio.hpp>
#include <deque>
#include <boost/enable_shared_from_this.hpp>
#include <boost/array.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace kit
{
    class Buffer;
}

namespace kit
{
    struct ITcpServerListener
    {
        virtual void HandleRecv(boost::shared_ptr<kit::Buffer>) = 0;
        virtual void OnClose(const boost::system::error_code& ec) = 0;
        virtual ~ITcpServerListener() 
        {
        }
    };

	class TcpServer
        : public boost::enable_shared_from_this<TcpServer>
	{
	public:
		TcpServer(boost::asio::ip::tcp::socket * socket);

        void SetHandler(boost::shared_ptr<ITcpServerListener> handler);

		void Recv();
		void HandleRecv(const boost::system::error_code& ec, size_t bytes_transferred);

        void Send(boost::shared_ptr<kit::Buffer> buffer);
		void HandleSend(const boost::system::error_code& ec, size_t bytes_transferred);

        void Close();

        boost::uint32_t GetIp() const;
        boost::uint16_t GetPort() const;

		boost::uint32_t GetRemoteIP() const;
	private:

        void DoSend(boost::shared_ptr<kit::Buffer> buffer);

		boost::asio::ip::tcp::socket * socket_;
        boost::shared_ptr<kit::Buffer> recv_buffer_;
        std::deque<boost::shared_ptr<kit::Buffer> > send_list_;
        boost::shared_ptr<ITcpServerListener> handler_;
	};
}

#endif