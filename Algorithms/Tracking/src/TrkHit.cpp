//................................................................................//
//CPP Libraries
#include <iostream>
#include <map>
#include <vector>

//................................................................................//
//ROOT
#include "TMath.h"
#include "TString.h"

//................................................................................//
//Tracking
#include "Algo/Util.h"
#include "Algo/TrkHit.h"

//................................................................................//
//TrkHit
//................................................................................//
//Constructor
//................................................................................//
TrkHit::TrkHit(const TrkHit &newTrkHit) : SimulatedHit(newTrkHit),
                                          if_in_track_(newTrkHit.if_in_track_),
                                          u_(newTrkHit.u_),
                                          v_(newTrkHit.v_)
{}

TrkHit::TrkHit(TrkHit &&newTrkHit) : SimulatedHit(std::move(newTrkHit)),
                                     if_in_track_(std::move(newTrkHit.if_in_track_)),
                                     u_(std::move(newTrkHit.u_)),
                                     v_(std::move(newTrkHit.v_))
{}

TrkHit::TrkHit(const SimulatedHit &newSimuHit) : SimulatedHit(newSimuHit),
                                                 if_in_track_(RETURN),
                                                 u_(RETURN),
                                                 v_(RETURN)
{}

void TrkHit::operator =(const TrkHit &oldTrkHit)
{
    if(this != &oldTrkHit)
    {
        id = oldTrkHit.id;
        CellID_X = oldTrkHit.CellID_X;
        CellID_Y = oldTrkHit.CellID_Y;
        CellID_Z = oldTrkHit.CellID_Z;
        X = oldTrkHit.X;         Y = oldTrkHit.Y;         Z = oldTrkHit.Z;
        T = oldTrkHit.T;
        E = oldTrkHit.E;

        if_in_track_ = oldTrkHit.if_in_track_;
        u_ = oldTrkHit.u_;       v_ = oldTrkHit.v_;
    }
}

//................................................................................//
//Get
//................................................................................//
