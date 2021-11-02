#ifndef _OFF_TRACKING
//................................................................................//
//CPP STL
#include <iostream>
#include <map>
#include <vector>
#include <cmath>

//................................................................................//
//ROOT
#include "TMath.h"

//................................................................................//
//Framework

//................................................................................//
//Tracking
#include "Algo/TrkHit.h"
#include "Algo/Digitization.h"

//................................................................................//
//Control
//................................................................................//

//................................................................................//
//Realization
//................................................................................//
//Separate trkHits into vectors by layers
void Cluster(const std::vector<TrkHit> &trkHits, TrkHitPVecMap &clusTrkHitMap)
{
//No clustering
    for(auto itTrkHit : trkHits)
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

/*
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
