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
//#include "Object/DMagnet.h"
#include "Object/ReconstructedParticle.h"

//................................................................................//
//Tracking
#include "Algo/TypeDef.h"

class DVertex;

class Fitter;

class DTrack
{
public:

    DTrack() = default;
    DTrack(const TrkHitSPVec &newHits,
           double newPreXc,
           double newPreYc,
           double newPreR);
    DTrack(const DTrack &oldTrack);
    DTrack(DTrack &&oldTrack);
    DTrack& operator=(const DTrack&);

    ~DTrack() {/*delete fitter_; fitter_ = nullptr;*/}; // DO NOT release fitter_, memory not owned here

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

    int Size() const {return hits_.size();}
    TrkHitSPVec& GetHits() {return hits_;}
    TrkHitSP At(int i) {return hits_.at(i);}
    TrkHitSP AtCellIdZ(int i);
    double GetQuality() const {return quality_;}

    double GetNdf() const {return ndf_;}
    double GetChi2();
    double GetChi2Algo() const {return chi2_algo_/ndf_;}
    double GetXSigma() const {return xSigma_;}
    double GetYSigma() const {return ySigma_;}
    double GetDeltaR(const DTrack *another) const;
    std::vector<double> GetExtrapolated(tracking::direction extrop_dir = tracking::dX);
    std::vector<double> GetCorrectionsX() const {return corrections_x_;}

    Fitter* GetFitter() {return fitter_;}

    friend std::ostream &operator<<(std::ostream &os, const DTrack &track);

//................................................................................//
//Set
//................................................................................//
//    void ExceptionHandler(const std::vector<double> &magnet);
    void LinkFitter(Fitter *fitter) {fitter_ = fitter;}
    void SetVerbose(int verbose) {verbose_ = verbose;}
    void SetPDG(int newPDG) {pdg_ = newPDG;}
    void SetSign(int newSign) {sign_ = newSign;}
    void SetPx(double newPx) {px_ = newPx;}
    void SetPy(double newPy) {py_ = newPy;}
    void SetPz(double newPz) {pz_ = newPz;}
    void SetPp(double newPp) {pp_ = newPp;}
    void SetPl(double newPl) {pl_ = newPl;}
    void SetNdf(double newNdf) {ndf_ = newNdf;}
    void SetXSigma(double newXSigma) {xSigma_ = newXSigma;}
    void SetYSigma(double newYSigma) {ySigma_ = newYSigma;}
    void SetECalSeedX(double newECalSeedX) {ECal_seed_x_ = newECalSeedX;}
    void SetECalSeedY(double newECalSeedY) {ECal_seed_y_ = newECalSeedY;}
    void SetECalDirctX(double newECalDirctX) {ECal_seed_px_ = newECalDirctX;}
    void SetECalDirctY(double newECalDirctY) {ECal_seed_py_ = newECalDirctY;}
    void SetECalQoP(double newECalQoP) {ECal_seed_pz_ = newECalQoP;}
    void SetChi2(double newChi2) {chi2_ = newChi2;}
    void SetChi2Algo(double newChi2) {chi2_algo_ = newChi2;}
    void Remove(int i);
    void SetVertex(const std::shared_ptr<DVertex> &vertex) {vertex_ = vertex;}
    static void SetResolutions(double x_resolution, double y_resolution, double z_resolution)
    { x_resolution_ = x_resolution; y_resolution_ = y_resolution; z_resolution_ = z_resolution; }

//................................................................................//
//Processor
//................................................................................//
//    void Fit(int method);
    std::vector<double> ExtrapolateTo(const std::vector<double> &planes_z, tracking::direction extrop_dir = tracking::dX);
    void Evaluate();
    void Reverse() {std::reverse(hits_.begin(), hits_.end());}

private:
//................................................................................//
//Verbose
    static double x_resolution_;
    static double y_resolution_;
    static double z_resolution_;

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
    TrkHitSPVec hits_;

//................................................................................//
//Link to vertex
    std::weak_ptr<DVertex> vertex_;
};

#endif
