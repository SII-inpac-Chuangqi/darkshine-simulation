#ifndef _OFF_TRACKING
#ifndef TRKHIT_H
#define TRKHIT_H

#ifndef RETURN
#define RETURN -INFINITY
#endif

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

    void SetU(double newU) {u = newU;}
    void SetV(double newV) {v = newV;}
//................................................................................//
//Get
//................................................................................//
    int GetId() const {return getId();}
    int GetCellIdX() const {return getCellIdX();}
    int GetCellIdY() const {return getCellIdY();}
    int GetCellIdZ() const {return getCellIdZ();}

    double GetX() const {return getX();}
    double GetY() const {return getY();}
    double GetZ() const {return getZ();}
    double GetE() const {return getE();}

    double GetU() const {return u;}
    double GetV() const {return v;}

protected:

    double u = RETURN;
    double v = RETURN;

private:
};

#endif
#endif
