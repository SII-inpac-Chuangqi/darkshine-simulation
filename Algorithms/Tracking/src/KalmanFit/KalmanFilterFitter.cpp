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
#include "Algo/TrkHit.h"
#include "Algo/KalmanFit/KalmanFilterFitter.h"
#include "Algo/Calibrator/NullCalibrator.h"

//................................................................................//
//Constructor
KalmanFilterFitter::KalmanFilterFitter(const TrkHitSPVec &track, Config config, int verbose) : config_(config), hitCov(2)
{
    verbose_ = verbose;

    try
    {
        if(track.size() < 4) throw -1;
        
        Init(track);
        Fit (track);
        Fill(track);
    }
    catch(int e)
    {
        if(verbose_ > 0)
            std::cerr << "[WARNING] ==> Fewer than 4 hits in this track" << std::endl;

        pp = 0.3*std::abs(config_.const_B)*config_.pre_R;
    }
    catch(genfit::Exception& e)
    {
        if(verbose_ > 0)
        {
            std::cerr << "[WARNING] ==> " << e.what();
            std::cerr << "[WARNING] ==> Exception, next track" << std::endl;
        }

        pp = 0.3*std::abs(config_.const_B)*config_.pre_R;
    }
}

//................................................................................//
//Processor
//................................................................................//
//Initialize the fitter, set up magnetic, material manager, track representation, fitter and track model
void KalmanFilterFitter::Init(const TrkHitSPVec &track)
{
    using namespace dunits;

    double pre_R = config_.pre_R;
    double B = config_.const_B;

    int pdg = -GetSign(track)*11;                  //pdg id, e- hypothesis
    pos = TVector3((*track.at(0)).GetX() * dss_to_genfit::mm,  //pre fitting results --postion,  mm->cm
                   (*track.at(0)).GetY() * dss_to_genfit::mm,  //
                   (*track.at(0)).GetZ() * dss_to_genfit::mm); //
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
void KalmanFilterFitter::Fit(const TrkHitSPVec &track)
{
    using namespace dunits;

    //Create vitual detector planes and fill the track
    int detId = 0;   //virtual detector
    int planeId = 0; //virtual plane
    int hitId = 0;
    TVectorD hitCoords(2);
    //genfit::PlanarMeasurement* measurement = nullptr;
    for(size_t i = 0; i < track.size(); i++)
    {
        auto hit = track.at(i);
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
                                                    nullptr);  //TrackPoint* trackPoint
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
void KalmanFilterFitter::Fill(const TrkHitSPVec &track)
{
    using namespace dunits;

    fitTrack->getFittedState().getPosMomCov(pos, mom, hitCov);

    px = -mom.Px() * genfit_to_dss::GeV;
    py = mom.Py() * genfit_to_dss::GeV;
    pz = -mom.Pz() * genfit_to_dss::GeV;
    pp = std::sqrt(mom.Pz() * mom.Pz() + mom.Px() * mom.Px()) * genfit_to_dss::GeV;
    pl = std::abs(mom.Py()) * genfit_to_dss::GeV;
    if (calibrator_)
        std::tie(pp, pl) = calibrator_->GetCalibratedP(pp, pl);

    double bChi2;
    double bNdf;
    fitter->getChiSquNdf(fitTrack, rep, bChi2, fChi2, bNdf, fNdf);

    {
        genfit::TrackPoint *tp = fitTrack->getPointWithMeasurementAndFitterInfo(0, rep);
        genfit::KalmanFittedStateOnPlane kfsop(*(static_cast<genfit::KalmanFitterInfo *>(tp->getFitterInfo(rep))->getBackwardUpdate()));
        genfit::SharedPlanePtr plane(new genfit::DetPlane(TVector3(0.,
                                                                   0.,
                                                                   (*track.at(0)).GetZ() * dss_to_genfit::mm),
                                                          TVector3(1, 0, 0),
                                                          TVector3(0, 1, 0)));
        rep->extrapolateToPlane(kfsop, plane);
        const TVectorD& state = kfsop.getState();
        //std::cout << "dimension of state: " << state.GetNoElements() << std::endl;
        //std::cout << "momemtum error: " << 1/abs(state[0])*1000 - sqrt(pp*pp + pl*pl) << std::endl;
        xSigma = state[3] * genfit_to_dss::cm - (*track.at(0)).GetX();
        //std::cout << "position error: " << xSigma << std::endl;
        ySigma = state[4] * genfit_to_dss::cm - (*track.at(0)).GetY();
    }

    double ECal_front_surface = dAnaData->getECalCenterZ() - 0.5*dAnaData->getECalLengthZ();
    if(std::isnormal(ECal_front_surface) || ECal_front_surface == 0.)
    {

        genfit::TrackPoint* tp = fitTrack->getPointWithMeasurementAndFitterInfo(0, rep);
        genfit::KalmanFittedStateOnPlane kfsop(*(static_cast<genfit::KalmanFitterInfo*>(tp->getFitterInfo(rep))->getBackwardUpdate()));
        genfit::SharedPlanePtr plane(new genfit::DetPlane(TVector3(0.,
                                                                   0.,
                                                                   ECal_front_surface * dss_to_genfit::mm),
                                                          TVector3(1, 0, 0),
                                                          TVector3(0, 1, 0)));
        rep->extrapolateToPlane(kfsop, plane);
        const TVectorD& state = kfsop.getState();
        //ECal_qop = 1./state[0]*1000.;
        //ECal_dirct_x = state[1];
        //ECal_dirct_y = state[2];
        ECal_seed_x = state[3] * genfit_to_dss::cm;
        ECal_seed_y = state[4] * genfit_to_dss::cm;
        //std::cout << ECal_seed_pz << std::endl;
        //std::cout << ECal_seed_y << std::endl;

        auto mom_on_ECal = kfsop.getMom();
        ECal_seed_px = -mom_on_ECal[0] * genfit_to_dss::GeV; //fix direction
        ECal_seed_py =  mom_on_ECal[1] * genfit_to_dss::GeV; //
        ECal_seed_pz = -mom_on_ECal[2] * genfit_to_dss::GeV; //
    }
}

//................................................................................//
//Get
//................................................................................//
//Calculate sign of charge of input track
/*
int KalmanFilterFitter::GetSign(const TrkHitSPVec &track)
{
    double xl  = track.at(track.size() - 1)->GetU();
    double xlr = track.at(track.size() - 2)->GetU();
    double xr  = track.at(0)->GetU();
    double xrl = track.at(1)->GetU();

    double zl  = track.at(track.size() - 1)->GetZ();
    double zlr = track.at(track.size() - 2)->GetZ();
    double zr  = track.at(0)->GetZ();
    double zrl = track.at(1)->GetZ();

    if(zr < zl)
    {
        std::swap(xl,  xr );
        std::swap(xlr, xrl);
        std::swap(zl,  zr );
        std::swap(zlr, zrl);
    }

    int s = 0;
    s = (xr - xrl)/sqrt((xr - xrl)*(xr - xrl) + (zr - zrl)*(zr - zrl)) >
        (xlr - xl)/sqrt((xl - xlr)*(xl - xlr) + (zl - zlr)*(zl - zlr)) ? 1 : -1;
    return s;
}
*/

std::vector<double> KalmanFilterFitter::ExtrapolateTo(const std::vector<double> &planes_z, tracking::direction extrop_dir)
{
    using namespace dunits;

    if(!rep || !fitTrack)
    {
        if(verbose_ > 0)
            std::cerr << "[WARNING] ==> No track to extrapolate" << std::endl;

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
                std::cerr << "[WARNING] ==> When extrapolating track at z=" << plane_z << "mm:" << std::endl;
                std::cerr << "              " << e.what();
            }

            if     (extrop_dir == tracking::dX) extrapolated.push_back(RETURN);
            else if(extrop_dir == tracking::dY) extrapolated.push_back(RETURN);
        }
    }

    return extrapolated;
}
