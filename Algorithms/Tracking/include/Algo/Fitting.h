#ifndef FITTING_H
#define FITTING_H

//................................................................................//
//CPP STL
#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include <cmath>
#include <initializer_list>

#ifndef RETURN
#define RETURN std::nan("RETURN")
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
    virtual void Init(const TrkHitPVec&, std::initializer_list<double>) {}
    virtual void Fit (const TrkHitPVec&, std::initializer_list<double>) {}
    virtual void Fill(const TrkHitPVec&, std::initializer_list<double>) {}

//................................................................................//
//Get
    virtual int    GetVerbose() const {return verbose_;}
    virtual double GetPx() const {return px;}
    virtual double GetPy() const {return py;}
    virtual double GetPz() const {return pz;}
    virtual double GetPp() const {return pp;}
    virtual double GetPl() const {return pl;}
    virtual double GetECalSeedX() const {return ECal_seed_x;}
    virtual double GetECalSeedY() const {return ECal_seed_y;}
    virtual double GetECalDirctX() const {return ECal_seed_px;}
    virtual double GetECalDirctY() const {return ECal_seed_py;}
    virtual double GetECalQoP() const {return ECal_seed_pz;}

    virtual double GetChi2() const {return fChi2;}
    virtual double GetXSigma() const {return xSigma;}
    virtual double GetYSigma() const {return ySigma;}

    virtual std::vector<double> ExtrapolateTo(const std::vector<double> &planes) {return {};}

//................................................................................//
//Set
    virtual void   SetVerbose(int verbose) {verbose_ = verbose;}

protected:
//................................................................................//
//Verbose
    int verbose_{0};

//................................................................................//
//Results
    double px{RETURN};
    double py{RETURN};
    double pz{RETURN};
    double pp{RETURN};
    double pl{RETURN};
    double ECal_seed_x{RETURN};
    double ECal_seed_y{RETURN};
    double ECal_seed_px{RETURN};
    double ECal_seed_py{RETURN};
    double ECal_seed_pz{RETURN};

    double fChi2{RETURN};
    double xSigma{RETURN};
    double ySigma{RETURN};
};

#endif
