#ifndef _KIT_URL_PARSER_H_
#define _KIT_URL_PARSER_H_

#include <string>

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

		unsigned int host_begin_pos_;
		unsigned int host_end_pos_;

		unsigned int port_begin_pos_;
		unsigned int port_end_pos_;

		unsigned int request_begin_pos_;
		unsigned int request_end_pos_;
    };
}

#endif