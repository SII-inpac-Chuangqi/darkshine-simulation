//................................................................................//
//CPP STL
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

//Get world node from TGeoManager
void Digitization::GetTrackerInfo(bool if_strip)
{
    if_strip_ = if_strip;

    if(if_strip_)
    {
        angles_tag_.clear();
        angles_rec_.clear();

        strip_no_tag_     = dAnaData->getStripNoTag();
        layer_width_tag_  = dAnaData->getStripWidthTag();
        layer_length_tag_ = dAnaData->getStripLengthTag();
        for(auto angle : dAnaData->getAnglesTag()) angles_tag_.push_back(angle);

        strip_no_rec_     = dAnaData->getStripNoRec();
        layer_width_rec_  = dAnaData->getStripWidthRec();
        layer_length_rec_ = dAnaData->getStripLengthRec();
        for(auto angle : dAnaData->getAnglesRec()) angles_rec_.push_back(angle);

        std::cout << "[INFO] ==> Strip model loaded in tracking" << std::endl;
    }
    else
        std::cout << "[WARNING] ==> Strip model not loaded in tracking" << std::endl;
}

//Separate tracker hits into vectors by layers
void Digitization::Layering(const std::vector<TrkHit> &trk1_hits, const std::vector<TrkHit> &trk2_hits, TrkHitPVecMap &clus_trkhit_map,
                            int detector)
{
    if(if_strip_)
    {
        TrkHitPVecMap map1;
        TrkHitPVecMap map2;
    
        for(auto it_trkhit : trk1_hits)
        {
            auto it_search_map = map1.find(it_trkhit.GetCellIdZ());
            if(it_search_map != map1.end())
                it_search_map->second.emplace_back(std::make_shared<TrkHit>(it_trkhit));
            else
            {
                TrkHitPVec temp_new_layer;
                temp_new_layer.emplace_back(std::make_shared<TrkHit>(it_trkhit));
                map1.insert(std::pair(it_trkhit.GetCellIdZ(), temp_new_layer));
            }
        }
    
        for(auto it_trkhit : trk2_hits)
        {
            auto it_search_map = map2.find(it_trkhit.GetCellIdZ());
            if(it_search_map != map2.end())
                it_search_map->second.emplace_back(std::make_shared<TrkHit>(it_trkhit));
            else
            {
                TrkHitPVec temp_new_layer;
                temp_new_layer.emplace_back(std::make_shared<TrkHit>(it_trkhit));
                map2.insert(std::pair(it_trkhit.GetCellIdZ(), temp_new_layer));
            }
        }
    
        double layer_width = (detector == tag) ? layer_width_tag_ : layer_width_rec_;
        double layer_length = (detector == tag) ? layer_length_tag_ : layer_length_rec_;
        int strip_no = (detector == tag) ? strip_no_tag_ : strip_no_rec_;
        std::vector<double> *angles = (detector == tag) ? &angles_tag_ : &angles_rec_;
    
        for(auto layer1 : map1)
        {
            auto it_find_layer2 = map2.find(layer1.first);
            if(it_find_layer2 != map2.end())
            {
                double angle = angles->at((layer1.first - 1)*2 + 1);
    
                auto layer2 = map2.at(layer1.first);
                for(auto hit1 : layer1.second)
                {
                    for(auto hit2 : layer2)
                    {
                        double smear1 = rnd_.Uniform(layer_width/strip_no) - 0.5*layer_width/strip_no;
                        double smear2 = rnd_.Uniform(layer_width/strip_no) - 0.5*layer_width/strip_no;
                        double x1 = hit1->GetX() + smear1;
                        double y1 = -x1/tan(angle) + ((hit2->GetCellIdX() - 0.5*(strip_no + 1))*layer_width/strip_no + smear2)/sin(angle);
    
                        if(std::abs(y1) < 0.5*layer_length)
                        {
                            TrkHit constructed_hit;
                            constructed_hit.SetX(x1);
                            constructed_hit.SetZ(hit1->GetZ());
                            constructed_hit.SetY(y1);
    
                            auto it_search_map = clus_trkhit_map.find(hit1->GetCellIdZ());
                            if(it_search_map != clus_trkhit_map.end())
                                it_search_map->second.emplace_back(std::make_shared<TrkHit>(constructed_hit));
                            else
                            {
                                TrkHitPVec temp_new_layer;
                                temp_new_layer.emplace_back(std::make_shared<TrkHit>(constructed_hit));
                                clus_trkhit_map.insert(std::pair(hit1->GetCellIdZ(), temp_new_layer));
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        for(auto it_trkhit : trk2_hits)
        {
            auto it_search_map = clus_trkhit_map.find(it_trkhit.GetCellIdZ());
            if(it_search_map != clus_trkhit_map.end())
                it_search_map->second.emplace_back(std::make_shared<TrkHit>(it_trkhit));
            else
            {
                TrkHitPVec temp_new_layer;
                temp_new_layer.emplace_back(std::make_shared<TrkHit>(it_trkhit));
                clus_trkhit_map.insert(std::pair(it_trkhit.GetCellIdZ(), temp_new_layer));
            }
        }
    }

    for(auto &layer : clus_trkhit_map)
    {
        for(auto &hit : layer.second)
        {
            hit->SetU(hit->GetX());
            hit->SetV(hit->GetY());
        }
    }
/*
    for(auto layer : clus_trkhit_map)
    {
        for(auto hit : layer.second)
            std::cout << hit->GetX() << "	";
        std::cout << std::endl;
    }
*/
}
