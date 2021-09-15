#ifndef _OFF_TRACKING
#ifndef DIGITIZATION_H
#define DIGITIZATION_H

#ifdef RM_UNIT
#define CUNIT 1
#else
#define CUNIT 10
#endif

#ifndef RETURN
#define RETURN -INFINITY
#endif

//................................................................................//
//CPP STL
#include <map>
#include <vector>

//................................................................................//
//ROOT
#include "TGeoManager.h"
#include "TGeoNode.h"
#include "TRandom.h"

//................................................................................//
//Framework
#include "Object/SimulatedHit.h"

//................................................................................//
//Tracking
#include "Algo/TypeDef.h"
#include "Algo/TrkHit.h"

enum {tag = 1, rec = 2};

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
    void Layering(const std::vector<TrkHit> &trk1Hits, const std::vector<TrkHit> &trk2Hits, TrkHitPVecMap &clusTrkHitMap,
                  int detector);

private:

    TGeoNode* world_;

    double layerWidthTag_ = RETURN;
    double layerLengthTag_ = RETURN;
    int stripNoTag_ = -1;
    std::vector<double> anglesTag_ = {};

    double layerWidthRec_ = RETURN;
    double layerLengthRec_ = RETURN;
    int stripNoRec_ = -1;
    std::vector<double> anglesRec_ = {};

    TRandom rnd_;
};


#endif
#endif
