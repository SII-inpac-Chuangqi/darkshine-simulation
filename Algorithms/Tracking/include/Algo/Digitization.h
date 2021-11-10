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

//Get tracker information
    void GetTrackerInfo(bool if_strip);
//Separate tracker hits into vectors by layers
    void Layering(const std::vector<TrkHit> &trk1_hits, const std::vector<TrkHit> &trk2_hits, TrkHitPVecMap &clusTrkHitMap,
                  int detector);

private:

    TGeoNode* world_;

    bool if_strip_{true};

    double layer_width_tag_{RETURN};
    double layer_length_tag_{RETURN};
    int strip_no_tag_{-1};
    std::vector<double> angles_tag_ = {};

    double layer_width_rec_{RETURN};
    double layer_length_rec_{RETURN};
    int strip_no_rec_{-1};
    std::vector<double> angles_rec_ = {};

    TRandom rnd_;
};


#endif
#endif
