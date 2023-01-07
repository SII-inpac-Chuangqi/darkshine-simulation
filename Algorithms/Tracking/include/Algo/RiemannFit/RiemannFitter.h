#ifndef RIEMANNFIT_RIEMANN_FITTER_H
#define RIEMANNFIT_RIEMANN_FITTER_H

//................................................................................//
//CPP Libraries
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
//Framework

//................................................................................//
//Tracking
#include "Algo/TypeDef.h"
#include "Algo/TrkHit.h"
#include "Algo/Fitter.h"

class RiemannFitter : public Fitter
{
public:
//................................................................................//
//Constructor
    RiemannFitter() {}
    RiemannFitter(const TrkHitPVec &track, std::initializer_list<double>);
    ~RiemannFitter() {};

    RiemannFitter(const RiemannFitter&) = delete;
    RiemannFitter& operator =(const RiemannFitter&) = delete;

//................................................................................//
//Processor
    virtual void Init(const TrkHitPVec &track, std::initializer_list<double>) override;
    virtual void Fit (const TrkHitPVec &track, std::initializer_list<double>) override;
    virtual void Fill(const TrkHitPVec &track, std::initializer_list<double>) override;

//................................................................................//
//Getter
    double GetVradmsIJ(const TMatrixD &PolarCoo, int i, int j,
                       const double &p /* MeV */, const double &q,
                       double (*MultipleScatteringError)(const double &p, const double &q));
private:
    double pre_theta_{0.};
};

#endif
