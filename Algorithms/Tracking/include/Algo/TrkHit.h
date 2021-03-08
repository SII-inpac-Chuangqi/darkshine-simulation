#ifndef TRKHIT_H
#define TRKHIT_H

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
//Framework
#include "Object/SimulatedHit.h"

//................................................................................//
//Tracking

class TrkHit : public SimulatedHit
{
public:
//................................................................................//
//Constructor
//................................................................................//
    TrkHit() {}
    TrkHit(const TrkHit &newTrkHit);
    TrkHit(TrkHit &&newTrkHit);
    TrkHit(const SimulatedHit &newSimuHit);
    virtual ~TrkHit() {}

    void operator =(const TrkHit &oldTrkHit);
//................................................................................//
//Set
//................................................................................//
    void SetId(int newId) {setId(newId);}
    void SetCellIdZ(int newIdZ) {setCellIdZ(newIdZ);}

    void SetX(double newX) {setX(newX);}
    void SetY(double newY) {setY(newY);}
    void SetZ(double newZ) {setZ(newZ);}
    void SetE(double newE) {setE(newE);}
//................................................................................//
//Get
//................................................................................//
    int GetId() {return getId();}
    int GetCellIdZ() {return getCellIdZ();}

    double GetX() {return getX();}
    double GetY() {return getY();}
    double GetZ() {return getZ();}
    std::vector<double> GetXYZ();
    double GetE() {return getE();}

protected:

private:
};

#endif
