#ifndef KALMAN_FITTING_H
#define KALMAN_FITTING_H

//................................................................................//
//CPP STL
#include <iostream>
#include <map>
#include <vector>
#include <memory>

//................................................................................//
//ROOT
#include "TMath.h"
#include "TString.h"
#include "TMatrixD.h"
#include "TMatrixDSym.h"

//................................................................................//
//GenFit
#include "GenFitInclude.h"

//................................................................................//
//Framework

//................................................................................//
//Tracking
#include "Algo/TypeDef.h"
#include "Algo/TrkHit.h"
#include "Algo/Fitting.h"

class KalmanFitting : public Fitting
{
public:
//................................................................................//
//Constructor
    KalmanFitting() {}
    KalmanFitting(const TrkHitPVec &track, double prePp, double B);
    ~KalmanFitting() {};

    KalmanFitting(const KalmanFitting&) = delete;
    KalmanFitting& operator =(const KalmanFitting&) = delete;

//................................................................................//
//Processor
    virtual void Init(const TrkHitPVec &track, double prePp, double B) override;
    virtual void Fit (const TrkHitPVec &track) override;
    virtual void Fill(const TrkHitPVec &track) override;

//................................................................................//
//Get
    virtual double GetPx() const override {return px;}
    virtual double GetPy() const override {return py;}
    virtual double GetPz() const override {return pz;}
    virtual double GetPp() const override {return pp;}
    virtual double GetPl() const override {return pl;}

    virtual double GetChi2() const override {return fChi2;}
    virtual double GetXSigma() const override {return xSigma;}
    virtual double GetYSigma() const override {return ySigma;}

    int GetSign(const TrkHitPVec &track);

private:
//................................................................................//
//Results
    double px = -999999.;
    double py = -999999.;
    double pz = -999999.;
    double pp = -999999.;
    double pl = -999999.;

    double fChi2 = -999.;
    double xSigma = -999.;
    double ySigma = -999.;

//................................................................................//
//
    genfit::AbsTrackRep *rep = nullptr;
    std::unique_ptr<genfit::AbsKalmanFitter> fitter = nullptr;
    genfit::Track *fitTrack = nullptr;

    TVector3 pos;
    TVector3 mom;
    TMatrixDSym hitCov;
};

#endif
