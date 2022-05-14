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
