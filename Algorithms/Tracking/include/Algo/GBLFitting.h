#ifndef GBL_FITTING_H
#define GBL_FITTING_H

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
//General Broken Lines
#include "GblTrajectory.h"

//................................................................................//
//Framework

//................................................................................//
//Tracking
#include "Algo/TypeDef.h"
#include "Algo/TrkHit.h"
#include "Algo/Fitting.h"
#include "Algo/GBLUtil.h"

class GBLFitting : public Fitting
{
public:
//................................................................................//
//Constructor
    GBLFitting() {}
    GBLFitting(const TrkHitPVec &track, std::initializer_list<double>);
    ~GBLFitting() {};

    GBLFitting(const GBLFitting&) = delete;
    GBLFitting& operator =(const GBLFitting&) = delete;

//................................................................................//
//Processor
    virtual void Init(const TrkHitPVec &track, std::initializer_list<double>) override;
    virtual void Fit (const TrkHitPVec &track, std::initializer_list<double>) override;
    virtual void Fill(const TrkHitPVec &track, std::initializer_list<double>) override;

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

private:
//................................................................................//
//Method specific
    

    gbl::GblDetectorLayer CreateLayerSit(const std::string aName, unsigned int layer,
                                         double xPos, double yPos, double zPos, double thickness, double uAngle,
                                         double uRes, double vAngle, double vRes);
};

#endif
