#ifndef _OFF_TRACKING
#ifndef DIGITIZATION_H
#define DIGITIZATION_H

//................................................................................//
//CPP STL
#include <map>
#include <vector>

//................................................................................//
//ROOT

//................................................................................//
//Framework
#include "Object/SimulatedHit.h"

//................................................................................//
//Tracking
#include "Algo/TypeDef.h"
#include "Algo/TrkHit.h"

//................................................................................//
//Realization
//................................................................................//
//Separate tracker hits into vectors by layers
void Digitization(const std::vector<TrkHit> &trk1Hits, const std::vector<TrkHit> &trk2Hits, TrkHitPVecMap &clusTrkHitMap);

#endif
#endif
