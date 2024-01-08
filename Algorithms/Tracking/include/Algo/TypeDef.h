#ifndef TRACKING_TYPEDEF_H
#define TRACKING_TYPEDEF_H
//................................................................................//
//C++
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
#include "Algo/HitPool.h"

using TrkHitP       = std::shared_ptr<TrkHit>;
using TrkHitPVec    = std::vector<std::shared_ptr<TrkHit>>;
using TrkHitPVecMap = std::map<int, std::vector<std::shared_ptr<TrkHit>>>;

using Key  = std::decay_t<decltype(((TrkHit*)nullptr)->GetCellIdZ())>;
using Pool = HitPool<Key, TrkHit>;
//std::function<std::decay_t<decltype(((TrkHit*)nullptr)->GetCellIdZ())>(const TrkHit&)> trk_hit_getter = &TrkHit::GetCellIdZ;

#endif
