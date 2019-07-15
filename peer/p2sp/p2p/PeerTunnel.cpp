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
            const PieceInfo & p = *(task_set_.begin());
            piece_timing_map_[p].Restart();

            task_set_.erase(p);
            requesting_count_++;
        }
    }

    void PeerTunnel::OnP2PTimer(unsigned int times)
    {
        CheckPieceTimeout();

        unsigned int last_window_size = window_size_;
        window_size_ = byte_speed_meter_.SecondByteSpeed() / 1024;

        if (window_size_ < last_window_size)
        {
            window_size_ = (9 * last_window_size + 1 * window_size_) / 10;
        }
    }

    void PeerTunnel::CheckPieceTimeout()
    {
        for (std::map<PieceInfo, kit::TickCounter>::iterator iter = piece_timing_map_.begin();
            iter != piece_timing_map_.end(); )
        {
            if (iter->second.Elapsed() > max_rtt_ + 1000)
            {
                OnPieceTimeout(iter->first);
                piece_timing_map_.erase(iter++);
            }
            else
            {
                ++iter;
            }
        }
    }

    void PeerTunnel::UpdateRTT(const PieceInfo & piece)
    {
        unsigned int rtt = piece_timing_map_[piece].Elapsed() * 1000;
        piece_timing_map_.erase(piece);

        if (rtt > max_rtt_)
        {
            max_rtt_ = rtt;
        }

        average_rtt_ = (average_rtt_ * 9 + rtt) / 10;
    }

    void PeerTunnel::OnPieceReceved(const PieceInfo & piece, unsigned int piece_size_in_byte)
    {
        UpdateRTT(piece);

        byte_speed_meter_.SubmitBytes(piece_size_in_byte);

        requesting_count_--;
        RequestAllPiece();
    }

    void PeerTunnel::OnPieceTimeout(const PieceInfo & piece)
    {
        requesting_count_--;
        RequestAllPiece();
    }
}