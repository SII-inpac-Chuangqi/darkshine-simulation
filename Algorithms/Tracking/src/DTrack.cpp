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
#include "Algo/DTrack.h"
//#include "Algo/KalmanFilterFitter.h"

//................................................................................//
//public:
//................................................................................//
//Constructor
//
DTrack::DTrack(const TrkHitPVec &newHits,
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
                                         ECal_seed_x_(oldTrack.ECal_seed_x_),
                                         ECal_seed_y_(oldTrack.ECal_seed_y_),
                                         ECal_seed_px_(oldTrack.ECal_seed_px_),
                                         ECal_seed_py_(oldTrack.ECal_seed_py_),
                                         ECal_seed_pz_(oldTrack.ECal_seed_pz_),

                                         quality_(oldTrack.quality_), //track properties

                                         By_(oldTrack.By_),           //detector properties

                                         ndf_(oldTrack.ndf_),         //fitting properties
                                         chi2_(oldTrack.chi2_),
                                         chi2_algo_(oldTrack.chi2_algo_),
                                         xSigma_(oldTrack.xSigma_),
                                         ySigma_(oldTrack.ySigma_),
                                         if_extrapolated_(oldTrack.if_extrapolated_),
                                         extrapolated_x_(oldTrack.extrapolated_x_),
                                         extrapolated_y_(oldTrack.extrapolated_y_),

                                         preR_(oldTrack.preR_),       //finding properties
                                         preXc_(oldTrack.preXc_),
                                         preYc_(oldTrack.preYc_),

                                         hits_(oldTrack.hits_)        //hits collection
{}

DTrack::DTrack(DTrack &&oldTrack) : pdg_(std::move(oldTrack.pdg_)),         //physical properties
                                    sign_(std::move(oldTrack.sign_)),
                                    px_(std::move(oldTrack.px_)),
                                    py_(std::move(oldTrack.py_)),
                                    pz_(std::move(oldTrack.pz_)),
                                    pp_(std::move(oldTrack.pp_)),
                                    pl_(std::move(oldTrack.pl_)),
                                    ECal_seed_x_(std::move(oldTrack.ECal_seed_x_)),
                                    ECal_seed_y_(std::move(oldTrack.ECal_seed_y_)),
                                    ECal_seed_px_(std::move(oldTrack.ECal_seed_px_)),
                                    ECal_seed_py_(std::move(oldTrack.ECal_seed_py_)),
                                    ECal_seed_pz_(std::move(oldTrack.ECal_seed_pz_)),

                                    quality_(std::move(oldTrack.quality_)), //track properties

                                    By_(std::move(oldTrack.By_)),           //detector properties

                                    ndf_(std::move(oldTrack.ndf_)),         //fitting properties
                                    chi2_(std::move(oldTrack.chi2_)),
                                    chi2_algo_(std::move(oldTrack.chi2_algo_)),
                                    xSigma_(std::move(oldTrack.xSigma_)),
                                    ySigma_(std::move(oldTrack.ySigma_)),
                                    if_extrapolated_(std::move(oldTrack.if_extrapolated_)),
                                    extrapolated_x_(std::move(oldTrack.extrapolated_x_)),
                                    extrapolated_y_(std::move(oldTrack.extrapolated_y_)),

                                    preR_(std::move(oldTrack.preR_)),       //finding properties
                                    preXc_(std::move(oldTrack.preXc_)),
                                    preYc_(std::move(oldTrack.preYc_)),

                                    hits_(oldTrack.hits_)                   //hits collection
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
    ECal_seed_x_ = old_track.ECal_seed_x_;
    ECal_seed_y_ = old_track.ECal_seed_y_;
    ECal_seed_px_ = old_track.ECal_seed_px_;
    ECal_seed_py_ = old_track.ECal_seed_py_;
    ECal_seed_pz_ = old_track.ECal_seed_pz_;

    quality_ = old_track.quality_;

    By_ = old_track.By_; 

    ndf_ = old_track.ndf_;
    chi2_ = old_track.chi2_;
    chi2_algo_ = old_track.chi2_algo_;
    xSigma_ = old_track.xSigma_;
    ySigma_ = old_track.ySigma_;
    if_extrapolated_ = old_track.if_extrapolated_;
    extrapolated_x_.clear();
    extrapolated_x_.assign(old_track.extrapolated_x_.begin(), old_track.extrapolated_x_.end());
    extrapolated_y_.clear();
    extrapolated_y_.assign(old_track.extrapolated_y_.begin(), old_track.extrapolated_y_.end());

    preR_ = old_track.preR_;
    preXc_ = old_track.preXc_;
    preYc_ = old_track.preYc_;

    hits_.clear();
    hits_.assign(old_track.hits_.begin(), old_track.hits_.end());

    return *this;
}

TrkHitP DTrack::AtCellIdZ(int i)
{
    for(const auto hit : hits_)
    {
        if(hit->GetCellIdZ() == i)
            return hit;
    }

    return nullptr;
}

double DTrack::GetChi2()
{
    if(hits_.size() < 4)
    {
        chi2_ = RETURN;
        return chi2_;
    }

    double mean_x = 0.;
    double mean_y = 0.;
    std::vector<double> track_x;
    std::vector<double> track_y;
    std::vector<double> track_z;
    for(const auto &hit : hits_)
    {
        track_x.push_back(hit->GetX());
        track_y.push_back(hit->GetY());
        track_z.push_back(hit->GetZ());

        mean_x += hit->GetX();
        mean_y += hit->GetY();
    }
    mean_x /= track_x.size();
    mean_y /= track_x.size();

    if(!if_extrapolated_)
    {
        extrapolated_x_.clear();
        extrapolated_y_.clear();
        extrapolated_x_ = this->ExtrapolateTo(track_z, tracking::dX);
        extrapolated_y_ = this->ExtrapolateTo(track_z, tracking::dY);

        if_extrapolated_ = true;
    }

    if(extrapolated_x_.size()*extrapolated_y_.size() == 0)
    {
        chi2_ = RETURN;
        return chi2_;
    }

    double std_variance = 0.;
    double deviation = 0.;
    for(size_t i = 0; i < track_x.size(); i++)
    {
        deviation += (track_x.at(i) - extrapolated_x_.at(i))*(track_x.at(i) - extrapolated_x_.at(i)) + 
                     (track_y.at(i) - extrapolated_y_.at(i))*(track_y.at(i) - extrapolated_y_.at(i));
        std_variance += (track_x.at(i) - mean_x)*(track_x.at(i) - mean_x) +
                        (track_y.at(i) - mean_y)*(track_y.at(i) - mean_y);
    }
    chi2_ = deviation/std_variance*track_x.size()/ndf_;

    return chi2_;
}

int DTrack::GetInitCellIdZ() const
{
    int init_cell_id_z = INT_MIN;

    for(const auto &hit : hits_)
    {
        if(-hit->GetCellIdZ() > init_cell_id_z)
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
        std::vector<double> track_z;
        for(const auto &hit : hits_) track_z.push_back(hit->GetZ());

        extrapolated_x_.clear();
        extrapolated_y_.clear();
        extrapolated_x_ = this->ExtrapolateTo(track_z, tracking::dX);
        extrapolated_y_ = this->ExtrapolateTo(track_z, tracking::dY);

        if_extrapolated_ = true;
    }

    if     (extrop_dir == tracking::dX) return extrapolated_x_;
    else if(extrop_dir == tracking::dY) return extrapolated_y_;

    return {};
}

void DTrack::ExceptionHandler(const std::vector<double> &magnet)
{
    if(verbose_ > 0 && magnet.size() != 3)
    {
        std::cerr << "[WARNING] ==> Magnet dimension != 3 in exception handler in DTrack, default magnet set"
                  << std::endl;
        By_ = -1.5;
        return;
    }

    By_ = magnet.at(1);
}

void DTrack::Remove(int i)
{
    hits_.erase(std::remove(hits_.begin(), hits_.end(), hits_.at(i)), hits_.end());
}

void DTrack::Fit(int method)
{
//    Fitter *fitter_ = nullptr;

    switch(method)
    {
        case tracking::dKalman  :
                                  fitter_ = new KalmanFilterFitter(hits_,
                                                                   {preR_,   //Fix to 2 ordered parameters! --bending radius as fitting seed
                                                                    By_},    //                             --magnet to manage exception condition
                                                                   verbose_  //Verbose
                                                                  );
                                  break;
        case tracking::dRiemann :
                                  fitter_ = new RiemannFitter(hits_, {preXc_, preYc_, preR_});
                                  if(verbose_ > 0)
                                      std::cout << "[INFO] ==> Riemann fit coming soon" << std::endl;
                                  break;
                                  break;
        case tracking::dNone  :
                                  if(verbose_ > 0)
                                      std::cout << "[INFO] ==> Fit not required" << std::endl;
                                  break;
        default :
                                  if(verbose_ > 0)
                                      std::cerr << "[WARNING] ==> Fit method not found. Use default GenFit Kalman fitter" << std::endl;
                                  fitter_ = new KalmanFilterFitter(hits_,
                                                                   {preR_,   //Fix to 2 ordered parameters! --bending radius as fitting seed
                                                                    By_},    //                             --magnet to manage exception condition
                                                                   verbose_  //Verbose
                                                                  );
    }

    if(fitter_)
    {
        px_ = fitter_->GetPx();
        py_ = fitter_->GetPy();
        pz_ = fitter_->GetPz();
        pp_ = fitter_->GetPp();
        ndf_ = fitter_->GetNdf();
        chi2_algo_ = fitter_->GetChi2();
        xSigma_ = fitter_->GetXSigma();
        ySigma_ = fitter_->GetYSigma();
        ECal_seed_x_ = fitter_->GetECalSeedX();
        ECal_seed_y_ = fitter_->GetECalSeedY();
        ECal_seed_px_ = fitter_->GetECalDirctX();
        ECal_seed_py_ = fitter_->GetECalDirctY();
        ECal_seed_pz_ = fitter_->GetECalQoP();
        //std::cout << ECal_seed_pz << std::endl;
        //std::cout << ndf_ << std::endl;

        corrections_x_ = fitter_->GetCorrectionsX();
    }
    else
        pp_ = 0.3*preR_*std::abs(By_);
}

std::vector<double> DTrack::ExtrapolateTo(const std::vector<double> &planes_z, tracking::direction extrop_dir)
{
    if(!fitter_)
    {
        if(verbose_ > 0) 
            std::cerr << "[WARNING] ==> No fitter to extrapolate" << std::endl;
        return {};
    }

    return fitter_->ExtrapolateTo(planes_z, extrop_dir);
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
