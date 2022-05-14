#ifndef RIEMANN_FITTING_H
#define RIEMANN_FITTING_H

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
//Framework

//................................................................................//
//Tracking
#include "Algo/TypeDef.h"
#include "Algo/TrkHit.h"
#include "Algo/Fitting.h"

class RiemannFitting : public Fitting
{
public:
//................................................................................//
//Constructor
    RiemannFitting() {}
    RiemannFitting(const TrkHitPVec &track, std::initializer_list<double>);
    ~RiemannFitting() {};

    RiemannFitting(const RiemannFitting&) = delete;
    RiemannFitting& operator =(const RiemannFitting&) = delete;

//................................................................................//
//Processor
    virtual void Init(const TrkHitPVec &track, std::initializer_list<double>) override;
    virtual void Fit (const TrkHitPVec &track, std::initializer_list<double>) override;
    virtual void Fill(const TrkHitPVec &track, std::initializer_list<double>) override;

//................................................................................//
//Get
//................................................................................//

//................................................................................//
//Method specific
    double GetTheta(const TrkHitPVec &track);

    TMatrixD GetCartCoo(const TrkHitPVec &track);
    TMatrixD GetPolarCoo(const TrkHitPVec &track);

    TMatrixD GetVcart0();
    TMatrixD GetJ1(const TrkHitPVec &track);
    TMatrixD GetJ2(const TrkHitPVec &track);
    TMatrixD GetVrad0(const TMatrixD &Vcart0, const TMatrixD &J1, const TMatrixD &J2);

    double GetSigma();
    double GetVradmsIJ(const TMatrixD &PolarCoo, int i, int j);
    TMatrixD GetVradms(const TMatrixD &PolarCoo);

    TMatrixD GetG(const TMatrixD &Vrad0, const TMatrixD &Vradms);
    TMatrixD GetW(const TMatrixD &G);

    TMatrixD GetXc(const TMatrixD &CartCoo, const TMatrixD &W);
    TMatrixD GetXg(const TMatrixD &CartCoo, const TMatrixD &Xc);
    TMatrixD GetNVecs(const TMatrixD &G, const TMatrixD &Xg);

private:
//................................................................................//
//Method specific
    double preR = RETURN;
    double preXc = RETURN;
    double preYc = RETURN;
    double preTheta = RETURN;

    int dim = -1;

    double c = RETURN;
    double n1 = RETURN;
    double n2 = RETURN;
    double n3 = RETURN;
};

#endif
