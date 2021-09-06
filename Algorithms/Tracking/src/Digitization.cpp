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

//Get world node from TGeoManager
void Digitization::GetWorldNode(TGeoNode* worldNode)
{
    std::cout << "world node: " << worldNode << std::endl;
}

//Separate tracker hits into vectors by layers
void Digitization::Layering(const std::vector<TrkHit> &trk1Hits, const std::vector<TrkHit> &trk2Hits, TrkHitPVecMap &clusTrkHitMap)
{
//No clustering
    for(auto itTrkHit : trk2Hits)
    {
        TrkHit digitedHit(itTrkHit);
        digitedHit.SetU(itTrkHit.GetX());
        digitedHit.SetV(itTrkHit.GetY());

        auto itSearchMap = clusTrkHitMap.find(itTrkHit.GetCellIdZ());
        if(itSearchMap != clusTrkHitMap.end())
            itSearchMap->second.emplace_back(std::make_shared<TrkHit>(digitedHit));
        else
        {
            TrkHitPVec tempNewLayer;
            tempNewLayer.emplace_back(std::make_shared<TrkHit>(digitedHit));
            clusTrkHitMap.insert(std::pair(itTrkHit.GetCellIdZ(), tempNewLayer));
        }
    }
}
#endif
