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

//................................................................................//
//Framework
#include "Object/SimulatedHit.h"
#include "Core/AnaData.h"

//................................................................................//
//GenFit
#include "KalmanFittedStateOnPlane.h"
#include "KalmanFitterInfo.h"

//................................................................................//
//Tracking
#include "Algo/TrkHit.h"
#include "Algo/KalmanFitting.h"

//................................................................................//
//Constructor
KalmanFitting::KalmanFitting(const TrkHitPVec &track, std::initializer_list<double> list, int verbose) : hitCov(2)
{
    verbose_ = verbose;

    try
    {
        if(track.size() < 3) throw -1;
        
        Init(track, list);
        Fit (track, {});
        Fill(track, {});
    }
    catch(int e)
    {
        if(verbose_ > 0)
            std::cerr << "[WARNING] ==> Fewer than 3 hits in this track" << std::endl;

        auto it = list.begin();
        double preR = *it; it++;
        double B = *it;
        pp = 0.3*std::abs(B)*preR;
    }
    catch(genfit::Exception& e)
    {
        if(verbose_ > 0)
        {
            std::cerr << "[WARNING] ==> " << e.what();
            std::cerr << "[WARNING] ==> Exception, next track" << std::endl;
        }

        auto it = list.begin();
        double preR = *it; it++;
        double B = *it;
        pp = 0.3*std::abs(B)*preR;
    }
}

//................................................................................//
//Processor
//................................................................................//
//Initialize the fitter, set up magnetic, material manager, track representation, fitter and track model
//void KalmanFitting::Init(const TrkHitPVec &track, double preR, double B)
void KalmanFitting::Init(const TrkHitPVec &track, std::initializer_list<double> list)
{
    auto it = list.begin();
    double preR = *it; it++;
    double B = *it;

    int pdg = -GetSign(track)*11;              //pdg id, e- hypothesis
    pos = TVector3((*track.at(0)).GetU()*0.1,  //pre fitting results --postion,  mm->cm
                   (*track.at(0)).GetV()*0.1,  //
                   (*track.at(0)).GetZ()*0.1); //
    mom = TVector3(0, 0, 0.3*B*preR*0.001);    //                    --momentum, MeV->GeV
    hitCov.UnitMatrix();                       //covariance matrix
    hitCov(0, 0) = 0.0006*0.0006;              //resolution, cm --x 6µm
    hitCov(1, 1) = 0.006*0.006;                //               --y 60µm

    //genfit::MaterialEffects::getInstance()->init(new genfit::TGeoMaterialInterface());
    //genfit::FieldManager::getInstance()->init(new genfit::ConstField(0., B*10., 0.)); //Magnet, T->kGs

    rep = new genfit::RKTrackRep(pdg);
    //fitter = new genfit::KalmanFitterRefTrack();
    fitter = std::make_unique<genfit::KalmanFitterRefTrack>();
    fitTrack = new genfit::Track(rep, pos, mom);
}

//................................................................................//
//Do the fit
void KalmanFitting::Fit(const TrkHitPVec &track, std::initializer_list<double>)
{
    //Create vitual detector planes and fill the track
    int detId = 0;   //virtual detector
    int planeId = 0; //virtual plane
    int hitId = 0;
    TVectorD hitCoords(2);
    //genfit::PlanarMeasurement* measurement = nullptr;
    for(size_t i = 0; i < track.size(); i++)
    {
        hitCoords[0] = 0.1*(*track.at(i)).GetU();
        hitCoords[1] = 0.1*(*track.at(i)).GetV();
        //virtual plane
        measurement = new genfit::PlanarMeasurement(hitCoords,
                                                    hitCov,
                                                    detId,
                                                    ++hitId,
                                                    nullptr);  //TrackPoint* trackPoint
        measurement->setPlane(genfit::SharedPlanePtr(new genfit::DetPlane(TVector3(0.,          //origin vector
                                                                                   0.,
                                                                                   (*track.at(i)).GetZ()*0.1),
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
void KalmanFitting::Fill(const TrkHitPVec &track, std::initializer_list<double>)
{
    fitTrack->getFittedState().getPosMomCov(pos, mom, hitCov);
    px = std::abs(mom.Px())*1000;                           //GeV->MeV
    py = std::abs(mom.Py())*1000;                           //
    pz = std::abs(mom.Pz())*1000;                           //
    pp = sqrt(mom.Pz()*mom.Pz() + mom.Px()*mom.Px())*1000.; //
    pl = std::abs(mom.Py())*1000;                           //

    double bChi2;
    double bNdf;
    fitter->getChiSquNdf(fitTrack, rep, bChi2, fChi2, bNdf, fNdf);

    {
        genfit::TrackPoint* tp = fitTrack->getPointWithMeasurementAndFitterInfo(0, rep);
        genfit::KalmanFittedStateOnPlane kfsop(*(static_cast<genfit::KalmanFitterInfo*>(tp->getFitterInfo(rep))->getBackwardUpdate()));
        genfit::SharedPlanePtr plane(new genfit::DetPlane(TVector3(0.,
                                                                   0.,
                                                                   (*track.at(0)).GetZ()*0.1),
                                                          TVector3(1, 0, 0),
                                                          TVector3(0, 1, 0)));
        rep->extrapolateToPlane(kfsop, plane);
        const TVectorD& state = kfsop.getState();
        //std::cout << "dimension of state: " << state.GetNoElements() << std::endl;
        //std::cout << "momemtum error: " << 1/abs(state[0])*1000 - sqrt(pp*pp + pl*pl) << std::endl;
        xSigma = state[3]*10 - (*track.at(0)).GetX();
        //std::cout << "position error: " << xSigma << std::endl;
        ySigma = state[4]*10 - (*track.at(0)).GetY();
    }

    double ECal_front_surface = dAnaData->getECalCenterZ() - 0.5*dAnaData->getECalLengthZ();
    if(std::isnormal(ECal_front_surface) || ECal_front_surface == 0.)
    {

        genfit::TrackPoint* tp = fitTrack->getPointWithMeasurementAndFitterInfo(0, rep);
        genfit::KalmanFittedStateOnPlane kfsop(*(static_cast<genfit::KalmanFitterInfo*>(tp->getFitterInfo(rep))->getBackwardUpdate()));
        genfit::SharedPlanePtr plane(new genfit::DetPlane(TVector3(0.,
                                                                   0.,
                                                                   ECal_front_surface*0.1),
                                                          TVector3(1, 0, 0),
                                                          TVector3(0, 1, 0)));
        rep->extrapolateToPlane(kfsop, plane);
        const TVectorD& state = kfsop.getState();
        //ECal_qop = 1./state[0]*1000.;
        //ECal_dirct_x = state[1];
        //ECal_dirct_y = state[2];
        ECal_seed_x = state[3]*10;
        ECal_seed_y = state[4]*10;
        //std::cout << ECal_seed_pz << std::endl;
        //std::cout << ECal_seed_y << std::endl;

        auto mom_on_ECal = kfsop.getMom();
        ECal_seed_px = -mom_on_ECal[0]*1000.; //fix direction
        ECal_seed_py =  mom_on_ECal[1]*1000.; //
        ECal_seed_pz = -mom_on_ECal[2]*1000.; //
    }
}

//................................................................................//
//Get
//................................................................................//
//Calculate sign of charge of input track
int KalmanFitting::GetSign(const TrkHitPVec &track)
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

std::vector<double> KalmanFitting::ExtrapolateTo(const std::vector<double> &planes_z, tracking::direction extrop_dir)
{
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
                                                                   plane_z*0.1),
                                                          TVector3(1, 0, 0),
                                                          TVector3(0, 1, 0)));
        rep->extrapolateToPlane(kfsop, plane);
        const TVectorD& state = kfsop.getState();
        if     (extrop_dir == tracking::dX) extrapolated.push_back(state[3]*10);
        else if(extrop_dir == tracking::dY) extrapolated.push_back(state[4]*10);
    }

    return extrapolated;
}
