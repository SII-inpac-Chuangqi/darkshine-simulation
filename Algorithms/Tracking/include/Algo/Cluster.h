#ifndef CLUSTER_H
#define CLUSTER_H

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
void Cluster(const std::vector<TrkHit> &trkHits, TrkHitPVecMap &clusTrkHitMap);

#endif
