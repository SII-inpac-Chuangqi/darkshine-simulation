#include "Algo/RiemannFit/RiemannFitter.h"

//................................................................................//
//Tracking
#include "Algo/RiemannFit/RiemannFitHelper.h"

//................................................................................//
//Constructor
RiemannFitter::RiemannFitter(const TrkHitPVec &/* track */, std::initializer_list<double>)
{
}

//................................................................................//
//Processor
void RiemannFitter::Init(const TrkHitPVec &/* track */, std::initializer_list<double>)
{
}

void RiemannFitter::Fit(const TrkHitPVec &/* track */, std::initializer_list<double>)
{
    TMatrixD polar_coo;
    this->GetVradmsIJ(polar_coo, 0, 0,
                      8000. /* momentum */, 1. /* charge */,
                      RiemannFitHelper::GetMultipleScatteringError);
}

void RiemannFitter::Fill(const TrkHitPVec&, std::initializer_list<double>)
{
}

//................................................................................//
//Getter
double RiemannFitter::GetVradmsIJ(const TMatrixD &PolarCoo, int i, int j,
                                  const double &p /* MeV */, const double &q,
                                  double (*MultipleScatteringError)(const double &p, const double &q))
{
    double sigma_ms = MultipleScatteringError(p, q);

    return 0.;
}
