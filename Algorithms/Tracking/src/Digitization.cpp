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
    for(int i = 0; i < world_->GetNdaughters(); i++)
    {
        auto *detector = dynamic_cast<TGeoNode*>(world_->GetDaughter(i));
        auto detectorName = TString(detector->GetVolume()->GetName());
        if(detectorName.Contains("Trk"))
        {
            for(int j = 0; j < detector->GetNdaughters(); j++)
            {
                auto *layer = dynamic_cast<TGeoNode*>(detector->GetDaughter(i));
                auto rotation = layer->GetMatrix()->GetRotationMatrix();
                std::cout << layer->GetVolume()->GetName() << ": " << TMath::ATan2(rotation[1], rotation[0]) << std::endl;
            }
        }
    }
}

//Separate tracker hits into vectors by layers
void Digitization::Layering(const std::vector<TrkHit> &trk1Hits, const std::vector<TrkHit> &trk2Hits, TrkHitPVecMap &clusTrkHitMap)
{
    TrkHitPVecMap map1;
    TrkHitPVecMap map2;

    for(auto itTrkHit : trk1Hits)
    {
        TrkHit digitedHit(itTrkHit);
        digitedHit.SetU(itTrkHit.GetX());
        digitedHit.SetV(itTrkHit.GetY());

        auto itSearchMap = map1.find(itTrkHit.GetCellIdZ());
        if(itSearchMap != map1.end())
            itSearchMap->second.emplace_back(std::make_shared<TrkHit>(digitedHit));
        else
        {
            TrkHitPVec tempNewLayer;
            tempNewLayer.emplace_back(std::make_shared<TrkHit>(digitedHit));
            map1.insert(std::pair(itTrkHit.GetCellIdZ(), tempNewLayer));
        }
    }

    for(auto itTrkHit : trk2Hits)
    {
        TrkHit digitedHit(itTrkHit);
        digitedHit.SetU(itTrkHit.GetX());
        digitedHit.SetV(itTrkHit.GetY());

        auto itSearchMap = map2.find(itTrkHit.GetCellIdZ());
        if(itSearchMap != map2.end())
            itSearchMap->second.emplace_back(std::make_shared<TrkHit>(digitedHit));
        else
        {
            TrkHitPVec tempNewLayer;
            tempNewLayer.emplace_back(std::make_shared<TrkHit>(digitedHit));
            map2.insert(std::pair(itTrkHit.GetCellIdZ(), tempNewLayer));
        }
    }

    for(auto layer1 : map1)
    {
        auto itFindLayer2 = map2.find(layer1.first);
        if(itFindLayer2 != map2.end())
        {
            int range = std::abs(angle_.at(layer1.first - 1))*layerLength_/(layerWidth_/stripNo_);
            std::cout << layer1.first << "	" << range << std::endl;

            auto layer2 = map2.at(layer1.first);
            for(auto hit1 : layer1.second)
            {
                for(auto hit2 : layer2)
                {
                    if(abs(hit2->GetCellIdX() - hit1->GetCellIdX()) < range)
                    {
                        TrkHit constructedHit;
                        constructedHit.SetU(hit1->GetX());
                        constructedHit.SetZ(hit1->GetZ());

                        double angle = angle_.at(layer1.first - 1);
                        double x1 = hit1->GetX();
                        double x2 = (hit2->GetCellIdX() - 0.5)*layerWidth_/stripNo_;
                        constructedHit.SetV(-x1/tan(angle) + x2/sin(angle));
                        constructedHit.SetV(0.);

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

    for(auto layer : clusTrkHitMap)
    {
        for(auto hit : layer.second)
        {
            std::cout << hit->GetX() << "       "
                      << hit->GetY() << "       "
                      << hit->GetZ() << "       "
                      << hit->GetU() << "       "
                      << hit->GetV() << std::endl;
        }
    }

    //clusTrkHitMap = map1;
}
#endif
