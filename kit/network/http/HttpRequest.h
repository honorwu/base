#ifndef _KIT_HTTPREQUEST_H_
#define _KIT_HTTPREQUEST_H_

#include <string>
#include <map>

namespace kit
{
    class HttpRequest
    {
    public:
        explicit HttpRequest(const std::string & http_request);
        void GetParameterInURL(const std::string & key, std::string & value);
    private:
        void Parse();
        std::string http_request_;
        std::string path_;
        std::string host_;
        std::map<std::string, std::string> url_parameters_;
    };
}

#endif