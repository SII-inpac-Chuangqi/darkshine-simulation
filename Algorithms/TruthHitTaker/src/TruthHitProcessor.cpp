//
// Created by Joseph Zhang on 10/26/23.
//

#include "Algorithms/TruthHitTaker/include/Algo/TruthHitProcessor.h"

//................................................................................//
//C++
#include <vector>
#include <memory>
#include <iostream>

//................................................................................//
//ROOT
#include "TGeoManager.h"
#include <Math/Vector4D.h>

//................................................................................//
//FRAMEWORK
#include "Core/AnaData.h"

//................................................................................//
//TRACKING
#include "Algo/TrkHit.h"
#include "Algo/Utils/Util.h"


TruthHitProcessor::TruthHitProcessor(string name, shared_ptr<EventStoreAndWriter> evtwrt) : AnaProcessor(
        std::move(name), std::move(evtwrt)){
    // Add description for this AnaProcessor
    Description = "Tracking truth hits and particles taken by Joseph ZHANG";
    RegisterIntParameter("verbose", "Verbose", &Verbose, 0);
    RegisterIntParameter("if_strip", "If use strip structures in trackers", &if_strip, 1);

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
    digitizer.SetClusterWidth(0.03);
    DTrack::SetResolutions(0.03, 0.03 / 0.05, 0.);

//................................................................................//
//Truth
//................................................................................//
/*
//    outputTree->Branch("TagTrk2_truth_hit_x", &TagTrk2_truth_hit_x, "TagTrk2_truth_hit_x/D");
//    outputTree->Branch("TagTrk2_truth_hit_y", &TagTrk2_truth_hit_y, "TagTrk2_truth_hit_y/D");
//    outputTree->Branch("TagTrk2_truth_hit_z", &TagTrk2_truth_hit_z, "TagTrk2_truth_hit_z/D");
//    outputTree->Branch("TagTrk2_truth_hit_t", &TagTrk2_truth_hit_t, "TagTrk2_truth_hit_t/D");
//    outputTree->Branch("TagTrk2_pp_truth_x", &TagTrk2_pp_truth_x, "TagTrk2_pp_truth_x/D");
//    outputTree->Branch("TagTrk2_pp_truth_y", &TagTrk2_pp_truth_y, "TagTrk2_pp_truth_y/D");
//    outputTree->Branch("TagTrk2_pp_truth_z", &TagTrk2_pp_truth_z, "TagTrk2_pp_truth_z/D");
//    outputTree->Branch("TagTrk2_pp_truth_e", &TagTrk2_pp_truth_e, "TagTrk2_pp_truth_e/D");
*/
    outputTree->Branch("tx", &RecTrk2_truth_hit_x, "RecTrk2_truth_hit_x/D");
    outputTree->Branch("ty", &RecTrk2_truth_hit_y, "RecTrk2_truth_hit_y/D");
    outputTree->Branch("tz", &RecTrk2_truth_hit_z, "RecTrk2_truth_hit_z/D");
    outputTree->Branch("tt", &RecTrk2_truth_hit_t, "RecTrk2_truth_hit_t/D");

    outputTree->Branch("tpx", &RecTrk2_pp_truth_x, "RecTrk2_pp_truth_x/D");
    outputTree->Branch("tpy", &RecTrk2_pp_truth_y, "RecTrk2_pp_truth_y/D");
    outputTree->Branch("tpz", &RecTrk2_pp_truth_z, "RecTrk2_pp_truth_z/D");
    outputTree->Branch("te", &RecTrk2_pp_truth_e, "RecTrk2_pp_truth_e/D");

    outputTree->Branch("deltapx", &deltapx, "deltapx/D");
    outputTree->Branch("deltapy", &deltapy, "deltapy/D");
    outputTree->Branch("deltapz", &deltapz, "deltapz/D");
    outputTree->Branch("deltae", &deltae, "deltae/D");

    outputTree->Branch("geometry_id", &geometry_id, "geometry_id/I");
    outputTree->Branch("particle_id", &particle_id, "particle_id/I");
    outputTree->Branch("event_id", &event_id, "event_id/I");
    outputTree->Branch("index", &index, "index/I");
}

void TruthHitProcessor::InitEvt() {
/*
//    TagTrk2_truth_hit_x = RETURN;
//    TagTrk2_truth_hit_y = RETURN;
//    TagTrk2_truth_hit_z = RETURN;
//    TagTrk2_truth_hit_t = RETURN;

//    TagTrk2_pp_truth_x = RETURN;
//    TagTrk2_pp_truth_y = RETURN;
//    TagTrk2_pp_truth_z = RETURN;
//    TagTrk2_pp_truth_e = RETURN;
*/
    RecTrk2_truth_hit_x = RETURN;
    RecTrk2_truth_hit_y = RETURN;
    RecTrk2_truth_hit_z = RETURN;
    RecTrk2_truth_hit_t = RETURN;

    RecTrk2_pp_truth_x = RETURN;
    RecTrk2_pp_truth_y = RETURN;
    RecTrk2_pp_truth_z = RETURN;
    RecTrk2_pp_truth_e = RETURN;

    deltapx=RETURN;
    deltapy=RETURN;
    deltapz=RETURN;
    deltae =RETURN;

    geometry_id  = RETURN;
    particle_id  = RETURN;
    event_id     = RETURN;
    index        = -1;

}

void TruthHitProcessor::FillTruth(DTruth *truth_info,
                                  std::vector<DStep*> *initial_steps,
                                  std::vector<TrkHit> raw_rectrk1_hits,
                                  std::vector<TrkHit> raw_rectrk2_hits) {
    dAnaData->LoadTruthInfo(truth_info);
//................................................................................//
/*
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
*/
    for (const auto& hit2 : raw_rectrk2_hits) {
        RecTrk2_truth_hit_x = hit2.GetX();
        RecTrk2_truth_hit_y = hit2.GetY();
        RecTrk2_truth_hit_z = hit2.GetZ();
        RecTrk2_truth_hit_t = hit2.getT();
        geometry_id         = hit2.getCellId();

        auto hit_pcontribs = hit2.getPContribution();
        for (auto pcon2 : hit_pcontribs) {
            //std::cout << "[EVT]" << "\t" << pcon2 <<std::endl;
            RecTrk2_pp_truth_x = pcon2.getPx();
            RecTrk2_pp_truth_y = pcon2.getPy();
            RecTrk2_pp_truth_z = pcon2.getPz();
            RecTrk2_pp_truth_e = pcon2.getEnergy();
            particle_id        = pcon2.getId();

            for (const auto& hit1 : raw_rectrk1_hits) {
                if (hit1.getCellId() == geometry_id) {
                    auto hit1_pcontribs = hit1.getPContribution();
                    for (auto pcon1 : hit1_pcontribs) {
                        deltapx = RecTrk2_pp_truth_x - pcon1.getPx();
                        deltapy = RecTrk2_pp_truth_y - pcon1.getPy();     
                        deltapz = RecTrk2_pp_truth_z - pcon1.getPz();     
                        deltae  = RecTrk2_pp_truth_e - pcon1.getEnergy();
                        index++;
                        // 将deltapx填充到outputTree
                        outputTree->Fill();
                    }
                }
            }//End loop for hit1
        }
    }

//    bool trackerFlag = false;
//    for (auto step : *initial_steps) {
//        if (InRecTrack(step->getX(), step->getY(), step->getZ()) && !trackerFlag) {
//            std::cout << step->getPx() << "\t" << step->getPy() << "\t" << step->getPz() << "\t" << step->getId() << std::endl;
//            trackerFlag = true;
//        }
//    }
}

void TruthHitProcessor::ProcessEvt(AnaEvent *evt) {
//Initialize vars
    this->InitEvt();
    const auto &step_collection = evt->getStepCollection();
    //const auto &MCCollection = evt->getMcParticleCollection();
    const auto &simuhit_collection = evt->getSimulatedHitCollection();

//Read
//    std::vector<TrkHit> raw_tagtrk1_hits;
//    std::vector<TrkHit> raw_tagtrk2_hits;
//    for (auto hit : *simuhit_collection.at("TagTrk1")) raw_tagtrk1_hits.emplace_back(*hit);
//    for (auto hit : *simuhit_collection.at("TagTrk2")) raw_tagtrk2_hits.emplace_back(*hit);

    std::vector<TrkHit> raw_rectrk1_hits;
    std::vector<TrkHit> raw_rectrk2_hits;
    for (auto hit : *simuhit_collection.at("RecTrk1")) raw_rectrk1_hits.emplace_back(*hit);
    for (auto hit : *simuhit_collection.at("RecTrk2")) raw_rectrk2_hits.emplace_back(*hit);

    event_id = evt->getEventId();
    const auto &initial_steps = step_collection.at("Initial_Particle_Step");

//Write truth
    this->FillTruth(evt->getTruthInfo(), initial_steps, raw_rectrk1_hits, raw_rectrk2_hits);
}

void TruthHitProcessor::CheckEvt(AnaEvent *evt) {
    if (!evt) cerr << "[Warning] ==> Empty event" << endl;
}

void TruthHitProcessor::End() {
    outputFile->Write();
    outputFile->Close();
    //cout<<"End!"<<endl;
}
