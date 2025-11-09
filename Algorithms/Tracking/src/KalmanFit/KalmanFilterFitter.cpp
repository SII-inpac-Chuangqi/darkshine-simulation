//................................................................................//
//C++
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

//................................................................................//
//ROOT
#include "TMath.h"
#include "TString.h"
#include "TGeoManager.h"

//................................................................................//
//Framework
#include "Object/SimulatedHit.h"
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
#include "Algo/KalmanFit/KalmanFilterFitter.h"
#include "Algo/Calibrator/NullCalibrator.h"

//................................................................................//
//Constructor
KalmanFilterFitter::KalmanFilterFitter(Config config, DTrackP track, int verbose) : config_(config), hitCov(2)
{
    track_   = track;
    verbose_ = verbose;

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

    rep = new genfit::RKTrackRep(pdg);
    //fitter = new genfit::KalmanFitterRefTrack();
    fitter = std::make_unique<genfit::KalmanFitterRefTrack>();
    fitTrack = new genfit::Track(rep, pos, mom);
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
        fitTrack->insertPoint(new genfit::TrackPoint(measurement, fitTrack));
    }

    //check
    fitTrack->checkConsistency();
    //fit
    fitter->processTrack(fitTrack);
 
}

//................................................................................//
//Fill results
void KalmanFilterFitter::Fill(const TrkHitSPVec &hits)
{
    using namespace dunits;

    fitTrack->getFittedState().getPosMomCov(pos, mom, hitCov);

    px_ = -mom.Px() * genfit_to_dss::GeV;
    py_ = mom.Py() * genfit_to_dss::GeV;
    pz_ = -mom.Pz() * genfit_to_dss::GeV;
    pp_ = std::sqrt(mom.Pz() * mom.Pz() + mom.Px() * mom.Px()) * genfit_to_dss::GeV;
    pl_ = std::abs(mom.Py()) * genfit_to_dss::GeV;
    if (calibrator_)
        std::tie(pp_, pl_) = calibrator_->GetCalibratedP(pp_, pl_);

    double bchi2;
    double bndf;
    fitter->getChiSquNdf(fitTrack, rep, bchi2, fchi2_, bndf, fndf_);

    {
        genfit::TrackPoint *tp = fitTrack->getPointWithMeasurementAndFitterInfo(0, rep);
        genfit::KalmanFittedStateOnPlane kfsop(*(static_cast<genfit::KalmanFitterInfo *>(tp->getFitterInfo(rep))->getBackwardUpdate()));
        genfit::SharedPlanePtr plane(new genfit::DetPlane(TVector3(0.,
                                                                   0.,
                                                                   (*hits.at(0)).GetZ() * dss_to_genfit::mm),
                                                          TVector3(1, 0, 0),
                                                          TVector3(0, 1, 0)));
        rep->extrapolateToPlane(kfsop, plane);
        const TVectorD& state = kfsop.getState();
        //std::cout << "dimension of state: " << state.GetNoElements() << std::endl;
        //std::cout << "momemtum error: " << 1/abs(state[0])*1000 - sqrt(pp*pp + pl*pl) << std::endl;
        x_sigma_ = state[3] * genfit_to_dss::cm - (*hits.at(0)).GetX();
        //std::cout << "position error: " << xSigma << std::endl;
        y_sigma_ = state[4] * genfit_to_dss::cm - (*hits.at(0)).GetY();
    }

    double pflow_front_surface = dAnaData->getECalCenterZ() - 0.5*dAnaData->getECalLengthZ();
    if(std::isnormal(pflow_front_surface) || pflow_front_surface == 0.)
    {

        genfit::TrackPoint* tp = fitTrack->getPointWithMeasurementAndFitterInfo(0, rep);
        genfit::KalmanFittedStateOnPlane kfsop(*(static_cast<genfit::KalmanFitterInfo*>(tp->getFitterInfo(rep))->getBackwardUpdate()));
        genfit::SharedPlanePtr plane(new genfit::DetPlane(TVector3(0.,
                                                                   0.,
                                                                   pflow_front_surface * dss_to_genfit::mm),
                                                          TVector3(1, 0, 0),
                                                          TVector3(0, 1, 0)));
        rep->extrapolateToPlane(kfsop, plane);
        const TVectorD& state = kfsop.getState();
        //pflow_qop_ = 1./state[0]*1000.;
        //pflow_dirct_x_ = state[1];
        //pflow_dirct_y_ = state[2];
        pflow_seed_x_ = state[3] * genfit_to_dss::cm;
        pflow_seed_y_ = state[4] * genfit_to_dss::cm;
        //std::cout << pflow_seed_pz_ << std::endl;
        //std::cout << pflow_seed_y_ << std::endl;

        auto mom_for_pflow = kfsop.getMom();
        pflow_seed_px_ = -mom_for_pflow[0] * genfit_to_dss::GeV; //fix direction
        pflow_seed_py_ =  mom_for_pflow[1] * genfit_to_dss::GeV; //
        pflow_seed_pz_ = -mom_for_pflow[2] * genfit_to_dss::GeV; //
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
std::vector<double> KalmanFilterFitter::ExtrapolateTo(const std::vector<double> &planes_z, tracking::direction extrop_dir)
{
    using namespace dunits;

    if(!rep || !fitTrack)
    {
        if(verbose_ > 0)
            std::cerr << "[WARNING] ==> No hits to extrapolate" << std::endl;

        return {};
    }

    std::vector<double> extrapolated;
    for(const auto &plane_z : planes_z)
    {
        genfit::TrackPoint* tp = fitTrack->getPointWithMeasurementAndFitterInfo(0, rep);
        genfit::KalmanFittedStateOnPlane kfsop(*(static_cast<genfit::KalmanFitterInfo*>(tp->getFitterInfo(rep))->getBackwardUpdate()));
        genfit::SharedPlanePtr plane(new genfit::DetPlane(TVector3(0.,
                                                                   0.,
                                                                   plane_z * dss_to_genfit::mm),
                                                          TVector3(1, 0, 0),
                                                          TVector3(0, 1, 0)));

        try
        {
            rep->extrapolateToPlane(kfsop, plane);
            const TVectorD& state = kfsop.getState();
            if     (extrop_dir == tracking::dX) extrapolated.push_back(state[3]*10);
            else if(extrop_dir == tracking::dY) extrapolated.push_back(state[4]*10);
        }
        catch(genfit::Exception& e)
        {
            if(verbose_ > 1)
            {
                std::cerr << "[WARNING] ==> When extrapolating hits at z=" << plane_z << "mm:" << std::endl;
                std::cerr << "              " << e.what();
            }

            if     (extrop_dir == tracking::dX) extrapolated.push_back(RETURN);
            else if(extrop_dir == tracking::dY) extrapolated.push_back(RETURN);
        }
    }

    return extrapolated;
}
