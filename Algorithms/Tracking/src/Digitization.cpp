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

void Digitization::SetIfSmear(bool if_smear)
{
    if_smear_ = if_smear;
    if(if_smear_) std::cerr << "[INFO] ==> Hits smeared in tracking" << std::endl;
}

//Get world node from TGeoManager
void Digitization::ReadTrackerInfo(bool if_strip)
{
    if_strip_ = if_strip;

    if(if_strip_)
    {
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
    else
        std::cout << "[WARNING] ==> Strip model not loaded in tracking" << std::endl;
}

//Separate tracker hits into vectors by layers
void Digitization::Layering(const std::vector<TrkHit> &trk1_hits, const std::vector<TrkHit> &trk2_hits, TrkHitPVecMap &reco_trkhit_map,
                            int detector)
{
    if(if_strip_)
    {
        std::vector<double> *layer_widths  = (detector == tracking::tag) ? &layer_width_tag_  : &layer_width_rec_;
        std::vector<double> *layer_lengths = (detector == tracking::tag) ? &layer_length_tag_ : &layer_length_rec_;
        std::vector<int>    *strip_nos = (detector == tracking::tag) ? &strip_no_tag_ : &strip_no_rec_;
        std::vector<double> *angles    = (detector == tracking::tag) ? &angles_tag_   : &angles_rec_;
 
        TrkHitPVec clustered_trk1_hits;
        TrkHitPVec clustered_trk2_hits;
        for(const auto &hit : trk1_hits) clustered_trk1_hits.push_back(std::make_shared<TrkHit>(hit));
        for(const auto &hit : trk2_hits) clustered_trk2_hits.push_back(std::make_shared<TrkHit>(hit));

        TrkHitPVecMap map1;
        this->InitHitMap(clustered_trk1_hits, map1);
        TrkHitPVecMap map2;
        this->InitHitMap(clustered_trk2_hits, map2);
   
        for(auto layer1 : map1)
        {
            auto it_find_layer2 = map2.find(layer1.first);
            if(it_find_layer2 != map2.end())
            {
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
    
                        if(std::abs(y1) < 0.5*layer_length)
                        {
                            std::shared_ptr<TrkHit> reconstructed_hit = std::make_shared<TrkHit>();
                            reconstructed_hit->SetX(x1);
                            reconstructed_hit->SetZ(hit1->GetZ());
                            reconstructed_hit->SetY(y1);
                            reconstructed_hit->setCellIdZ(hit1->GetCellIdZ());

                            this->InsertHitMap(reconstructed_hit, reco_trkhit_map);
                        }
                    }
                }
            }
        }
    }
    else
    {
        for(const auto &it_trkhit : trk2_hits)
        {
            std::shared_ptr<TrkHit> reconstructed_hit = std::make_shared<TrkHit>(it_trkhit);

            this->InsertHitMap(reconstructed_hit, reco_trkhit_map);
        }
    }

    for(auto &layer : reco_trkhit_map)
    {
        //std::cout << layer.second.at(0)->GetCellIdZ() << "\t";

        for(auto &hit : layer.second)
        {
            hit->SetU(hit->GetX());
            hit->SetV(hit->GetY());

            //std::cout << hit->GetX() << "\t";
        }
        //std::cout << std::endl;
    }
}

void Digitization::InitHitMap(const TrkHitPVec &trk_hits, TrkHitPVecMap &trk_hit_map)
{

    TrkHitPVecMap unclustered_trk_hit_map;
    for(const auto &trk_hit : trk_hits)
        this->InsertHitMap(trk_hit, unclustered_trk_hit_map);

    for(const auto &layer : unclustered_trk_hit_map)
    {
        std::vector<Point> unclustered_trk_points;
        this->MakeClusterPoints(layer.second, unclustered_trk_points);

        std::vector<Cluster*> trk_clusters;
        this->MakeCluster(unclustered_trk_points, trk_clusters);
        //std::cout << "before cluster:\t" << unclustered_trk_points.size() << "\tafter cluster:\t" << trk_clusters.size() << std::endl;

        TrkHitPVec clustered_layer;
        for(const auto cluster : trk_clusters)
        {
            std::shared_ptr cluster_hit = std::make_shared<TrkHit>();
            cluster_hit->SetX(cluster->GetCenterX());
            cluster_hit->SetZ(layer.second.at(0)->GetZ());
            cluster_hit->SetCellIdZ(layer.second.at(0)->GetCellIdZ());
            clustered_layer.push_back(cluster_hit);
        }
        trk_hit_map.insert(std::pair<int, TrkHitPVec>(layer.second.at(0)->GetCellIdZ(), clustered_layer));

        for(size_t i = 0; i < trk_clusters.size(); i++)
        {
            delete trk_clusters.at(i);
            trk_clusters.at(i) = nullptr;
        }
    }
}

void Digitization::InsertHitMap(const TrkHitP &trk_hit, TrkHitPVecMap &trk_hit_map)
{
    auto it_search_map = trk_hit_map.find(trk_hit->GetCellIdZ());
    if(it_search_map != trk_hit_map.end())
        it_search_map->second.emplace_back(trk_hit);
    else
        trk_hit_map.insert(std::pair<int, TrkHitPVec>(trk_hit->GetCellIdZ(), {trk_hit}));
}

int Digitization::GetNextClusterSeed(const vector<Point> &points)
{
    for(size_t i = 0; i < points.size(); i++)
    {
        if(!points.at(i).cluster)
            return i;
    }

    return -1;
}

void Digitization::MakeClusterPoints(const TrkHitPVec &trk_hits,
                                     std::vector<Point> &points)
{
    points.clear();

    for(const auto &trk_hit : trk_hits)
        points.push_back(Point{trk_hit->GetX(), trk_hit->GetE(), false});
}

void Digitization::MakeCluster(std::vector<Point> &points, std::vector<Cluster*> &clusters)
{
    int n_lonely_point(points.size());
    int seed(0);

    while(n_lonely_point)
    {
        auto cluster = new Cluster();
        cluster->SetCenter(points.at(seed).x, 0.);

        //std::cout << "pre\tcurr" << std::endl;

        double shift(INFINITY);
        while(std::abs(shift) > 3e-2)
        {
            for(auto &point : points)
            {
                if(GetClusterDistance(point, *(cluster->center)) < cluster_width_ && !point.cluster)
                {
                    cluster->points.push_back(&point);
                    point.cluster = true;
                    n_lonely_point--;
                }
            }

            double new_center_x(cluster->GetNewCenterX());
            double old_center_x(cluster->center->x);
            shift = new_center_x - old_center_x;
            cluster->SetCenter(new_center_x, 0.);

            //std::cout << old_center_x << "\t" << new_center_x << std::endl;
        }
        //std::cout << std::endl;

        clusters.push_back(cluster);

        seed = GetNextClusterSeed(points);
        //std::cout << "next seed: " << seed << std::endl;
    }
}
