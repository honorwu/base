#include "PeerTunnel.h"

namespace peer
{
    void PeerTunnel::Start()
    {

    }

    void PeerTunnel::Stop()
    {

    }

    void PeerTunnel::AssignPiece(const PieceInfo & piece)
    {
        task_set_.insert(piece);
    }

    void PeerTunnel::RequestAllPiece()
    {
        while (requesting_count_ < window_size_ && !task_set_.empty())
        {
            // request piece
            task_set_.erase(task_set_.begin());
            requesting_count_++;
        }
    }

    void PeerTunnel::OnPieceReceved(const PieceInfo & piece)
    {
        requesting_count_--;
        RequestAllPiece();
    }

    void PeerTunnel::OnPieceTimeout(const PieceInfo & piece)
    {
        requesting_count_--;
        RequestAllPiece();
    }
}