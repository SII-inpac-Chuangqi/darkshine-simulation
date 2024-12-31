#ifndef RIEMANNFIT_RIEMANN_FITTER_H
#define RIEMANNFIT_RIEMANN_FITTER_H

//................................................................................//
//C++
#include <map>
#include <vector>
#include <memory>
#include <functional>
#include <utility>

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
    struct Config
    {
        double const_B;
        double pre_Xc;
        double pre_Yc;
        double pre_R;
        int max_trial = 100;
    };

public:
//................................................................................//
//Constructor
    RiemannFitter() {}
    RiemannFitter(const TrkHitSPVec &track, Config config, int verbose = 0);
    ~RiemannFitter() {};

    RiemannFitter(const RiemannFitter&) = delete;
    RiemannFitter& operator =(const RiemannFitter&) = delete;

//................................................................................//
//Processor
    virtual void Init(const TrkHitSPVec &track, std::initializer_list<double>) override;
    virtual void Fit (const TrkHitSPVec &track, std::initializer_list<double>) override;
    virtual void Fill(const TrkHitSPVec &track, std::initializer_list<double>) override;

//................................................................................//
//Getter
//................................................................................//
//Estimate theta(angle between pT and magnet) from track measureements
    double GetTheta(const TrkHitSPVec &track);

//................................................................................//
//Get Corrections from inhomogeneous magnet
    std::vector<double> GetCorrectionsX() const override {return corrections_x_;}

private:
//................................................................................//
//Get hit measurements projected on the paraboloid surface in Cartesian coordinates
//     1 2 ... dim
//    _          _
// 1 | u u ... u  |
// 2 | v v ... v  |
// 3 |_s s ... s _|
//
// s = u*u + v*v
//
    TMatrixD GetCartCoo(const TrkHitSPVec &track);

//................................................................................//
//Get hit measurements projected on the paraboloid surface in polar coordinates
//     1 2 ... dim
//    _          _
// 1 | r r ... r  |
// 2 |_φ φ ... φ _|
//
// r = sqrt(u*u + v*v)
// φ = atan(v, u)
//
    TMatrixD GetPolarCoo(const TrkHitSPVec &track);

//................................................................................//
//Get multiple scattering covariance matrix element
//Multiple scattering variance function MultipleScatteringError from fit helper RiemannFitHelper::GetMultipleScatteringError
    template <typename ... FArgs, typename ... Args>
    double GetVradmsIJ(const TMatrixD &polar_coo, int i, int j,
                       std::function<double(FArgs...)> error_func,
                       Args && ... error_para)
    {
        double sigma_ms = error_func(std::forward<Args>(error_para)...);
    
        double v_radms_i_j = 0.;
        double ii = 0., jj = 0.;
        const double *element = polar_coo.GetMatrixArray();
        ii = *(element + i);
        jj = *(element + j);
    
        int k_max = (i < j) ? i : j;
        for(int k = 0; k < k_max; k++)
        {
            double kk = *(element + k);
            v_radms_i_j += (ii - kk)*(jj - kk)*sigma_ms*sigma_ms/sin(pre_theta_)/sin(pre_theta_);
        }
    
        return v_radms_i_j;
    }

//................................................................................//
//Get multiple scattering covariance matrix
    TMatrixD GetVradms(const TMatrixD &polar_coo);

//................................................................................//
//Get covariance matrix of measurement in Cartesian coordinates
//Measurement scattering variance function MultipleScatteringError from fit helper RiemannFitHelper::GetMeasurementError
//  _                                      _
// | σx^2                                   |
// |      σx^2                              |
// |           ...                          |
// |               σx^2                     |
// |                    σz^2                |
// |                         ...            |
// |                             σz^2       |
// |_                                 σz^2 _|
//
//σi = resolution i
// ideally, σx = width of the cluster
//          σy = width of the cluster/angle between tracker layers
//          σz = 0
//
//Covariance matrix of a measurement
//  _                           _
// | σx^2     cov(x,y) cov(x,z)  |
// | cov(x,y) σy^2     cov(y,z)  |
// |_cov(x,z) cov(y,z) σz^2     _|
//
    TMatrixD GetVcart0();

//................................................................................//
//Get Jacobian matrix from Cartesian to polar coordinate
    TMatrixD GetJ1(const TrkHitSPVec &track);

//................................................................................//
//Get Jacobian matrix from R-Φ to RΦ-R
    TMatrixD GetJ2(const TrkHitSPVec &track);

//................................................................................//
//Get covariance matrix of measurement in RΦ-R coordinate
    TMatrixD GetVrad0(const TMatrixD &v_cart0, const TMatrixD &j1, const TMatrixD &j2);

//................................................................................//
//Get inverse of the final covariance matrix
// G = V^-1, V = V_rad_ms (multiple scattering) + V_rad_0 (measurement)
    TMatrixD GetG(const TMatrixD &v_rad0, const TMatrixD &v_radms, const TMatrixD &v_radx);

//................................................................................//
//Get weights
//
//        Σj G_ij
// w_i = —————————
//       ΣjΣk G_jk
//
    TMatrixD GetW(const TMatrixD &g);

//................................................................................//
//Get weighted center
    TMatrixD GetXc(const TMatrixD &cart_coo, const TMatrixD &w);

//................................................................................//
//Get weighted center
    TMatrixD GetXg(const TMatrixD &cart_coo, const TMatrixD &x_c);

//................................................................................//
//Get normal vector of fitteed plane
    TMatrixD GetNormalVecs(const TMatrixD &g, const TMatrixD &x_g);

//................................................................................//
//Get Corrections from inhomogeneous magnet 
    std::vector<double> GetDeltax(const TrkHitSPVec &track);
//Get covariance matrix of delta x in Cartesian coordinates
    TMatrixD GetVcartx(const TrkHitSPVec &track);

//................................................................................//
//Get covariance matrix of delta x in RΦ-R coordinate
    TMatrixD GetVradx(const TMatrixD &v_cartx, const TMatrixD &j1, const TMatrixD &j2);

private:
    Config config_;

//................................................................................//
//Dimesion of the matrices, or, No. of hits in the track
    int dim_{0};
//................................................................................//
//Prefit properties
    double pre_R_{0.};
    double pre_Xc_{0.};
    double pre_Yc_{0.};
    double pre_theta_{0.};

//................................................................................//
//Fit results
    double c_{0.};
    double n1_{0.};
    double n2_{0.};
    double n3_{0.};

//................................................................................//
//Corrections from inhomogeneous magnet    
    std::vector<double> corrections_x_;
};

#endif
