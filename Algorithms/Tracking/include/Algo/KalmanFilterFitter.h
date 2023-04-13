#ifndef KALMAN_FILTER_FITTER_H
#define KALMAN_FILTER_FITTER_H

//................................................................................//
//C++
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
#include "Algo/Fitter.h"

class KalmanFilterFitter : public Fitter
{
public:
//................................................................................//
//Constructor
    KalmanFilterFitter() {}
    KalmanFilterFitter(const TrkHitPVec &track, std::initializer_list<double>, int verbose = 0);
    ~KalmanFilterFitter()
    {
        //delete measurement; measurement = nullptr;
        delete fitTrack; fitTrack = nullptr;
        //delete fitter; fitter = nullptr;
    };

    KalmanFilterFitter(const KalmanFilterFitter&) = delete;
    KalmanFilterFitter& operator =(const KalmanFilterFitter&) = delete;

//................................................................................//
//Processor
    virtual void Init(const TrkHitPVec &track, std::initializer_list<double>) override;
    virtual void Fit (const TrkHitPVec &track, std::initializer_list<double>) override;
    virtual void Fill(const TrkHitPVec &track, std::initializer_list<double>) override;

//................................................................................//
//Get
    //int GetSign(const TrkHitPVec &track);
    virtual std::vector<double> ExtrapolateTo(const std::vector<double> &planes_z, tracking::direction extrop_dir = tracking::dX);

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
