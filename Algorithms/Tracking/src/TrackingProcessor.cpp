#include "Algo/TrackingProcessor.h"

//................................................................................//
//C++
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <algorithm>

//................................................................................//
//ROOT
#include "TString.h"
#include "TGeoManager.h"
#include <Math/Vector4D.h>
//#include <TLorentzVector.h>

//................................................................................//
//GENFIT
#include "Algo/KalmanFit/GenFitInclude.h"

//................................................................................//
//FRAMEWORK
#include "Core/AnaData.h"
#include "Utility/Units.h"

//................................................................................//
//TRACKING
//#include "Algo/TypeDef.h"
#include "Algo/Utils/Util.h"
#include "Algo/RiemannFit/RiemannFitHelper.h"
#include "Algo/Vertex/DVertex.h"
#include "Algo/Vertex/VertexFinder.h"

TrackingProcessor::TrackingProcessor(string name, shared_ptr<EventStoreAndWriter> evtwrt) : AnaProcessor(
        std::move(name), std::move(evtwrt)) {
    /*
     *
     *  DEFINE Processor explicitly with NAME in ControlManager.cpp L.37
     *  DEFINE Processor explicitly with NAME in ControlManager.cpp L.37
     *  DEFINE Processor explicitly with NAME in ControlManager.cpp L.37
     *
     */

    // Add description for this AnaProcessor
    Description = "Tracking by Yi-Fan Zhu";

    RegisterIntParameter("verbose", "Verbose", &Verbose, 0);
    RegisterIntParameter("clean", "Clean mode: no truth information", &clean, 1);
    RegisterIntParameter("if_strip", "If use strip structures in trackers", &if_strip, 1);
    RegisterIntParameter("if_smear", "If smear hits in strip structure", &if_smear, 1);
    RegisterIntParameter("if_backwards", "", &if_backwards, 0);
    RegisterIntParameter("Tag_fit_method",
                         "Specify fitting method: 0, no fine fitting; 1, Kalman fitting; 2, Riemann fitting",
                         &Tag_fit_method,
                         2);
    RegisterIntParameter("Rec_fit_method",
                         "Specify fitting method: 0, no fine fitting; 1, Kalman fitting; 2, Riemann fitting",
                         &Rec_fit_method,
                         2);
    RegisterDoubleParameter("con_field", "Const magnet field", &con_field, -1.5);
    RegisterIntParameter("skip_hits_geq", "Skip tagging/recoil tracker reconstruction if total hits number >= N in this tracker region (N<=0: infinite)", &skip_hits_geq, 40);
    RegisterDoubleParameter("remove_hit_less_E", "[MeV] Remove small energy deposition that should not counted as a hit. Apply on raw hits.", &remove_hit_less_E, 0.02);
    RegisterIntParameter("processes", "Processes to run", &process_, tracking::dAll);
}

void TrackingProcessor::Begin() {
//................................................................................//
//Setup algos
//................................................................................//
    DTrack::SetResolutions(0.03, 0.03/0.05, 0.);

    digitizer_.ReadTrackerInfo(if_strip);
    digitizer_.SetIfSmear(if_smear);
    digitizer_.SetClusterWidth(0.1);

    tag_seeder_config_.verbose = Verbose;
    rec_seeder_config_.verbose = Verbose;
    seed_finder_.Connect(&TrkHit::GetX, &TrkHit::GetZ, &TrkHit::GetY);
    tag_finder_config_.verbose = Verbose;
    rec_finder_config_.verbose = Verbose;

    genfit_config_.propagator = &propagator_;

//................................................................................//
//Load fitter info
//................................................................................//
    magnets = dAnaData->getMagFieldVec();
    if(magnets.size() != 3 || !magnets.at(0) || !magnets.at(1) || !magnets.at(2))
        dAnaData->setConstMagnetField({0., con_field, 0.});

    using namespace dunits;

    if(Tag_fit_method == tracking::dKalman || Rec_fit_method == tracking::dKalman)
    {
        genfit::MaterialEffects::getInstance()->init(new genfit::TGeoMaterialInterface());
        if (magnets.size() == 3 && magnets.at(0) && magnets.at(1) && magnets.at(2)) {
            genfit::FieldManager::getInstance()->init(new genfit::MapField(*(magnets.at(0)),
                                                                           *(magnets.at(1)),
                                                                           *(magnets.at(2)),
                                                                           dss_to_genfit::T)); //T->kGs
        } else {
            genfit::FieldManager::getInstance()->init(new genfit::ConstField(dAnaData->getMagnetFieldAt({0., 0., 0.}).at(0)*dss_to_genfit::T,
                                                                             dAnaData->getMagnetFieldAt({0., 0., 0.}).at(1)*dss_to_genfit::T,
                                                                             dAnaData->getMagnetFieldAt({0., 0., 0.}).at(2)*dss_to_genfit::T));
        }
    }

    if(Tag_fit_method == tracking::dRiemann || Rec_fit_method == tracking::dRiemann)
    {
        //RiemannFitHelper::CreateInstance();
        //dRFitHelper->SetVerbose(Verbose);
        RiemannFitHelper::SetVerbose(Verbose);
        RiemannFitHelper::SetMagnetAtOrigin(dAnaData->getMagnetFieldAt({0., 0., 0.}).at(0),
                                            dAnaData->getMagnetFieldAt({0., 0., 0.}).at(1),
                                            dAnaData->getMagnetFieldAt({0., 0., 0.}).at(2));
        RiemannFitHelper::SetTrackerLayerThickness(dAnaData->getLayerThicknessRec().at(0));
        RiemannFitHelper::SetMeasurementError(digitizer_.GetClusterWidth(), dAnaData->getAnglesRec().at(0));
    }

//................................................................................//
//Register dp_ana.root
//................................................................................//
//Truth
//................................................................................//
    if (!clean) {
        EvtWrt->RegisterIntVariable("TagTrk2_No", &TagTrk2_No, "TagTrk2_No/I");
        EvtWrt->RegisterDoubleVariable("TagTrk2_pp_truth_ini", &TagTrk2_pp_truth_ini, "TagTrk2_pp_truth_ini/D");
        EvtWrt->RegisterDoubleVariable("TagTrk2_pp_truth_fin", &TagTrk2_pp_truth_fin, "TagTrk2_pp_truth_fin/D");
        EvtWrt->RegisterOutVariable("TagTrk2_truth_hit_x", &TagTrk2_truth_hit_x);
        EvtWrt->RegisterOutVariable("TagTrk2_truth_hit_y", &TagTrk2_truth_hit_y);
        EvtWrt->RegisterOutVariable("TagTrk2_truth_hit_z", &TagTrk2_truth_hit_z);
        EvtWrt->RegisterOutVariable("TagTrk2_truth_hit_e", &TagTrk2_truth_hit_e);
        EvtWrt->RegisterIntVariable("TagTrk2_track_No_truth", &TagTrk2_track_No_truth, "TagTrk2_track_No_truth/I");
        EvtWrt->RegisterOutVariable("TagTrk2_truth_state_x", &TagTrk2_truth_state_x);
        EvtWrt->RegisterOutVariable("TagTrk2_truth_state_y", &TagTrk2_truth_state_y);
        EvtWrt->RegisterOutVariable("TagTrk2_truth_state_z", &TagTrk2_truth_state_z);

        EvtWrt->RegisterIntVariable("RecTrk2_No", &RecTrk2_No, "RecTrk2_No/I");
        EvtWrt->RegisterDoubleVariable("RecTrk2_pp_truth_ini", &RecTrk2_pp_truth_ini, "RecTrk2_pp_truth_ini/D");
        EvtWrt->RegisterDoubleVariable("RecTrk2_pp_truth_fin", &RecTrk2_pp_truth_fin, "RecTrk2_pp_truth_fin/D");
        EvtWrt->RegisterOutVariable("RecTrk2_truth_hit_x", &RecTrk2_truth_hit_x);
        EvtWrt->RegisterOutVariable("RecTrk2_truth_hit_y", &RecTrk2_truth_hit_y);
        EvtWrt->RegisterOutVariable("RecTrk2_truth_hit_z", &RecTrk2_truth_hit_z);
        EvtWrt->RegisterOutVariable("RecTrk2_truth_hit_e", &RecTrk2_truth_hit_e);
        EvtWrt->RegisterIntVariable("RecTrk2_track_No_truth", &RecTrk2_track_No_truth, "RecTrk2_track_No_truth/I");
        EvtWrt->RegisterOutVariable("RecTrk2_truth_state_x", &RecTrk2_truth_state_x);
        EvtWrt->RegisterOutVariable("RecTrk2_truth_state_y", &RecTrk2_truth_state_y);
        EvtWrt->RegisterOutVariable("RecTrk2_truth_state_z", &RecTrk2_truth_state_z);
        EvtWrt->RegisterOutVariable("Trk_contrib_pdg", &Trk_contrib_pdg, "PDG Id of the hit particle (requires save_mcp_helper)");
        EvtWrt->RegisterOutVariable("Trk_contrib_create_process", &Trk_contrib_create_process, "Creation process of the hit particle (requires save_mcp_helper)");
        EvtWrt->RegisterOutVariable("Trk_contrib_z", &Trk_contrib_z, "Position of the raw tracker hit in Z direction [cm] (requires save_mcp_helper)");
        EvtWrt->RegisterOutVariable("Trk_deposit_E", &Trk_deposit_E, "Deposit energy of the raw tracker hit [MeV] (requires save_mcp_helper)");
        EvtWrt->RegisterOutVariable("Trk_contrib_E", &Trk_contrib_E, "Energy of the raw tracker hit particle [MeV] (requires save_mcp_helper)");
        EvtWrt->RegisterOutVariable("Trk_contrib_Initial_count",&Trk_contrib_Initial_count, "Hit count from the Initial particle(s) (requires save_mcp_helper)");
        EvtWrt->RegisterOutVariable("Trk_contrib_conv_count"   ,&Trk_contrib_conv_count, "Hit count from the G4GammaConversion final states (requires save_mcp_helper)");
        EvtWrt->RegisterOutVariable("Trk_contrib_eIoni_count"  ,&Trk_contrib_eIoni_count, "Hit count from the G4eIonisation final states (requires save_mcp_helper)");
        EvtWrt->RegisterOutVariable("Trk_contrib_compt_count"  ,&Trk_contrib_compt_count, "Hit count from the G4ComptonScattering final states (requires save_mcp_helper)");
        EvtWrt->RegisterOutVariable("Trk_contrib_eBrem_count"  ,&Trk_contrib_eBrem_count, "Hit count from the G4eBremsstrahlung final states (requires save_mcp_helper)");
        EvtWrt->RegisterOutVariable("Trk_contrib_phot_count"   ,&Trk_contrib_phot_count, "Hit count from the G4PhotoElectricEffect final states (requires save_mcp_helper)");
    }
//................................................................................//
//Reconstructed
//................................................................................//
    EvtWrt->RegisterIntVariable("TagTrk2_track_No", &TagTrk2_track_No, "TagTrk2_track_No/I");
    EvtWrt->RegisterOutVariable("TagTrk2_pp", &TagTrk2_pp);
    EvtWrt->RegisterOutVariable("TagTrk2_track_chi2",      &TagTrk2_track_chi2);
    EvtWrt->RegisterOutVariable("TagTrk2_track_chi2_algo", &TagTrk2_track_chi2_algo);

    if (!clean) {
        EvtWrt->RegisterIntVariable("TagTrk2_seed_No", &TagTrk2_seed_No, "TagTrk2_seed_No/I");
//        EvtWrt->RegisterOutVariable("TagTrk2_track_quality", &TagTrk2_track_quality);
        EvtWrt->RegisterOutVariable("TagTrk2_track_x_sigma", &TagTrk2_track_x_sigma);
        EvtWrt->RegisterOutVariable("TagTrk2_track_y_sigma", &TagTrk2_track_y_sigma);
    }

//................................................................................//
    EvtWrt->RegisterIntVariable("RecTrk2_track_No", &RecTrk2_track_No, "RecTrk2_track_No/I");
    EvtWrt->RegisterOutVariable("RecTrk2_pp", &RecTrk2_pp);
//    EvtWrt->RegisterOutVariable("RecTrk2_fixed_pp", &RecTrk2_fixed_pp);
    EvtWrt->RegisterOutVariable("RecTrk2_track_chi2",      &RecTrk2_track_chi2);
    EvtWrt->RegisterOutVariable("RecTrk2_track_chi2_algo", &RecTrk2_track_chi2_algo);

    if (!clean) {
        EvtWrt->RegisterIntVariable("RecTrk2_seed_No", &RecTrk2_seed_No, "RecTrk2_seed_No/I");

//        EvtWrt->RegisterOutVariable("RecTrk2_track_quality", &RecTrk2_track_quality);
        EvtWrt->RegisterOutVariable("RecTrk2_track_x_sigma", &RecTrk2_track_x_sigma);
        EvtWrt->RegisterOutVariable("RecTrk2_track_y_sigma", &RecTrk2_track_y_sigma);

        EvtWrt->RegisterOutVariable("RecTrk2_track_x", &RecTrk2_track_x);
        EvtWrt->RegisterOutVariable("RecTrk2_track_y", &RecTrk2_track_y);
        EvtWrt->RegisterOutVariable("RecTrk2_track_z", &RecTrk2_track_z);

        EvtWrt->RegisterOutVariable("RecTrk2_track_extrapolated_x", &RecTrk2_track_extrapolated_x);
        EvtWrt->RegisterOutVariable("RecTrk2_track_extrapolated_y", &RecTrk2_track_extrapolated_y);
        EvtWrt->RegisterOutVariable("RecTrk2_track_corrections_x",  &RecTrk2_track_corrections_x);

        EvtWrt->RegisterOutVariable("RecTrk2_track_preA", &RecTrk2_track_preA);
        EvtWrt->RegisterOutVariable("RecTrk2_track_preB", &RecTrk2_track_preB);
        EvtWrt->RegisterOutVariable("RecTrk2_track_preR", &RecTrk2_track_preR);
    }

    EvtWrt->RegisterOutVariable("RecTrk2_vertex_z", &RecTrk2_vertex_z);

    EvtWrt->RegisterOutVariable("ECal_seed_x_truth",  &ECal_seed_x_truth);
    EvtWrt->RegisterOutVariable("ECal_seed_y_truth",  &ECal_seed_y_truth);
    EvtWrt->RegisterOutVariable("ECal_seed_px_truth", &ECal_seed_px_truth);
    EvtWrt->RegisterOutVariable("ECal_seed_py_truth", &ECal_seed_py_truth);
    EvtWrt->RegisterOutVariable("ECal_seed_pz_truth", &ECal_seed_pz_truth);
    EvtWrt->RegisterOutVariable("ECal_seed_e_truth",  &ECal_seed_e_truth);
    EvtWrt->RegisterOutVariable("ECal_seed_theta_truth", &ECal_seed_theta_truth);
    EvtWrt->RegisterOutVariable("ECal_seed_phi_truth", &ECal_seed_phi_truth);
    EvtWrt->RegisterOutVariable("ECal_seed_pdg", &ECal_seed_pdg);
    EvtWrt->RegisterOutVariable("ECal_seed_id_rec_track", &ECal_seed_id_rec_track);

    EvtWrt->RegisterOutVariable("ECal_seed_x",  &ECal_seed_x);
    EvtWrt->RegisterOutVariable("ECal_seed_y",  &ECal_seed_y);
    EvtWrt->RegisterOutVariable("ECal_seed_px", &ECal_seed_px);
    EvtWrt->RegisterOutVariable("ECal_seed_py", &ECal_seed_py);
    EvtWrt->RegisterOutVariable("ECal_seed_pz", &ECal_seed_pz);

    if(Verbose == 0) return;
    TString process_info;
    if      (process_ == tracking::dVertex) process_info = "vertexing";
    else if (process_ == tracking::dFit)    process_info = "fitting";
    else if (process_ == tracking::dFind)   process_info = "finding";
    else if (process_ == tracking::dDigi)   process_info = "digitization";
    std::cout << "[Info] ==> Tracking will run processes until" << process_info << std::endl;
}

void TrackingProcessor::ProcessEvt(AnaEvent *evt) {
    [[maybe_unused]] bool if_initial_steps(false);
    [[maybe_unused]] bool if_raw_tag_hits(false);
    [[maybe_unused]] bool if_raw_rec_hits(false);
    [[maybe_unused]] bool if_raw_tag_hit_number(false);
    [[maybe_unused]] bool if_raw_rec_hit_number(false);
    [[maybe_unused]] bool if_reco_tag_hits(false);
    [[maybe_unused]] bool if_reco_rec_hits(false);

//................................................................................//
//Initialize vars
    this->InitEvt();

    const auto &step_collection = evt->getStepCollection();
    //const auto &MCCollection = evt->getMcParticleCollection();
    const auto &simuhit_collection = evt->getSimulatedHitCollection();

    auto it_find_step = step_collection.find("Initial_Particle_Step");
    auto it_find_tag1 = simuhit_collection.find("TagTrk1");
    auto it_find_tag2 = simuhit_collection.find("TagTrk2");
    auto it_find_rec1 = simuhit_collection.find("RecTrk1");
    auto it_find_rec2 = simuhit_collection.find("RecTrk2");

    if (it_find_step == step_collection.end() ||
        it_find_tag1 == simuhit_collection.end() ||
        it_find_tag2 == simuhit_collection.end() ||
        it_find_rec1 == simuhit_collection.end() ||
        it_find_rec2 == simuhit_collection.end())
        return;

    if_initial_steps = true;
    if_raw_tag_hits = true;
    if_raw_rec_hits = true;

//................................................................................//
//Read
    //const auto &mc = MCCollection.at("RawMCParticle");
    const auto &initial_steps = step_collection.at("Initial_Particle_Step");

    std::vector<TrkHit> raw_tagtrk1_hits;
    std::vector<TrkHit> raw_tagtrk2_hits;
    for (auto hit : *simuhit_collection.at("TagTrk1")) {
        if (hit->getE() < remove_hit_less_E) continue;
        raw_tagtrk1_hits.emplace_back(*hit);
    }
    for (auto hit : *simuhit_collection.at("TagTrk2")){
        if (hit->getE() < remove_hit_less_E) continue;
        raw_tagtrk2_hits.emplace_back(*hit);
    }

    std::vector<TrkHit> raw_rectrk1_hits;
    std::vector<TrkHit> raw_rectrk2_hits;
    for (auto hit : *simuhit_collection.at("RecTrk1")) {
        if (hit->getE() < remove_hit_less_E) continue;
        raw_rectrk1_hits.emplace_back(*hit);
    }
    for (auto hit : *simuhit_collection.at("RecTrk2")) {
        if (hit->getE() < remove_hit_less_E) continue;
        raw_rectrk2_hits.emplace_back(*hit);
    }

//................................................................................//
//Tag tracker

    if (IsValidHitSize(raw_tagtrk2_hits) && IsValidHitSize(raw_tagtrk1_hits))
    {
        if_raw_tag_hit_number = true;

//Digitization
        digitizer_.Layering(raw_tagtrk1_hits, raw_tagtrk2_hits, &tag_hit_pool_, tracking::dTag);

        if(tag_hit_pool_.size())
        {
            if_reco_tag_hits = true;

            if(if_raw_tag_hit_number && if_reco_tag_hits)
            {
//Seeding
                SeedContainer_t seeds;
                seed_finder_.Run(tag_seeder_config_, seeds, &tag_hit_pool_);
                TagTrk2_seed_No = seeds.size();

//Finding, by pre-fitting
                tag_finder_.Run(tag_finder_config_, &tag_hit_pool_, seeds);
                tag_tracks_ = tag_finder_.GetTracks();
    
//Fit, by Genfit, Kalman filter/by Riemann fitting
                TagTrk2_track_No = tag_tracks_.size();

                for (auto &track : tag_tracks_)
                {
                    track->SetVerbose(Verbose);
                    if(if_backwards) track->Reverse();

                    WrappedFitter fitter;
                    if      (Tag_fit_method == tracking::dKalman)  fitter.Run(genfit_config_, track);
                    else if (Tag_fit_method == tracking::dRiemann) fitter.Run(riemann_config_, track);
                }
            }
        }
    }

//................................................................................//
//Recoil tracker
    if (IsValidHitSize(raw_rectrk2_hits))
    {
        if_raw_rec_hit_number = true;

//Digitization
        digitizer_.Layering(raw_rectrk1_hits, raw_rectrk2_hits, &rec_hit_pool_, tracking::dRec);

        if(rec_hit_pool_.size())
        {
            if_reco_rec_hits = true;

            if(if_raw_rec_hit_number && if_reco_rec_hits)
            {
//Seeding
                SeedContainer_t seeds;
                seed_finder_.Run(rec_seeder_config_, seeds, &rec_hit_pool_);
                RecTrk2_seed_No = seeds.size();

//Finding, by pre-fitting
                rec_finder_.Run(rec_finder_config_, &rec_hit_pool_, seeds);
                rec_tracks_ = rec_finder_.GetTracks();

//Fit, by Genfit, Kalman filter/by Riemann fitting
                RecTrk2_track_No = rec_tracks_.size();
          
                for (auto &track : rec_tracks_) {
                    track->SetVerbose(Verbose);
                    if(if_backwards) track->Reverse();

                    WrappedFitter fitter;
                    if      (Rec_fit_method == tracking::dKalman)  fitter.Run(genfit_config_, track);
                    else if (Rec_fit_method == tracking::dRiemann) fitter.Run(riemann_config_, track);
                }
            }
        }
    }

//................................................................................//
//Sort tracks by P
    std::sort(tag_tracks_.begin(), tag_tracks_.end(), [](std::shared_ptr<DTrack> &track1, std::shared_ptr<DTrack> &track2)
                                                      { return track1->GetPp() > track2->GetPp(); } );
    std::sort(rec_tracks_.begin(), rec_tracks_.end(), [](std::shared_ptr<DTrack> &track1, std::shared_ptr<DTrack> &track2)
                                                          { return track1->GetPp() > track2->GetPp(); } );
/*
    for(const auto &track : rec_tracks_)
    {
        std::cout << "track: " << track.get() << std::endl;
        for(int i = 0; i < track->GetSize(); i++)
            std::cout << "hit " << i << ": " << track->At(i)->GetTrack().get() << std::endl;
    }
    std::cout << std::endl;
*/

//Vertex
/*
    if(rec_tracks_.size() > 1)
    {
        VertexFinder vertex_finder(&rec_tracks_);
        vertex_finder.FindVertexes(&rec_vertexes_);

        for(const auto &vertex: rec_vertexes_)
            RecTrk2_vertex_z.push_back(vertex->GetZ());
    }
*/
//................................................................................//
//Fill
    for(auto &track : tag_tracks_)
    {
        TagTrk2_pp.push_back(track->GetPp());
//            TagTrk2_track_chi2.push_back(track->GetChi2());
    
        if (!clean)
        {
            TagTrk2_track_chi2_algo.push_back(track->GetChi2Algo());
//            TagTrk2_track_quality.push_back(track->GetQuality());
            TagTrk2_track_x_sigma.push_back(track->GetXSigma());
            TagTrk2_track_y_sigma.push_back(track->GetYSigma());
        }
    }

    for(auto &track : rec_tracks_)
    {
        RecTrk2_pp.push_back(track->GetPp());
//            RecTrk2_track_chi2.push_back(track->GetChi2());

        ECal_seed_x.push_back(track->GetPFlowSeedX());
        ECal_seed_y.push_back(track->GetPFlowSeedY());
        ECal_seed_px.push_back(track->GetPFlowDirctX());
        ECal_seed_py.push_back(track->GetPFlowDirctY());
        ECal_seed_pz.push_back(track->GetPFlowQoP());

        if (!clean) {
            RecTrk2_track_chi2_algo.push_back(track->GetChi2Algo());
//            RecTrk2_track_quality.push_back(track->GetQuality());
            RecTrk2_track_x_sigma.push_back(track->GetXSigma());
            RecTrk2_track_y_sigma.push_back(track->GetYSigma());
     
            std::vector<double> track_x;
            std::vector<double> track_y;
            std::vector<double> track_z;
            for(int hit = 0; hit < track->Size(); hit++)
            {
                track_x.push_back(track->At(hit)->GetX());
                track_y.push_back(track->At(hit)->GetY());
                track_z.push_back(track->At(hit)->GetZ());
            }
            RecTrk2_track_x.push_back(track_x);
            RecTrk2_track_y.push_back(track_y);
            RecTrk2_track_z.push_back(track_z);

            //auto extrapolated_x = track->GetExtrapolated(tracking::dX);
            //auto extrapolated_y = track->GetExtrapolated(tracking::dY);
            //RecTrk2_track_extrapolated_x.push_back(extrapolated_x);
            //RecTrk2_track_extrapolated_y.push_back(extrapolated_y);

            RecTrk2_track_corrections_x.push_back(track->GetCorrectionsX());

            RecTrk2_track_preA.push_back(track->GetPreXc());
            RecTrk2_track_preB.push_back(track->GetPreYc());
            RecTrk2_track_preR.push_back(track->GetPreR());
        }
    }

//................................................................................//
//Write truth
    this->FillTruth(evt->getTruthInfo(), initial_steps, simuhit_collection, raw_tagtrk2_hits, raw_rectrk2_hits);
}

void TrackingProcessor::CheckEvt(AnaEvent *evt) {
   //cout << "check" << endl;
    if (!evt) cerr << "[Warning] ==> Empty event" << endl;
}

void TrackingProcessor::End() {
    //cout<<"End!"<<endl;
}
