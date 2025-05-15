#ifndef TRACKING_TYPEDEF_H
#define TRACKING_TYPEDEF_H
//................................................................................//
//C++
#include <map>
#include <vector>
#include <memory>
#include <type_traits>

//................................................................................//
//ROOT

//................................................................................//
//Framework

//................................................................................//
//Tracking
//#include "Algo/TrkHit.h"
//#include "Algo/HitPool.h"

class TrkHit;

using TrkHitSP       = std::shared_ptr<TrkHit>;
using TrkHitSPVec    = std::vector<std::shared_ptr<TrkHit>>;
using TrkHitSPVecMap = std::map<int, std::vector<std::shared_ptr<TrkHit>>>;

//using TrkHitP       = TrkHit*;
//using TrkHitPVec    = std::vector<TrkHit*>;
//using TrkHitPVecMap = std::map<int, std::vector<TrkHit*>>;

//................................................................................//
namespace tracking
{
//Fit methods implemented in Dark Shine tracking
//-- dNone: No method specified, return pre-fitting results from track finding
//-- dKalman: Kalman fitter_ from GenFit
    enum FitMethods {dNone, dKalman, dRiemann};

    enum direction {dX, dY, dZ};
}

#endif
