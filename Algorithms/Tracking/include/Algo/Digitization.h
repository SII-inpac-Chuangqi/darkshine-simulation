#ifndef DIGITIZATION_H
#define DIGITIZATION_H

#ifndef RETURN
#define RETURN std::nan("RETURN")
#endif

//................................................................................//
//C++
#include <map>
#include <vector>

//................................................................................//
//ROOT
//#include "TGeoNode.h"
#include "TRandom.h"

//................................................................................//
//Framework
#include "Object/SimulatedHit.h"

//................................................................................//
//Tracking
#include "Algo/TypeDef.h"
#include "Algo/TrkHit.h"

namespace tracking
{
    enum detector {tag = 1, rec = 2};
}

class Digitization
{
public:

    Digitization() = default;
    ~Digitization() = default;

    void SetIfSmear(bool if_smear);
//................................................................................//
//Get tracker geo information
    void ReadTrackerInfo(bool if_strip);
//................................................................................//
//Separate tracker hits into vectors by layers
    void Layering(const std::vector<TrkHit> &trk1_hits, const std::vector<TrkHit> &trk2_hits,
                  Pool *pool,
                  int detector);
//................................................................................//
//Set strip cluster width
    void SetClusterWidth(double cluster_width) {cluster_width_ = cluster_width;}

    double GetClusterWidth() const {return cluster_width_;}

private:

    void InitHitMap(const TrkHitPVec &trk_hits, TrkHitPVecMap &trk_hit_map);
    void InsertHitMap(const TrkHitP &trk_hit, TrkHitPVecMap &trk_hit_map);

    bool if_strip_{true};
    bool if_smear_{true};

    double cluster_width_ = 0.2;

    std::vector<double> layer_width_tag_ = {};
    std::vector<double> layer_length_tag_ = {};
    std::vector<int>    strip_no_tag_ = {};
    std::vector<double> angles_tag_ = {};

    std::vector<double> layer_width_rec_ = {};
    std::vector<double> layer_length_rec_ = {};
    std::vector<int>    strip_no_rec_ = {};
    std::vector<double> angles_rec_ = {};

    TRandom rnd_;
};

#endif
