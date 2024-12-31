#include "Algo/RiemannFit/RiemannFitter.h"

//................................................................................//
//ROOT
#include "TMath.h"
#include "TArrayD.h"
#include "TMatrixDSymEigen.h"

//................................................................................//
//Tracking
#include "Algo/RiemannFit/RiemannFitHelper.h"

//................................................................................//
//Constructor
RiemannFitter::RiemannFitter(const TrkHitSPVec &track, Config config, int verbose) : config_(config)
{
    verbose_ = verbose;

    pre_Xc_ = config_.pre_Xc;
    pre_Yc_ = config_.pre_Yc;
    pre_R_ = config_.pre_R; 

    try
    {
        this->Init(track, {});
        this->Fit (track, {});
        this->Fill(track, {});
    }
    catch(...)
    {
        std::cerr << "[Error] ==> Unexpected error in RiemannFitter" << std::endl;
        pp = 0.3*std::abs(config_.const_B)*config_.pre_R;
        return;
    }
}

//................................................................................//
//Processor
void RiemannFitter::Init(const TrkHitSPVec &track, std::initializer_list<double> /*list*/)
{
    dim_ = track.size();
    corrections_x_ = this->GetDeltax(track);
    this->GetTheta(track);
}

void RiemannFitter::Fit(const TrkHitSPVec &track, std::initializer_list<double>)
{
    c_ = 0.;
    n1_ = 0.;
    n2_ = 0.;
    n3_ = 0.;

    for(int i = 0; i < config_.max_trial; i++)
    {
        TMatrixD cart_coo(GetCartCoo(track));
        //cart_coo.Print();
        TMatrixD polar_coo(GetPolarCoo(track));
        //polar_coo.Print();
        
        TMatrixD v_cart0(GetVcart0());
        //v_cart0.Print();
        TMatrixD v_cartx(GetVcartx(track));
        //v_cartx.Print();
        TMatrixD j1(GetJ1(track));
        //j1.Print();
        TMatrixD j2(GetJ2(track));
        //j2.Print();
        TMatrixD v_rad0(GetVrad0(v_cart0, j1, j2));
        //v_rad0.Print();
        TMatrixD v_radms(GetVradms(polar_coo));
        //v_radms.Print();
        TMatrixD v_radx(GetVradx(v_cartx, j1, j2));
        //v_radx.Print();
        TMatrixD g(GetG(v_rad0, v_radms, v_radx));
        //g.Print();
        TMatrixD w(GetW(g));
        //w.Print();
        TMatrixD x_c(GetXc(cart_coo, w));
        //x_c.Print();
        TMatrixD x_g(GetXg(cart_coo, x_c));
        //x_g.Print();
        TMatrixD normal_vecs(GetNormalVecs(g, x_g));
        //normal_vecs.Print();

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

//        std::cout << i << ": " << std::endl;
//        std::cout << " -- " << pre_Xc_ << ", \t" << curr_Xc << ", \t" << std::abs(curr_Xc/(pre_Xc_ + curr_Xc)) << std::endl;
//        std::cout << " -- " << pre_Yc_ << ", \t" << curr_Yc << ", \t" << std::abs(curr_Yc/(pre_Yc_ + curr_Yc)) << std::endl;
//        std::cout << " -- " << pre_R_  << ", \t" << curr_R  << ", \t" << std::abs((curr_R - pre_R_)/pre_R_) << std::endl;

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

    //std::cout << 0.3*abs(RiemannFitHelper::GetMagnetAtOrigin(tracking::dY)*sqrt(1 - n3_*n3_*n3_*n3_ - 4*c_*n3_)*0.5/n3_) << " MeV" << std::endl;
}

void RiemannFitter::Fill(const TrkHitSPVec& track, std::initializer_list<double>)
{
    auto s = GetSign(track);
    double y = 0.5*(track.at(0)->GetY() + track.at(dim_ - 1)->GetY());
    double z = 0.5*(track.at(0)->GetZ() + track.at(dim_ - 1)->GetZ());
    double x = -s*sqrt(pre_R_*pre_R_ - (z - pre_Yc_)*(z - pre_Yc_)) + pre_Xc_;
    //double y = track.at(0)->GetY();
    //double z = track.at(0)->GetZ();
    //double x = track.at(0)->GetX();
    //std::cout << 0.5*(track.at(0)->GetX() + track.at(dim_ - 1)->GetX()) << "\t" << x << std::endl;
    pp = 0.3*abs(RiemannFitHelper::GetMagnetY(x, y, z)*sqrt(1 - n3_*n3_*n3_*n3_ - 4*c_*n3_)*0.5/n3_);
}

//................................................................................//
//Getter
double RiemannFitter::GetTheta(const TrkHitSPVec &track)
{
    double ax = track.at(0)->GetX() - pre_Xc_;
    double ay = track.at(0)->GetZ() - pre_Yc_;
    double bx = track.at(track.size() - 1)->GetX() - pre_Xc_;
    double by = track.at(track.size() - 1)->GetZ() - pre_Yc_;

    double phi = atan((ax*by - ay*bx)/(ax*bx + ay*by));
    pre_theta_ = atan(phi*pre_R_/(track.at(track.size() - 1)->GetY() - track.at(0)->GetX())) + TMath::Pi()/2;

    return pre_theta_;
}

//................................................................................//
//Get hit measurements projected on the paraboloid surface in Cartesian coordinates
TMatrixD RiemannFitter::GetCartCoo(const TrkHitSPVec &track)
{
    int s = GetSign(track);	
    TArrayD data(3*dim_);
    for (int i = 0; i < dim_; i++)
    {
        double u = track.at(i)->GetX() - pre_Xc_ + std::abs(GetDeltax(track)[i])*s;
        //double u = track.at(i)->GetX() - pre_Xc_;
        double v = track.at(i)->GetZ() - pre_Yc_;
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
TMatrixD RiemannFitter::GetPolarCoo(const TrkHitSPVec &track)
{
    int s = GetSign(track);
    TArrayD data(2*dim_);
    for (int i = 0; i < dim_; i++)
    {
        double u = track.at(i)->GetX() - pre_Xc_ + std::abs(GetDeltax(track)[i])*s;
        double v = track.at(i)->GetZ() - pre_Yc_;
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
                                           0.3*RiemannFitHelper::GetMagnetAtOrigin(tracking::dY)*pre_R_ // momentum, MeV
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
                data[j + 2*i*dim_] = RiemannFitHelper::GetMeasurementError(tracking::dX)*                 //variance of x, mm*mm
                                     RiemannFitHelper::GetMeasurementError(tracking::dX);
                data[j + dim_ + 2*(i + dim_)*dim_] = RiemannFitHelper::GetMeasurementError(tracking::dZ)* //variance of z, mm
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
std::vector<double> RiemannFitter::GetDeltax(const TrkHitSPVec &track)    
{
    double pT = 0.3 * RiemannFitHelper::GetMagnetAtOrigin(tracking::dY) * pre_R_; // momentum, MeV

    std::vector<double> Xk(dim_, 0.0);
    std::vector<double> Yk(dim_, 0.0);
    std::vector<double> Zk(dim_, 0.0);
    std::vector<double> Bk(dim_, 0.0);
    std::vector<double> Ak(dim_, 0.0);
    std::vector<double> deltaSinak(dim_, 0.0);
    std::vector<double> deltaXk(dim_, 0.0);

    for (int i = 0; i < dim_; i++)
    {
        Xk[i] = track.at(i)->GetX();
        Yk[i] = track.at(i)->GetY();
        Zk[i] = track.at(i)->GetZ();
        Bk[i] = RiemannFitHelper::GetMagnetY(Xk[i], Yk[i], Zk[i]);
        if(i == 0)
            Ak[i] = 0.0;
        else
//            Ak[i]=(Bk[i-1] - Bk[0]) * (Zk[i] - Zk[i-1]) + 0.5 * (Bk[i] - Bk[i-1]) * (Zk[i] + Zk[i-1]) - (Bk[i] - Bk[i-1]) * Zk[i-1];
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

//    TArrayD data(dim_);
//    for (int i = 0; i<dim_; i++)
//    {
//        data[i]=deltaXk[i];
//    }

    return deltaXk;
}

//..............................................................................//
//Get covariance matrix of dx in Cartesian coordinates 
TMatrixD RiemannFitter::GetVcartx(const TrkHitSPVec &track)
{
    TArrayD data(4*dim_*dim_);
    auto D = GetDeltax(track);
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
TMatrixD RiemannFitter::GetJ1(const TrkHitSPVec &track)
{
    int s = GetSign(track);
    TArrayD data(4*dim_*dim_);
    for (int i = 0; i < dim_; i++)
    {
        double u = track.at(i)->GetX() - pre_Xc_ + std::abs(GetDeltax(track)[i])*s;
        double v = track.at(i)->GetZ() - pre_Yc_;
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
TMatrixD RiemannFitter::GetJ2(const TrkHitSPVec &track)
{
    int s = GetSign(track);
    TArrayD data(2*dim_*dim_);
    for (int i = 0; i < dim_; i++)
    {
        double u = track.at(i)->GetX() - pre_Xc_ + std::abs(GetDeltax(track)[i])*s;
        double v = track.at(i)->GetZ() - pre_Yc_;

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
    //f.Invert();
    //TMatrixD g(f, TMatrixD::kPlus, v_radx);
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
