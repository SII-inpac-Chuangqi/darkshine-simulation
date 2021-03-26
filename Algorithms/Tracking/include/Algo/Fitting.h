#ifndef FITTING_H
#define FITTING_H

//................................................................................//
//CPP STL
#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include <cmath>

#ifndef RETURN
#define RETURN INFINITY
#endif

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
    virtual void Init(const TrkHitPVec&, double, double) {}
    virtual void Fit (const TrkHitPVec&)                 {}
    virtual void Fill(const TrkHitPVec&)                 {}

//................................................................................//
//Get
    virtual double GetPx() const {return RETURN;}
    virtual double GetPy() const {return RETURN;}
    virtual double GetPz() const {return RETURN;}
    virtual double GetPp() const {return RETURN;}
    virtual double GetPl() const {return RETURN;}

    virtual double GetChi2() const {return RETURN;}
    virtual double GetXSigma() const {return RETURN;}
    virtual double GetYSigma() const {return RETURN;}

};

#endif
