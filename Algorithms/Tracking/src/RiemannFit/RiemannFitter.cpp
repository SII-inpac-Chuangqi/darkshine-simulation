#include "Algo/RiemannFit/RiemannFitter.h"

//................................................................................//
//ROOT
#include "TMath.h"
#include "TArrayD.h"

//................................................................................//
//Tracking
#include "Algo/RiemannFit/RiemannFitHelper.h"

//................................................................................//
//Constructor
RiemannFitter::RiemannFitter(const TrkHitPVec &/* track */, std::initializer_list<double>)
{
}

//................................................................................//
//Processor
void RiemannFitter::Init(const TrkHitPVec &track, std::initializer_list<double>)
{
    dim_ = track.size();
    this->GetTheta(track);
}

void RiemannFitter::Fit(const TrkHitPVec &track, std::initializer_list<double>)
{
    TMatrixD cart_coo(GetCartCoo(track));
    TMatrixD polar_coo(GetPolarCoo(track));
    TMatrixD vradms(GetVradms(polar_coo));
}

void RiemannFitter::Fill(const TrkHitPVec&, std::initializer_list<double>)
{
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

    double vradms_i_j = 0.;
    double ii = 0., jj = 0.;
    const double *element = polar_coo.GetMatrixArray();
    ii = *(element + i);
    jj = *(element + j);

    int k_max = (i < j) ? i : j;
    for(int k = 0; k < k_max; k++)
    {
        double kk = *(element + k);
        vradms_i_j += (ii - kk)*(jj - kk)*sigma_ms*sigma_ms/sin(pre_theta_)/sin(pre_theta_);
    }

    return vradms_i_j;
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

    TMatrixD vradms(dim_, dim_);
    vradms.SetMatrixArray(data.GetArray());

    return vradms;
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
                data[j + 2*i*dim_] = 6*1e-3;          //resolution x: 6μm->mm
                data[j + dim_ + 2*i*dim_] = 0;
                data[j + 2*(i + dim_)*dim_] = 0;
                data[j + dim_ + 2*(i + dim_)*dim_] = 0; //resolution z: should be 0 ideally
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

    TMatrixD vcart0(2*dim_, 2*dim_);
    vcart0.SetMatrixArray(data.GetArray());

    return vcart0;
}
