#ifndef _OFF_TRACKING

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

//................................................................................//
//Tracking
#include "Algo/TrkHit.h"
#include "Algo/Digitization.h"

//Get world node from TGeoManager
void Digitization::GetTrackerInfo(bool if_strip)
{
    if_strip_ = if_strip;

    world_ = dynamic_cast<TGeoNode*>(gGeoManager->GetListOfNodes()->At(0));

    anglesTag_.clear();
    anglesRec_.clear();

    for(int i = 0; i < world_->GetNdaughters(); i++)
    {
        auto *detector = dynamic_cast<TGeoNode*>(world_->GetDaughter(i));
        auto detectorName = TString(detector->GetVolume()->GetName());
        if(detectorName.Contains("Trk"))
        {
            auto *detectorShape = dynamic_cast<TGeoBBox*>(detector->GetVolume()->GetShape());

            if(detectorName.Contains("TAG"))
            {
                stripNoTag_ = detector->GetDaughter(0)->GetNdaughters();
                layerWidthTag_ = CUNIT*detectorShape->GetDX();
                layerLengthTag_ = CUNIT*detectorShape->GetDY();
            }
            else if(detectorName.Contains("REC"))
            {
                stripNoRec_ = detector->GetDaughter(0)->GetNdaughters();
                layerWidthRec_ = CUNIT*detectorShape->GetDX();
                layerLengthRec_ = CUNIT*detectorShape->GetDY();
            }

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

    std::cout << stripNoTag_ << std::endl;
}

//Separate tracker hits into vectors by layers
void Digitization::Layering(const std::vector<TrkHit> &trk1Hits, const std::vector<TrkHit> &trk2Hits, TrkHitPVecMap &clusTrkHitMap,
                            int detector)
{
    if(if_strip_)
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
    
        double layerWidth = (detector == tag) ? layerWidthTag_ : layerWidthRec_;
        double layerLength = (detector == tag) ? layerLengthTag_ : layerLengthRec_;
        int stripNo = (detector == tag) ? stripNoTag_ : stripNoRec_;
        std::vector<double> *angles = (detector == tag) ? &anglesTag_ : &anglesRec_;
    
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
                        double smear1 = rnd_.Uniform(layerWidth/stripNo) - 0.5*layerWidth/stripNo;
                        double smear2 = rnd_.Uniform(layerWidth/stripNo) - 0.5*layerWidth/stripNo;
                        double x1 = hit1->GetX() + smear1;
                        double y = -x1/tan(angle) + ((hit2->GetCellIdX() - 0.5*(stripNo + 1))*layerWidth/stripNo + smear2)/sin(angle);
    
                        if(std::abs(y) < 0.5*layerLength)
                        {
                            TrkHit constructedHit;
                            constructedHit.SetU(x1);
                            constructedHit.SetZ(hit1->GetZ());
                            constructedHit.SetV(y);
    
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
    else
    {
        for(auto itTrkHit : trk1Hits)
        {
            auto itSearchMap = clusTrkHitMap.find(itTrkHit.GetCellIdZ());
            if(itSearchMap != clusTrkHitMap.end())
                itSearchMap->second.emplace_back(std::make_shared<TrkHit>(itTrkHit));
            else
            {
                TrkHitPVec tempNewLayer;
                tempNewLayer.emplace_back(std::make_shared<TrkHit>(itTrkHit));
                clusTrkHitMap.insert(std::pair(itTrkHit.GetCellIdZ(), tempNewLayer));
            }
        }
    }

/*

    for(auto layer : clusTrkHitMap)
    {
        for(auto hit : layer.second)
        {
            hit->SetU(hit->GetX());
            hit->SetV(hit->GetY());
        }
    }
*/
}
#endif
