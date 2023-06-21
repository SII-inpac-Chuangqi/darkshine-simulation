#include "Algo/Vertex/VertexFinder.h"

VertexFinder::VertexFinder(std::vector<std::shared_ptr<DTrack>> *tracks)
{
    tracks_ = tracks;
    for(const auto &track : *tracks_)
        std::cout << track.get() << "\t" << track->GetChi2() << "\t" << track->GetChi2Algo() << "\t" << track->GetPp() << std::endl;
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

    std::cout << "good tracks: " << std::endl;
    for(const auto &track : *tracks_)
    {
        //if(track == leading_track) continue;
        if(track->GetChi2Algo() > 3000. && track->GetChi2() > 30.) continue;

        std::cout << track.get() << "\t" << track->GetChi2() << "\t" << track->GetChi2Algo() << "\t" << track->GetPp() << std::endl;

        int stair_no = track->GetInitCellIdZ();

        if(spiral_staircase_.find(stair_no) == spiral_staircase_.end())
        {
            spiral_staircase_.emplace(stair_no, std::make_shared<Stair>(stair_no));
            spiral_staircase_.at(stair_no)->stair_height_ = track->AtCellIdZ(stair_no)->GetZ();
        }

        spiral_staircase_.at(stair_no)->slabs_.push_back(track);
        spiral_staircase_.at(stair_no)->splits_.push_back({track->AtCellIdZ(stair_no)->GetX(),
                                                           track->AtCellIdZ(stair_no)->GetY(),
                                                           track->AtCellIdZ(stair_no)->GetZ(),
                                                          }); 
    }

    std::cout << spiral_staircase_.size() << std::endl;
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
    clusterer.SetClusterWidth(0.5);

    for(size_t i = 0; i < stair->slabs_.size(); i++)
    {
        double *splits = new double[3];
        splits[0] = stair->splits_.at(i).at(0);
        splits[1] = stair->splits_.at(i).at(1);
        splits[2] = stair->splits_.at(i).at(2);
        clusterer.CreatePoint(&stair->slabs_.at(i), 1, splits, 1.);

        delete[] splits; splits = nullptr;
    }

    //clusterer.ShowPoints();
    clusterer.FindClusters();

    auto n_cluster = clusterer.GetNClusters();
    for(size_t i = 0; i < n_cluster; i++)
    {
        std::cout << "vertex " << i << std::endl;
/*
        if(clusterer.GetClusterSize(i) == 1)
        {
            if(clusterer.GetListOfClusteredObjects(i).at(0)->get() == tracks_->at(0).get())
                continue;
        }
*/

        auto clustered_tracks = clusterer.GetListOfClusteredObjects(i);
        for(const auto &track : clustered_tracks)
            std::cout << (*track).get() << "\t" << (*track)->GetChi2() << "\t" << (*track)->GetPp() << std::endl;

        vertexes_->push_back(std::make_shared<DVertex>());
        vertexes_->back()->SetZ(stair->stair_height_);
    }
}
