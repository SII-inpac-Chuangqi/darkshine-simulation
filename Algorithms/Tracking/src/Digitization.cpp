#ifndef _OFF_TRACKING

#ifndef RETURN
#define RETURN -INFINITY
#endif

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

//................................................................................//
//Framework

//................................................................................//
//Tracking
#include "Algo/TrkHit.h"
#include "Algo/Digitization.h"

//Get world node from TGeoManager
void Digitization::GetWorldNode(TGeoNode* worldNode)
{
    world_ = worldNode;
}

void Digitization::GetTrackerInfo()
{
    anglesTag_.clear();
    anglesRec_.clear();

    for(int i = 0; i < world_->GetNdaughters(); i++)
    {
        auto *detector = dynamic_cast<TGeoNode*>(world_->GetDaughter(i));
        auto detectorName = TString(detector->GetVolume()->GetName());
        if(detectorName.Contains("Trk"))
        {
            for(int j = 0; j < detector->GetNdaughters(); j++)
            {
                auto *layer = dynamic_cast<TGeoNode*>(detector->GetDaughter(j));
                auto layerName = TString(layer->GetVolume()->GetName());
                auto rotation = layer->GetMatrix()->GetRotationMatrix();
                if(layerName.Contains("Tag"))
                    anglesTag_.push_back(std::asin(rotation[1]));
                else if(layerName.Contains("Rec"))
                    anglesRec_.push_back(std::asin(rotation[1]));
            }
        }
    }
}

//Separate tracker hits into vectors by layers
void Digitization::Layering(const std::vector<TrkHit> &trk1Hits, const std::vector<TrkHit> &trk2Hits, TrkHitPVecMap &clusTrkHitMap,
                            int detector)
{
    TrkHitPVecMap map1;
    TrkHitPVecMap map2;

    for(auto itTrkHit : trk1Hits)
    {
        auto itSearchMap = map1.find(itTrkHit.GetCellIdZ());
        if(itSearchMap != map1.end())
            itSearchMap->second.emplace_back(std::make_shared<TrkHit>(itTrkHit));
        else
        {
            TrkHitPVec tempNewLayer;
            tempNewLayer.emplace_back(std::make_shared<TrkHit>(itTrkHit));
            map1.insert(std::pair(itTrkHit.GetCellIdZ(), tempNewLayer));
        }
    }

    for(auto itTrkHit : trk2Hits)
    {
        auto itSearchMap = map2.find(itTrkHit.GetCellIdZ());
        if(itSearchMap != map2.end())
            itSearchMap->second.emplace_back(std::make_shared<TrkHit>(itTrkHit));
        else
        {
            TrkHitPVec tempNewLayer;
            tempNewLayer.emplace_back(std::make_shared<TrkHit>(itTrkHit));
            map2.insert(std::pair(itTrkHit.GetCellIdZ(), tempNewLayer));
        }
    }

    double layerWidth = RETURN;
    double layerLength = RETURN;
    int stripNo = -1;
    std::vector<double> *angles = nullptr;
    if(detector == tag)
    {
        layerWidth = layerWidthTag_;
        layerLength = layerLengthTag_;
        stripNo = stripNoTag_;
        angles = &anglesTag_;
    }
    else if(detector == rec)
    {
        layerWidth = layerWidthRec_;
        layerLength = layerLengthRec_;
        stripNo = stripNoRec_;
        angles = &anglesRec_;
    }

    for(auto layer1 : map1)
    {
        auto itFindLayer2 = map2.find(layer1.first);
        if(itFindLayer2 != map2.end())
        {
            double angle = angles->at((layer1.first - 1)*2 + 1);

            auto layer2 = map2.at(layer1.first);
            for(auto hit1 : layer1.second)
            {
                for(auto hit2 : layer2)
                {
                    double x1 = hit1->GetX();
                    double y1 = -x1/tan(angle) + (hit2->GetCellIdX() - (stripNo + 1)/2)/sin(angle)*layerWidth/stripNo;

                    if(abs(y1) < 0.5*layerLength)
                    {
                        TrkHit constructedHit;
                        constructedHit.SetU(hit1->GetX());
                        constructedHit.SetZ(hit1->GetZ());
                        constructedHit.SetV(y1);

                        auto itSearchMap = clusTrkHitMap.find(hit1->GetCellIdZ());
                        if(itSearchMap != clusTrkHitMap.end())
                            itSearchMap->second.emplace_back(std::make_shared<TrkHit>(constructedHit));
                        else
                        {
                            TrkHitPVec tempNewLayer;
                            tempNewLayer.emplace_back(std::make_shared<TrkHit>(constructedHit));
                            clusTrkHitMap.insert(std::pair(hit1->GetCellIdZ(), tempNewLayer));
                        }
                    }
                }
            }
        }
    }
}
#endif
