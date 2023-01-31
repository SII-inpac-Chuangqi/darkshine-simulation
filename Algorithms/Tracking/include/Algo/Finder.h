#ifndef FINDER_H
#define FINDER_H

//................................................................................//
//CPP Libraries
#include <iostream>
#include <map>
#include <vector>
#include <memory>

//................................................................................//
//ROOT

//................................................................................//
//Framework

//................................................................................//
//Tracking
#include "Algo/TypeDef.h"
#include "Algo/TrkHit.h"

class Finder
{
public:
//................................................................................//
//Constructor
    Finder() {}
    virtual ~Finder() {}

    Finder(const Finder&) = delete;
    Finder& operator =(const Finder&) = delete;

//................................................................................//
//Get
    virtual double GetR      (int i) const = 0;
    virtual double GetCenterX(int i) const = 0;
    virtual double GetCenterY(int i) const = 0;
    virtual double GetChi2   (int i) const = 0;
    virtual int    GetTrackNo(     ) const = 0;

    virtual std::vector<double> GetMagnetFieldAt(double x, double y, double z);

    virtual std::vector<TrkHitPVec>::iterator First() = 0;
    virtual std::vector<TrkHitPVec>::iterator Last () = 0;
};

#endif
