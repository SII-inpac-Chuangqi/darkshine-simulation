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

void Digitization(std::vector<TrkHit> &rawHits)
{
    for(auto &hit : rawHits) hit.SetU(hit.GetX());
    for(auto &hit : rawHits) hit.SetV(hit.GetY());
    //for(auto &hit : rawHits) hit.SetU((int)(hit.GetX()/0.02)*0.02 + std::copysign(1.0, hit.GetX())*0.01);
    //for(auto &hit : rawHits) hit.SetV((int)(hit.GetY()/0.2)*0.2 + std::copysign(1.0, hit.GetY())*0.1);
}
#endif