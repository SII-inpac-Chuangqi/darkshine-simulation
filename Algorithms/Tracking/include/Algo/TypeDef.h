#ifndef _OFF_TRACKING
#ifndef TYPEDEF_H
#define TYPEDEF_H
//................................................................................//
//CPP STL
#include <iostream>
#include <map>
#include <vector>

//................................................................................//
//ROOT

//................................................................................//
//Framework

//................................................................................//
//Tracking
#include "Algo/TrkHit.h"

typedef std::shared_ptr<TrkHit>                             TrkHitP;
typedef std::vector<std::shared_ptr<TrkHit>>                TrkHitPVec;
typedef std::map<int, std::vector<std::shared_ptr<TrkHit>>> TrkHitPVecMap;

#endif
#endif
