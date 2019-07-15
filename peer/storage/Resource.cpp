#include "Resource.h"
#include "../p2sp/Downloader.h"
#include "Chunk.h"

namespace peer
{
    void Resource::AttachDownloader(std::shared_ptr<Downloader> downloader)
    {
        downloaders_.insert(downloader);
    }

    void Resource::DetachDownloader(std::shared_ptr<Downloader> downloader)
    {
        downloaders_.erase(downloader);
    }

    void Resource::AddPiece(const PieceInfo & piece, std::shared_ptr<kit::Buffer> buffer)
    {
        if (!chunks_[piece.chunk_id_])
        {
            chunks_[piece.chunk_id_].reset(new Chunk(piece.chunk_id_));
        }

        chunks_[piece.chunk_id_]->SetPiece(piece.piece_id_, buffer);

        PushHttpResponseData();
    }

    void Resource::PushHttpResponseData()
    {
        for (std::set<std::shared_ptr<Downloader> >::iterator iter = downloaders_.begin();
            iter != downloaders_.end(); ++iter)
        {
            PieceInfo & push_position = (*iter)->GetPushPosition();

            std::shared_ptr<Chunk> chunk = chunks_[push_position.chunk_id_];

            for (unsigned int i = push_position.piece_id_; i < chunk->GetChunkSize(); i++)
            {
                std::shared_ptr<kit::Buffer> buffer = chunk->GetPiece(i);
                if (!buffer)
                {
                    (*iter)->OnRecvPieceBuffer(buffer);
                }
                else
                {
                    break;
                }
            }
        }
        
    }
}