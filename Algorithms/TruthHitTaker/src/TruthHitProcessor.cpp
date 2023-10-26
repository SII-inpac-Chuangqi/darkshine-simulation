//
// Created by Joseph Zhang on 10/26/23.
//

#include "Algorithms/TruthHitTaker/include/Algo/TruthHitProcessor.h"

//................................................................................//
//C++
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <algorithm>

//................................................................................//
//ROOT
#include "TGeoManager.h"
#include <Math/Vector4D.h>

//................................................................................//
//FRAMEWORK
#include "Core/AnaData.h"

//................................................................................//
//TRACKING
#include "Algo/TypeDef.h"
#include "Algo/Utils/Util.h"
#include "Algo/TrkHit.h"


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

// Open a ROOT file for writing
    TString outputFileName = "dp_hits.root"; // 指定输出文件名
    outputFile = new TFile(outputFileName, "RECREATE");

// Create a new TTree
    outputTree = new TTree("hits", "hits");

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

//    outputTree->Branch("TagTrk2_truth_hit_x", &TagTrk2_truth_hit_x, "TagTrk2_truth_hit_x/D");
//    outputTree->Branch("TagTrk2_truth_hit_y", &TagTrk2_truth_hit_y, "TagTrk2_truth_hit_y/D");
//    outputTree->Branch("TagTrk2_truth_hit_z", &TagTrk2_truth_hit_z, "TagTrk2_truth_hit_z/D");
//    outputTree->Branch("TagTrk2_truth_hit_t", &TagTrk2_truth_hit_t, "TagTrk2_truth_hit_t/D");

    outputTree->Branch("RecTrk2_truth_hit_x", &RecTrk2_truth_hit_x, "RecTrk2_truth_hit_x/D");
    outputTree->Branch("RecTrk2_truth_hit_y", &RecTrk2_truth_hit_y, "RecTrk2_truth_hit_y/D");
    outputTree->Branch("RecTrk2_truth_hit_z", &RecTrk2_truth_hit_z, "RecTrk2_truth_hit_z/D");
    outputTree->Branch("RecTrk2_truth_hit_t", &RecTrk2_truth_hit_t, "RecTrk2_truth_hit_t/D");

//    outputTree->Branch("TagTrk2_pp_truth_x", &TagTrk2_pp_truth_x, "TagTrk2_pp_truth_x/D");
//    outputTree->Branch("TagTrk2_pp_truth_y", &TagTrk2_pp_truth_y, "TagTrk2_pp_truth_y/D");
//    outputTree->Branch("TagTrk2_pp_truth_z", &TagTrk2_pp_truth_z, "TagTrk2_pp_truth_z/D");
//    outputTree->Branch("TagTrk2_pp_truth_e", &TagTrk2_pp_truth_e, "TagTrk2_pp_truth_e/D");
                                                      
    outputTree->Branch("RecTrk2_pp_truth_x", &RecTrk2_pp_truth_x, "RecTrk2_pp_truth_x/D");
    outputTree->Branch("RecTrk2_pp_truth_y", &RecTrk2_pp_truth_y, "RecTrk2_pp_truth_y/D");
    outputTree->Branch("RecTrk2_pp_truth_z", &RecTrk2_pp_truth_z, "RecTrk2_pp_truth_z/D");
    outputTree->Branch("RecTrk2_pp_truth_e", &RecTrk2_pp_truth_e, "RecTrk2_pp_truth_e/D");

}

void TruthHitProcessor::InitEvt() {

//    TagTrk2_truth_hit_x = RETURN;
//    TagTrk2_truth_hit_y = RETURN;
//    TagTrk2_truth_hit_z = RETURN;
//    TagTrk2_truth_hit_t = RETURN;

    RecTrk2_truth_hit_x = RETURN;
    RecTrk2_truth_hit_y = RETURN;
    RecTrk2_truth_hit_z = RETURN;
    RecTrk2_truth_hit_t = RETURN;

//    TagTrk2_pp_truth_x = RETURN;
//    TagTrk2_pp_truth_y = RETURN;
//    TagTrk2_pp_truth_z = RETURN;
//    TagTrk2_pp_truth_e = RETURN;

    RecTrk2_pp_truth_x = RETURN;
    RecTrk2_pp_truth_y = RETURN;
    RecTrk2_pp_truth_z = RETURN;
    RecTrk2_pp_truth_e = RETURN;

}


void TruthHitProcessor::FillTruth(DTruth *truth_info,
 //                                 std::vector<TrkHit> raw_tagtrk2_hits,
                                  std::vector<TrkHit> raw_rectrk2_hits) {
//................................................................................//
//    int TagTrk2_No = raw_tagtrk2_hits.size();
//    for (int i = 0; i < TagTrk2_No; ++i) {
//        const auto& hit = raw_tagtrk2_hits.at(i);
//        TagTrk2_truth_hit_x = hit.GetX();
//        TagTrk2_truth_hit_y = hit.GetY();
//        TagTrk2_truth_hit_z = hit.GetZ();
//        TagTrk2_truth_hit_t = hit.getT();
//
//        auto hit_pcontribs = hit.getPContribution();
//        for (auto pcon : hit_pcontribs) {
//            TagTrk2_pp_truth_x = pcon.getPx();
//            TagTrk2_pp_truth_y = pcon.getPy();
//            TagTrk2_pp_truth_z = pcon.getPz();
//            TagTrk2_pp_truth_e = pcon.getEnergy();
//            outputTree->Fill();
//        }
//    }

    int RecTrk2_No = raw_rectrk2_hits.size();
    for (int i = 0; i < RecTrk2_No; ++i) {
        const auto& hit = raw_rectrk2_hits.at(i);
        RecTrk2_truth_hit_x = hit.GetX();
        RecTrk2_truth_hit_y = hit.GetY();
        RecTrk2_truth_hit_z = hit.GetZ();
        RecTrk2_truth_hit_t = hit.getT();

        auto hit_pcontribs = hit.getPContribution();
        for (auto pcon : hit_pcontribs) {
            RecTrk2_pp_truth_x = pcon.getPx();
            RecTrk2_pp_truth_y = pcon.getPy();
            RecTrk2_pp_truth_z = pcon.getPz();
            RecTrk2_pp_truth_e = pcon.getEnergy();
            outputTree->Fill();
        }
    }

    //................................................................................//

}

void TruthHitProcessor::ProcessEvt(AnaEvent *evt) {
    [[maybe_unused]] bool if_initial_steps(false);
    [[maybe_unused]] bool if_raw_tag_hits(false);
    [[maybe_unused]] bool if_raw_rec_hits(false);
    [[maybe_unused]] bool if_raw_tag_hit_number(false);
    [[maybe_unused]] bool if_raw_rec_hit_number(false);

    std::vector<double> magnet_at_origin = {magnets.size() && magnets.at(0) ? magnets.at(0)->GetField(0., 0., 0.) : 0.,
                                            magnets.size() && magnets.at(1) ? magnets.at(1)->GetField(0., 0., 0.) : con_field,
                                            magnets.size() && magnets.at(2) ? magnets.at(2)->GetField(0., 0., 0.) : 0.};

//................................................................................//
////Initialize vars
    this->InitEvt();
//
    const auto &step_collection = evt->getStepCollection();
    //const auto &MCCollection = evt->getMcParticleCollection();
    const auto &simuhit_collection = evt->getSimulatedHitCollection();

//................................................................................//
//Read
//        //const auto &mc = MCCollection.at("RawMCParticle");
//        const auto &initial_steps = step_collection.at("Initial_Particle_Step");
//
//    std::vector<TrkHit> raw_tagtrk1_hits;
//    std::vector<TrkHit> raw_tagtrk2_hits;
//    for (auto hit : *simuhit_collection.at("TagTrk1")) raw_tagtrk1_hits.emplace_back(*hit);
//    for (auto hit : *simuhit_collection.at("TagTrk2")) raw_tagtrk2_hits.emplace_back(*hit);

    std::vector<TrkHit> raw_rectrk1_hits;
    std::vector<TrkHit> raw_rectrk2_hits;
    for (auto hit : *simuhit_collection.at("RecTrk1")) raw_rectrk1_hits.emplace_back(*hit);
    for (auto hit : *simuhit_collection.at("RecTrk2")) raw_rectrk2_hits.emplace_back(*hit);
//

//Write truth
    this->FillTruth(evt->getTruthInfo(), raw_rectrk2_hits);

}


void TruthHitProcessor::CheckEvt(AnaEvent *evt) {
    if (!evt) cerr << "[Warning] ==> Empty event" << endl;
}

void TruthHitProcessor::End() {
    outputFile->Write();
    outputFile->Close();
    //cout<<"End!"<<endl;
}
