#ifndef RIEMANNFIT_RIEMANN_FITTER_H
#define RIEMANNFIT_RIEMANN_FITTER_H

//................................................................................//
//CPP Libraries
#include <map>
#include <vector>
#include <memory>

//................................................................................//
//ROOT
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
//................................................................................//
//Estimate theta(angle between pT and magnet) from track measureements
    double GetTheta(const TrkHitPVec &track);

//................................................................................//
//Get hit measurements projected on the paraboloid surface in Cartesian coordinate
//  _          _
// | u u ... u  |
// | v v ... v  |
// |_s s ... s _|
//
// s = u*u + v*v
//................................................................................//
    TMatrixD GetCartCoo(const TrkHitPVec &track);

    double GetVradmsIJ(const TMatrixD &PolarCoo, int i, int j,
                       const double &p, // momentum, MeV
                       double (*MultipleScatteringError)(const double &p));
private:
    int dim_{0};
    double pre_R_{0.};
    double pre_Xc_{0.};
    double pre_Yc_{0.};
    double pre_theta_{0.};
};

#endif
