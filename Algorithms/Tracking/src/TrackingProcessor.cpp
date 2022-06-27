//................................................................................//
//CPP STL
#include <vector>
#include <map>
#include <memory>
#include <string>

//................................................................................//
//ROOT
#include "TString.h"
#include "TGeoManager.h"

//................................................................................//
//GENFIT
#include "Algo/GenFitInclude.h"

//................................................................................//
//FRAMEWORK
#include "Algo/TrackingProcessor.h"
#include "Core/AnaData.h"

//................................................................................//
//TRACKING
#include "Algo/TypeDef.h"
#include "Algo/Util.h"
#include "Algo/TrkHit.h"
#include "Algo/GreedyFinding.h"
#include "Algo/DTrack.h"

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

    RegisterIntParameter("clean", "Clean mode: no truth information", &clean, 1);
    RegisterIntParameter("if_strip", "If use strip structures in trackers", &if_strip, 1);
    RegisterIntParameter("if_smear", "If smear hits in strip structure", &if_smear, 1);
    RegisterIntParameter("Tag_fit_method",
                         "Specify fitting method: 0, no fine fitting; 1, Kalman fitting",
                         &Tag_fit_method, 1);
    RegisterIntParameter("Rec_fit_method",
                         "Specify fitting method: 0, no fine fitting; 1, Kalman fitting",
                         &Rec_fit_method, 1);
    RegisterDoubleParameter("con_field", "Const magnet field", &con_field, -1.5);
}

void TrackingProcessor::Begin() {
//................................................................................//
//Load Geometry
//................................................................................//
    digitizer.GetTrackerInfo(if_strip);
    digitizer.SetIfSmear(if_smear);

//................................................................................//
//Load magnet
//................................................................................//
    magnets = dAnaData->getMagFieldVec();
    if(Tag_fit_method == dKalman || Rec_fit_method == dKalman)
    {
        genfit::MaterialEffects::getInstance()->init(new genfit::TGeoMaterialInterface());
        if (magnets.size() == 3 && magnets.at(0) && magnets.at(1) && magnets.at(2)) {
            genfit::FieldManager::getInstance()->init(new genfit::MapField(*(magnets.at(0)),
                                                                           *(magnets.at(1)),
                                                                           *(magnets.at(2)),
                                                                           genfit::Tesla)); //T->kGs
        } else {
            genfit::FieldManager::getInstance()->init(new genfit::ConstField(0., con_field*10., 0.));
        }
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
        EvtWrt->RegisterOutVariable("TagTrk2_x", &TagTrk2_x);
        EvtWrt->RegisterOutVariable("TagTrk2_y", &TagTrk2_y);
        EvtWrt->RegisterOutVariable("TagTrk2_z", &TagTrk2_z);
        EvtWrt->RegisterOutVariable("TagTrk2_e", &TagTrk2_e);
        EvtWrt->RegisterIntVariable("TagTrk2_track_No_truth", &TagTrk2_track_No_truth, "TagTrk2_track_No_truth/I");

        EvtWrt->RegisterIntVariable("RecTrk2_No", &RecTrk2_No, "RecTrk2_No/I");
        EvtWrt->RegisterDoubleVariable("RecTrk2_pp_truth_ini", &RecTrk2_pp_truth_ini, "RecTrk2_pp_truth_ini/D");
        EvtWrt->RegisterDoubleVariable("RecTrk2_pp_truth_fin", &RecTrk2_pp_truth_fin, "RecTrk2_pp_truth_fin/D");
        EvtWrt->RegisterOutVariable("RecTrk2_x", &RecTrk2_x);
        EvtWrt->RegisterOutVariable("RecTrk2_y", &RecTrk2_y);
        EvtWrt->RegisterOutVariable("RecTrk2_z", &RecTrk2_z);
        EvtWrt->RegisterOutVariable("RecTrk2_e", &RecTrk2_e);
        EvtWrt->RegisterIntVariable("RecTrk2_track_No_truth", &RecTrk2_track_No_truth, "RecTrk2_track_No_truth/I");
    }
//................................................................................//
//Reconstructed
//................................................................................//
    EvtWrt->RegisterIntVariable("TagTrk2_track_No", &TagTrk2_track_No, "TagTrk2_track_No/I");
    EvtWrt->RegisterOutVariable("TagTrk2_pp", &TagTrk2_pp);
    EvtWrt->RegisterOutVariable("TagTrk2_track_chi2", &TagTrk2_track_chi2);

    if (!clean) {
        EvtWrt->RegisterOutVariable("TagTrk2_track_quality", &TagTrk2_track_quality);
        EvtWrt->RegisterOutVariable("TagTrk2_track_x_sigma", &TagTrk2_track_x_sigma);
        EvtWrt->RegisterOutVariable("TagTrk2_track_y_sigma", &TagTrk2_track_y_sigma);
    }

//................................................................................//
    EvtWrt->RegisterIntVariable("RecTrk2_track_No", &RecTrk2_track_No, "RecTrk2_track_No/I");
    EvtWrt->RegisterOutVariable("RecTrk2_pp", &RecTrk2_pp);
    EvtWrt->RegisterOutVariable("RecTrk2_track_chi2", &RecTrk2_track_chi2);

    if (!clean) {
        EvtWrt->RegisterOutVariable("RecTrk2_track_quality", &RecTrk2_track_quality);
        EvtWrt->RegisterOutVariable("RecTrk2_track_x_sigma", &RecTrk2_track_x_sigma);
        EvtWrt->RegisterOutVariable("RecTrk2_track_y_sigma", &RecTrk2_track_y_sigma);

        EvtWrt->RegisterOutVariable("RecTrk2_track_x", &RecTrk2_track_x); 
        EvtWrt->RegisterOutVariable("RecTrk2_track_y", &RecTrk2_track_y);
        EvtWrt->RegisterOutVariable("RecTrk2_track_z", &RecTrk2_track_z);
    }

/*
    EvtWrt->RegisterOutVariable("ECal_seed_x",  &ECal_seed_x, "", false);
    EvtWrt->RegisterOutVariable("ECal_seed_y",  &ECal_seed_y, "", false);
    EvtWrt->RegisterOutVariable("ECal_seed_px", &ECal_seed_px, "", false);
    EvtWrt->RegisterOutVariable("ECal_seed_py", &ECal_seed_py, "", false);
    EvtWrt->RegisterOutVariable("ECal_seed_pz", &ECal_seed_pz, "", false);
*/

    EvtWrt->RegisterOutVariable("ECal_seed_x_truth",  &ECal_seed_x_truth);
    EvtWrt->RegisterOutVariable("ECal_seed_y_truth",  &ECal_seed_y_truth);
    EvtWrt->RegisterOutVariable("ECal_seed_px_truth", &ECal_seed_px_truth);
    EvtWrt->RegisterOutVariable("ECal_seed_py_truth", &ECal_seed_py_truth);
    EvtWrt->RegisterOutVariable("ECal_seed_pz_truth", &ECal_seed_pz_truth);

    EvtWrt->RegisterOutVariable("ECal_seed_x",  &ECal_seed_x);
    EvtWrt->RegisterOutVariable("ECal_seed_y",  &ECal_seed_y);
    EvtWrt->RegisterOutVariable("ECal_seed_px", &ECal_seed_px);
    EvtWrt->RegisterOutVariable("ECal_seed_py", &ECal_seed_py);
    EvtWrt->RegisterOutVariable("ECal_seed_pz", &ECal_seed_pz);
}

void TrackingProcessor::CleanEvt() {
    if(!clean)
    {
        std::vector<double>().swap(TagTrk2_x);
        std::vector<double>().swap(TagTrk2_y);
        std::vector<double>().swap(TagTrk2_z);
        std::vector<double>().swap(TagTrk2_e);
        std::vector<double>().swap(RecTrk2_x);
        std::vector<double>().swap(RecTrk2_y);
        std::vector<double>().swap(RecTrk2_z);
        std::vector<double>().swap(RecTrk2_e);
    }

    TagTrk2_track_No_truth = 0;
    RecTrk2_track_No_truth = 0;
    TagTrk2_track_No = 0;
    RecTrk2_track_No = 0;

    TagTrk2_pp_truth_ini = RETURN;
    TagTrk2_pp_truth_fin = RETURN;
    RecTrk2_pp_truth_ini = RETURN;
    RecTrk2_pp_truth_fin = RETURN;

    std::vector<double>().swap(TagTrk2_pp);
    std::vector<double>().swap(TagTrk2_track_chi2);
    std::vector<double>().swap(TagTrk2_track_quality);
    std::vector<double>().swap(TagTrk2_track_x_sigma);
    std::vector<double>().swap(TagTrk2_track_y_sigma);

    std::vector<double>().swap(RecTrk2_pp);
    std::vector<double>().swap(RecTrk2_track_chi2);
    std::vector<double>().swap(RecTrk2_track_quality);
    std::vector<double>().swap(RecTrk2_track_x_sigma);
    std::vector<double>().swap(RecTrk2_track_y_sigma);

    std::vector<std::vector<double>>().swap(RecTrk2_track_x);
    std::vector<std::vector<double>>().swap(RecTrk2_track_y);
    std::vector<std::vector<double>>().swap(RecTrk2_track_z);

    std::vector<double>().swap(ECal_seed_x_truth);
    std::vector<double>().swap(ECal_seed_y_truth);
    std::vector<double>().swap(ECal_seed_px_truth);
    std::vector<double>().swap(ECal_seed_py_truth);
    std::vector<double>().swap(ECal_seed_pz_truth);

    std::vector<double>().swap(ECal_seed_x);
    std::vector<double>().swap(ECal_seed_y);
    std::vector<double>().swap(ECal_seed_px);
    std::vector<double>().swap(ECal_seed_py);
    std::vector<double>().swap(ECal_seed_pz);
}

void TrackingProcessor::FillTruth(AnaEvent *evt,
                                  std::vector<DStep*> *initial_steps,
                                  std::vector<TrkHit> raw_tagtrk2_hits,
                                  std::vector<TrkHit> raw_rectrk2_hits) {
/*
        dAnaData->LoadTruthMcPHelper(evt->getMcPHelperCollection());
        dAnaData->PrintTruthMcPHelper();
        auto init_elec = dAnaData->getInitialElectron();
        auto init_elec = dAnaData->getInitialElectron();
        if(init_elec)
        {
            ECal_seed_x_truth.push_back(init_elec->getX());
            ECal_seed_y_truth.push_back(init_elec->getY());
            ECal_seed_px_truth.push_back(init_elec->getPx());
            ECal_seed_py_truth.push_back(init_elec->getPy());
            ECal_seed_pz_truth.push_back(init_elec->getPz());
        }
        else
        {
            ECal_seed_x_truth.push_back(std::nan("RETURN"));
            ECal_seed_y_truth.push_back(std::nan("RETURN"));
            ECal_seed_px_truth.push_back(std::nan("RETURN"));
            ECal_seed_py_truth.push_back(std::nan("RETURN"));
            ECal_seed_pz_truth.push_back(std::nan("RETURN"));
        }
*/

        dAnaData->LoadTruthInfo(evt->getTruthInfo());
        //dAnaData->PrintTruthInfo();

        TagTrk2_track_No_truth = dAnaData->getNTruthTracks(DTruth::DTruthDetPV::TagTrk);
        RecTrk2_track_No_truth = dAnaData->getNTruthTracks(DTruth::DTruthDetPV::RecTrk);
        auto tracks = dAnaData->getTruthTracksAtECalFront();
        for(auto track : tracks)
        {
            ECal_seed_x_truth.push_back(track->vertex[0]);
            ECal_seed_y_truth.push_back(track->vertex[1]);
            ECal_seed_px_truth.push_back(track->momentum[0]);
            ECal_seed_py_truth.push_back(track->momentum[1]);
            ECal_seed_pz_truth.push_back(track->momentum[2]);
        }

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
            TagTrk2_x.push_back(raw_tagtrk2_hits.at(i).GetX());
            TagTrk2_y.push_back(raw_tagtrk2_hits.at(i).GetY());
            TagTrk2_z.push_back(raw_tagtrk2_hits.at(i).GetZ());
            TagTrk2_e.push_back(raw_tagtrk2_hits.at(i).GetE());
        }
 
        RecTrk2_No = raw_rectrk2_hits.size();
 
        trackerFlag = false;
        for (auto step : *initial_steps) {
            if (InRecTrack(step->getX(), step->getY(), step->getZ()) && !trackerFlag) {
                RecTrk2_pp_truth_ini = sqrt(step->getPx() * step->getPx() +
                                            step->getPz() * step->getPz());
                trackerFlag = true;
            } else if (!InTagTrack(step->getX(), step->getY(), step->getZ()) && trackerFlag) {
                RecTrk2_pp_truth_fin = sqrt(step->getPx() * step->getPx() +
                                            step->getPz() * step->getPz());
                break;
            }
        }
 
        for (int i = 0; i < RecTrk2_No; ++i) {
            RecTrk2_x.push_back(raw_rectrk2_hits.at(i).GetX());
            RecTrk2_y.push_back(raw_rectrk2_hits.at(i).GetY());
            RecTrk2_z.push_back(raw_rectrk2_hits.at(i).GetZ());
            RecTrk2_e.push_back(raw_rectrk2_hits.at(i).GetE());
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

    std::vector<double> magnet_at_origin = {magnets.at(0) ? magnets.at(0)->GetField(0., 0., 0.) : 0.,
                                            magnets.at(1) ? magnets.at(1)->GetField(0., 0., 0.) : con_field,
                                            magnets.at(2) ? magnets.at(2)->GetField(0., 0., 0.) : 0.};

//................................................................................//
//Initialize vars
    this->CleanEvt();

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
        std::vector<DTrack> tag_tracks;

        TrkHitPVecMap clus_tag_trkhit_map;
        if (raw_tagtrk2_hits.size() < 20 && raw_tagtrk2_hits.size() > 2)
        {
            if_raw_tag_hit_number = true;

//Digitization
            digitizer.Layering(raw_tagtrk1_hits, raw_tagtrk2_hits, clus_tag_trkhit_map, tag);            

            if(clus_tag_trkhit_map.size())
            {
                if_reco_tag_hits = true;

                if(if_raw_tag_hit_number && if_reco_tag_hits)
                {
//Finding, by pre-fitting
                    std::vector<TrkHitPVec> vec_tag_track;
                    GreedyFinding find_tag(clus_tag_trkhit_map);
                    vec_tag_track.assign(find_tag.First(), find_tag.Last());
        
//Fitting, by Genfit, Kalman filter/by Riemann fitting
                    TagTrk2_track_No = find_tag.GetTrackNo();
        
                    for (int i = 0; i < find_tag.GetTrackNo(); i++)
                    {
        
                        TrkHitPVec tag_track_hits((*(vec_tag_track.begin() + i)).begin(), (*(vec_tag_track.begin() + i)).end());
                        DTrack track(tag_track_hits,
                                     find_tag.GetR(i),       //used in Kalman filter
                                     find_tag.GetCenterX(i), //not used in Kalman filter, reserved
                                     find_tag.GetCenterY(i), //not used in Kalman filter, reserved
                                     magnet_at_origin);      //magnet vector to handle exception
                        track.Fit(Tag_fit_method);           //choose fitting method: Kalman filter
                        track.Evaluate();

                        tag_tracks.push_back(track);
                    }
                }
            }
        }

//................................................................................//
//Recoil tracker
        std::vector<DTrack> rec_tracks;

        TrkHitPVecMap clus_rec_trkhit_map;
        if (raw_rectrk2_hits.size() < 20 && raw_rectrk2_hits.size() > 2)
        {
            if_raw_rec_hit_number = true;

//Digitization
            digitizer.Layering(raw_rectrk1_hits, raw_rectrk2_hits, clus_rec_trkhit_map, rec);            

            if(clus_rec_trkhit_map.size())
            {
                if_reco_rec_hits = true;

                if(if_raw_rec_hit_number && if_reco_rec_hits)
                {
//Finding, by pre-fitting
                    std::vector<TrkHitPVec> vec_rec_track;
                    GreedyFinding find_rec(clus_rec_trkhit_map);
                    vec_rec_track.assign(find_rec.First(), find_rec.Last());

//Fitting, by Genfit, Kalman filter/by Riemann fitting
                    RecTrk2_track_No = find_rec.GetTrackNo();
              
                    for (int i = 0; i < find_rec.GetTrackNo(); i++) {
                        TrkHitPVec rec_track((*(vec_rec_track.begin() + i)).begin(), (*(vec_rec_track.begin() + i)).end());
                        DTrack track(rec_track,
                                     find_rec.GetR(i),       //used in Kalman filter
                                     find_rec.GetCenterX(i), //not used in Kalman filter, reserved
                                     find_rec.GetCenterY(i), //not used in Kalman filter, reserved
                                     magnet_at_origin);      //magnet vector to handle exception
                        track.Fit(Rec_fit_method);           //choose fitting method: Kalman filter
                        track.Evaluate();

                        rec_tracks.push_back(track);
                    }
                }
            }
        }

//................................................................................//
//Post-processing
        for(auto &track : tag_tracks)
        {
            TagTrk2_pp.push_back(track.GetPp());
            TagTrk2_track_chi2.push_back(track.GetChi2());
        
            if (!clean)
            {
                TagTrk2_track_quality.push_back(track.GetQuality());
                TagTrk2_track_x_sigma.push_back(track.GetXSigma());
                TagTrk2_track_y_sigma.push_back(track.GetYSigma());
            }
        }

        for(auto &track : rec_tracks)
        {
            RecTrk2_pp.push_back(track.GetPp());
            RecTrk2_track_chi2.push_back(track.GetChi2());

            ECal_seed_x.push_back(track.GetECalSeedX());
            ECal_seed_y.push_back(track.GetECalSeedY());
            ECal_seed_px.push_back(track.GetECalDirctX());
            ECal_seed_py.push_back(track.GetECalDirctY());
            ECal_seed_pz.push_back(track.GetECalQoP());
         
            if (!clean) {
                RecTrk2_track_quality.push_back(track.GetQuality());
                RecTrk2_track_x_sigma.push_back(track.GetXSigma());
                RecTrk2_track_y_sigma.push_back(track.GetYSigma());
         
                std::vector<double> track_x;
                std::vector<double> track_y;
                std::vector<double> track_z;
                for(int hit = 0; hit < track.GetSize(); hit++)
                {
                    track_x.push_back(track.At(hit)->GetX());
                    track_y.push_back(track.At(hit)->GetY());
                    track_z.push_back(track.At(hit)->GetZ());
                }
         
                RecTrk2_track_x.push_back(track_x);
                RecTrk2_track_y.push_back(track_y);
                RecTrk2_track_z.push_back(track_z);
            }
        }

//................................................................................//
//Write truth
        this->FillTruth(evt, initial_steps, raw_tagtrk2_hits, raw_rectrk2_hits);
    }
}

void TrackingProcessor::CheckEvt(AnaEvent *evt) {
   //cout << "check" << endl;
    if (!evt) cerr << "[Warning] ==> Empty event" << endl;
}

void TrackingProcessor::End() {
    //cout<<"End!"<<endl;
}
