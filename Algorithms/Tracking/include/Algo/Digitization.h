#ifndef _OFF_TRACKING
#ifndef DIGITIZATION_H
#define DIGITIZATION_H

//................................................................................//
//CPP STL
#include <map>
#include <vector>

//................................................................................//
//ROOT
#include "TGeoManager.h"
#include "TGeoNode.h"

//................................................................................//
//Framework
#include "Object/SimulatedHit.h"

//................................................................................//
//Tracking
#include "Algo/TypeDef.h"
#include "Algo/TrkHit.h"

class Digitization
{
public:
    Digitization() = default;
    ~Digitization() = default;

//Get world node from TGeoManager
    void GetWorldNode(TGeoNode* worldNode);
//Separate tracker hits into vectors by layers
    void Layering(const std::vector<TrkHit> &trk1Hits, const std::vector<TrkHit> &trk2Hits, TrkHitPVecMap &clusTrkHitMap);

private:
};


#endif
#endif
