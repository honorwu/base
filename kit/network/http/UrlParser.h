#ifndef _KIT_URL_PARSER_H_
#define _KIT_URL_PARSER_H_

#include <string>
#include <boost/cstdint.hpp>

namespace kit
{
    class UrlParser
    {
    public:
        UrlParser(std::string url);
		std::string GetHost();
		std::string GetPort();
        std::string GetRequest();
    private:
        void Parse();
        std::string url_;

		boost::uint32_t host_begin_pos_;
		boost::uint32_t host_end_pos_;

		boost::uint32_t port_begin_pos_;
		boost::uint32_t port_end_pos_;

        boost::uint32_t request_begin_pos_;
        boost::uint32_t request_end_pos_;
    };
}

#endif