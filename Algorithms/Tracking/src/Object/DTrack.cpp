//................................................................................//
//C++
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <climits>

//................................................................................//
//ROOT

//................................................................................//
//Framework

//................................................................................//
//Tracking
#include "Algo/Object/DTrack.h"
#include "Algo/KalmanFit/KalmanFilterFitter.h"
#include "Algo/RiemannFit/RiemannFitter.h"
#include "Algo/Calibrator/NullCalibrator.h"

//................................................................................//
//public:
//................................................................................//
//Constructor
double DTrack::x_resolution_ = 0.;
double DTrack::y_resolution_ = 0.;
double DTrack::z_resolution_ = 0.;

DTrack::DTrack(const TrkHitSPVec &newHits,
               double newPreR,
               double newPreXc,
               double newPreYc) : preR_(newPreR),
                                  preXc_(newPreXc),
                                  preYc_(newPreYc),
                                  hits_(newHits)
{
}

DTrack::DTrack(const DTrack &oldTrack) : pdg_(oldTrack.pdg_),         //physical properties
                                         sign_(oldTrack.sign_),
                                         px_(oldTrack.px_),
                                         py_(oldTrack.py_),
                                         pz_(oldTrack.pz_),
                                         pp_(oldTrack.pp_),
                                         pl_(oldTrack.pl_),
                                         pflow_seed_x_(oldTrack.pflow_seed_x_),
                                         pflow_seed_y_(oldTrack.pflow_seed_y_),
                                         pflow_seed_px_(oldTrack.pflow_seed_px_),
                                         pflow_seed_py_(oldTrack.pflow_seed_py_),
                                         pflow_seed_pz_(oldTrack.pflow_seed_pz_),

                                         quality_(oldTrack.quality_), //track properties

                                         By_(oldTrack.By_),           //detector properties

                                         ndf_(oldTrack.ndf_),         //fitting properties
                                         chi2_(oldTrack.chi2_),
                                         chi2_algo_(oldTrack.chi2_algo_),
                                         xSigma_(oldTrack.xSigma_),
                                         ySigma_(oldTrack.ySigma_),
                                         if_extrapolated_(oldTrack.if_extrapolated_),
                                         extrapolated_xs_(oldTrack.extrapolated_xs_),
                                         extrapolated_ys_(oldTrack.extrapolated_ys_),

                                         preR_(oldTrack.preR_),       //finding properties
                                         preXc_(oldTrack.preXc_),
                                         preYc_(oldTrack.preYc_),

                                         hits_(oldTrack.hits_),       //hits collection

                                         vertex_(oldTrack.vertex_)    //vertex
{}

DTrack::DTrack(DTrack &&oldTrack) : pdg_(std::move(oldTrack.pdg_)),         //physical properties
                                    sign_(std::move(oldTrack.sign_)),
                                    px_(std::move(oldTrack.px_)),
                                    py_(std::move(oldTrack.py_)),
                                    pz_(std::move(oldTrack.pz_)),
                                    pp_(std::move(oldTrack.pp_)),
                                    pl_(std::move(oldTrack.pl_)),
                                    pflow_seed_x_(std::move(oldTrack.pflow_seed_x_)),
                                    pflow_seed_y_(std::move(oldTrack.pflow_seed_y_)),
                                    pflow_seed_px_(std::move(oldTrack.pflow_seed_px_)),
                                    pflow_seed_py_(std::move(oldTrack.pflow_seed_py_)),
                                    pflow_seed_pz_(std::move(oldTrack.pflow_seed_pz_)),

                                    quality_(std::move(oldTrack.quality_)), //track properties

                                    By_(std::move(oldTrack.By_)),           //detector properties

                                    ndf_(std::move(oldTrack.ndf_)),         //fitting properties
                                    chi2_(std::move(oldTrack.chi2_)),
                                    chi2_algo_(std::move(oldTrack.chi2_algo_)),
                                    xSigma_(std::move(oldTrack.xSigma_)),
                                    ySigma_(std::move(oldTrack.ySigma_)),
                                    if_extrapolated_(std::move(oldTrack.if_extrapolated_)),
                                    extrapolated_xs_(std::move(oldTrack.extrapolated_xs_)),
                                    extrapolated_ys_(std::move(oldTrack.extrapolated_ys_)),

                                    preR_(std::move(oldTrack.preR_)),       //finding properties
                                    preXc_(std::move(oldTrack.preXc_)),
                                    preYc_(std::move(oldTrack.preYc_)),

                                    hits_(oldTrack.hits_),                  //hits collection

                                    vertex_(std::move(oldTrack.vertex_))    //vertex
{
    oldTrack.hits_.clear();
}

DTrack& DTrack::operator=(const DTrack &old_track)
{
    if(this == &old_track) return *this;

    verbose_ = old_track.verbose_;

    pdg_ = old_track.pdg_;
    sign_ = old_track.sign_;
    px_ = old_track.px_;
    py_ = old_track.py_;
    pz_ = old_track.pz_;
    pp_ = old_track.pp_;
    pl_ = old_track.pl_;
    pflow_seed_x_ = old_track.pflow_seed_x_;
    pflow_seed_y_ = old_track.pflow_seed_y_;
    pflow_seed_px_ = old_track.pflow_seed_px_;
    pflow_seed_py_ = old_track.pflow_seed_py_;
    pflow_seed_pz_ = old_track.pflow_seed_pz_;

    quality_ = old_track.quality_;

    By_ = old_track.By_; 

    ndf_ = old_track.ndf_;
    chi2_ = old_track.chi2_;
    chi2_algo_ = old_track.chi2_algo_;
    xSigma_ = old_track.xSigma_;
    ySigma_ = old_track.ySigma_;
    if_extrapolated_ = old_track.if_extrapolated_;
    extrapolated_xs_ = old_track.extrapolated_xs_;
    extrapolated_ys_ = old_track.extrapolated_ys_;

    preR_ = old_track.preR_;
    preXc_ = old_track.preXc_;
    preYc_ = old_track.preYc_;

    hits_ = old_track.hits_;

    vertex_ = old_track.vertex_;

    return *this;
}

TrkHitSP DTrack::AtCellIdZ(int i)
{
    for(auto hit : hits_)
    {
        if(hit->GetCellIdZ() == i)
            return hit;
    }

    return nullptr;
}

int DTrack::GetInitCellIdZ() const
{
    int init_cell_id_z = INT_MAX;

    for(const auto &hit : hits_)
    {
        if(hit->GetCellIdZ() < init_cell_id_z)
            init_cell_id_z = hit->GetCellIdZ();
    }

    return init_cell_id_z;
}

double DTrack::GetDeltaR(const DTrack *another) const
{
    int size = this->hits_.size() < another->hits_.size() ? this->hits_.size() : another->hits_.size();

    double delta_R = 0.;
    for(int i = 0; i < size; i++)
        delta_R += std::hypot(this->hits_.at(i)->GetX() - another->hits_.at(i)->GetX(),
                              this->hits_.at(i)->GetY() - another->hits_.at(i)->GetY());
    delta_R /= size;

    return delta_R;
}

std::vector<double> DTrack::GetExtrapolated(tracking::direction extrop_dir)
{
    if(!if_extrapolated_)
    {
        std::vector<double> track_zs;
        for(const auto &hit : hits_) track_zs.push_back(hit->GetZ());

        extrapolated_xs_.clear();
        extrapolated_ys_.clear();
        auto [extrapolated_moms, extrapolated_poss] = fitter_->ExtrapolateToPlanes(track_zs);
        for(const auto &extrapolated_pos : extrapolated_poss)
        {
            extrapolated_xs_.push_back(extrapolated_pos[0]);           
            extrapolated_ys_.push_back(extrapolated_pos[1]);
        }

        if_extrapolated_ = true;
    }

    if     (extrop_dir == tracking::dX) return extrapolated_xs_;
    else if(extrop_dir == tracking::dY) return extrapolated_ys_;

    return {};
}

void DTrack::Remove(int i)
{
    hits_.erase(std::remove(hits_.begin(), hits_.end(), hits_.at(i)), hits_.end());
}

std::ostream &operator<<(std::ostream &os, const DTrack &track)
{
    for(const auto &hit : track.hits_) os << *hit << std::endl;
    return os;
}

void DTrack::Evaluate()
{
/*
    if(hits_.size() <= 0)
    {
        quality_ = 0;
        return;
    }

    std::map<int, int> particleCount;
    for(auto hit : hits_)
    {
        auto contribution = hit->getPContribution();
        if(contribution.size())
        {
            for(auto particle : contribution)
            {
                auto itSearchParticle = particleCount.find(particle.getId());
                if(itSearchParticle != particleCount.end())
                    itSearchParticle->second++;
                else
                    particleCount.insert(std::pair(particle.getId(), 1));
            }
        }
    }

    for(auto count : particleCount)
    {
        if(1.*count.second/hits.size() > quality_)
            quality_ = 1.*count.second/hits.size();
    }
*/
}
