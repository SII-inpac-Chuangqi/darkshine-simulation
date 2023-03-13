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
RiemannFitter::RiemannFitter(const TrkHitPVec &track, std::initializer_list<double> list)
{
    this->Init(track, list);
    this->Fit (track, {});
    this->Fill(track, {});
}

//................................................................................//
//Processor
void RiemannFitter::Init(const TrkHitPVec &track, std::initializer_list<double> list)
{
    auto it = list.begin();
    pre_Xc_ = *it; it++;
    pre_Yc_ = *it; it++;
    pre_R_  = *it;

    dim_ = track.size();
    this->GetTheta(track);
}

void RiemannFitter::Fit(const TrkHitPVec &track, std::initializer_list<double>)
{
    c_ = 0.;
    n1_ = 0.;
    n2_ = 0.;
    n3_ = 0.;

    for(int i = 0; i < 100; i++)
    {
        //std::cout << pre_Xc_ << std::endl;
        //std::cout << pre_Yc_ << std::endl;
        //std::cout << pre_R_ << std::endl;

        TMatrixD cart_coo(GetCartCoo(track));
        //cart_coo.Print();
        TMatrixD polar_coo(GetPolarCoo(track));
        //polar_coo.Print();
        
        TMatrixD v_cart0(GetVcart0());
        //v_cart0.Print();
        TMatrixD j1(GetJ1(track));
        //j1.Print();
        TMatrixD j2(GetJ2(track));
        //j2.Print();
        TMatrixD v_rad0(GetVrad0(v_cart0, j1, j2));
        //v_rad0.Print();
        TMatrixD v_radms(GetVradms(polar_coo));
        //v_radms.Print();
        TMatrixD v_cartx(GetVcartx(track));
        v_cartx.Print();
        TMatrixD g(GetG(v_rad0, v_radms));
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
        if(std::abs(curr_Xc - pre_Xc_)/pre_Xc_ < 1e-6 &&
           std::abs(curr_Yc - pre_Yc_)/pre_Yc_ < 1e-6 &&
           std::abs(curr_R - pre_R_)/pre_R_ < 1e-6)
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

void RiemannFitter::Fill(const TrkHitPVec&, std::initializer_list<double>)
{
    pp = 0.3*abs(RiemannFitHelper::GetMagnetAtOrigin(tracking::dY)*sqrt(1 - n3_*n3_*n3_*n3_ - 4*c_*n3_)*0.5/n3_);
}

//................................................................................//
//Getter
double RiemannFitter::GetTheta(const TrkHitPVec &track)
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
TMatrixD RiemannFitter::GetCartCoo(const TrkHitPVec &track)
{
    TArrayD data(3*dim_);
    for (int i = 0; i < dim_; i++)
    {
        double u = track.at(i)->GetX() - pre_Xc_;
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
TMatrixD RiemannFitter::GetPolarCoo(const TrkHitPVec &track)
{
    TArrayD data(2*dim_);
    for (int i = 0; i < dim_; i++)
    {
        double u = track.at(i)->GetX() - pre_Xc_;
        double v = track.at(i)->GetZ() - pre_Yc_;
        data[i] = sqrt(u*u + v*v);
        data[i + dim_] = TMath::ATan2(v, u);
    }

    TMatrixD polar_coo(2, dim_);
    polar_coo.SetMatrixArray(data.GetArray());

    return polar_coo;
}

//................................................................................//
//Get multiple scattering covariance matrix element
double RiemannFitter::GetVradmsIJ(const TMatrixD &polar_coo, int i, int j,
                                  const double &p, // Momentum, MeV
                                  double (*MultipleScatteringError)(const double &p))
{
    double sigma_ms = MultipleScatteringError(p);

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
TMatrixD RiemannFitter::GetVradms(const TMatrixD &polar_coo)
{
    TArrayD data(dim_*dim_);
    for (int i = 0; i < dim_; i++)
    {
        for (int j = 0; j < dim_; j++)
        {
            data[j + i*dim_] = GetVradmsIJ(polar_coo,
                                           i, // i
                                           j, // j

                                           0.3*RiemannFitHelper::GetMagnetAtOrigin(tracking::dY)*pre_R_, // momentum, MeV
                                           RiemannFitHelper::GetMultipleScatteringError);
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

//..............................................................................//
//Get covariance matrix of dx in Cartesian coordinates 
TMatrixD RiemannFitter::GetVcartx(const TrkHitPVec &track)
{
    double pT=0.3 * RiemannFitHelper::GetMagnetAtOrigin(tracking::dY) * pre_R_; // momentum, MeV    

    double Zk[dim_];
    double Bk[dim_];
    double Ak[dim_];
    double deltaSinak[dim_];
    double deltaXk[dim_];

    for (int i =0; i < dim_; i++)
    {
        Zk[i] = track.at(i)->GetZ() - pre_Yc_;
        Bk[i] = RiemannFitHelper::GetMagnetY(0.0, 0.0, Zk[i]);
        if(i==0)
        {
            Ak[i]=0.0;
        }
        else
        {
            Ak[i]=(Bk[i-1] - Bk[0])*(Zk[i] - Zk[i-1]) - 0.5 * (Bk[i] - Bk[i-1]) * (Zk[i] + Zk[i-1]);
        }
        
        double temp = 0.0;
        for (int j =0; j <i+1; j++)
        {
            temp = temp + Ak[j];
        }
        deltaSinak[i] = 0.3 / pT * temp;

        if(i==0)
        {
        deltaXk[i] = 0;
        }
        else
        {
            deltaXk[i] = (Zk[i] - Zk[i-1]) * (deltaSinak[i] + deltaSinak[i-1]) * 0.5;
        }
    }


    TArrayD data(4*dim_*dim_);
    for (int i = 0; i < dim_; i++)
    {
        for (int j = 0; j < dim_; j++)
        {
            if(i==j)
            {
                data[j + 2*i*dim_] = deltaXk[i] * deltaXk[i];

            }
        }
    }

    TMatrixD v_cartx(2*dim_, 2*dim_);
    v_cartx.SetMatrixArray(data.GetArray());

    return v_cartx;
}


//................................................................................//
//Get Jacobian matrix from Cartesian to polar coordinate
TMatrixD RiemannFitter::GetJ1(const TrkHitPVec &track)
{
    TArrayD data(4*dim_*dim_);
    for (int i = 0; i < dim_; i++)
    {
        double u = track.at(i)->GetX() - pre_Xc_;
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
TMatrixD RiemannFitter::GetJ2(const TrkHitPVec &track)
{
    TArrayD data(2*dim_*dim_);
    for (int i = 0; i < dim_; i++)
    {
        double u = track.at(i)->GetX() - pre_Xc_;
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
TMatrixD RiemannFitter::GetG(const TMatrixD &v_rad0, const TMatrixD &v_radms)
{
    TMatrixD g(v_rad0, TMatrixD::kPlus, v_radms);
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
    TMatrixDSymEigen sym_eigen_v(sym);
    TMatrixD eigen_vec = sym_eigen_v.GetEigenVectors();

    return eigen_vec;
}
