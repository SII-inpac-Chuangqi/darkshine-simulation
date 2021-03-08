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
    KalmanFitting(const TrkHitPVec &track, double prePp, double B);
    ~KalmanFitting() {};

    KalmanFitting(const KalmanFitting&) = delete;
    KalmanFitting& operator =(const KalmanFitting&) = delete;

//................................................................................//
//Processor
    void Init(const TrkHitPVec &track, double prePp, double B) override;
    void Fit (const TrkHitPVec &track) override;
    void Fill(const TrkHitPVec &track) override;

//................................................................................//
//Get
    double GetPp() const override {return pp;}
    double GetPl() const override {return pl;}

    double GetChi2() const override {return fChi2;}
    double GetXSigma() const override {return xSigma;}
    double GetYSigma() const override {return ySigma;}

    int GetSign(const TrkHitPVec &track);

private:
//................................................................................//
//Results
    double pp = -999.;
    double pl = -999.;

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
