#include "UrlParser.h"

namespace kit
{
    UrlParser::UrlParser(std::string url)
        : url_(url)
    {
        Parse();
    }

    void UrlParser::Parse()
    {
		unsigned int begin_pos = 0, end_pos = 0;

		begin_pos = url_.find(':', 0);
		end_pos = url_.find('/', 0);
		if (begin_pos != std::string::npos && begin_pos < end_pos && begin_pos < url_.find('.', 0))
		{
			begin_pos = url_.find('/', end_pos + 1) + 1;
			host_begin_pos_ = begin_pos;
		}
		else
		{
			host_begin_pos_ = 0;
			begin_pos = host_begin_pos_;
		}

		std::string s = url_.substr(0, url_.find('/', begin_pos));
		end_pos = s.find(':', begin_pos);
		if (end_pos != std::string::npos)
		{
			port_begin_pos_ = end_pos + 1;
			begin_pos = end_pos;
		}

		end_pos = url_.find('/', begin_pos);
		if (end_pos != std::string::npos)
		{
            if (port_begin_pos_ != std::string::npos)
            {
                port_end_pos_ = end_pos;
            }
            request_begin_pos_ = end_pos;
            request_end_pos_ = url_.length();
			host_end_pos_ = end_pos;
		}
		else
		{
			url_ = url_ + '/';
			end_pos = url_.find('/', begin_pos);

			if (port_begin_pos_ != std::string::npos)
            {
				port_end_pos_ = end_pos;
            }

			host_end_pos_ = end_pos;
		}
    }

	std::string UrlParser::GetHost()
	{
		if (host_begin_pos_ != std::string::npos)
		{
			return url_.substr(host_begin_pos_, host_end_pos_ - host_begin_pos_);
		}
		else
		{
			return "";
		}
	}

	std::string UrlParser::GetPort()
	{
		if (port_begin_pos_ != std::string::npos)
		{
			return url_.substr(port_begin_pos_, port_end_pos_ - port_begin_pos_);
		}
		else
		{
			return "";
		}
	}

    std::string UrlParser::GetRequest()
    {
        if (request_begin_pos_ != std::string::npos)
        {
            return url_.substr(request_begin_pos_, request_end_pos_ - request_begin_pos_);
        }
        else
        {
            return "";
        }
    }
}