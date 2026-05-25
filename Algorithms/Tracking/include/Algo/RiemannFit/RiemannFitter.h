#ifndef RIEMANNFIT_RIEMANN_FITTER_H
#define RIEMANNFIT_RIEMANN_FITTER_H

//................................................................................//
//C++
#include <array>
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
#include "Algo/Object/TrkHit.h"
#include "Algo/Fitter.h"
#include "Algo/Propagator/Propagator.h"

class RiemannFitter final : public Fitter
{
public:
    struct Config
    {
        int max_trial = 100;
        double const_B = -1.5;
        Propagator *propagator = nullptr;
    };

public:
//................................................................................//
//Constructor
    RiemannFitter() {}
    RiemannFitter(Config config, DTrackP track, int verbose = 0);
    ~RiemannFitter();

    RiemannFitter(const RiemannFitter&) = delete;
    RiemannFitter& operator =(const RiemannFitter&) = delete;

//................................................................................//
//Processor
    virtual void Init(const TrkHitSPVec &track) override;
    virtual void Fit (const TrkHitSPVec &track) override;
    virtual void Fit3D(const TrkHitSPVec &track);   // 3D helix extension
    virtual void Fill(const TrkHitSPVec &track) override;

//................................................................................//
//Getter
//................................................................................//
//Estimate theta(angle between pT and magnet) from track measureements
    double GetTheta(const TrkHitSPVec &track);

//................................................................................//
//Get Corrections from inhomogeneous magnet
    virtual std::vector<double> GetCorrectionsX() const override {return corrections_x_;}

//................................................................................//
//Extrapolate momentum to planes using propagator
    std::tuple<std::vector<vector3D>, std::vector<vector3D>> ExtrapolateToPlanes(const std::vector<double> &planes_z) override
    {
        if(!propagator_) return {};
        vector3D mom_in = {px_, py_, pz_};
        std::vector<vector3D> mom_outs, pos_outs, plane_poss, plane_normals;
        mom_outs.reserve(planes_z.size()); pos_outs.reserve(planes_z.size());
        for(const auto &z : planes_z)
        {
            plane_poss.push_back({0., 0., z});
            plane_normals.push_back({0., 0., 1.});
        }
        propagator_->ExtrapolateToPlanes(mom_in, plane_poss, plane_normals, mom_outs, pos_outs);
        return {mom_outs, pos_outs};
    }
    std::tuple<vector3D, vector3D> ExtrapolateToPlane(const double &plane_z) override
    {
        if(!propagator_) return {};
        vector3D mom_in = {px_, py_, pz_};
        vector3D plane_pos = {0., 0., plane_z};
        vector3D plane_normal = {0., 0., 1.};
        vector3D mom_out, pos_out;
        propagator_->ExtrapolateToPlane(mom_in, plane_pos, plane_normal, mom_out, pos_out);
        return {mom_out, pos_out};
    }

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

//................................................................................//
// 3D Helix Extension
// Compute arc lengths along the fitted circle for each hit
    std::vector<double> ComputeArcLengths(const TrkHitSPVec &hits, int sign) const;

// Linear fit of y-coordinate vs arc length (returns {y0, tan_lambda, r_squared})
    std::array<double, 3> FitYvsArcLength(const std::vector<double> &arc_lengths,
                                           const TrkHitSPVec &hits) const;

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

    // Cached magnet field at origin (Tesla) — computed once, reused in GetDeltax and GetVradms
    double magnet_at_origin_{0.};

//................................................................................//
//Fit results
    double c_{0.};
    double n1_{0.};
    double n2_{0.};
    double n3_{0.};

//................................................................................//
//Corrections from inhomogeneous magnet    
    std::vector<double> corrections_x_;

//................................................................................//
// 3D Helix fit results
    double y0_{0.};           // y-offset at reference hit (along beam axis)
    double tan_lambda_{0.};   // dip angle tangent = py / pT
    double y_chi2_{RETURN};   // chi2-like quality of y-vs-arc-length linear fit
};

#endif
