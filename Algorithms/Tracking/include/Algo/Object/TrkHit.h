#ifndef TRKHIT_H
#define TRKHIT_H

#ifndef RETURN
#define RETURN std::nan("RETURN")
#endif

//................................................................................//
//C++
#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include <bitset>

//................................................................................//
//ROOT
#include "TMath.h"
#include "TString.h"
#include "TVectorD.h"
#include "TMatrixDSym.h"

//................................................................................//
//Framework
#include "Object/SimulatedHit.h"

//................................................................................//
//Tracking

class DTrack;

class TrkHit : public SimulatedHit
{
private:
    using HitStatus = std::bitset<2>;

    static constexpr HitStatus seeded{0b01};
    static constexpr HitStatus tracked{0b10};

public:
//................................................................................//
//Constructor
//................................................................................//
    TrkHit() : u_vec_(2), v_vec_(2), uv_cov_(2), xy_cov_(2) {}
    TrkHit(const TrkHit &newTrkHit);
    TrkHit(TrkHit &&newTrkHit);
    TrkHit(const SimulatedHit &newSimuHit);
    virtual ~TrkHit() {}

    void operator =(const TrkHit &oldTrkHit);
//................................................................................//
//Set
//................................................................................//
    void SetId(int newId) {setId(newId);}
    void SetCellIdZ(int newIdZ) {setCellIdZ(newIdZ);}

    void SetX(double newX) {setX(newX);}
    void SetY(double newY) {setY(newY);}
    void SetZ(double newZ) {setZ(newZ);}
    void SetE(double newE) {setE(newE);}

    void SetU(double newU) {u_ = newU;}
    void SetV(double newV) {v_ = newV;}
    void SetZErr(double newZErr) {z_err_ = newZErr;}
    void SetUVec(const TVectorD &newUVec) {u_vec_ = newUVec;}
    void SetVVec(const TVectorD &newVVec) {v_vec_ = newVVec;}
    void SetUVCov(const TMatrixDSym &newCov) {uv_cov_ = newCov;}
    void SetXYCov(const TMatrixDSym &newCov) {xy_cov_ = newCov;}

    void SetSeeded() {status_ |= TrkHit::seeded;}
    void SetUnSeeded() {status_ &= (~TrkHit::seeded);}

    void SetTracked() {status_ |= TrkHit::tracked;}
    void SetUnTracked() {status_ &= (~TrkHit::tracked);}
    void SetTrack(const std::shared_ptr<DTrack> &track) {track_ = track;}
//................................................................................//
//Get
//................................................................................//
    int GetId() const {return getId();}
    int GetCellIdX() const {return getCellIdX();}
    int GetCellIdY() const {return getCellIdY();}
    int GetCellIdZ() const {return getCellIdZ();}

    double GetX() const {return getX();}
    double GetY() const {return getY();}
    double GetZ() const {return getZ();}
    double GetXErr() const {return sqrt(xy_cov_(0, 0));}
    double GetYErr() const {return sqrt(xy_cov_(1, 1));}
    double GetZErr() const {return z_err_;}
    double GetE() const {return getE();}

    double GetU() const {return u_;}
    double GetV() const {return v_;}
    double GetUVec(int i) const {return u_vec_[i];}
    double GetVVec(int i) const {return v_vec_[i];}
    auto GetUVCov() const {return uv_cov_;}
    auto GetXYCov() const {return xy_cov_;}
    double GetUVCov(int i, int j) const {return uv_cov_(i, j);}
    double GetXYCov(int i, int j) const {return xy_cov_(i, j);}

    bool Seeded() {return (status_ & TrkHit::seeded) == TrkHit::seeded;}

    bool Tracked() {return (status_ & TrkHit::tracked) == TrkHit::tracked;}
    std::shared_ptr<DTrack> GetTrack() const {return track_.lock();}

    // Update xy and xy_cov from uv and uv_cov
    void UpdateXY();

protected:

    bool if_in_track_{false};
    HitStatus status_{0b00};

    double u_{RETURN};
    double v_{RETURN};
    double z_err_{RETURN};
    TVectorD u_vec_;
    TVectorD v_vec_;
    TMatrixDSym uv_cov_;
    TMatrixDSym xy_cov_;

    std::weak_ptr<DTrack> track_;

private:
};

//using Key  = std::decay_t<decltype(((TrkHit*)nullptr)->GetCellIdZ())>;
using Key = std::invoke_result_t<decltype(&TrkHit::GetCellIdZ), TrkHit>;

#endif
