#ifndef _PEER_HTTP_TUNNEL_H_
#define _PEER_HTTP_TUNNEL_H_

namespace peer
{
    class HttpTunnel
    {
    public:
        void Start();
        void Stop();

        void Resume();
        void Pause();
    private:
    };
}

#endif