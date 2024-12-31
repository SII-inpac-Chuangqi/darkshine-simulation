#ifndef FITTER_H
#define FITTER_H

//................................................................................//
//C++
#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include <cmath>
//#include <initializer_list>

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

namespace tracking
{
    enum direction {dX, dY, dZ};
    class Calibrator;
}

class Fitter
{
public:
//................................................................................//
//Constructor
    Fitter() {}
    virtual ~Fitter() {}

    Fitter(const Fitter&) = delete;
    Fitter& operator =(const Fitter&) = delete;

//................................................................................//
//Processor
    virtual void Init(const TrkHitSPVec&) {}
    virtual void Fit (const TrkHitSPVec&) {}
    virtual void Fill(const TrkHitSPVec&) {}

//................................................................................//
//Get
    virtual int    GetVerbose() const {return verbose_;}
    virtual int    GetSign(const TrkHitSPVec &track);
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

    virtual double GetNdf()  const {return fNdf;}
    virtual double GetChi2() const {return fChi2;}
    virtual double GetXSigma() const {return xSigma;}
    virtual double GetYSigma() const {return ySigma;}

    virtual std::vector<double> ExtrapolateTo([[maybe_unused]] const std::vector<double> &planes_z,
                                              [[maybe_unused]] tracking::direction extrop_dir = tracking::dX)
                                             {return {};}
    virtual std::vector<double> GetCorrectionsX() const {return {};}

//................................................................................//
//Set
    virtual void   SetVerbose(int verbose) {verbose_ = verbose;}

    virtual void   SetCalibrator(tracking::Calibrator *calibrator) {calibrator_ = calibrator;}
    virtual tracking::Calibrator* GetCalibrator() const {return calibrator_;}

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

    double fNdf{0.};
    double fChi2{RETURN};
    double xSigma{RETURN};
    double ySigma{RETURN};

    tracking::Calibrator *calibrator_{nullptr};
};

#endif
