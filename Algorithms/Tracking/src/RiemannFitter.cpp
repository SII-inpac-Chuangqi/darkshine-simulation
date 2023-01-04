//................................................................................//
//CPP STL
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

//................................................................................//
//ROOT
#include "TMath.h"
#include "TString.h"
#include "TGeoManager.h"
#include "TArrayD.h"
#include "TMatrixDSym.h"
#include "TMatrixDSymEigen.h"

//................................................................................//
//Framework

//................................................................................//
//Tracking
#include "Algo/TrkHit.h"
#include "Algo/RiemannFitter.h"

//................................................................................//
//Constructor
RiemannFitter::RiemannFitter(const TrkHitPVec &track,
                               std::initializer_list<double> list)
{
    auto it = list.begin();
    preR = *it;  it++;
    preXc = *it; it++;
    preYc = *it; it++;
    double B = *it;

    try
    {
        if(track.size() < 3) throw -1;

        Init(track, {});
        Fit(track,  {});
        Fill(track, {B});
    }
    catch(int e)
    {
        std::cerr <<"Exception, next track" << std::endl;
        pp = 0.3*B*preR;
    }
}

//................................................................................//
//Processor
//................................................................................//
//Initialize the fitter, set up magnetic, material manager, track representation, fitter and track model
void RiemannFitter::Init(const TrkHitPVec &track, std::initializer_list<double>)
{
    dim = track.size();
    preTheta = GetTheta(track);
}

//................................................................................//
//Do the fit
void RiemannFitter::Fit(const TrkHitPVec &track, std::initializer_list<double>)
{
    for(int i = 0; i < 100; i++)
    {
        TMatrixD CartCoo(GetCartCoo(track));
        //CartCoo.Print();
        TMatrixD PolarCoo(GetPolarCoo(track));
        //PolarCoo.Print();
    
        TMatrixD Vcart0(GetVcart0());
        //Vcart0.Print();
        TMatrixD J1(GetJ1(track));
        //J1.Print();
        TMatrixD J2(GetJ2(track));
        //J2.Print();
        TMatrixD Vrad0(GetVrad0(Vcart0, J1, J2));
        //Vrad0.Print();
        TMatrixD Vradms(GetVradms(PolarCoo));
        //Vradms.Print();
        TMatrixD G(GetG(Vrad0, Vradms));
        //G.Print();
        TMatrixD W(GetW(G));
        //W.Print();
        TMatrixD Xc(GetXc(CartCoo, W));
        //Xc.Print();
        TMatrixD Xg(GetXg(CartCoo, Xc));
        //Xg.Print();
        TMatrixD NVecs(GetNVecs(G, Xg));
        //NVecs.Print();
        const double *getXc = Xc.GetMatrixArray();
        const double *getNVec = NVecs.GetMatrixArray();
        c = - *getXc**getNVec
            - *(getXc + 1)**(getNVec + 3)
            - *(getXc + 2)**(getNVec + 6);
        n1 = *getNVec;
        n2 = *(getNVec + 3);
        n3 = *(getNVec + 6);
        //std::cout << -0.5*n1/n3 << std::endl;
        //std::cout << -0.5*n2/n3 << std::endl;

        if((preXc - abs(n1/n3)/abs(preXc)) < 1e-6 &&
           (preYc - abs(n2/n3))/abs(preYc) < 1e-6 &&
           abs(preR - abs(sqrt(1 - n3*n3*n3*n3 - 4*c*n3)*0.5/n3))/preR < 1e-6)
        {
            //fChi2 = 
            break;
        }
        else
        {
            preXc += -0.5*n1/n3;
            preYc += -0.5*n2/n3;
            preR = abs(sqrt(1 - n3*n3*n3*n3 - 4*c*n3)*0.5/n3);
        }
    }


}

//................................................................................//
//Fill results
void RiemannFitter::Fill(const TrkHitPVec&, std::initializer_list<double> list)
{
    auto it = list.begin();
    double B = *it;

    pp = 0.3*abs(B*sqrt(1 - n3*n3*n3*n3 - 4*c*n3)*0.5/n3);
}

//................................................................................//
//Get
//................................................................................//
//Calculate helix slope from prefitting
double RiemannFitter::GetTheta(const TrkHitPVec &track)
{
    double ax = track.at(0)->GetX() - preXc;
    double ay = track.at(0)->GetZ() - preYc;
    double bx = track.at(track.size() - 1)->GetX() - preXc;
    double by = track.at(track.size() - 1)->GetZ() - preYc;

    double phi = atan((ax*by - ay*bx)/(ax*bx + ay*by));
    preTheta = atan(phi*preR/(track.at(track.size() - 1)->GetY() - track.at(0)->GetX())) + TMath::Pi()/2;

    return preTheta;
}

//................................................................................//
//Paraboloid projection in Cartesian coordinate
TMatrixD RiemannFitter::GetCartCoo(const TrkHitPVec &track)
{
    TArrayD data(3*dim);
    for (int i = 0; i < dim; i++)
    {
        double u = track.at(i)->GetX() - preXc;
        double v = track.at(i)->GetZ() - preYc;
        data[i] = u;
        data[i + dim] = v;
        data[i + 2*dim] = u*u + v*v;
    }

    TMatrixD CartCoo(3, dim);
    CartCoo.SetMatrixArray(data.GetArray());

    return CartCoo;    
}

//................................................................................//
//Paraboloid projection in Polar coordinate
TMatrixD RiemannFitter::GetPolarCoo(const TrkHitPVec &track)
{
    TArrayD data(2*dim);
    for (int i = 0; i < dim; i++)
    {
        double u = track.at(i)->GetX() - preXc;
        double v = track.at(i)->GetZ() - preYc;
        data[i] = sqrt(u*u + v*v);
        data[i + dim] = TMath::ATan2(v, u);
        
    }

    TMatrixD PolarCoo(2, dim);
    PolarCoo.SetMatrixArray(data.GetArray());

    return PolarCoo;
}

//................................................................................//
//Covariance matrix of measurement in Cartesian coordinate
TMatrixD RiemannFitter::GetVcart0()
{
    TArrayD data(4*dim*dim);
    for (int i = 0; i < dim; i++)
    {
        for(int j = 0; j < dim; j++)
        {
            if(i == j)
            {
                data[j + 2*i*dim] = 6*1e-3;             //resolution x: 6μm->mm
                data[j + dim + 2*i*dim] = 0;
                data[j + 2*(i + dim)*dim] = 0;
                data[j + dim + 2*(i + dim)*dim] = 0; //resolution z: should be 0 ideally
            }
            else
            {
                data[j + 2*i*dim] = 0;
                data[j + dim + 2*i*dim] = 0;
                data[j + 2*(i + dim)*dim] = 0;
                data[j + dim + 2*(i + dim)*dim] = 0;
            }
        }
    }

    TMatrixD Vcart0(2*dim, 2*dim);
    Vcart0.SetMatrixArray(data.GetArray());

    return Vcart0;
}

//................................................................................//
//Jacobian matrix from Cartesian to polar coordinate
TMatrixD RiemannFitter::GetJ1(const TrkHitPVec &track)
{
    TArrayD data(4*dim*dim);
    for (int i = 0; i < dim; i++)
    {
        double u = track.at(i)->GetX() - preXc;
        double v = track.at(i)->GetZ() - preYc;
        double h = sqrt(u*u + v*v);

        for(Int_t j = 0; j < dim; j++)
        {
            if(i == j)
            {
                data[j + 2*i*dim] = u/h;
                data[j + dim + 2*i*dim] = v/h;
                data[j + 2*(i + dim)*dim] = -v/h/h;
                data[j + dim + 2*(i + dim)*dim] = u/h/h;
            }
            else
            {
                data[j + 2*i*dim] = 0;
                data[j + dim + 2*i*dim] = 0;
                data[j + 2*(i + dim)*dim] = 0;
                data[j + dim + 2*(i + dim)*dim] = 0;
            }
        }
    }

    TMatrixD J1(2*dim, 2*dim);
    J1.SetMatrixArray(data.GetArray());

    return J1;
}

//................................................................................//
//Jacobian matrix from R-Φ to RΦ-R
TMatrixD RiemannFitter::GetJ2(const TrkHitPVec &track)
{
    TArrayD data(2*dim*dim);
    for (int i = 0; i < dim; i++)
    {
        double u = track.at(i)->GetX() - preXc;
        double v = track.at(i)->GetZ() - preYc;

        for(Int_t j = 0; j < dim; j++)
        {
            if(i == j)
            {
                data[j + 2*i*dim] = TMath::ATan2(v, u);
                data[j + dim + 2*i*dim] = sqrt(u*u + v*v);
            }
            else
            {
                data[j + 2*i*dim] = 0;
                data[j + dim + 2*i*dim] = 0;
            }
        }
    }

    TMatrixD J2(dim, 2*dim);
    J2.SetMatrixArray(data.GetArray());

    return J2;
}

//................................................................................//
//Covariance matrix of measurement in RΦ-R coordinate
TMatrixD RiemannFitter::GetVrad0(const TMatrixD &Vcart0,
                                  const TMatrixD &J1, const TMatrixD &J2)
{
    TMatrixD Vrad0(J2*J1*Vcart0, TMatrixD::kMultTranspose, J2*J1);
    return Vrad0;
}

//................................................................................//
//Variance from multiple scattering
double RiemannFitter::GetSigma()
{
    return 13.6/8000*sqrt(0.1/93.70)*(1 + 0.038*log(0.1/93.70));
}

//................................................................................//
//Element of ovariance matrix of multiple scattering
double RiemannFitter::GetVradmsIJ(const TMatrixD &PolarCoo, int i, int j)
{
    double vradmsIJ = 0.;

    double ii = 0., jj = 0.;
    const double *element = PolarCoo.GetMatrixArray();
    ii = *(element + i);
    jj = *(element + j);

    int kMax = (i < j) ? i : j;
    for(int k = 0; k < kMax; k++)
    {
        double kk = *(element + k);
        vradmsIJ += (ii - kk)*(jj - kk)*GetSigma()*GetSigma()/sin(preTheta)/sin(preTheta);
    }

    return vradmsIJ;
}

//................................................................................//
//Covariance matrix of multiple scattering
TMatrixD RiemannFitter::GetVradms(const TMatrixD &PolarCoo)
{
    TArrayD data(dim*dim);

    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            data[j + i*dim] = GetVradmsIJ(PolarCoo, i, j);
        }
    }

    TMatrixD Vradms(dim, dim);
    Vradms.SetMatrixArray(data.GetArray());

    return Vradms;
}

//................................................................................//
//Weight
TMatrixD RiemannFitter::GetG(const TMatrixD &Vrad0, const TMatrixD &Vradms)
{
    TMatrixD G(Vrad0, TMatrixD::kPlus, Vradms);
    G.Invert();

    return G;
}

//................................................................................//
//Weight of center of gravity
TMatrixD RiemannFitter::GetW(const TMatrixD &G)
{
    const double *element = G.GetMatrixArray();

    double sum = G.Sum();
    TArrayD data(dim);
    for(int i = 0; i < dim; i++)
    {
        double sumj = 0.;
        for(int j = 0; j < dim; j++)
        {
            sumj += *(element + j + i*dim);
        }
        data[i] = sumj/sum;
    }
    
    TMatrixD W(1, dim);
    W.SetMatrixArray(data.GetArray());

    return W;
}

//................................................................................//
//Center of gravity
TMatrixD RiemannFitter::GetXc(const TMatrixD &CartCoo, const TMatrixD &W)
{
    TMatrixD Xc(CartCoo, TMatrixD::kMultTranspose, W);
    return Xc;
}

//................................................................................//
//Center of gravity
TMatrixD RiemannFitter::GetXg(const TMatrixD &CartCoo, const TMatrixD &Xc)
{
    TMatrixD Xg(CartCoo);
    const double *element = Xc.GetMatrixArray();
    double xc = *element;
    double yc = *(element + 1);
    double hc = *(element + 2);

    TMatrixDRow(Xg, 0) += -xc;
    TMatrixDRow(Xg, 1) += -yc;
    TMatrixDRow(Xg, 2) += -hc;

    return Xg;
}

//................................................................................//
//Normal vector of fitteed plane
TMatrixD RiemannFitter::GetNVecs(const TMatrixD &G, const TMatrixD &Xg)
{
    TMatrixD XGX(Xg*G, TMatrixD::kMultTranspose, Xg);
    const double *element = XGX.GetMatrixArray();
    double data[9] = {0.};

    for(int i = 0; i < 9; i++)
        data[i] = *(element + i);
    
    TMatrixDSym Sym(3, data);
    TMatrixDSymEigen SymEigenV(Sym);
    TMatrixD EigenVec = SymEigenV.GetEigenVectors();

    return EigenVec;
}
