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
    KalmanFitting(const TrkHitPVec &track, std::initializer_list<double>);
    ~KalmanFitting()
    {
        //delete measurement; measurement = nullptr;
        delete fitTrack; fitTrack = nullptr;
        //delete fitter; fitter = nullptr;
    };

    KalmanFitting(const KalmanFitting&) = delete;
    KalmanFitting& operator =(const KalmanFitting&) = delete;

//................................................................................//
//Processor
    virtual void Init(const TrkHitPVec &track, std::initializer_list<double>) override;
    virtual void Fit (const TrkHitPVec &track, std::initializer_list<double>) override;
    virtual void Fill(const TrkHitPVec &track, std::initializer_list<double>) override;

//................................................................................//
//Get
    //virtual double GetPx() const override {return px;}
    //virtual double GetPy() const override {return py;}
    //virtual double GetPz() const override {return pz;}
    //virtual double GetPp() const override {return pp;}
    //virtual double GetPl() const override {return pl;}
    //virtual double GetECalSeedX() const override {return ECal_seed_x;}
    //virtual double GetECalSeedY() const override {return ECal_seed_y;}
    //virtual double GetECalDirctX() const override {return ECal_dirct_x;}
    //virtual double GetECalDirctY() const override {return ECal_dirct_y;}
    //virtual double GetECalQoP() const override {return ECal_qop;}

    //virtual double GetChi2() const override {return fChi2;}
    //virtual double GetXSigma() const override {return xSigma;}
    //virtual double GetYSigma() const override {return ySigma;}

    int GetSign(const TrkHitPVec &track);

private:
//................................................................................//
//Method specific
    genfit::AbsTrackRep *rep = nullptr;
    //genfit::AbsKalmanFitter *fitter = nullptr;
    std::unique_ptr<genfit::KalmanFitterRefTrack> fitter = std::make_unique<genfit::KalmanFitterRefTrack>();
    genfit::Track *fitTrack = nullptr;

    genfit::PlanarMeasurement *measurement = nullptr;

    TVector3 pos;
    TVector3 mom;
    TMatrixDSym hitCov;
};

#endif
