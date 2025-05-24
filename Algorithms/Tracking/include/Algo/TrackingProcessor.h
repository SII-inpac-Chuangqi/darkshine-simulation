#ifndef TRACKINGPROCESSOR_H
#define TRACKINGPROCESSOR_H

#ifndef RETURN
#define RETURN std::nan("RETURN")
#endif

//................................................................................//
//C++
#include <utility>

//................................................................................//
//FRAMEWORK
#include "Core/AnaProcessor.h"

//................................................................................//
//TRACKING
//#include "Algo/TrkHit.h"
#include "Algo/TypeDef.h"
#include "Algo/Object/DTrack.h"
#include "Algo/Object/seed.h"
#include "Algo/Vertex/DVertex.h"
#include "Algo/Digitization.h"

#include "Algo/WrappedSeedFinder.h"
#include "Algo/Seeding/seed_finder.h"
#include "Algo/GreedyFinder.h"
#include "Algo/KalmanFit/KalmanFilterFitter.h"
#include "Algo/RiemannFit/RiemannFitter.h"

namespace tracking
{
    enum process {dAll = 0, dVertex = 1, dFit = 2, dFind = 3, dDigi = 4};
}

class TrackingProcessor : public AnaProcessor
{
public:
    using Pool_t = HitPool<Key, TrkHit>;
    using Seed_t = Seed<TrkHit>; 
//    using SeedFinder_t = SeedFinder<Seed_t, TrkHitSPVecMap, TrkHitSP>; 
    using SeedFinder_t = WrappedSeedFinder;
    using SeedContainer_t = std::vector<Seed_t>;

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
    void FillTruth(DTruth *truth_info, std::vector<DStep*> *stepIni, const SimulatedHitMap &simu_hits,
                   const std::vector<TrkHit> &rawTagTrk2Hits, const std::vector<TrkHit> &rawRecTrk2Hits);

    void ProcessEvt(AnaEvent* evt) override;

    void CheckEvt(AnaEvent* evt) override;

    void End() override;

private:
    bool IsValidHitSize(const std::vector<TrkHit>& trkhits) {
        return (trkhits.size() > 2 && (skip_hits_geq <= 0 || trkhits.size() < (unsigned long)skip_hits_geq));
    }

private:
    SeedFinder_t seed_finder_;

    SeedFinder_t::SeedFinderConfig seed_finder_config_;
    GreedyFinder::Config finding_config_;
    KalmanFilterFitter::Config genfit_config_;
    RiemannFitter::Config riemann_config_;

//................................................................................//
//Parameters from config file
//................................................................................//
//Clean mode
//-- 0: False, store all vars in output files
//-- 1: True, store reco level only
    int clean{0};
//................................................................................//
//Strip structure flag
//-- 0: False
//-- 1: True, automatically load strip structures from geometry
    int if_strip{1};
//................................................................................//
//Smearing
//-- 0: False
//-- 1: True, add smear in hit reconstruction
    int if_smear{1};
    double remove_hit_less_E{0.02};

    int if_backwards{0};

//................................................................................//
//Processing mask
//-- dAll:    run all processes
//-- dVertex: run all processes until vertexing
//-- dFit:    run all processes until fitting
//-- dFind:   run all processes until finding
//-- dDigi:   run all processes until digitization
    int process_{tracking::dAll};

//................................................................................//
//Fit method
//-- 0/dNone: No method specified, return pre-fitting results from track finding
//-- 1/dKalman: Kalman filter fitter from GenFit
    int Tag_fit_method{-1};
    int Rec_fit_method{-1};
    int skip_hits_geq{-1};
//................................................................................//
//Magnet
//Const magnet field value to be used in const magnet condition or handle exceptions
    double con_field{-1.5};

//................................................................................//
//Digitization
//................................................................................//
    Digitization digitizer_;

//................................................................................//
//Magnet
//................................................................................//
    std::vector<DMagnet*> magnets;

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
    std::vector<double> TagTrk2_truth_hit_x{};
    std::vector<double> TagTrk2_truth_hit_y{};
    std::vector<double> TagTrk2_truth_hit_z{};
    std::vector<double> TagTrk2_truth_hit_e{};
//States from DTruth
    std::vector<std::vector<double>> TagTrk2_truth_state_x{};
    std::vector<std::vector<double>> TagTrk2_truth_state_y{};
    std::vector<std::vector<double>> TagTrk2_truth_state_z{};
//No of truth tracks from DTruth
    int TagTrk2_track_No_truth{0};
    
    int RecTrk2_No{-1};
    double RecTrk2_pp_truth_ini{RETURN};
    double RecTrk2_pp_truth_fin{RETURN};
    std::vector<double> RecTrk2_truth_hit_x{};
    std::vector<double> RecTrk2_truth_hit_y{};
    std::vector<double> RecTrk2_truth_hit_z{};
    std::vector<double> RecTrk2_truth_hit_e{};
    int RecTrk2_track_No_truth{0};

    std::vector<std::vector<double>> RecTrk2_truth_state_x{};
    std::vector<std::vector<double>> RecTrk2_truth_state_y{};
    std::vector<std::vector<double>> RecTrk2_truth_state_z{};
    std::vector<int> Trk_contrib_pdg{};
    std::vector<TString> Trk_contrib_create_process{};
    std::vector<double> Trk_contrib_z{};
    std::vector<double> Trk_contrib_E{};
    std::vector<double> Trk_deposit_E{};
    int Trk_contrib_Initial_count{};
    int Trk_contrib_conv_count{};
    int Trk_contrib_eIoni_count{};
    int Trk_contrib_compt_count{};
    int Trk_contrib_eBrem_count{};
    int Trk_contrib_phot_count{};

//................................................................................//
//Reconstructed
//................................................................................//
//We store hits here in the pools, so all tracks and vertices do not hold the memory
    Pool_t tag_hit_pool_;
    Pool_t rec_hit_pool_;

    std::vector<std::shared_ptr<DTrack>> tag_tracks_;
    std::vector<std::shared_ptr<DTrack>> rec_tracks_;

//No of seeds
    int RecTrk2_seed_No{-1};

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
    std::vector<std::vector<double>> RecTrk2_track_corrections_x{};

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

//................................................................................//
    std::vector<std::shared_ptr<DVertex>> rec_vertexes_;
    std::vector<double> RecTrk2_vertex_z{};
};

#endif
