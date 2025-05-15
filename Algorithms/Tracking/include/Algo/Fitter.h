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
#include "Algo/Object/TrkHit.h"

namespace tracking
{
    class Calibrator;
}

class DTrack;

class Fitter
{
protected:
    using DTrackP = std::shared_ptr<DTrack>;

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
    virtual double GetPx() const {return px_;}
    virtual double GetPy() const {return py_;}
    virtual double GetPz() const {return pz_;}
    virtual double GetPp() const {return pp_;}
    virtual double GetPl() const {return pl_;}
    virtual double GetECalSeedX() const {return ECal_seed_x_;}
    virtual double GetECalSeedY() const {return ECal_seed_y_;}
    virtual double GetECalDirctX() const {return ECal_seed_px_;}
    virtual double GetECalDirctY() const {return ECal_seed_py_;}
    virtual double GetECalQoP() const {return ECal_seed_pz_;}

    virtual double GetNdf()  const {return fndf_;}
    virtual double GetChi2() const {return fchi2_;}
    virtual double GetXSigma() const {return x_sigma_;}
    virtual double GetYSigma() const {return y_sigma_;}

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
    double px_{RETURN};
    double py_{RETURN};
    double pz_{RETURN};
    double pp_{RETURN};
    double pl_{RETURN};
    double ECal_seed_x_{RETURN};
    double ECal_seed_y_{RETURN};
    double ECal_seed_px_{RETURN};
    double ECal_seed_py_{RETURN};
    double ECal_seed_pz_{RETURN};

    double fndf_{0.};
    double fchi2_{RETURN};
    double x_sigma_{RETURN};
    double y_sigma_{RETURN};

    tracking::Calibrator *calibrator_{nullptr};

    DTrackP track_;
};

#endif
