#include "Algo/KalmanFit/KalmanFilterFitter.h"

//................................................................................//
//C++
#include <iostream>
#include <vector>
#include <algorithm>

//................................................................................//
//ROOT
#include "TMath.h"
#include "TString.h"
#include "TGeoManager.h"

//................................................................................//
//Framework
#include "Core/AnaData.h"
#include "Utility/Units.h"

//................................................................................//
//GenFit
#include "KalmanFittedStateOnPlane.h"
#include "KalmanFitterInfo.h"

//................................................................................//
//Tracking
#include "Algo/Object/TrkHit.h"
#include "Algo/Object/DTrack.h"
#include "Algo/Calibrator/NullCalibrator.h"
#include "Algo/Propagator/Propagator.h"
#include "Algo/Propagator/GFPropagator.h"

//................................................................................//
//Constructor
KalmanFilterFitter::KalmanFilterFitter(Config config, DTrackP track, int verbose) : config_(config), hitCov(2)
{
    track_   = track;
    verbose_ = verbose;

    propagator_ = config.propagator;

    track_->LinkFitter(this);

    auto hits = track_->GetHits();

    try
    {
        if(track->Size() < 4) throw -1;
        
        Init(hits);
        Fit (hits);
        Fill(hits);
    }
    catch(int e)
    {
        if(verbose_ > 0)
            std::cerr << "[WARNING] ==> Fewer than 4 hits in this track" << std::endl;

        pp_ = 0.3*std::abs(config_.const_B)*track_->GetPreR();
    }
    catch(genfit::Exception& e)
    {
        if(verbose_ > 0)
        {
            std::cerr << "[WARNING] ==> " << e.what();
            std::cerr << "[WARNING] ==> Exception, next track" << std::endl;
        }

        pp_ = 0.3*std::abs(config_.const_B)*track_->GetPreR();
    }
}

KalmanFilterFitter::~KalmanFilterFitter()
{
    track_->LinkFitter(nullptr);

    //delete measurement; measurement = nullptr;
    delete fit_track_; fit_track_ = nullptr;
    //delete fitter; fitter = nullptr;
}

//................................................................................//
//Processor
//................................................................................//
//Initialize the fitter, set up magnetic, material manager, hits representation, fitter and hits model
void KalmanFilterFitter::Init(const TrkHitSPVec &hits)
{
    using namespace dunits;

    double pre_R = track_->GetPreR();
    double B = config_.const_B;

    int pdg = -GetSign(hits)*11;                  //pdg id, e- hypothesis
    pos = TVector3((*hits.at(0)).GetX() * dss_to_genfit::mm,  //pre fitting results --postion,  mm->cm
                   (*hits.at(0)).GetY() * dss_to_genfit::mm,  //
                   (*hits.at(0)).GetZ() * dss_to_genfit::mm); //
    mom = TVector3(0, 0, 0.3*B*pre_R * dss_to_genfit::MeV);    //                    --momentum, MeV->GeV
    hitCov.UnitMatrix();                           //covariance matrix

    //genfit::MaterialEffects::getInstance()->init(new genfit::TGeoMaterialInterface());
    //genfit::FieldManager::getInstance()->init(new genfit::ConstField(0., B*10., 0.)); //Magnet, T->kGs

    rep_ = new genfit::RKTrackRep(pdg);
    //fitter = new genfit::KalmanFitterRefTrack();
    fitter = std::make_unique<genfit::KalmanFitterRefTrack>();
    fit_track_ = new genfit::Track(rep_, pos, mom);
}

//................................................................................//
//Do the fit
void KalmanFilterFitter::Fit(const TrkHitSPVec &hits)
{
    using namespace dunits;

    //Create vitual detector planes and fill the hits
    int detId = 0;   //virtual detector
    int planeId = 0; //virtual plane
    int hitId = 0;
    TVectorD hitCoords(2);
    //genfit::PlanarMeasurement* measurement = nullptr;
    for(size_t i = 0; i < hits.size(); i++)
    {
        auto hit = hits.at(i);
        hitCoords[0] = hit->GetX() * dss_to_genfit::mm;
        hitCoords[1] = hit->GetY() * dss_to_genfit::mm;
        hitCov(0, 0) = hit->GetXYCov(0, 0) * dss_to_genfit::mm*dss_to_genfit::mm;
        hitCov(0, 1) = hit->GetXYCov(0, 1) * dss_to_genfit::mm*dss_to_genfit::mm;
        hitCov(1, 0) = hit->GetXYCov(1, 0) * dss_to_genfit::mm*dss_to_genfit::mm;
        hitCov(1, 1) = hit->GetXYCov(1, 1) * dss_to_genfit::mm*dss_to_genfit::mm;

        //virtual plane
        measurement = new genfit::PlanarMeasurement(hitCoords,
                                                    hitCov,
                                                    detId,
                                                    ++hitId,
                                                    nullptr);  //TrackPoint* hitsPoint
        measurement->setPlane(genfit::SharedPlanePtr(new genfit::DetPlane(TVector3(0.,          //origin vector
                                                                                   0.,
                                                                                   hit->GetZ() * dss_to_genfit::mm),
                                                                          TVector3(1, 0, 0),   //spanning vector u
                                                                          TVector3(0, 1, 0))), //spanning vector v
                                                                          ++planeId);
        fit_track_->insertPoint(new genfit::TrackPoint(measurement, fit_track_));
    }

    //check
    fit_track_->checkConsistency();
    //fit
    fitter->processTrack(fit_track_);
 
}

//................................................................................//
//Fill results
void KalmanFilterFitter::Fill(const TrkHitSPVec &hits)
{
    using namespace dunits;

    fit_track_->getFittedState().getPosMomCov(pos, mom, hitCov);

    px_ = -mom.Px() * genfit_to_dss::GeV;
    py_ = mom.Py() * genfit_to_dss::GeV;
    pz_ = -mom.Pz() * genfit_to_dss::GeV;
    pp_ = std::sqrt(mom.Pz() * mom.Pz() + mom.Px() * mom.Px()) * genfit_to_dss::GeV;
    pl_ = std::abs(mom.Py()) * genfit_to_dss::GeV;
    if (calibrator_)
        std::tie(pp_, pl_) = calibrator_->GetCalibratedP(pp_, pl_);

    double bchi2;
    double bndf;
    fitter->getChiSquNdf(fit_track_, rep_, bchi2, fchi2_, bndf, fndf_);

    auto [ending_mom, ending_pos] = this->ExtrapolateToPlane(hits.at(0)->GetZ());
    x_sigma_ = ending_pos[0] - hits.at(0)->GetX();
    y_sigma_ = ending_pos[1] - hits.at(0)->GetY();

    double extrapolated_surface = config_.extrapolated_surface;
    if(std::isnormal(extrapolated_surface) || extrapolated_surface == 0.)
    {
        auto [pflow_mom, pflow_pos] = this->ExtrapolateToPlane(extrapolated_surface);

        pflow_seed_x_ = pflow_pos[0];
        pflow_seed_y_ = pflow_pos[1];
        //std::cout << pflow_seed_y_ << std::endl;

        pflow_seed_px_ = pflow_mom[0];
        pflow_seed_py_ = pflow_mom[1];
        pflow_seed_pz_ = pflow_mom[2];
    }

    track_->SetPx(px_);
    track_->SetPy(py_);
    track_->SetPz(pz_);
    track_->SetPp(pp_);
    track_->SetNdf(fndf_);
    track_->SetChi2Algo(fchi2_);
    track_->SetXSigma(x_sigma_);
    track_->SetYSigma(y_sigma_);
    track_->SetPFlowSeedX(pflow_seed_x_);
    track_->SetPFlowSeedY(pflow_seed_y_);
    track_->SetPFlowDirctX(pflow_seed_px_);
    track_->SetPFlowDirctY(pflow_seed_py_);
    track_->SetPFlowQoP(pflow_seed_pz_);
}

//................................................................................//
//Get
//................................................................................//
std::tuple<std::vector<Fitter::vector3D>, std::vector<Fitter::vector3D>> KalmanFilterFitter::ExtrapolateToPlanes(const std::vector<double> &planes_z)
{
    if(!propagator_)
    {
        std::cerr << "[WARNING] ==> No fitter in KalmanFilterFitter" << std::endl;
        return {};
    }

    if(dynamic_cast<GFPropagator*>(propagator_) == nullptr)
    {
        std::cerr << "[WARNING] ==> Currently only GFPropagator supported in KalmanFilterFitter." << std::endl;
        return {};
    }

    std::vector<vector3D> mom_outs;
    std::vector<vector3D> pos_outs;

    mom_outs.reserve(planes_z.size());
    pos_outs.reserve(planes_z.size());

    auto gf_propagator = dynamic_cast<GFPropagator*>(propagator_);

    gf_propagator->ExtroplateToPlanesWithExistingRep(planes_z, fit_track_, rep_, mom_outs, pos_outs);
    
    return {mom_outs, pos_outs};
}

std::tuple<Fitter::vector3D, Fitter::vector3D> KalmanFilterFitter::ExtrapolateToPlane(const double &plane_z)
{
    if(!propagator_)
    {
        std::cerr << "[WARNING] ==> No fitter in KalmanFilterFitter" << std::endl;
        return {};
    }

    if(dynamic_cast<GFPropagator*>(propagator_) == nullptr)
    {
        std::cerr << "[WARNING] ==> Currently only GFPropagator supported in KalmanFilterFitter." << std::endl;
        return {};
    }

    std::vector<vector3D> mom_outs;
    std::vector<vector3D> pos_outs;

    auto gf_propagator = dynamic_cast<GFPropagator*>(propagator_);

    gf_propagator->ExtroplateToPlanesWithExistingRep({plane_z}, fit_track_, rep_, mom_outs, pos_outs);

    return {mom_outs.at(0), pos_outs.at(0)};
}
