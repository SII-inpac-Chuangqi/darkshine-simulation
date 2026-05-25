#include "Algo/RiemannFit/RiemannFitter.h"

//................................................................................//
//ROOT
#include "TMath.h"
#include "TArrayD.h"
#include "TMatrixDSymEigen.h"

//................................................................................//
//Tracking
#include "Algo/RiemannFit/RiemannFitHelper.h"
#include "Algo/Object/DTrack.h"
#include "Algo/Propagator/Propagator.h"

//................................................................................//
//Constructor
RiemannFitter::RiemannFitter(Config config, DTrackP track, int verbose) : config_(config)
{
    track_   = track;
    verbose_ = verbose;

    propagator_ = config.propagator;
    track_->LinkFitter(this);

    pre_Xc_ = track_->GetPreXc();
    pre_Yc_ = track_->GetPreYc();
    pre_R_ = track_->GetPreR(); 

    auto hits = track_->GetHits();

    try
    {
        this->Init(hits);
        this->Fit (hits);
        this->Fit3D(hits);      // 3D helix extension
        this->Fill(hits);
    }
    catch(...)
    {
        std::cerr << "[Error] ==> Unexpected error in RiemannFitter" << std::endl;
        pp_ = 0.3*std::abs(config_.const_B)*track_->GetPreR();
        return;
    }
}

RiemannFitter::~RiemannFitter()
{
    track_->LinkFitter(nullptr);
}

//................................................................................//
//Processor
void RiemannFitter::Init(const TrkHitSPVec &hits)
{
    dim_ = hits.size();
    magnet_at_origin_ = RiemannFitHelper::GetMagnetAtOrigin(tracking::dY);
    corrections_x_ = this->GetDeltax(hits);
    this->GetTheta(hits);
}

void RiemannFitter::Fit(const TrkHitSPVec &hits)
{
    c_ = 0.;
    n1_ = 0.;
    n2_ = 0.;
    n3_ = 0.;

    for(int i = 0; i < config_.max_trial; i++)
    {
        corrections_x_ = this->GetDeltax(hits);
        TMatrixD cart_coo(GetCartCoo(hits));
        TMatrixD polar_coo(GetPolarCoo(hits));
        
        TMatrixD v_cart0(GetVcart0());
        TMatrixD v_cartx(GetVcartx(hits));
        TMatrixD j1(GetJ1(hits));
        TMatrixD j2(GetJ2(hits));
        TMatrixD v_rad0(GetVrad0(v_cart0, j1, j2));
        TMatrixD v_radms(GetVradms(polar_coo));
        TMatrixD v_radx(GetVradx(v_cartx, j1, j2));
        TMatrixD g(GetG(v_rad0, v_radms, v_radx));
        TMatrixD w(GetW(g));
        TMatrixD x_c(GetXc(cart_coo, w));
        TMatrixD x_g(GetXg(cart_coo, x_c));
        TMatrixD normal_vecs(GetNormalVecs(g, x_g));

        const double *get_x_c = x_c.GetMatrixArray();
        const double *get_normal_vec = normal_vecs.GetMatrixArray();
        c_ = - *get_x_c**get_normal_vec
            - *(get_x_c + 1)**(get_normal_vec + 3)
            - *(get_x_c + 2)**(get_normal_vec + 6);
        n1_ = *get_normal_vec;
        n2_ = *(get_normal_vec + 3);
        n3_ = *(get_normal_vec + 6);

        double curr_R = std::abs(sqrt(1 - n3_*n3_*n3_*n3_ - 4*c_*n3_)*0.5/n3_);
        double curr_Xc = -0.5*n1_/n3_;
        double curr_Yc = -0.5*n2_/n3_;

        if(std::abs(curr_Xc/(pre_Xc_ + curr_Xc)) < 1e-4 &&
           std::abs(curr_Yc/(pre_Yc_ + curr_Yc)) < 1e-4 &&
           std::abs((curr_R - pre_R_)/pre_R_) < 1e-4)
            break;
        else
        {
            pre_R_ = curr_R;
            pre_Xc_ += curr_Xc;
            pre_Yc_ += curr_Yc;
        }
    }
}

//................................................................................//
// 3D Helix Extension
// Compute arc lengths along the fitted circle for each hit
std::vector<double> RiemannFitter::ComputeArcLengths(const TrkHitSPVec &hits, int sign) const
{
    std::vector<double> arc_lengths;
    arc_lengths.reserve(hits.size());

    if(hits.size() < 2 || pre_R_ <= 0.)
    {
        // Return zero arc lengths — linear fit will be degenerate
        arc_lengths.assign(hits.size(), 0.);
        return arc_lengths;
    }

    // Reference: first hit
    double x_ref = hits.at(0)->GetX() - pre_Xc_;
    double z_ref = hits.at(0)->GetZ() - pre_Yc_;
    double phi_ref = std::atan2(z_ref, x_ref);

    arc_lengths.push_back(0.);  // s_0 = 0

    for(size_t i = 1; i < hits.size(); i++)
    {
        double xi = hits.at(i)->GetX() - pre_Xc_;
        double zi = hits.at(i)->GetZ() - pre_Yc_;
        double phi_i = std::atan2(zi, xi);

        // Compute signed angular difference
        double dphi = phi_i - phi_ref;
        // Unwrap to shortest path consistent with sign
        // For a circle, hits should be ordered monotonically in phi
        if(sign > 0 && dphi < 0.) dphi += 2.*TMath::Pi();
        if(sign < 0 && dphi > 0.) dphi -= 2.*TMath::Pi();

        arc_lengths.push_back(pre_R_ * dphi);
    }

    return arc_lengths;
}

// Linear fit of y-coordinate vs arc length
std::array<double, 3> RiemannFitter::FitYvsArcLength(
    const std::vector<double> &arc_lengths,
    const TrkHitSPVec &hits) const
{
    // Returns {y0, tan_lambda, r_squared}
    size_t n = arc_lengths.size();
    if(n < 2)
        return {hits.at(0)->GetY(), 0., 0.};

    double sum_s = 0., sum_y = 0., sum_ss = 0., sum_sy = 0.;
    for(size_t i = 0; i < n; i++)
    {
        double s = arc_lengths[i];
        double y = hits.at(i)->GetY();
        sum_s  += s;
        sum_y  += y;
        sum_ss += s * s;
        sum_sy += s * y;
    }

    double denom = n * sum_ss - sum_s * sum_s;
    double tan_lambda = 0.;
    double y0 = 0.;

    if(std::abs(denom) > 1e-12)
    {
        tan_lambda = (n * sum_sy - sum_s * sum_y) / denom;
        y0 = (sum_y - tan_lambda * sum_s) / n;
    }
    else
    {
        // Degenerate: all arc lengths equal (single point or zero R)
        y0 = sum_y / n;
        tan_lambda = 0.;
    }

    // Compute R² quality metric
    double y_mean = sum_y / n;
    double ss_res = 0., ss_tot = 0.;
    for(size_t i = 0; i < n; i++)
    {
        double y_pred = y0 + tan_lambda * arc_lengths[i];
        ss_res += (hits.at(i)->GetY() - y_pred) * (hits.at(i)->GetY() - y_pred);
        ss_tot += (hits.at(i)->GetY() - y_mean) * (hits.at(i)->GetY() - y_mean);
    }
    double r_squared = (ss_tot > 1e-12) ? 1. - ss_res / ss_tot : 0.;

    return {y0, tan_lambda, r_squared};
}

void RiemannFitter::Fit3D(const TrkHitSPVec &hits)
{
    auto s = GetSign(hits);

    std::vector<double> arc_lengths = ComputeArcLengths(hits, s);
    auto [y0, tan_lambda, r_squared] = FitYvsArcLength(arc_lengths, hits);

    y0_ = y0;
    tan_lambda_ = tan_lambda;
    y_chi2_ = r_squared;  // higher = better linear fit
}

void RiemannFitter::Fill(const TrkHitSPVec& hits)
{
    auto s = GetSign(hits);
    double y = 0.5*(hits.at(0)->GetY() + hits.at(dim_ - 1)->GetY());
    double z = 0.5*(hits.at(0)->GetZ() + hits.at(dim_ - 1)->GetZ());
    double x = -s*sqrt(pre_R_*pre_R_ - (z - pre_Yc_)*(z - pre_Yc_)) + pre_Xc_;
    pp_ = 0.3*abs(RiemannFitHelper::GetMagnetY(x, y, z)*sqrt(1 - n3_*n3_*n3_*n3_ - 4*c_*n3_)*0.5/n3_);

    // Compute transverse momentum vector at first hit from circle tangent
    double h0x = hits.at(0)->GetX();
    double h0z = hits.at(0)->GetZ();
    double dx = h0x - pre_Xc_;
    double dz = h0z - pre_Yc_;
    double norm = std::hypot(dx, dz);

    // Transverse momentum components (xz-plane)
    double pT_x = (norm > 0) ? s * (-dz / norm) * pp_ : pp_;
    double pT_z = (norm > 0) ? s * (dx / norm) * pp_ : 0.;

    // 3D: use dip angle from helix fit to get full momentum
    double cos_lambda = 1. / std::sqrt(1. + tan_lambda_ * tan_lambda_);
    double sin_lambda = tan_lambda_ * cos_lambda;

    double p_total = pp_ / cos_lambda;  // total momentum
    pl_ = p_total * sin_lambda;         // longitudinal (py)

    px_ = pT_x * cos_lambda;  // scale transverse components to total momentum
    py_ = pl_;
    pz_ = pT_z * cos_lambda;

    track_->SetPp(p_total);
    track_->SetPx(px_);
    track_->SetPy(py_);
    track_->SetPz(pz_);
    track_->SetPl(pl_);

    // Extrapolate using propagator if available
    if(propagator_)
    {
        auto [ending_mom, ending_pos] = this->ExtrapolateToPlane(hits.at(0)->GetZ());
        x_sigma_ = ending_pos[0] - hits.at(0)->GetX();
        y_sigma_ = ending_pos[1] - hits.at(0)->GetY();
        track_->SetXSigma(x_sigma_);
        track_->SetYSigma(y_sigma_);
    }
}

//................................................................................//
//Getter
double RiemannFitter::GetTheta(const TrkHitSPVec &hits)
{
    double ax = hits.at(0)->GetX() - pre_Xc_;
    double ay = hits.at(0)->GetZ() - pre_Yc_;
    double bx = hits.at(hits.size() - 1)->GetX() - pre_Xc_;
    double by = hits.at(hits.size() - 1)->GetZ() - pre_Yc_;

    double phi = atan((ax*by - ay*bx)/(ax*bx + ay*by));
    pre_theta_ = atan(phi*pre_R_/(hits.at(hits.size() - 1)->GetY() - hits.at(0)->GetX())) + TMath::Pi()/2;

    return pre_theta_;
}

//................................................................................//
//Get hit measurements projected on the paraboloid surface in Cartesian coordinates
TMatrixD RiemannFitter::GetCartCoo(const TrkHitSPVec &hits)
{
    int s = GetSign(hits);	
    TArrayD data(3*dim_);
    for (int i = 0; i < dim_; i++)
    {
        double u = hits.at(i)->GetX() - pre_Xc_ + std::abs(corrections_x_[i])*s;
        double v = hits.at(i)->GetZ() - pre_Yc_;
        data[i] = u;
        data[i + dim_] = v;
        data[i + 2*dim_] = u*u + v*v;
    }

    TMatrixD cart_coo(3, dim_);
    cart_coo.SetMatrixArray(data.GetArray());

    return cart_coo;
}

//................................................................................//
//Get hit measurements projected on the paraboloid surface in polar coordinates
TMatrixD RiemannFitter::GetPolarCoo(const TrkHitSPVec &hits)
{
    int s = GetSign(hits);
    TArrayD data(2*dim_);
    for (int i = 0; i < dim_; i++)
    {
        double u = hits.at(i)->GetX() - pre_Xc_ + std::abs(corrections_x_[i])*s;
        double v = hits.at(i)->GetZ() - pre_Yc_;
        data[i] = sqrt(u*u + v*v);
        data[i + dim_] = TMath::ATan2(v, u);
    }

    TMatrixD polar_coo(2, dim_);
    polar_coo.SetMatrixArray(data.GetArray());

    return polar_coo;
}

//................................................................................//
//Get multiple scattering covariance matrix
TMatrixD RiemannFitter::GetVradms(const TMatrixD &polar_coo)
{
    std::function<double(double)> ms_error_func = RiemannFitHelper::GetMultipleScatteringError;

    TArrayD data(dim_*dim_);
    for (int i = 0; i < dim_; i++)
    {
        for (int j = 0; j < dim_; j++)
        {
            data[j + i*dim_] = GetVradmsIJ(polar_coo,
                                           i, // i
                                           j, // j
                                           ms_error_func,
                                           0.3*magnet_at_origin_*pre_R_ // momentum, MeV
                                          );
        }
    }

    TMatrixD v_radms(dim_, dim_);
    v_radms.SetMatrixArray(data.GetArray());

    return v_radms;
}

//................................................................................//
//Get covariance matrix of measurement in Cartesian coordinates
TMatrixD RiemannFitter::GetVcart0()
{
    TArrayD data(4*dim_*dim_);
    for (int i = 0; i < dim_; i++)
    {
        for(int j = 0; j < dim_; j++)
        {
            if(i == j)
            {
                data[j + 2*i*dim_] = RiemannFitHelper::GetMeasurementError(tracking::dX)*
                                     RiemannFitHelper::GetMeasurementError(tracking::dX);
                data[j + dim_ + 2*(i + dim_)*dim_] = RiemannFitHelper::GetMeasurementError(tracking::dZ)*
                                                     RiemannFitHelper::GetMeasurementError(tracking::dZ);
            }
        }
    }

    TMatrixD v_cart0(2*dim_, 2*dim_);
    v_cart0.SetMatrixArray(data.GetArray());

    return v_cart0;
}
//................................................................................//
//Get delta x
std::vector<double> RiemannFitter::GetDeltax(const TrkHitSPVec &hits)    
{
    double pT = 0.3 * magnet_at_origin_ * pre_R_; // momentum, MeV

    std::vector<double> Xk(dim_, 0.0);
    std::vector<double> Yk(dim_, 0.0);
    std::vector<double> Zk(dim_, 0.0);
    std::vector<double> Bk(dim_, 0.0);
    std::vector<double> Ak(dim_, 0.0);
    std::vector<double> deltaSinak(dim_, 0.0);
    std::vector<double> deltaXk(dim_, 0.0);

    for (int i = 0; i < dim_; i++)
    {
        Xk[i] = hits.at(i)->GetX();
        Yk[i] = hits.at(i)->GetY();
        Zk[i] = hits.at(i)->GetZ();
        Bk[i] = RiemannFitHelper::GetMagnetY(Xk[i], Yk[i], Zk[i]);
        if(i == 0)
            Ak[i] = 0.0;
        else
            Ak[i] = 0.5 * (Bk[i] + Bk[i-1] - 2 * Bk[0]) * (Zk[i] - Zk[i-1]);

        double temp = 0.0;
        for(int j = 0; j < i + 1; j++)
            temp = temp + Ak[j];
        deltaSinak[i] = 0.3/pT*temp;

        if(i == 0)
            deltaXk[i] = 0;
        else
            deltaXk[i] = (Zk[i] - Zk[i - 1])*(deltaSinak[i] + deltaSinak[i - 1])*0.5;
    }

    return deltaXk;
}

//..............................................................................//
//Get covariance matrix of dx in Cartesian coordinates 
TMatrixD RiemannFitter::GetVcartx(const TrkHitSPVec &hits)
{
    TArrayD data(4*dim_*dim_);
    const auto &D = corrections_x_;
    for(int i = 0; i < dim_; i++)
    {
        for(int j = 0; j < dim_; j++)
        {
            if(i == j)
                data[j + 2*i*dim_] = D.at(i)*D.at(i);
        }
    }

    TMatrixD v_cartx(2*dim_, 2*dim_);
    v_cartx.SetMatrixArray(data.GetArray());

    return v_cartx;
}


//................................................................................//
//Get Jacobian matrix from Cartesian to polar coordinate
TMatrixD RiemannFitter::GetJ1(const TrkHitSPVec &hits)
{
    int s = GetSign(hits);
    TArrayD data(4*dim_*dim_);
    for (int i = 0; i < dim_; i++)
    {
        double u = hits.at(i)->GetX() - pre_Xc_ + std::abs(corrections_x_[i])*s;
        double v = hits.at(i)->GetZ() - pre_Yc_;
        double h = sqrt(u*u + v*v);

        for(Int_t j = 0; j < dim_; j++)
        {
            if(i == j)
            {
                data[j + 2*i*dim_] = u/h;
                data[j + dim_ + 2*i*dim_] = v/h;
                data[j + 2*(i + dim_)*dim_] = -v/h/h;
                data[j + dim_ + 2*(i + dim_)*dim_] = u/h/h;
            }
            else
            {
                data[j + 2*i*dim_] = 0;
                data[j + dim_ + 2*i*dim_] = 0;
                data[j + 2*(i + dim_)*dim_] = 0;
                data[j + dim_ + 2*(i + dim_)*dim_] = 0;
            }
        }
    }

    TMatrixD j1(2*dim_, 2*dim_);
    j1.SetMatrixArray(data.GetArray());

    return j1;
}

//................................................................................//
//Get Jacobian matrix from R-Φ to RΦ-R
TMatrixD RiemannFitter::GetJ2(const TrkHitSPVec &hits)
{
    int s = GetSign(hits);
    TArrayD data(2*dim_*dim_);
    for (int i = 0; i < dim_; i++)
    {
        double u = hits.at(i)->GetX() - pre_Xc_ + std::abs(corrections_x_[i])*s;
        double v = hits.at(i)->GetZ() - pre_Yc_;

        for(Int_t j = 0; j < dim_; j++)
        {
            if(i == j)
            {
                data[j + 2*i*dim_] = TMath::ATan2(v, u);
                data[j + dim_ + 2*i*dim_] = sqrt(u*u + v*v);
            }
            else
            {
                data[j + 2*i*dim_] = 0;
                data[j + dim_ + 2*i*dim_] = 0;
            }
        }
    }

    TMatrixD j2(dim_, 2*dim_);
    j2.SetMatrixArray(data.GetArray());

    return j2;
}

//................................................................................//
//Get covariance matrix of measurement in RΦ-R coordinate
TMatrixD RiemannFitter::GetVrad0(const TMatrixD &v_cart0, const TMatrixD &j1, const TMatrixD &j2)
{
    TMatrixD v_rad0(j2*j1*v_cart0, TMatrixD::kMultTranspose, j2*j1);
    return v_rad0;
}

//................................................................................//
//Get covariance matrix of dx in RΦ-R coordinate
TMatrixD RiemannFitter::GetVradx(const TMatrixD &v_cartx, const TMatrixD &j1, const TMatrixD &j2)
{
    TMatrixD v_radx(j2*j1*v_cartx, TMatrixD::kMultTranspose, j2*j1);
    return v_radx;
}

//................................................................................//
//Get final covariance matrix
TMatrixD RiemannFitter::GetG(const TMatrixD &v_rad0, const TMatrixD &v_radms, [[maybe_unused]] const TMatrixD &v_radx)
{
    TMatrixD g(v_rad0, TMatrixD::kPlus, v_radms);
    if (g.Determinant() != 0)
        g.Invert();

    return g;
}

//................................................................................//
//Get weights
TMatrixD RiemannFitter::GetW(const TMatrixD &G)
{
    const double *element = G.GetMatrixArray();

    double sum = G.Sum();
    TArrayD data(dim_);
    for(int i = 0; i < dim_; i++)
    {
        double sumj = 0.;
        for(int j = 0; j < dim_; j++)
        {
            sumj += *(element + j + i*dim_);
        }
        data[i] = sumj/sum;
    }

    TMatrixD w(1, dim_);
    w.SetMatrixArray(data.GetArray());

    return w;
}

//................................................................................//
//Get weighted center
TMatrixD RiemannFitter::GetXc(const TMatrixD &cart_coo, const TMatrixD &w)
{
    TMatrixD x_c(cart_coo, TMatrixD::kMultTranspose, w);
    return x_c;
}

//................................................................................//
//Get weighted center
TMatrixD RiemannFitter::GetXg(const TMatrixD &cart_coo, const TMatrixD &x_c)
{
    TMatrixD x_g(cart_coo);
    const double *element = x_c.GetMatrixArray();
    double xc = *element;
    double yc = *(element + 1);
    double hc = *(element + 2);

    TMatrixDRow(x_g, 0) += -xc;
    TMatrixDRow(x_g, 1) += -yc;
    TMatrixDRow(x_g, 2) += -hc;

    return x_g;
}

//................................................................................//
//Get normal vector of fitteed plane
TMatrixD RiemannFitter::GetNormalVecs(const TMatrixD &g, const TMatrixD &x_g)
{
    TMatrixD xgx(x_g*g, TMatrixD::kMultTranspose, x_g);
    const double *element = xgx.GetMatrixArray();
    double data[9] = {0.};

    for(int i = 0; i < 9; i++)
        data[i] = *(element + i);

    TMatrixDSym sym(3, data);
    sym.SetTol(1e-9);
    TMatrixDSymEigen sym_eigen_v(sym);
    TMatrixD eigen_vec = sym_eigen_v.GetEigenVectors();

    return eigen_vec;
}
