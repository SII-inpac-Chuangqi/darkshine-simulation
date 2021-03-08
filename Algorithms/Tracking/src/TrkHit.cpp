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
TrkHit::TrkHit(const TrkHit &newTrkHit) : SimulatedHit(newTrkHit)
{}

TrkHit::TrkHit(TrkHit &&newTrkHit) : SimulatedHit(newTrkHit)
{}

TrkHit::TrkHit(const SimulatedHit &newSimuHit) : SimulatedHit(newSimuHit)
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
    }
}

//................................................................................//
//Get
//................................................................................//
std::vector<double> TrkHit::GetXYZ()
{
    std::vector<double> temp = {X, Y, Z};
    return temp;
}
