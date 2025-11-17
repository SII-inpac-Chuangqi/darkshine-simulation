#include "Algo/TrackingProcessor.h"

//................................................................................//
//C++

//................................................................................//
//ROOT
#include "TString.h"
#include "TGeoManager.h"
#include <Math/Vector4D.h>
//#include <TLorentzVector.h>

//................................................................................//
//GENFIT

//................................................................................//
//FRAMEWORK
#include "Core/AnaData.h"
#include "Utility/Units.h"

//................................................................................//
//TRACKING

void TrackingProcessor::InitEvt() {
//We clear vertices first, then tracks, finally pools which hold the hits' memory
    rec_vertexes_.clear(); rec_vertexes_.shrink_to_fit();

    tag_tracks_.clear(); tag_tracks_.shrink_to_fit();
    rec_tracks_.clear(); rec_tracks_.shrink_to_fit();

    tag_hit_pool_.Clear();
    rec_hit_pool_.Clear();

    TagTrk2_truth_hit_x.clear();
    TagTrk2_truth_hit_y.clear();
    TagTrk2_truth_hit_z.clear();
    TagTrk2_truth_hit_e.clear();
    RecTrk2_truth_hit_x.clear();
    RecTrk2_truth_hit_y.clear();
    RecTrk2_truth_hit_z.clear();
    RecTrk2_truth_hit_e.clear();

    TagTrk2_truth_state_x.clear();
    TagTrk2_truth_state_y.clear();
    TagTrk2_truth_state_z.clear();
    RecTrk2_truth_state_x.clear();
    RecTrk2_truth_state_y.clear();
    RecTrk2_truth_state_z.clear();
    Trk_contrib_pdg.clear();
    Trk_contrib_create_process.clear();
    Trk_contrib_z.clear();
    Trk_contrib_E.clear();
    Trk_deposit_E.clear();
    Trk_contrib_Initial_count = 0;
    Trk_contrib_conv_count = 0;
    Trk_contrib_eIoni_count = 0;
    Trk_contrib_compt_count = 0;
    Trk_contrib_eBrem_count = 0;
    Trk_contrib_phot_count = 0;

    TagTrk2_seed_No = -1;
    RecTrk2_seed_No = -1;

    TagTrk2_track_No_truth = 0;
    RecTrk2_track_No_truth = 0;
    TagTrk2_track_No = -1;
    RecTrk2_track_No = -1;

    TagTrk2_pp_truth_ini = RETURN;
    TagTrk2_pp_truth_fin = RETURN;
    RecTrk2_pp_truth_ini = RETURN;
    RecTrk2_pp_truth_fin = RETURN;

    TagTrk2_pp.clear();
    TagTrk2_track_chi2.clear();
    TagTrk2_track_chi2_algo.clear();
//    TagTrk2_track_quality.clear();
    TagTrk2_track_x_sigma.clear();
    TagTrk2_track_y_sigma.clear();

    target_seed_x_truth.clear();
    target_seed_y_truth.clear();
    target_seed_px_truth.clear();
    target_seed_py_truth.clear();
    target_seed_pz_truth.clear();

    target_seed_x.clear();
    target_seed_y.clear();
    target_seed_px.clear();
    target_seed_py.clear();
    target_seed_pz.clear();

    RecTrk2_pp.clear();
//    RecTrk2_fixed_pp.clear();
    RecTrk2_track_chi2.clear();
    RecTrk2_track_chi2_algo.clear();
//    RecTrk2_track_quality.clear();
    RecTrk2_track_x_sigma.clear();
    RecTrk2_track_y_sigma.clear();

    RecTrk2_track_x.clear();
    RecTrk2_track_y.clear();
    RecTrk2_track_z.clear();

    RecTrk2_track_extrapolated_x.clear();
    RecTrk2_track_extrapolated_y.clear();
    RecTrk2_track_corrections_x.clear();

    RecTrk2_track_preA.clear();
    RecTrk2_track_preB.clear();
    RecTrk2_track_preR.clear();

    ECal_seed_x_truth.clear();
    ECal_seed_y_truth.clear();
    ECal_seed_px_truth.clear();
    ECal_seed_py_truth.clear();
    ECal_seed_pz_truth.clear();
    ECal_seed_e_truth.clear();
    ECal_seed_theta_truth.clear();
    ECal_seed_phi_truth.clear();
    ECal_seed_pdg.clear();
    ECal_seed_id_rec_track.clear();

    ECal_seed_x.clear();
    ECal_seed_y.clear();
    ECal_seed_px.clear();
    ECal_seed_py.clear();
    ECal_seed_pz.clear();

    RecTrk2_vertex_z.clear();

//Now we init the pools
    tag_hit_pool_.Init();
    rec_hit_pool_.Init();
}

void TrackingProcessor::FillTruth(DTruth *truth_info,
                                  std::vector<DStep*> *initial_steps,
                                  const SimulatedHitMap &simu_hits,
                                  const std::vector<TrkHit> &raw_tagtrk2_hits,
                                  const std::vector<TrkHit> &raw_rectrk2_hits) {

    dAnaData->LoadTruthInfo(truth_info);
    //dAnaData->PrintTruthInfo();

    TagTrk2_track_No_truth = dAnaData->getNTruthTracks(DTruth::DTruthDetPV::TagTrk);
    RecTrk2_track_No_truth = dAnaData->getNTruthTracks(DTruth::DTruthDetPV::RecTrk);

    auto truth_states_at_target = dAnaData->getTruthStatesAtTarget();
//    for(const auto &[state, state_info] : truth_states_at_target)
//        std::cout << "state id: " << std::get<0>(state_info)
//                  << ", pdg: " << std::get<1>(state_info) << std::endl;

    auto truth_states_at_ECal = dAnaData->getTruthStatesAtECalFront();
    auto n_truth_states_at_ECal = truth_states_at_ECal.size();

    //std::vector<std::pair<id, std::pair<state, pdg>>>
    std::vector<std::pair<int, std::pair<const DTruthState*, int>>> truth_states_at_ECal_sorted; // If not match std::get<0>=-1
    
    //match rec track
    int id_rec_track=-1;
    for(const auto &track : rec_tracks_)
    {
        id_rec_track++;
        int min_id(-1);
        double min_dis(INFINITY);

        for(size_t i = 0; i < truth_states_at_ECal.size(); i++)
        {
            double dis = (std::hypot(truth_states_at_ECal.at(i).first->vertex[0] - track->GetPFlowSeedX(),
                                     truth_states_at_ECal.at(i).first->vertex[1] - track->GetPFlowSeedY())
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

    for(const auto &track : truth_states_at_ECal){ // appending other truth tracks (unmatched)
        truth_states_at_ECal_sorted.push_back(std::make_pair(-1, track));
    }

    //sanity check
    if(Verbose > 0 && n_truth_states_at_ECal != truth_states_at_ECal_sorted.size()) {
        std::cerr << "[WARNING] ==> Number of sorted truth tracks changed" << std::endl;
        return;
    }

    auto temp_v = new ROOT::Math::PxPyPzEVector();
    for(const auto &[truth_state_id, truth_state_sorted] : truth_states_at_ECal_sorted)
    {
        auto track = truth_state_sorted.first;
        auto pdg = truth_state_sorted.second;
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
        ECal_seed_id_rec_track.push_back(truth_state_id);
    }
    delete temp_v;

    if (!clean) {
    // Fill pcontrib
        for (const auto &[collection_name, hit_collection]: simu_hits) {
            if(collection_name.substr(3,3) != "Trk") continue;
            for (const auto &hit: *hit_collection) {
                if (hit->getPContribution().empty()) continue;
//                if (hit->getE() < remove_hit_less_E) continue;
                Trk_contrib_pdg.emplace_back(hit->getPContribution().at(0).getPdg());
                std::string proc_name = hit->getPContribution().at(0).getCreateProcess().empty() ? "Initial" : hit->getPContribution().at(0).getCreateProcess();
                Trk_contrib_create_process.emplace_back(proc_name);
                Trk_contrib_z.emplace_back(hit->getZ());
                Trk_contrib_E.emplace_back(hit->getPContribution().at(0).getEnergy());
                Trk_deposit_E.emplace_back(hit->getE());
                if (proc_name == "Initial") Trk_contrib_Initial_count++;
                else if (proc_name == "conv") Trk_contrib_conv_count++;
                else if (proc_name == "eIoni") Trk_contrib_eIoni_count++;
                else if (proc_name == "compt") Trk_contrib_compt_count++;
                else if (proc_name == "eBrem") Trk_contrib_eBrem_count++;
                else if (proc_name == "phot") Trk_contrib_phot_count++;
            }
        }

        TagTrk2_No = raw_tagtrk2_hits.size();
 
        bool trackerFlag = false;
        for (const auto &step : *initial_steps) {
            if (tracking::InTagTrack(step->getX(), step->getY(), step->getZ()) && !trackerFlag) {
                TagTrk2_pp_truth_ini = sqrt(step->getPx() * step->getPx() +
                                            step->getPz() * step->getPz());
                trackerFlag = true;
            } else if (!tracking::InTagTrack(step->getX(), step->getY(), step->getZ()) && trackerFlag) {
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

        auto truth_tracks_in_tag = dAnaData->getTruthTracks(DTruth::DTruthDetPV::TagTrk);
        for(const auto &truth_track : truth_tracks_in_tag)
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

            TagTrk2_truth_state_x.push_back(truth_state_x);
            TagTrk2_truth_state_y.push_back(truth_state_y);
            TagTrk2_truth_state_z.push_back(truth_state_z);
        }

        for(const auto &[state, state_info] : truth_states_at_target)
        {
            target_seed_x_truth.push_back(state->vertex[0]);
            target_seed_y_truth.push_back(state->vertex[1]);
            target_seed_px_truth.push_back(state->momentum[0]);
            target_seed_py_truth.push_back(state->momentum[1]);
            target_seed_pz_truth.push_back(state->momentum[2]);
        }
 
        RecTrk2_No = raw_rectrk2_hits.size();
 
        trackerFlag = false;
        for (auto step : *initial_steps) {
            if (tracking::InRecTrack(step->getX(), step->getY(), step->getZ()) && !trackerFlag) {
                RecTrk2_pp_truth_ini = sqrt(step->getPx() * step->getPx() +
                                            step->getPz() * step->getPz());
                trackerFlag = true;
            } else if (!tracking::InRecTrack(step->getX(), step->getY(), step->getZ()) && trackerFlag) {
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
