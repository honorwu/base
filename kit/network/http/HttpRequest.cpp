#include "kit/network/http/HttpRequest.h"

namespace kit
{
    HttpRequest::HttpRequest(const std::string & http_request)
        : http_request_(http_request)
    {
        Parse();
    }

    void HttpRequest::Parse()
    {
        size_t pos = http_request_.find(' ');
        path_ = http_request_.substr(pos+1);

        pos = path_.find(' ');
        path_ = path_.substr(0, pos);

        pos = path_.find('?');

        if (pos == std::string::npos)
        {
            return;
        }

        std::string path = path_.substr(pos + 1);

        do 
        {
            pos = path.find('&');
            std::string line = path.substr(0, pos);
            
            if (pos == std::string::npos)
            {
                path.clear();
            }
            else
            {
                path = path.substr(pos + 1);
            }
            
            size_t and_pos = line.find('=');
            if (and_pos != std::string::npos)
            {
                url_parameters_[line.substr(0, and_pos)] = line.substr(and_pos + 1);
            }

        } while (!path.empty());
    }

    void HttpRequest::GetParameterInURL(const std::string & key, std::string & value)
    {
        if (path_.empty())
        {
            Parse();
        }

        std::map<std::string, std::string>::const_iterator iter = url_parameters_.find(key);
        if (iter != url_parameters_.end())
        {
            value = iter->second;
        }
    }
}