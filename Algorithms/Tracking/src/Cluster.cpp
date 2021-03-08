//................................................................................//
//CPP STL
#include <iostream>
#include <map>
#include <vector>

//................................................................................//
//ROOT
#include "TMath.h"

//................................................................................//
//Framework

//................................................................................//
//Tracking
#include "Algo/TrkHit.h"
#include "Algo/Cluster.h"

//................................................................................//
//Control
//................................................................................//

//................................................................................//
//Realization
//................................................................................//
//Separate trkHits into vectors by layers
//
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
