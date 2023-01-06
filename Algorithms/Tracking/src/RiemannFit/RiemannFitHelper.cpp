#include "Algo/RiemannFit/RiemannFitHelper.h"
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

double RiemannFitHelper::GetMultipleScatteringError(const double &p, const double &q, const double &x)
{
    double m = 0.511; // e+/-, 0.511MeV
    double b = p/std::sqrt(p*p + m*m);
    return 13.6/b*p*q*std::sqrt(x/9.370*10.)*(1 + 0.038*std::log(x/9.370*10.));
}

int RiemannFitHelper::verbose_ = 0;
