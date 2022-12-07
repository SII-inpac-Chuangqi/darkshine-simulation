#ifndef DTRACK_H
#define DTRACK_H

#ifndef RETURN
#define RETURN std::nan("RETURN")
#endif

//................................................................................//
//CPP STL
#include <iostream>
#include <cmath>

//................................................................................//
//ROOT

//................................................................................//
//Framework
#include "Object/DMagnet.h"
#include "Object/ReconstructedParticle.h"

//................................................................................//
//Tracking
#include "Algo/TypeDef.h"
#include "Algo/KalmanFitting.h"

//................................................................................//
//Fitting methods implemented in Dark Shine tracking
//-- dNone: No method specified, return pre-fitting results from track finding
//-- dKalman: Kalman fitter_ from GenFit
namespace tracking
{
    enum FittingMethods {dNone, dKalman};
}

class DTrack
{
public:

    DTrack() = default;
    DTrack(const TrkHitPVec &newHits,
           double newPreR, 
           double newPreXc,
           double newPreYc);
    DTrack(const DTrack &oldTrack);
    DTrack(DTrack &&oldTrack);
    DTrack& operator=(const DTrack&);

    ~DTrack() {delete fitter_; fitter_ = nullptr;};

//................................................................................//
//Get
//................................................................................//
    int GetVerbose() const {return verbose_;}
    int GetPDG()  const {return pdg_;}
    int GetSign() const {return sign_;}
    double GetPx() const {return px_;}
    double GetPy() const {return py_;}
    double GetPz() const {return pz_;}
    double GetPp() const {return pp_;}
    double GetPl() const {return py_;}
    double GetPreR()  const {return preR_;}
    double GetPreXc() const {return preXc_;}
    double GetPreYc() const {return preYc_;}
    double GetECalSeedX() const {return ECal_seed_x_;}
    double GetECalSeedY() const {return ECal_seed_y_;}
    double GetECalDirctX() const {return ECal_seed_px_;}
    double GetECalDirctY() const {return ECal_seed_py_;}
    double GetECalQoP() const {return ECal_seed_pz_;}

    int GetSize() const {return hits_.size();}
    TrkHitP At(int i) {return hits_.at(i);}
    double GetQuality() const {return quality_;}

    double GetChi2();
    double GetChi2Algo() const {return chi2_algo_;}
    double GetXSigma() const {return xSigma_;}
    double GetYSigma() const {return ySigma_;}
    std::vector<double> GetExtrapolated(tracking::direction extrop_dir = tracking::dX);

//................................................................................//
//Set
//................................................................................//
    void ExceptionHandler(const std::vector<double> &magnet);
    void SetVerbose(int verbose) {verbose_ = verbose;}
    void SetPDG(int newPDG)   {pdg_ = newPDG;}
    void SetSign(int newSign) {sign_ = newSign;}
    void SetPx(double newPx)  {px_ = newPx;}
    void SetPy(double newPy)  {py_ = newPy;}
    void SetPz(double newPz)  {pz_ = newPz;}
    void SetChi2(double newChi2) {chi2_ = newChi2;}

//................................................................................//
//Processor
//................................................................................//
    void Fit(int method);
    std::vector<double> ExtrapolateTo(const std::vector<double> &planes_z, tracking::direction extrop_dir = tracking::dX);
    void Evaluate();
    void Reverse() {std::reverse(hits_.begin(), hits_.end());}

private:
//................................................................................//
//Verbose
    int verbose_{0};

//................................................................................//
//Physical properties
    int pdg_{11};
    int sign_{-1};
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

//................................................................................//
//Track properties
    double quality_{RETURN};

//................................................................................//
//Detector properties
    double By_{RETURN}; // manage problematic condition

//................................................................................//
//Fitting properties
    double chi2_{RETURN};
    double chi2_algo_{RETURN};
    double xSigma_{RETURN};
    double ySigma_{RETURN};
    bool   if_extrapolated_{false};
    std::vector<double> extrapolated_x_;
    std::vector<double> extrapolated_y_;

//................................................................................//
//Finding properties
    double preR_{RETURN};
    double preXc_{RETURN};
    double preYc_{RETURN};

//................................................................................//
//Fitter
    Fitting *fitter_{nullptr};
 
//................................................................................//
//Hits collection
    TrkHitPVec hits_;
};

#endif
