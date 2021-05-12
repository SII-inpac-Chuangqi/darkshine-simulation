#ifndef _OFF_TRACKING
#ifndef FINDING_H
#define FINDING_H

//................................................................................//
//CPP STL
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

class Finding
{
public:
//................................................................................//
//Constructor
    Finding() {}
    virtual ~Finding() {}

    Finding(const Finding&) = delete;
    Finding& operator =(const Finding&) = delete;

//................................................................................//
//Get
    virtual double GetR      (int i) const = 0;
    virtual double GetCenterX(int i) const = 0;
    virtual double GetCenterY(int i) const = 0;
    virtual double GetChi2   (int i) const = 0;
    virtual int    GetTrackNo(     ) const = 0;

    virtual std::vector<TrkHitPVec>::iterator First() = 0;
    virtual std::vector<TrkHitPVec>::iterator Last () = 0;
};

#endif
#endif
