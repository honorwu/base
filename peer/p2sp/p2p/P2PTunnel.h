#ifndef _PEER_P2P_TUNNEL_H_
#define _PEER_P2P_TUNNEL_H_

#include <string>

namespace peer
{
    class P2PTunnel
    {
    public:
        void Start();
        void Stop();

        void Resume();
        void Pause();
    private:
        std::string url_;
    };
}

#endif