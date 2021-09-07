#ifndef _OFF_TRACKING
#ifndef DIGITIZATION_H
#define DIGITIZATION_H

#ifdef RM_UNIT
    #define CUNIT 1
#else
    #define CUNIT 10
#endif

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
//Get tracker information
    void GetTrackerInfo();
//Separate tracker hits into vectors by layers
    void Layering(const std::vector<TrkHit> &trk1Hits, const std::vector<TrkHit> &trk2Hits, TrkHitPVecMap &clusTrkHitMap);

private:

    TGeoNode* world_;

    double layerWidth_ = 100.;
    double layerLength_ = 200.;
    int stripNo_ = 100;
    std::vector<double> angle_ = {.05, -.05, .05, -.05, .05, -.05, .05};
};


#endif
#endif
