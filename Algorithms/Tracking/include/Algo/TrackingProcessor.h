#ifndef TRACKINGPROCESSOR_H
#define TRACKINGPROCESSOR_H

#ifndef RETURN
#define RETURN std::nan("RETURN")
#endif

//................................................................................//
//CPP Libraries
#include <utility>

//................................................................................//
//FRAMEWORK
#include "Core/AnaProcessor.h"

//................................................................................//
//TRACKING
#include "Algo/TrkHit.h"
#include "Algo/DTrack.h"
#include "Algo/Digitization.h"

using namespace std;

class TrackingProcessor : public AnaProcessor
{
public:
    // Must initialized with Name
    explicit TrackingProcessor(string name, shared_ptr<EventStoreAndWriter> evtwrt);
    ~TrackingProcessor() {};

    void Begin() override;
//................................................................................//
//Initialize all vars to be stored in output files
    void InitEvt() override;
//................................................................................//
//Fill truth variables
    void FillTruth(DTruth *truth_info, std::vector<DStep*> *stepIni,
                   std::vector<TrkHit> rawTagTrk2Hits, std::vector<TrkHit> rawRecTrk2Hits);

    void ProcessEvt(AnaEvent* evt) override;

    void CheckEvt(AnaEvent* evt) override;

    void End() override;

private:

//................................................................................//
//Parameters from config file
//................................................................................//
//Clean mode
//-- 0: False, store all vars in output files
//-- 1: True, store reco level only
    int clean{0};
//................................................................................//
//Strip structure
//-- 0: False
//-- 1: True, automatically load strip structures from geometry
    int if_strip{1};
//................................................................................//
//Smear
//-- 0: False
//-- 1: True, add smear in hit reconstruction
    int if_smear{1};
//................................................................................//
//Fit method
//-- 0/dNone: No method specified, return pre-fitting results from track finding
//-- 1/dKalman: Kalman filter fitter from GenFit
    int Tag_fit_method{-1};
    int Rec_fit_method{-1};
//................................................................................//
//Magnet
//Const magnet field value to be used in const magnet condition or handle exceptions
    double con_field{-1.5};

//................................................................................//
//Geometry
//................................................................................//
    Digitization digitizer;

//................................................................................//
//Magnet
//................................................................................//
    std::vector<DMagnet *> magnets;

//................................................................................//
//Truth
//................................................................................//
//Number of truth hit
    int TagTrk2_No{-1};
//Momentum entering Tag Tracker from Initial Particle Step
    double TagTrk2_pp_truth_ini{RETURN};
//Momentum exiting Tag Tracker from Initial Particle Step
    double TagTrk2_pp_truth_fin{RETURN};
//x, y, z & energy deposition of truth hits
    std::vector<double> TagTrk2_x{};
    std::vector<double> TagTrk2_y{};
    std::vector<double> TagTrk2_z{};
    std::vector<double> TagTrk2_e{};
//No of truth tracks from DTruth
    int TagTrk2_track_No_truth{0};
    
    int RecTrk2_No{-1};
    double RecTrk2_pp_truth_ini{RETURN};
    double RecTrk2_pp_truth_fin{RETURN};
    std::vector<double> RecTrk2_x{};
    std::vector<double> RecTrk2_y{};
    std::vector<double> RecTrk2_z{};
    std::vector<double> RecTrk2_e{};
    int RecTrk2_track_No_truth{0};

//................................................................................//
//Reconstructed
//................................................................................//
    std::vector<DTrack*> tag_tracks_;
    std::vector<DTrack*> rec_tracks_;

//No of reconstructed tracks
    int TagTrk2_track_No{-1};
//Reconstructed momentum perpendicular to magnet
    std::vector<double> TagTrk2_pp{};
//Reconstructed momentum parallel to magnet
    std::vector<double> TagTrk2_pl{};

    std::vector<double> TagTrk2_track_quality{};
//χ2 of reconstructed tracks from helix fitting
    std::vector<double> TagTrk2_track_chi2{};
    std::vector<double> TagTrk2_track_chi2_algo{};
//x/y resolution of reconstructed tracks from helix fitting
    std::vector<double> TagTrk2_track_x_sigma{};
    std::vector<double> TagTrk2_track_y_sigma{};

//................................................................................//
    int RecTrk2_track_No{-1};
    std::vector<double> RecTrk2_pp{};
    std::vector<double> RecTrk2_fixed_pp{};
    std::vector<double> RecTrk2_pl{};

    std::vector<double> RecTrk2_track_quality{};
    std::vector<double> RecTrk2_track_chi2{};
    std::vector<double> RecTrk2_track_chi2_algo{};
    std::vector<double> RecTrk2_track_x_sigma{};
    std::vector<double> RecTrk2_track_y_sigma{};

//x, y & z of reconstucted hits that in tracks
    std::vector<std::vector<double>> RecTrk2_track_x{};
    std::vector<std::vector<double>> RecTrk2_track_y{};
    std::vector<std::vector<double>> RecTrk2_track_z{};

    std::vector<std::vector<double>> RecTrk2_track_extrapolated_x{};
    std::vector<std::vector<double>> RecTrk2_track_extrapolated_y{};

    std::vector<double> RecTrk2_track_preA{};
    std::vector<double> RecTrk2_track_preB{};
    std::vector<double> RecTrk2_track_preR{};

//................................................................................//
    std::vector<double> ECal_seed_x_truth{};
    std::vector<double> ECal_seed_y_truth{};
    std::vector<double> ECal_seed_px_truth{};
    std::vector<double> ECal_seed_py_truth{};
    std::vector<double> ECal_seed_pz_truth{};
    std::vector<double> ECal_seed_e_truth{};
    std::vector<double> ECal_seed_theta_truth{};
    std::vector<double> ECal_seed_phi_truth{};
    std::vector<int> ECal_seed_pdg{};
    std::vector<int> ECal_seed_id_rec_track{};

    std::vector<double> ECal_seed_x{};
    std::vector<double> ECal_seed_y{};
    std::vector<double> ECal_seed_px{};
    std::vector<double> ECal_seed_py{};
    std::vector<double> ECal_seed_pz{};
};

#endif
