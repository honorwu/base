#ifndef _KIT_HTTP_RESPONSE_H_
#define _KIT_HTTP_RESPONSE_H_

#include <string>
#include <boost/shared_ptr.hpp>

namespace kit
{
    class HttpResponse
    {
    public:
        static boost::shared_ptr<HttpResponse> ParseFromBuffer(std::string response);

        unsigned int GetStatusCode();
        unsigned int GetContentLength();
        std::pair<unsigned int, unsigned int> GetRange();
    private:
        explicit HttpResponse(const std::string & response)
            : status_code_(0)
            , content_length_(0)
            , range_begin_(0)
            , range_end_(0)
        {
            Pause(response);
        }

        void Pause(std::string response);
        std::string GetLineValue(const std::string & response, const std::string & key);

        unsigned int status_code_;
        unsigned int content_length_;
        unsigned int range_begin_;
        unsigned int range_end_;
    };
}

#endif
