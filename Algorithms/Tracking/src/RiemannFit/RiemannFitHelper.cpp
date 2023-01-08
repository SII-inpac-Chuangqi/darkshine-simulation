#include "Algo/RiemannFit/RiemannFitHelper.h"

int    RiemannFitHelper::verbose_ = 0;
double RiemannFitHelper::tracker_layer_thickness_ = 0.;

/*
RiemannFitHelper *dRFitHelper = nullptr;

RiemannFitHelper *RiemannFitHelper::CreateInstance()
{
    if (dRFitHelper == nullptr)
        dRFitHelper = new RiemannFitHelper();
    return dRFitHelper;
}
*/

RiemannFitHelper::RiemannFitHelper()
{
}

double RiemannFitHelper::GetMultipleScatteringError(const double &p)
{
    double m = 0.511; // e+/-, 0.511MeV
    double q = 1.;    // e+/-
    double b = p/std::sqrt(p*p + m*m);
    return 13.6/b*p*q*std::sqrt(tracker_layer_thickness_/9.370*10.)*(1 + 0.038*std::log(tracker_layer_thickness_/9.370*10.));
}
