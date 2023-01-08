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
    TMatrixD CartCoo(GetCartCoo(track));
    TMatrixD polar_coo;
    this->GetVradmsIJ(polar_coo,
                      0, // i
                      0, // j

                      8000., // momentum, MeV
                      RiemannFitHelper::GetMultipleScatteringError);
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
//Get hit measurements projected on the paraboloid surface in Cartesian coordinate
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

    TMatrixD CartCoo(3, dim_);
    CartCoo.SetMatrixArray(data.GetArray());

    return CartCoo;
}

double RiemannFitter::GetVradmsIJ(const TMatrixD &PolarCoo, int i, int j,
                                  const double &p, // Momentum, MeV
                                  double (*MultipleScatteringError)(const double &p))
{
    double sigma_ms = MultipleScatteringError(p);

    return 0.;
}
