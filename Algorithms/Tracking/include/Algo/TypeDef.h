#ifndef TRACKING_TYPEDEF_H
#define TRACKING_TYPEDEF_H
//................................................................................//
//C++
#include <iostream>
#include <map>
#include <vector>
#include <type_traits>

//................................................................................//
//ROOT

//................................................................................//
//Framework

//................................................................................//
//Tracking
#include "Algo/TrkHit.h"
#include "Algo/HitPool.h"

using TrkHitSP       = std::shared_ptr<TrkHit>;
using TrkHitSPVec    = std::vector<std::shared_ptr<TrkHit>>;
using TrkHitSPVecMap = std::map<int, std::vector<std::shared_ptr<TrkHit>>>;

//using TrkHitP       = TrkHit*;
//using TrkHitPVec    = std::vector<TrkHit*>;
//using TrkHitPVecMap = std::map<int, std::vector<TrkHit*>>;

//using Key  = std::decay_t<decltype(((TrkHit*)nullptr)->GetCellIdZ())>;
using Key = std::invoke_result_t<decltype(&TrkHit::GetCellIdZ), TrkHit>;
using Pool = HitPool<Key, TrkHit>;
//std::function<std::decay_t<decltype(((TrkHit*)nullptr)->GetCellIdZ())>(const TrkHit&)> trk_hit_getter = &TrkHit::GetCellIdZ;

#endif
