#ifndef DTRACK_H
#define DTRACK_H

#ifndef RETURN
#define RETURN std::nan("RETURN")
#endif

//................................................................................//
//C++
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
#include "Algo/KalmanFilterFitter.h"
#include "Algo/RiemannFit/RiemannFitHelper.h"

class DVertex;

//................................................................................//
//Fit methods implemented in Dark Shine tracking
//-- dNone: No method specified, return pre-fitting results from track finding
//-- dKalman: Kalman fitter_ from GenFit
namespace tracking
{
    enum FitMethods {dNone, dKalman, dRiemann};
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
    std::shared_ptr<DVertex> GetVertex() const {return vertex_.lock();}
//................................................................................//
//Correction on reco momentum in recoil tracker ( p_rcs ) by comparing the peak
//values of reco and truth level momenta with different beam energy
//
// p_fixed = 4.517e-6 * p_rcs^2 + 0.89787 * p_rcs
//
    double GetFixedPp() const {return 0.000004517*pp_*pp_ + 0.89787*pp_;}
    double GetPl() const {return py_;}
    double GetPreR()  const {return preR_;}
    double GetPreXc() const {return preXc_;}
    double GetPreYc() const {return preYc_;}
    double GetECalSeedX() const {return ECal_seed_x_;}
    double GetECalSeedY() const {return ECal_seed_y_;}
    double GetECalDirctX() const {return ECal_seed_px_;}
    double GetECalDirctY() const {return ECal_seed_py_;}
    double GetECalQoP() const {return ECal_seed_pz_;}
    int GetInitCellIdZ() const;

    int GetSize() const {return hits_.size();}
    TrkHitP At(int i) {return hits_.at(i);}
    TrkHitP AtCellIdZ(int i);
    double GetQuality() const {return quality_;}

    double GetNdf() const {return ndf_;}
    double GetChi2();
    double GetChi2Algo() const {return chi2_algo_;}
    double GetXSigma() const {return xSigma_;}
    double GetYSigma() const {return ySigma_;}
    double GetDeltaR(const DTrack *another) const;
    std::vector<double> GetExtrapolated(tracking::direction extrop_dir = tracking::dX);
    std::vector<double> GetCorrectionsX() const {return corrections_x_;}

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
    void Remove(int i);
    void SetVertex(const std::shared_ptr<DVertex> &vertex) {vertex_ = vertex;}

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
//Fit properties
    double ndf_{0.};
    double chi2_{RETURN};
    double chi2_algo_{RETURN};
    double xSigma_{RETURN};
    double ySigma_{RETURN};
    bool   if_extrapolated_{false};
    std::vector<double> extrapolated_x_;
    std::vector<double> extrapolated_y_;
    std::vector<double> corrections_x_;

//................................................................................//
//Finding properties
    double preR_{RETURN};
    double preXc_{RETURN};
    double preYc_{RETURN};

//................................................................................//
//Fitter
    Fitter *fitter_{nullptr};
 
//................................................................................//
//Hits collection
    TrkHitPVec hits_;

//................................................................................//
//Link to vertex
    std::weak_ptr<DVertex> vertex_;
};

#endif
