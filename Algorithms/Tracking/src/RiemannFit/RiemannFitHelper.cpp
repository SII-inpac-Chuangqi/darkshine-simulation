#include "Algo/RiemannFit/RiemannFitHelper.h"

int    RiemannFitHelper::verbose_ = 0;
double RiemannFitHelper::tracker_layer_thickness_ = 0.;
double RiemannFitHelper::magnet_at_origin_[3];
double RiemannFitHelper::measurement_error_[3];

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

void RiemannFitHelper::SetMeasurementError(double cluster_width, double angle)
{
    measurement_error_[0] = cluster_width/std::sqrt(12);
    measurement_error_[1] = cluster_width/angle/std::sqrt(12);
    measurement_error_[2] = 0.;
}

//................................................................................//
//Multiple scattering variance
double RiemannFitHelper::GetMultipleScatteringError(const double &p)
{
    double m = 0.511; // e+/-, 0.511MeV
    double q = 1.;    // e+/-
    double b = p/std::sqrt(p*p + m*m);
    return 13.6/b*p*q*std::sqrt(tracker_layer_thickness_/9.370*10.)*(1 + 0.038*std::log(tracker_layer_thickness_/9.370*10.));
}
