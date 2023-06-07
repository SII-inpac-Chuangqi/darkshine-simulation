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
#include "Algo/GenFitInclude.h"

//................................................................................//
//FRAMEWORK
#include "Core/AnaData.h"

//................................................................................//
//TRACKING
#include "Algo/TypeDef.h"
#include "Algo/Utils/Util.h"
#include "Algo/TrkHit.h"
#include "Algo/GreedyFinder.h"
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
    RegisterIntParameter("Tag_fit_method",
                         "Specify fitting method: 0, no fine fitting; 1, Kalman fitting",
                         &Tag_fit_method,
                         1);
    RegisterIntParameter("Rec_fit_method",
                         "Specify fitting method: 0, no fine fitting; 1, Kalman fitting",
                         &Rec_fit_method,
                         1);
    RegisterDoubleParameter("con_field", "Const magnet field", &con_field, -1.5);
}

void TrackingProcessor::Begin() {
//................................................................................//
//Load geometry
//................................................................................//
    digitizer.ReadTrackerInfo(if_strip);
    digitizer.SetIfSmear(if_smear);
    digitizer.SetClusterWidth(0.03);

    DTrack::SetResolutions(0.03, 0.03/0.05, 0.);

//................................................................................//
//Load fitter info
//................................................................................//
    magnets = dAnaData->getMagFieldVec();
    if(magnets.size() != 3 || !magnets.at(0) || !magnets.at(1) || !magnets.at(2))
        dAnaData->setConstMagnetField({0., con_field, 0.});

    if(Tag_fit_method == tracking::dKalman || Rec_fit_method == tracking::dKalman)
    {
        genfit::MaterialEffects::getInstance()->init(new genfit::TGeoMaterialInterface());
        if (magnets.size() == 3 && magnets.at(0) && magnets.at(1) && magnets.at(2)) {
            genfit::FieldManager::getInstance()->init(new genfit::MapField(*(magnets.at(0)),
                                                                           *(magnets.at(1)),
                                                                           *(magnets.at(2)),
                                                                           genfit::Tesla)); //T->kGs
        } else {
            genfit::FieldManager::getInstance()->init(new genfit::ConstField(dAnaData->getMagnetFieldAt({0., 0., 0.}).at(0)*10.,
                                                                             dAnaData->getMagnetFieldAt({0., 0., 0.}).at(1)*10., //T->kGs
                                                                             dAnaData->getMagnetFieldAt({0., 0., 0.}).at(2)*10.));
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
        RiemannFitHelper::SetMeasurementError(digitizer.GetClusterWidth(), dAnaData->getAnglesRec().at(0));
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
    }
//................................................................................//
//Reconstructed
//................................................................................//
    EvtWrt->RegisterIntVariable("TagTrk2_track_No", &TagTrk2_track_No, "TagTrk2_track_No/I");
    EvtWrt->RegisterOutVariable("TagTrk2_pp", &TagTrk2_pp);
    EvtWrt->RegisterOutVariable("TagTrk2_track_chi2",      &TagTrk2_track_chi2);
    EvtWrt->RegisterOutVariable("TagTrk2_track_chi2_algo", &TagTrk2_track_chi2_algo);

    if (!clean) {
        EvtWrt->RegisterOutVariable("TagTrk2_track_quality", &TagTrk2_track_quality);
        EvtWrt->RegisterOutVariable("TagTrk2_track_x_sigma", &TagTrk2_track_x_sigma);
        EvtWrt->RegisterOutVariable("TagTrk2_track_y_sigma", &TagTrk2_track_y_sigma);
    }

//................................................................................//
    EvtWrt->RegisterIntVariable("RecTrk2_track_No", &RecTrk2_track_No, "RecTrk2_track_No/I");
    EvtWrt->RegisterOutVariable("RecTrk2_pp", &RecTrk2_pp);
    EvtWrt->RegisterOutVariable("RecTrk2_fixed_pp", &RecTrk2_fixed_pp);
    EvtWrt->RegisterOutVariable("RecTrk2_track_chi2",      &RecTrk2_track_chi2);
    EvtWrt->RegisterOutVariable("RecTrk2_track_chi2_algo", &RecTrk2_track_chi2_algo);

    if (!clean) {
        EvtWrt->RegisterOutVariable("RecTrk2_track_quality", &RecTrk2_track_quality);
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
}

void TrackingProcessor::InitEvt() {

    tag_tracks_.clear(); tag_tracks_.shrink_to_fit();
    rec_tracks_.clear(); rec_tracks_.shrink_to_fit();

    std::vector<double>().swap(TagTrk2_truth_hit_x);
    std::vector<double>().swap(TagTrk2_truth_hit_y);
    std::vector<double>().swap(TagTrk2_truth_hit_z);
    std::vector<double>().swap(TagTrk2_truth_hit_e);
    std::vector<double>().swap(RecTrk2_truth_hit_x);
    std::vector<double>().swap(RecTrk2_truth_hit_y);
    std::vector<double>().swap(RecTrk2_truth_hit_z);
    std::vector<double>().swap(RecTrk2_truth_hit_e);

    std::vector<std::vector<double>>().swap(TagTrk2_truth_state_x);
    std::vector<std::vector<double>>().swap(TagTrk2_truth_state_y);
    std::vector<std::vector<double>>().swap(TagTrk2_truth_state_z);
    std::vector<std::vector<double>>().swap(RecTrk2_truth_state_x);
    std::vector<std::vector<double>>().swap(RecTrk2_truth_state_y);
    std::vector<std::vector<double>>().swap(RecTrk2_truth_state_z);

    TagTrk2_track_No_truth = 0;
    RecTrk2_track_No_truth = 0;
    TagTrk2_track_No = -1;
    RecTrk2_track_No = -1;

    TagTrk2_pp_truth_ini = RETURN;
    TagTrk2_pp_truth_fin = RETURN;
    RecTrk2_pp_truth_ini = RETURN;
    RecTrk2_pp_truth_fin = RETURN;

    std::vector<double>().swap(TagTrk2_pp);
    std::vector<double>().swap(TagTrk2_track_chi2);
    std::vector<double>().swap(TagTrk2_track_chi2_algo);
    std::vector<double>().swap(TagTrk2_track_quality);
    std::vector<double>().swap(TagTrk2_track_x_sigma);
    std::vector<double>().swap(TagTrk2_track_y_sigma);

    std::vector<double>().swap(RecTrk2_pp);
    std::vector<double>().swap(RecTrk2_fixed_pp);
    std::vector<double>().swap(RecTrk2_track_chi2);
    std::vector<double>().swap(RecTrk2_track_chi2_algo);
    std::vector<double>().swap(RecTrk2_track_quality);
    std::vector<double>().swap(RecTrk2_track_x_sigma);
    std::vector<double>().swap(RecTrk2_track_y_sigma);

    std::vector<std::vector<double>>().swap(RecTrk2_track_x);
    std::vector<std::vector<double>>().swap(RecTrk2_track_y);
    std::vector<std::vector<double>>().swap(RecTrk2_track_z);

    std::vector<std::vector<double>>().swap(RecTrk2_track_extrapolated_x);
    std::vector<std::vector<double>>().swap(RecTrk2_track_extrapolated_y);
    std::vector<std::vector<double>>().swap(RecTrk2_track_corrections_x);

    std::vector<double>().swap(RecTrk2_track_preA);
    std::vector<double>().swap(RecTrk2_track_preB);
    std::vector<double>().swap(RecTrk2_track_preR);

    std::vector<double>().swap(ECal_seed_x_truth);
    std::vector<double>().swap(ECal_seed_y_truth);
    std::vector<double>().swap(ECal_seed_px_truth);
    std::vector<double>().swap(ECal_seed_py_truth);
    std::vector<double>().swap(ECal_seed_pz_truth);
    std::vector<double>().swap(ECal_seed_e_truth);
    std::vector<double>().swap(ECal_seed_theta_truth);
    std::vector<double>().swap(ECal_seed_phi_truth);
    std::vector<int>().swap(ECal_seed_pdg);
    std::vector<int>().swap(ECal_seed_id_rec_track);

    std::vector<double>().swap(ECal_seed_x);
    std::vector<double>().swap(ECal_seed_y);
    std::vector<double>().swap(ECal_seed_px);
    std::vector<double>().swap(ECal_seed_py);
    std::vector<double>().swap(ECal_seed_pz);

    rec_vertexes_   .clear(); rec_vertexes_   .shrink_to_fit();
    RecTrk2_vertex_z.clear(); RecTrk2_vertex_z.shrink_to_fit();
}

void TrackingProcessor::FillTruth(DTruth *truth_info,
                                  std::vector<DStep*> *initial_steps,
                                  std::vector<TrkHit> raw_tagtrk2_hits,
                                  std::vector<TrkHit> raw_rectrk2_hits) {

        dAnaData->LoadTruthInfo(truth_info);
        //dAnaData->PrintTruthInfo();

        TagTrk2_track_No_truth = dAnaData->getNTruthTracks(DTruth::DTruthDetPV::TagTrk);
        RecTrk2_track_No_truth = dAnaData->getNTruthTracks(DTruth::DTruthDetPV::RecTrk);

        auto truth_states_at_ECal = dAnaData->getTruthStatesAtECalFront();
        auto n_truth_states_at_ECal = truth_states_at_ECal.size();

        std::vector<std::pair<int, std::pair<const DTruthState*, int>>> truth_states_at_ECal_sorted; // If not match std::get<0>=-1
        // //first sort by truth E
        // std::sort(truth_states_at_ECal.begin(), truth_states_at_ECal.end(),
        //             [&](std::pair<const DTruthState*, int> A, std::pair<const DTruthState*, int> B) -> bool {
        //                     return A.second->E > B.second->E;
        //         });
        
        //then match rec track
        int id_rec_track=-1;
        for(auto &track : rec_tracks_)
        {
            id_rec_track++;
            int min_id(-1);
            double min_dis(INFINITY);

            for(size_t i = 0; i < truth_states_at_ECal.size(); i++)
            {
                double dis = (std::hypot(truth_states_at_ECal.at(i).first->vertex[0] - track->GetECalSeedX(),
                                         truth_states_at_ECal.at(i).first->vertex[1] - track->GetECalSeedY())
                             );
                if(dis < min_dis) {min_dis = dis; min_id = i;}
            }

            if(min_id >= 0 && min_id < static_cast<int>(truth_states_at_ECal.size()))
            {
                truth_states_at_ECal_sorted.push_back(std::make_pair(id_rec_track, truth_states_at_ECal.at(min_id)));
                truth_states_at_ECal.at(min_id).first = nullptr;
                truth_states_at_ECal.erase(truth_states_at_ECal.begin() + min_id);
            }
        }

        for(auto track : truth_states_at_ECal){ // appending other truth tracks (unmatched)
            truth_states_at_ECal_sorted.push_back(std::make_pair(-1, track));
        }

        //sanity check
        if(Verbose > 0 && n_truth_states_at_ECal != truth_states_at_ECal_sorted.size()) {
            std::cerr << "[WARNING] ==> Number of sorted truth tracks changed" << std::endl;
            return;
        }

        auto temp_v = new ROOT::Math::PxPyPzEVector();
        for(auto truth_state_sorted : truth_states_at_ECal_sorted)
        {
            auto track = truth_state_sorted.second.first;
            auto pdg = truth_state_sorted.second.second;
            temp_v->SetPxPyPzE(track->momentum[0], track->momentum[1], track->momentum[2], track->E);

            ECal_seed_x_truth.push_back(track->vertex[0]);
            ECal_seed_y_truth.push_back(track->vertex[1]);
            ECal_seed_px_truth.push_back(track->momentum[0]);
            ECal_seed_py_truth.push_back(track->momentum[1]);
            ECal_seed_pz_truth.push_back(track->momentum[2]);
            ECal_seed_e_truth.push_back(track->E);
            ECal_seed_theta_truth.push_back(temp_v->Theta());
            ECal_seed_phi_truth.push_back(temp_v->Phi());
            ECal_seed_pdg.push_back(pdg);
            ECal_seed_id_rec_track.push_back(truth_state_sorted.first);
        }
        delete temp_v;

    if (!clean) {
        TagTrk2_No = raw_tagtrk2_hits.size();
 
        bool trackerFlag = false;
        for (auto step : *initial_steps) {
            if (InTagTrack(step->getX(), step->getY(), step->getZ()) && !trackerFlag) {
                TagTrk2_pp_truth_ini = sqrt(step->getPx() * step->getPx() +
                                            step->getPz() * step->getPz());
                trackerFlag = true;
            } else if (!InTagTrack(step->getX(), step->getY(), step->getZ()) && trackerFlag) {
                TagTrk2_pp_truth_fin = sqrt(step->getPx() * step->getPx() +
                                            step->getPz() * step->getPz());
                break;
            }
        }
 
        for (int i = 0; i < TagTrk2_No; ++i) {
            TagTrk2_truth_hit_x.push_back(raw_tagtrk2_hits.at(i).GetX());
            TagTrk2_truth_hit_y.push_back(raw_tagtrk2_hits.at(i).GetY());
            TagTrk2_truth_hit_z.push_back(raw_tagtrk2_hits.at(i).GetZ());
            TagTrk2_truth_hit_e.push_back(raw_tagtrk2_hits.at(i).GetE());
        }
 
        RecTrk2_No = raw_rectrk2_hits.size();
 
        trackerFlag = false;
        for (auto step : *initial_steps) {
            if (InRecTrack(step->getX(), step->getY(), step->getZ()) && !trackerFlag) {
                RecTrk2_pp_truth_ini = sqrt(step->getPx() * step->getPx() +
                                            step->getPz() * step->getPz());
                trackerFlag = true;
            } else if (!InRecTrack(step->getX(), step->getY(), step->getZ()) && trackerFlag) {
                RecTrk2_pp_truth_fin = sqrt(step->getPx() * step->getPx() +
                                            step->getPz() * step->getPz());
                break;
            }
        }
 
        for (int i = 0; i < RecTrk2_No; ++i) {
            RecTrk2_truth_hit_x.push_back(raw_rectrk2_hits.at(i).GetX());
            RecTrk2_truth_hit_y.push_back(raw_rectrk2_hits.at(i).GetY());
            RecTrk2_truth_hit_z.push_back(raw_rectrk2_hits.at(i).GetZ());
            RecTrk2_truth_hit_e.push_back(raw_rectrk2_hits.at(i).GetE());
        }

        auto truth_tracks_in_rec = dAnaData->getTruthTracks(DTruth::DTruthDetPV::RecTrk);
        for(const auto &truth_track : truth_tracks_in_rec)
        {
            std::vector<double> truth_state_x;
            std::vector<double> truth_state_y;
            std::vector<double> truth_state_z;

            for(const auto &state : truth_track.second)
            {
                truth_state_x.push_back(state->vertex[0]);
                truth_state_y.push_back(state->vertex[1]);
                truth_state_z.push_back(state->vertex[2]);
            }

            RecTrk2_truth_state_x.push_back(truth_state_x);
            RecTrk2_truth_state_y.push_back(truth_state_y);
            RecTrk2_truth_state_z.push_back(truth_state_z);
        }
    }
}

void TrackingProcessor::ProcessEvt(AnaEvent *evt) {
    [[maybe_unused]] bool if_initial_steps(false);
    [[maybe_unused]] bool if_raw_tag_hits(false);
    [[maybe_unused]] bool if_raw_rec_hits(false);
    [[maybe_unused]] bool if_raw_tag_hit_number(false);
    [[maybe_unused]] bool if_raw_rec_hit_number(false);
    [[maybe_unused]] bool if_reco_tag_hits(false);
    [[maybe_unused]] bool if_reco_rec_hits(false);

    std::vector<double> magnet_at_origin = {magnets.size() && magnets.at(0) ? magnets.at(0)->GetField(0., 0., 0.) : 0.,
                                            magnets.size() && magnets.at(1) ? magnets.at(1)->GetField(0., 0., 0.) : con_field,
                                            magnets.size() && magnets.at(2) ? magnets.at(2)->GetField(0., 0., 0.) : 0.};

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
    if (it_find_step != step_collection.end() &&
        it_find_tag1 != simuhit_collection.end() &&
        it_find_tag2 != simuhit_collection.end() &&
        it_find_rec1 != simuhit_collection.end() &&
        it_find_rec2 != simuhit_collection.end())
    {
        if_initial_steps = true;
        if_raw_tag_hits = true;
        if_raw_rec_hits = true;

//................................................................................//
//Read
        //const auto &mc = MCCollection.at("RawMCParticle");
        const auto &initial_steps = step_collection.at("Initial_Particle_Step");

        std::vector<TrkHit> raw_tagtrk1_hits;
        std::vector<TrkHit> raw_tagtrk2_hits;
        for (auto hit : *simuhit_collection.at("TagTrk1")) raw_tagtrk1_hits.emplace_back(*hit);
        for (auto hit : *simuhit_collection.at("TagTrk2")) raw_tagtrk2_hits.emplace_back(*hit);

        std::vector<TrkHit> raw_rectrk1_hits;
        std::vector<TrkHit> raw_rectrk2_hits;
        for (auto hit : *simuhit_collection.at("RecTrk1")) raw_rectrk1_hits.emplace_back(*hit);
        for (auto hit : *simuhit_collection.at("RecTrk2")) raw_rectrk2_hits.emplace_back(*hit);

//................................................................................//
//Tag tracker
        TrkHitPVecMap clus_tag_trkhit_map;
        //if (raw_tagtrk2_hits.size() < 20 && raw_tagtrk2_hits.size() > 2)
        if (raw_tagtrk2_hits.size() > 2)
        {
            if_raw_tag_hit_number = true;

//Digitization
            digitizer.Layering(raw_tagtrk1_hits, raw_tagtrk2_hits, clus_tag_trkhit_map, tracking::tag);            

            if(clus_tag_trkhit_map.size())
            {
                if_reco_tag_hits = true;

                if(if_raw_tag_hit_number && if_reco_tag_hits)
                {
//Finding, by pre-fitting
                    GreedyFinder find_tag(clus_tag_trkhit_map);
                    find_tag.FillTracks(&tag_tracks_);
        
//Fit, by Genfit, Kalman filter/by Riemann fitting
                    TagTrk2_track_No = find_tag.GetTrackNo();

                    for (auto &track : tag_tracks_)
                    {
                        track->SetVerbose(Verbose);
                        int size = track->GetSize();
                        double x = 0.;
                        double y = 0.5*(track->At(0)->GetY() + track->At(size - 1)->GetY());
                        double z = 0.5*(track->At(0)->GetZ() + track->At(size - 1)->GetZ());
                        std::vector<double> magnet_at_median = {dAnaData->getMagnetFieldAt({x, y, z}).at(0)*10.,
                                                                dAnaData->getMagnetFieldAt({x, y, z}).at(1)*10.,
                                                                dAnaData->getMagnetFieldAt({x, y, z}).at(2)*10.};
                        track->ExceptionHandler(magnet_at_origin);
                        track->Reverse();
                        track->Fit(Tag_fit_method);            //choose fitting method: Kalman filter/Riemann fit
                        //track->Evaluate();
                    }
                }
            }
        }

//................................................................................//
//Recoil tracker
        TrkHitPVecMap clus_rec_trkhit_map;
        //if (raw_rectrk2_hits.size() < 20 && raw_rectrk2_hits.size() > 2)
        if (raw_rectrk2_hits.size() > 2)
        {
            if_raw_rec_hit_number = true;

//Digitization
            digitizer.Layering(raw_rectrk1_hits, raw_rectrk2_hits, clus_rec_trkhit_map, tracking::rec);            

            if(clus_rec_trkhit_map.size())
            {
                if_reco_rec_hits = true;

                if(if_raw_rec_hit_number && if_reco_rec_hits)
                {
//Finding, by pre-fitting
                    GreedyFinder find_rec(clus_rec_trkhit_map);
                    find_rec.FillTracks(&rec_tracks_);

//Fit, by Genfit, Kalman filter/by Riemann fitting
                    RecTrk2_track_No = find_rec.GetTrackNo();
              
                    for (auto &track : rec_tracks_) {
                        track->SetVerbose(Verbose);
                        int size = track->GetSize();
                        double x = 0.;
                        double y = 0.5*(track->At(0)->GetY() + track->At(size - 1)->GetY());
                        double z = 0.5*(track->At(0)->GetZ() + track->At(size - 1)->GetZ());
                        std::vector<double> magnet_at_median = {dAnaData->getMagnetFieldAt({x, y, z}).at(0)*10.,
                                                                dAnaData->getMagnetFieldAt({x, y, z}).at(1)*10.,
                                                                dAnaData->getMagnetFieldAt({x, y, z}).at(2)*10.};
                        track->ExceptionHandler(magnet_at_origin);
                        track->Reverse();
                        track->Fit(Tag_fit_method);            //choose fitting method: Kalman filter/Riemann fit
                        //track->Evaluate();
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

        if(rec_tracks_.size() > 1)
        {
            VertexFinder vertex_finder(&rec_tracks_);
            vertex_finder.FindVertexes(&rec_vertexes_);

            for(const auto &vertex: rec_vertexes_)
                RecTrk2_vertex_z.push_back(vertex->GetZ());
        }

//................................................................................//
//Fill
        for(auto &track : tag_tracks_)
        {
            TagTrk2_pp.push_back(track->GetPp());
            TagTrk2_track_chi2.push_back(track->GetChi2());
        
            if (!clean)
            {
                TagTrk2_track_chi2_algo.push_back(track->GetChi2Algo());
                TagTrk2_track_quality.push_back(track->GetQuality());
                TagTrk2_track_x_sigma.push_back(track->GetXSigma());
                TagTrk2_track_y_sigma.push_back(track->GetYSigma());
            }
        }

        for(auto &track : rec_tracks_)
        {
            RecTrk2_pp.push_back(track->GetPp());
            RecTrk2_fixed_pp.push_back(track->GetFixedPp());
            RecTrk2_track_chi2.push_back(track->GetChi2());

            ECal_seed_x.push_back(track->GetECalSeedX());
            ECal_seed_y.push_back(track->GetECalSeedY());
            ECal_seed_px.push_back(track->GetECalDirctX());
            ECal_seed_py.push_back(track->GetECalDirctY());
            ECal_seed_pz.push_back(track->GetECalQoP());

            if (!clean) {
                RecTrk2_track_chi2_algo.push_back(track->GetChi2Algo());
                RecTrk2_track_quality.push_back(track->GetQuality());
                RecTrk2_track_x_sigma.push_back(track->GetXSigma());
                RecTrk2_track_y_sigma.push_back(track->GetYSigma());
         
                std::vector<double> track_x;
                std::vector<double> track_y;
                std::vector<double> track_z;
                for(int hit = 0; hit < track->GetSize(); hit++)
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
        this->FillTruth(evt->getTruthInfo(), initial_steps, raw_tagtrk2_hits, raw_rectrk2_hits);
    }
}

void TrackingProcessor::CheckEvt(AnaEvent *evt) {
   //cout << "check" << endl;
    if (!evt) cerr << "[Warning] ==> Empty event" << endl;
}

void TrackingProcessor::End() {
    //cout<<"End!"<<endl;
}
