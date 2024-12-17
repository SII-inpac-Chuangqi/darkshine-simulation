//................................................................................//
//C++
#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <cmath>

//................................................................................//
//ROOT
#include "TMath.h"
#include "TGeoBBox.h"
#include "TRandom.h"

//................................................................................//
//Framework
#include "Core/AnaData.h"

//................................................................................//
//Tracking
#include "Algo/TrkHit.h"
#include "Algo/Digitization.h"
#include "Algo/Utils/Clusterer.h"

void Digitization::SetIfSmear(bool if_smear)
{
    if_smear_ = if_smear;
    if(if_smear_) std::cerr << "[INFO] ==> Hits smeared in tracking" << std::endl;
}

//Get world node from TGeoManager
void Digitization::ReadTrackerInfo(bool if_strip)
{
    if_strip_ = if_strip;

    if(!if_strip_)
    {
        std::cout << "[WARNING] ==> Strip model not loaded in tracking" << std::endl;
        return;
    }

    strip_no_tag_.clear();
    layer_width_tag_.clear();
    layer_length_tag_.clear();
    angles_tag_.clear();

    strip_no_rec_.clear();
    layer_width_rec_.clear();
    layer_length_rec_.clear();
    angles_rec_.clear();

    strip_no_tag_     = dAnaData->getStripNoTag();
    layer_width_tag_  = dAnaData->getLayerWidthTag();
    layer_length_tag_ = dAnaData->getLayerLengthTag();
    angles_tag_       = dAnaData->getAnglesTag();

    strip_no_rec_     = dAnaData->getStripNoRec();
    layer_width_rec_  = dAnaData->getLayerWidthRec();
    layer_length_rec_ = dAnaData->getLayerLengthRec();
    angles_rec_       = dAnaData->getAnglesRec();

    std::cout << "[INFO] ==> Strip model loaded in tracking" << std::endl;
}

//Separate tracker hits into vectors by layers
void Digitization::Layering(const std::vector<TrkHit> &trk1_hits, const std::vector<TrkHit> &trk2_hits,
                            Pool *pool,
                            int detector)
{
    std::function<Key(const TrkHit&)> trk_hit_getter = &TrkHit::GetCellIdZ;
    pool->SetKeyGetter(trk_hit_getter);

    if(!if_strip_)
    {
        for(const auto &it_trkhit : trk2_hits)
            pool->AddHit(it_trkhit);

        return;
    }
    
    std::vector<double> *layer_widths  = (detector == tracking::dTag) ? &layer_width_tag_  : &layer_width_rec_;
    std::vector<double> *layer_lengths = (detector == tracking::dTag) ? &layer_length_tag_ : &layer_length_rec_;
    std::vector<int>    *strip_nos = (detector == tracking::dTag) ? &strip_no_tag_ : &strip_no_rec_;
    std::vector<double> *angles    = (detector == tracking::dTag) ? &angles_tag_   : &angles_rec_;
 
    TrkHitSPVec clustered_trk1_hits;
    TrkHitSPVec clustered_trk2_hits;
    for(const auto &hit : trk1_hits) clustered_trk1_hits.push_back(std::make_shared<TrkHit>(hit));
    for(const auto &hit : trk2_hits) clustered_trk2_hits.push_back(std::make_shared<TrkHit>(hit));

    TrkHitSPVecMap map1;
    this->InitHitMap(clustered_trk1_hits, map1);
    TrkHitSPVecMap map2;
    this->InitHitMap(clustered_trk2_hits, map2);
   
    for(auto layer1 : map1)
    {
        auto it_find_layer2 = map2.find(layer1.first);
        if(it_find_layer2 == map2.end())
            continue;

        double angle        = angles->at((layer1.first - 1)*2 + 1);
        double strip_no     = strip_nos->at((layer1.first - 1)*2 + 1);
        double layer_width  = layer_widths->at((layer1.first - 1)*2 + 1);
        double layer_length = layer_lengths->at((layer1.first - 1)*2 + 1); 
        //std::cout << strip_no << std::endl;
    
        auto layer2 = map2.at(layer1.first);
        for(auto hit1 : layer1.second)
        {
            for(auto hit2 : layer2)
            {
                double smear1 = 0.;
                double smear2 = 0.;
                if(if_smear_)
                {
                    smear1 = rnd_.Uniform(layer_width/strip_no) - 0.5*layer_width/strip_no;
                    smear2 = rnd_.Uniform(layer_width/strip_no) - 0.5*layer_width/strip_no;
                }
                //double x1 = hit1->GetX() + smear1;
                //double y1 = -x1/tan(angle) + ((hit2->GetCellIdX() - 0.5*(strip_no + 1))*layer_width/strip_no + smear2)/sin(angle);
                double x1 = hit1->GetX() + smear1;
                double x2 = hit2->GetX() + smear2;
                double y1 = -x1/tan(angle) + x2/sin(angle);
    
                if(!(std::abs(y1) < 0.5*layer_length))
                    continue;

                auto hit = std::make_shared<TrkHit>();
                hit->SetCellIdZ(hit1->GetCellIdZ());
                pool->AddHit(std::move(hit));
                pool->Back()->SetX(x1);
                pool->Back()->SetZ(hit1->GetZ());
                pool->Back()->SetY(y1);
            }
        }
    }

    //std::cout << std::endl;
    //pool->Print();
}

void Digitization::InitHitMap(const TrkHitSPVec &trk_hits, TrkHitSPVecMap &trk_hit_map)
{
    TrkHitSPVecMap unclustered_trk_hit_map;
    for(const auto &trk_hit : trk_hits)
        this->InsertHitMap(trk_hit, unclustered_trk_hit_map);

    for(auto &layer : unclustered_trk_hit_map)
    {
        Clusterer<TrkHitSP> clusterer;
        clusterer.SetClusterWidth(cluster_width_);

        for(auto &hit : layer.second)
        {
            double *splits = new double[1];
            splits[0] = hit->GetX();
            clusterer.CreatePoint(&hit, 1, splits, hit->GetE());

            delete[] splits; splits = nullptr;
        }

        //clusterer.ShowPoints();
        clusterer.FindClusters();

        TrkHitSPVec clustered_layer;
        auto n_hits = clusterer.GetNClusters();
        for(size_t i = 0; i < n_hits; i++)
        {
            clustered_layer.push_back(std::make_shared<TrkHit>());
            clustered_layer.back()->SetX(clusterer.GetClusterCenterSplits(i).at(0));
            clustered_layer.back()->SetZ(layer.second.at(0)->GetZ());
            clustered_layer.back()->SetCellIdZ(layer.first);
        }
        trk_hit_map.insert(std::pair<int, TrkHitSPVec>(layer.first, clustered_layer));
    }
    //std::cout << std::endl;
}

void Digitization::InsertHitMap(const TrkHitSP &trk_hit, TrkHitSPVecMap &trk_hit_map)
{
    auto it_search_map = trk_hit_map.find(trk_hit->GetCellIdZ());
    if(it_search_map != trk_hit_map.end())
        it_search_map->second.emplace_back(trk_hit);
    else
        trk_hit_map.insert(std::pair<int, TrkHitSPVec>(trk_hit->GetCellIdZ(), {trk_hit}));
}
