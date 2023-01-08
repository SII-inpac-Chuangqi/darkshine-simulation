#ifndef RIEMANNFIT_RIEMANN_FIT_HELPER_H
#define RIEMANNFIT_RIEMANN_FIT_HELPER_H

#include "Algo/RiemannFit/RiemannFitter.h"

class RiemannFitHelper
{
public:
    RiemannFitHelper(const RiemannFitHelper&) = delete;
    RiemannFitHelper &operator=(RiemannFitHelper const&) = delete;

    //static RiemannFitHelper *CreateInstance();

    static void SetVerbose(int verbose) {verbose_ = verbose;}

    static void SetTrackerLayerThickness(double tracker_layer_thickness) {tracker_layer_thickness_ = tracker_layer_thickness;}

//................................................................................//
//Multiple scattering
//................................................................................//
//                     ___
//      13.6MeV       / x                 x
// σθ = ————————*q*  / ——— [1 + 0.038*ln(———)]
//        βpc      \/   X0                X0
// 
// x : thickness of the material
// X0: radiation length of the material, Si: 9.370cm, https://pdg.lbl.gov/2010/AtomicNuclearProperties/HTML_PAGES/014.html
//................................................................................//
    static double GetMultipleScatteringError(const double &p /* momentum, MeV */);
    //static double GetMeasurementError();
    //static double GetMagnetInducedError();

private:
    RiemannFitHelper();

    static int verbose_;

    static double tracker_layer_thickness_;
};

//extern RiemannFitHelper *dRFitHelper;

#endif
