#include "Algo/Vertex/VertexFinder.h"

VertexFinder::VertexFinder(const std::vector<std::shared_ptr<DTrack>> &tracks)
{
    tracks_ = tracks;
}

void VertexFinder::FindVertexes()
{
    this->BuildSpiralStaircase();
    for(const auto &stair : spiral_staircase_)
    {
        this->FindClusterInStair(stair.second);
    }
}

void VertexFinder::BuildSpiralStaircase()
{
    spiral_staircase_.clear();

    auto leading_track = tracks_.at(0);

    for(const auto &track : tracks_)
    {
        int stair_no = track->GetInitCellIdZ();

        double leading_x = 0.;
        double leading_y = 0.;
        double leading_z = 0.;
        auto leading_hit = leading_track->AtCellIdZ(stair_no);
        if(leading_hit)
        {
            leading_x = leading_track->AtCellIdZ(stair_no)->GetX();
            leading_y = leading_track->AtCellIdZ(stair_no)->GetY();
            leading_z = leading_track->AtCellIdZ(stair_no)->GetZ();
        }

        if(spiral_staircase_.find(stair_no) == spiral_staircase_.end())
            spiral_staircase_.emplace(stair_no, std::make_shared<Stair>(stair_no));

        spiral_staircase_.at(stair_no)->slabs_.push_back(track);
        spiral_staircase_.at(stair_no)->splits_.push_back({track->AtCellIdZ(stair_no)->GetX() - leading_x,
                                                           track->AtCellIdZ(stair_no)->GetY() - leading_y,
                                                           track->AtCellIdZ(stair_no)->GetZ() - leading_z,
                                                          }); 
    }
/*
    for(const auto &stair : spiral_staircase_)
    {
        std::cout << stair.first << "\t";
        for(const auto &split : stair.second->splits_)
            std::cout << split.at(0) << "\t" << split.at(1) << "\t" << split.at(2) << ",\t";
        std::cout << std::endl;
    }
*/
}

void VertexFinder::FindClusterInStair(const std::shared_ptr<Stair> &stair)
{
    Clusterer<DTrack> clusterer;

    for(size_t i = 0; i < stair->slabs_.size(); i++)
    {
        double *splits = new double[3];
        splits[0] = stair->splits_.at(i).at(0);
        splits[1] = stair->splits_.at(i).at(1);
        splits[2] = stair->splits_.at(i).at(2);
        clusterer.CreatePoint(stair->slabs_.at(i), 3, splits, 1.);
    }
}
