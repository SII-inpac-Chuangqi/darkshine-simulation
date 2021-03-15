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
void Cluster(const std::vector<TrkHit> &trkHits, TrkHitPVecMap &clusTrkHitMap);

void Digitization(std::vector<TrkHit> &rawHits);

#endif
