//................................................................................//
//CPP STL
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
                                          u(newTrkHit.u),
                                          v(newTrkHit.v)
{}

TrkHit::TrkHit(TrkHit &&newTrkHit) : SimulatedHit(std::move(newTrkHit)),
                                     u(std::move(newTrkHit.u)),
                                     v(std::move(newTrkHit.v))
{}

TrkHit::TrkHit(const SimulatedHit &newSimuHit) : SimulatedHit(newSimuHit),
                                                 u(RETURN),
                                                 v(RETURN)
{}

void TrkHit::operator =(const TrkHit &oldTrkHit)
{
    if(this != &oldTrkHit)
    {
        id = oldTrkHit.id;
        CellID_X = oldTrkHit.CellID_X;
        CellID_Y = oldTrkHit.CellID_Y;
        CellID_Z = oldTrkHit.CellID_Z;
        X = oldTrkHit.X;       Y = oldTrkHit.Y;        Z = oldTrkHit.Z;
        T = oldTrkHit.T;
        E = oldTrkHit.E;

        u = oldTrkHit.u;       v = oldTrkHit.v;
    }
}

//................................................................................//
//Get
//................................................................................//
