#include "Algo/Vertex/VertexFinder.h"

VertexFinder::VertexFinder(const std::vector<std::shared_ptr<DTrack>> &tracks)
{
    tracks_ = tracks;
}

void VertexFinder::FindVertexes()
{
    this->BuildSpiralStaircase();
}

void VertexFinder::BuildSpiralStaircase()
{
    spiral_staircase_.clear();

    for(const auto &track : tracks_)
    {
        int stair_no = track->GetInitCellIdZ();

        if(spiral_staircase_.find(stair_no) == spiral_staircase_.end())
        {
            spiral_staircase_.emplace(stair_no, std::make_shared<Stair>(stair_no));
            spiral_staircase_.at(stair_no)->slabs_.push_back(track);
        }
        else
            spiral_staircase_.at(stair_no)->slabs_.push_back(track);
    }
/*
    for(const auto &stair : spiral_staircase_)
    {
        std::cout << stair.first << "\t";
        for(const auto &slab : stair.second->slabs_)
            std::cout << slab->GetFixedPp() << "\t";
        std::cout << std::endl;
    }
*/
}
