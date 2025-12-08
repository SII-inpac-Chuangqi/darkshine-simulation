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
#include "TVectorD.h"
#include "TMatrixDSym.h"

//................................................................................//
//Framework
#include "Core/AnaData.h"

//................................................................................//
//Tracking
#include "Algo/Digitization.h"
#include "Algo/Utils/Clusterer.h"

// Utillity function, determine whether a point (x,y) is inside a rectangle with length a, width b, and rotated by thta
bool in_rectangle(double x, double y, double a, double b, double theta)
{
    double x_prime = x * cos(theta) + y * sin(theta);
    double y_prime = -x * sin(theta) + y * cos(theta);

    return abs(x_prime) < 0.5 * a && abs(y_prime) < 0.5 * b;
}

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
        {
//            std::cout << "layer " << layer1.first << " from layer 1 in layer 2 not found" << std::endl;
            continue;
        }

        double angle1       = angles->at((layer1.first - 1)*2);
        double angle2       = angles->at((layer1.first - 1)*2 + 1);
        double strip_no     = strip_nos->at((layer1.first - 1)*2 + 1);
        double layer_width  = layer_widths->at((layer1.first - 1)*2 + 1);
        double layer_length = layer_lengths->at((layer1.first - 1)*2 + 1); 

        double u_err_2 = layer_width*layer_width / strip_no / strip_no / 12.;

        auto layer2 = map2.at(layer1.first);
        for(const auto &hit1 : layer1.second)
        {
            for(const auto &hit2 : layer2)
            {
                double smear1 = 0.;
                double smear2 = 0.;
                if(if_smear_)
                {
                    smear1 = rnd_.Uniform(layer_width/strip_no) - 0.5*layer_width/strip_no;
                    smear2 = rnd_.Uniform(layer_width/strip_no) - 0.5*layer_width/strip_no;
                }
                double x1 = hit1->GetX() + smear1;
                double y1 = hit1->GetY();
                double x2 = hit2->GetX() + smear2;
                double y2 = hit2->GetY();
                
                double u = x1 * cos(angle1) - y1 * sin(angle1);
                double v = x2 * cos(angle2) - y2 * sin(angle2);

                TVectorD u_vec(2), v_vec(2);
                TMatrixDSym uv_cov(2);

                u_vec[0] = cos(angle1);
                u_vec[1] = -sin(angle1);
                v_vec[0] = cos(angle2);
                v_vec[1] = -sin(angle2);

                uv_cov(0, 0) = u_err_2;
                uv_cov(1, 1) = u_err_2;
                uv_cov(0, 1) = 0.;
                uv_cov(1, 0) = 0.;
                
                auto hit = std::make_shared<TrkHit>();
                hit->SetCellIdZ(hit1->GetCellIdZ());
                hit->SetZ((hit1->GetZ() + hit2->GetZ()) / 2.);
                hit->SetZErr(abs(hit1->GetZ() - hit2->GetZ()) / 2.);
                hit->SetU(u);
                hit->SetV(v);
                hit->SetUVec(u_vec);
                hit->SetVVec(v_vec);
                hit->SetUVCov(uv_cov);

                hit->UpdateXY(); // Derive x and y from u and v

                if (!in_rectangle(hit->GetX(), hit->GetY(), layer_width, layer_length, angle1) ||
                    !in_rectangle(hit->GetX(), hit->GetY(), layer_width, layer_length, angle2))
                    continue;

                pool->AddHit(std::move(hit));
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

    double *splits = new double[2];

    for(auto &layer : unclustered_trk_hit_map)
    {
        Clusterer<TrkHitSP> clusterer;
        clusterer.SetClusterWidth(cluster_width_);

        for(auto &hit : layer.second)
        {
            splits[0] = hit->GetX();
            splits[1] = hit->GetY();
            clusterer.CreatePoint(&hit, 2, splits, hit->GetE());
        }

        // clusterer.ShowPoints();
        clusterer.FindClusters();

        TrkHitSPVec clustered_layer;
        auto n_hits = clusterer.GetNClusters();
        for(size_t i = 0; i < n_hits; i++)
        {
            clustered_layer.push_back(std::make_shared<TrkHit>());
            clustered_layer.back()->SetX(clusterer.GetClusterCenterSplits(i).at(0));
            clustered_layer.back()->SetY(clusterer.GetClusterCenterSplits(i).at(1));
            clustered_layer.back()->SetZ(layer.second.at(0)->GetZ());
            clustered_layer.back()->SetCellIdZ(layer.first);
        }
        trk_hit_map.insert(std::pair<int, TrkHitSPVec>(layer.first, clustered_layer));
    }
    //std::cout << std::endl;

    delete[] splits; splits = nullptr;


}

void Digitization::InsertHitMap(const TrkHitSP &trk_hit, TrkHitSPVecMap &trk_hit_map)
{
    auto it_search_map = trk_hit_map.find(trk_hit->GetCellIdZ());
    if(it_search_map != trk_hit_map.end())
        it_search_map->second.emplace_back(trk_hit);
    else
        trk_hit_map.insert(std::pair<int, TrkHitSPVec>(trk_hit->GetCellIdZ(), {trk_hit}));
}
