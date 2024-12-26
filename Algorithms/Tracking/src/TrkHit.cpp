//................................................................................//
//C++
#include <iostream>
#include <map>
#include <vector>

//................................................................................//
//ROOT
#include "TMath.h"
#include "TString.h"

//................................................................................//
//Tracking
#include "Algo/Utils/Util.h"
#include "Algo/TrkHit.h"

//................................................................................//
//TrkHit
//................................................................................//
//Constructor
//................................................................................//
TrkHit::TrkHit(const TrkHit &newTrkHit) : SimulatedHit(newTrkHit),
                                          if_in_track_(newTrkHit.if_in_track_),
                                          u_(newTrkHit.u_),
                                          v_(newTrkHit.v_),
                                          z_err_(newTrkHit.z_err_),
                                          u_vec_(newTrkHit.u_vec_),
                                          v_vec_(newTrkHit.v_vec_),
                                          uv_cov_(newTrkHit.uv_cov_),
                                          xy_cov_(newTrkHit.xy_cov_),
                                          track_(newTrkHit.track_)
{}

TrkHit::TrkHit(TrkHit &&newTrkHit) : SimulatedHit(std::move(newTrkHit)),
                                     if_in_track_(std::move(newTrkHit.if_in_track_)),
                                     u_(std::move(newTrkHit.u_)),
                                     v_(std::move(newTrkHit.v_)),
                                     z_err_(std::move(newTrkHit.z_err_)),
                                     u_vec_(std::move(newTrkHit.u_vec_)),
                                     v_vec_(std::move(newTrkHit.v_vec_)),
                                     uv_cov_(std::move(newTrkHit.uv_cov_)),
                                     xy_cov_(std::move(newTrkHit.xy_cov_)),
                                     track_(std::move(newTrkHit.track_))
{}

TrkHit::TrkHit(const SimulatedHit &newSimuHit) : SimulatedHit(newSimuHit),
                                                 if_in_track_(RETURN),
                                                 u_(RETURN),
                                                 v_(RETURN),
                                                 z_err_(RETURN),
                                                 u_vec_(2),
                                                 v_vec_(2),
                                                 uv_cov_(2),
                                                 xy_cov_(2)
{
    double err = 1e-4;
    TMatrixDSym cov(2);
    cov(0, 0) = err;
    cov(1, 1) = err;
    cov(0, 1) = 0.;
    cov(1, 0) = 0.;

    uv_cov_ = cov;
    xy_cov_ = cov;
}

void TrkHit::operator =(const TrkHit &oldTrkHit)
{
    if(this != &oldTrkHit)
    {
        id = oldTrkHit.id;
        CellID_X = oldTrkHit.CellID_X;
        CellID_Y = oldTrkHit.CellID_Y;
        CellID_Z = oldTrkHit.CellID_Z;
        X = oldTrkHit.X;         Y = oldTrkHit.Y;         Z = oldTrkHit.Z;
        T = oldTrkHit.T;
        E = oldTrkHit.E;

        if_in_track_ = oldTrkHit.if_in_track_;
        u_ = oldTrkHit.u_;       v_ = oldTrkHit.v_;
        z_err_ = oldTrkHit.z_err_;
        u_vec_ = oldTrkHit.u_vec_;
        v_vec_ = oldTrkHit.v_vec_;
        uv_cov_ = oldTrkHit.uv_cov_;
        xy_cov_ = oldTrkHit.xy_cov_;

        track_ = oldTrkHit.track_;
    }
}

void TrkHit::UpdateXY()
{
    TMatrixDSym A(2);
    A(0, 0) = u_vec_[0];
    A(0, 1) = u_vec_[1];
    A(1, 0) = v_vec_[0];
    A(1, 1) = v_vec_[1];

    TMatrixD A_inv = A.Invert();
    TMatrixD A_inv_T = A_inv;
    A_inv_T.T();

    TVectorD uv(2);
    uv[0] = u_;
    uv[1] = v_;

    TVectorD xy = A_inv * uv;
    this->SetX(xy[0]);
    this->SetY(xy[1]);

    auto xy_cov = A_inv * uv_cov_ * A_inv_T;
    xy_cov_(0, 0) = xy_cov(0, 0);
    xy_cov_(0, 1) = xy_cov(0, 1);
    xy_cov_(1, 0) = xy_cov(1, 0);
    xy_cov_(1, 1) = xy_cov(1, 1);
    // std::cout << "uv_cov_: ((" << uv_cov_(0, 0) << ", " << uv_cov_(0, 1) << "), (" << uv_cov_(1, 0) << ", " << uv_cov_(1, 1) << "))" << std::endl;
    // std::cout << "A_inv: ((" << A_inv(0, 0) << ", " << A_inv(0, 1) << "), (" << A_inv(1, 0) << ", " << A_inv(1, 1) << "))" << std::endl;
    // std::cout << "A_inv_T: ((" << A_inv_T(0, 0) << ", " << A_inv_T(0, 1) << "), (" << A_inv_T(1, 0) << ", " << A_inv_T(1, 1) << "))" << std::endl;
    // std::cout << "xy_cov_: ((" << xy_cov_(0, 0) << ", " << xy_cov_(0, 1) << "), (" << xy_cov_(1, 0) << ", " << xy_cov_(1, 1) << "))" << std::endl;

}

//................................................................................//
//Get
//................................................................................//
