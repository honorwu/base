#include "HttpResponse.h"
#include <assert.h>

namespace kit
{
    std::shared_ptr<HttpResponse> HttpResponse::ParseFromBuffer(std::string response)
    {
        return std::shared_ptr<HttpResponse>(new HttpResponse(response));
    }

    void HttpResponse::Pause(std::string response)
    {
        std::string status = response.substr(9, 3);
        status_code_ = atoi(status.c_str());

        std::string content_length = GetLineValue(response, "Content-Length:");
        content_length_ = atoi(content_length.c_str());

        std::string content_range = GetLineValue(response, "Content-Range:");
        if (content_range.length() > 0)
        {
            unsigned int pos = content_range.find('-');
            if (pos == std::string::npos)
            {
                return;
            }

            range_begin_ = atoi(content_range.substr(0, pos).c_str());
        }
    }

    std::string HttpResponse::GetLineValue(const std::string & response, const std::string & key)
    {
        unsigned int pos = response.find(key);
        if (pos == std::string::npos)
        {
            return "";
        }

        std::string value = response.substr(pos);

        pos = value.find("\r\n");

        if (pos == std::string::npos)
        {
            assert(false);
            return "";
        }

        value = value.substr(0, pos);
        value = value.substr(key.length() + 1);
        return value;
    }

    unsigned int HttpResponse::GetStatusCode()
    {
        return status_code_;
    }

    unsigned int HttpResponse::GetContentLength()
    {
        return content_length_;
    }

    std::pair<unsigned int, unsigned int> HttpResponse::GetRange()
    {
        return std::make_pair(range_begin_, range_end_);
    }
}