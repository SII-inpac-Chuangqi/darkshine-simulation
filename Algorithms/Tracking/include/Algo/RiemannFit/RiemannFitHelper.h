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

    static void SetMagnetAtOrigin(double Bx, double By, double Bz)
    {magnet_at_origin_[0] = Bx; magnet_at_origin_[1] = By; magnet_at_origin_[2] = Bz;}
    static double GetMagnetAtOrigin(unsigned int i) {if(i < 3) return magnet_at_origin_[i]; return 0.;}

    static void SetTrackerLayerThickness(double tracker_layer_thickness) {tracker_layer_thickness_ = tracker_layer_thickness;}
//................................................................................//
//Multiple scattering variance
//................................................................................//
//                     ___
//      13.6MeV       / x                 x
// σθ = ————————*q*  / ——— [1 + 0.038*ln(———)]
//        βpc      \/   X0                X0
// 
//x : thickness of the material
//X0: radiation length of the material, Si: 9.370cm, https://pdg.lbl.gov/2010/AtomicNuclearProperties/HTML_PAGES/014.html
//................................................................................//
    static double GetMultipleScatteringError(const double &p /* momentum, MeV */);

//................................................................................//
//Measurement variance induced by tracker strips
//................................................................................//
//resolution of x: width of cluster/sqrt(12)
//              y: width of cluster/sin(angle between strips)/sqrt(12)
//              z: 0
//
//      |\  |   |
//      | \ |   |
//      |  \|   |
//      |   |   |      
//      |   |\__| —————— angle between strips
//      |   | \ | 
//      |   |  \|
//
//      \___  ___/
//          \/
//   width of cluster    
//................................................................................//
    static void SetMeasurementError(double cluster_width, double angle);
    static double GetMeasurementError(unsigned int i) {if(i < 3) return measurement_error_[i]; return 0.;}

    //static double GetMagnetInducedError();

private:
    RiemannFitHelper();

    static int verbose_;

    static double tracker_layer_thickness_;

    static double magnet_at_origin_[3];

    static double measurement_error_[3];
};

//extern RiemannFitHelper *dRFitHelper;

#endif
