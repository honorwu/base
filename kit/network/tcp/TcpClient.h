#ifndef _KIT_TCP_CLIENT_H_
#define _kit_TCP_CLIENT_H_

#include <string>
#include <boost/asio.hpp>
#include <deque>
#include <boost/shared_ptr.hpp>

namespace kit
{
    class Buffer;
}

namespace kit
{
    struct ITcpClientListener
    {
        virtual void HandleConnect(const boost::system::error_code & ec) = 0;
        virtual void HandleRecv(boost::shared_ptr<kit::Buffer> buffer) = 0;
        virtual ~ITcpClientListener() 
        {
        }
    };

    class TcpClient
    {
    public:
        TcpClient(boost::asio::io_service & io_service, const std::string & server, boost::uint16_t port,
            boost::shared_ptr<ITcpClientListener> listener);
        void Connect();
        void HandleConnect(const boost::system::error_code & ec, boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

        void Resolve();
        void HandleResolve(const boost::system::error_code & ec, boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

        void Send(boost::shared_ptr<kit::Buffer> buffer);
        void HandleSend(const boost::system::error_code& ec, size_t bytes_transferred);

        void Recv();
        void HandleRecv(const boost::system::error_code& ec, size_t bytes_transferred);

        void Close();

    private:
        void DoSend(boost::shared_ptr<kit::Buffer> buffer);

        boost::asio::ip::tcp::socket socket_;
        boost::asio::ip::tcp::resolver resolver_;

        std::string server_;
        boost::uint16_t port_;

        std::deque<boost::shared_ptr<kit::Buffer> > send_list_;
        boost::shared_ptr<kit::Buffer> recv_buffer_;        

        boost::shared_ptr<ITcpClientListener> listener_;
    };
}

#endif