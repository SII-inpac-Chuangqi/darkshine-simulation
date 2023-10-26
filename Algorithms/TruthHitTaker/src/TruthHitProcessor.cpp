//
// Created by Joseph Zhang on 10/26/23.
//

#include "TruthHitProcessor.h"

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

TruthHitProcessor::TruthHitProcessor(string name, shared_ptr<EventStoreAndWriter> evtwrt) : AnaProcessor(
        std::move(name), std::move(evtwrt)){
    // Add description for this AnaProcessor
    Description = "****";

    RegisterIntParameter("verbose", "Verbose", &Verbose, 0);
    RegisterIntParameter("if_strip", "If use strip structures in trackers", &if_strip, 1);
    RegisterIntParameter("if_smear", "If smear hits in strip structure", &if_smear, 1);
    RegisterDoubleParameter("con_field", "Const magnet field", &con_field, -1.5);

}

void TruthHitProcessor::Begin() {
//................................................................................//
//Load geometry
//................................................................................//
    digitizer.ReadTrackerInfo(if_strip);
    digitizer.SetIfSmear(if_smear);
    digitizer.SetClusterWidth(0.03);
    DTrack::SetResolutions(0.03, 0.03 / 0.05, 0.);

//................................................................................//
//Load fitter info
//................................................................................//
    magnets = dAnaData->getMagFieldVec();
    if (magnets.size() != 3 || !magnets.at(0) || !magnets.at(1) || !magnets.at(2))
        dAnaData->setConstMagnetField({0., con_field, 0.});

//................................................................................//
//Register dp_ana.root
//................................................................................//
//Truth
//................................................................................//

    EvtWrt->RegisterDoubleVariable("TagTrk2_pp_truth_ini", &TagTrk2_pp_truth_ini, "TagTrk2_pp_truth_ini/D");
    EvtWrt->RegisterDoubleVariable("TagTrk2_pp_truth_fin", &TagTrk2_pp_truth_fin, "TagTrk2_pp_truth_fin/D");
    EvtWrt->RegisterOutVariable("TagTrk2_truth_hit_x", &TagTrk2_truth_hit_x);
    EvtWrt->RegisterOutVariable("TagTrk2_truth_hit_y", &TagTrk2_truth_hit_y);
    EvtWrt->RegisterOutVariable("TagTrk2_truth_hit_z", &TagTrk2_truth_hit_z);
    EvtWrt->RegisterOutVariable("TagTrk2_truth_hit_e", &TagTrk2_truth_hit_e);

    EvtWrt->RegisterDoubleVariable("RecTrk2_pp_truth_ini", &RecTrk2_pp_truth_ini, "RecTrk2_pp_truth_ini/D");
    EvtWrt->RegisterDoubleVariable("RecTrk2_pp_truth_fin", &RecTrk2_pp_truth_fin, "RecTrk2_pp_truth_fin/D");
    EvtWrt->RegisterOutVariable("RecTrk2_truth_hit_x", &RecTrk2_truth_hit_x);
    EvtWrt->RegisterOutVariable("RecTrk2_truth_hit_y", &RecTrk2_truth_hit_y);
    EvtWrt->RegisterOutVariable("RecTrk2_truth_hit_z", &RecTrk2_truth_hit_z);
    EvtWrt->RegisterOutVariable("RecTrk2_truth_hit_e", &RecTrk2_truth_hit_e);
}

void TruthHitProcessor::InitEvt() {


    std::vector<double>().swap(TagTrk2_truth_hit_x);
    std::vector<double>().swap(TagTrk2_truth_hit_y);
    std::vector<double>().swap(TagTrk2_truth_hit_z);
    std::vector<double>().swap(TagTrk2_truth_hit_e);
    std::vector<double>().swap(RecTrk2_truth_hit_x);
    std::vector<double>().swap(RecTrk2_truth_hit_y);
    std::vector<double>().swap(RecTrk2_truth_hit_z);
    std::vector<double>().swap(RecTrk2_truth_hit_e);

    TagTrk2_pp_truth_ini = RETURN;
    TagTrk2_pp_truth_fin = RETURN;
    RecTrk2_pp_truth_ini = RETURN;
    RecTrk2_pp_truth_fin = RETURN;

}

void TruthHitProcessor::FillTruth(DTruth *truth_info,
                                  std::vector<DStep*> *initial_steps,
                                  std::vector<TrkHit> raw_tagtrk2_hits,
                                  std::vector<TrkHit> raw_rectrk2_hits) {}
}

void TruthHitProcessor::ProcessEvt(AnaEvent *evt) {
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

        }


        }

//................................................................................//
//Write truth
        this->FillTruth();
    }
}

void TruthHitProcessor::CheckEvt(AnaEvent *evt) {
    //cout << "check" << endl;
    if (!evt) cerr << "[Warning] ==> Empty event" << endl;
}

void TruthHitProcessor::End() {
    //cout<<"End!"<<endl;
}

TruthHitProcessor::~TruthHitProcessor() {}
