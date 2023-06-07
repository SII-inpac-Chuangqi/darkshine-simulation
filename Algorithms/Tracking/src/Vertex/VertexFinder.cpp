#include "Algo/Vertex/VertexFinder.h"

VertexFinder::VertexFinder(std::vector<std::shared_ptr<DTrack>> *tracks)
{
    tracks_ = tracks;
}

void VertexFinder::FindVertexes(std::vector<std::shared_ptr<DVertex>> *vertexes)
{
    vertexes_ = vertexes;
    vertexes_->clear();

    this->BuildSpiralStaircase();
    for(const auto &stair : spiral_staircase_)
        this->FindClusterInStair(stair.second);

//    for(const auto &vertex : *vertexes)
//        std::cout << vertex->GetZ() << std::endl;
}

void VertexFinder::BuildSpiralStaircase()
{
    spiral_staircase_.clear();

    auto leading_track = tracks_->at(0);

    for(const auto &track : *tracks_)
    {
        //if(track == leading_track) continue;

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
        {
            spiral_staircase_.emplace(stair_no, std::make_shared<Stair>(stair_no));
            spiral_staircase_.at(stair_no)->stair_height_ = track->AtCellIdZ(stair_no)->GetZ();
        }

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
    Clusterer<std::shared_ptr<DTrack>> clusterer;
    clusterer.SetClusterWidth(20.);

    for(size_t i = 0; i < stair->slabs_.size(); i++)
    {
        double *splits = new double[3];
        splits[0] = stair->splits_.at(i).at(0);
        splits[1] = stair->splits_.at(i).at(1);
        splits[2] = stair->splits_.at(i).at(2);
        clusterer.CreatePoint(&stair->slabs_.at(i), 2, splits, 1.);

        delete[] splits; splits = nullptr;
    }

    //clusterer.ShowPoints();
    clusterer.FindClusters();

    auto n_cluster = clusterer.GetNClusters();
    for(size_t i = 0; i < n_cluster; i++)
    {
        if(clusterer.GetClusterSize(i) == 1)
        {
            if(*clusterer.GetListOfClusteredObjects(i).at(0) == tracks_->at(0))
                continue;
        }

        //auto splits = clusterer.GetClusterCenterSplits(i);
        //std::cout << splits.size() << std::endl;

        auto clustered_tracks = clusterer.GetListOfClusteredObjects(i);
        //for(const auto &track : clustered_tracks)
        //    std::cout << (*track).get() << std::endl;

        vertexes_->push_back(std::make_shared<DVertex>());
        vertexes_->back()->SetZ(stair->stair_height_);
    }
}
