#ifndef FITTING_H
#define FITTING_H

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

class Fitting
{
public:
//................................................................................//
//Constructor
    Fitting() {}
    virtual ~Fitting() {}

    Fitting(const Fitting&) = delete;
    Fitting& operator =(const Fitting&) = delete;

//................................................................................//
//Processor
    virtual void Init(const TrkHitPVec &track, double prePp, double B) = 0;
    virtual void Fit (const TrkHitPVec &track)                         = 0;
    virtual void Fill(const TrkHitPVec &track)                         = 0;

//................................................................................//
//Get
    virtual double GetPp() const = 0;
    virtual double GetPl() const = 0;

    virtual double GetChi2() const = 0;
    virtual double GetXSigma() const = 0;
    virtual double GetYSigma() const = 0;
};

#endif
