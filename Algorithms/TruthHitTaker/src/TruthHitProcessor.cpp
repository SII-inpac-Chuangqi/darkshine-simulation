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
#include <TParticlePDG.h>

//................................................................................//
//FRAMEWORK
#include "Core/AnaData.h"

//................................................................................//
//TRACKING
#include "Algo/TrkHit.h"
#include "Algo/Utils/Util.h"


TruthHitProcessor::TruthHitProcessor(string name, shared_ptr<EventStoreAndWriter> evtwrt) : AnaProcessor(
        std::move(name), std::move(evtwrt)){
    /*
     *
     *  DEFINE Processor explicitly with NAME in ControlManager.cpp L.37
     *  DEFINE Processor explicitly with NAME in ControlManager.cpp L.37
     *  DEFINE Processor explicitly with NAME in ControlManager.cpp L.37
     *
     */

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
    outputTree->Branch("event_id", &event_id);
    outputTree->Branch("geometry_id", &geometry_id, "geometry_id/l");
    outputTree->Branch("particle_id", &particle_id, "particle_id/l");

    outputTree->Branch("tx", &RecTrk2_truth_hit_x);
    outputTree->Branch("ty", &RecTrk2_truth_hit_y);
    outputTree->Branch("tz", &RecTrk2_truth_hit_z);
    outputTree->Branch("tt", &RecTrk2_truth_hit_t);

    outputTree->Branch("tpx", &RecTrk2_pp_truth_x);
    outputTree->Branch("tpy", &RecTrk2_pp_truth_y);
    outputTree->Branch("tpz", &RecTrk2_pp_truth_z);
    outputTree->Branch("te", &RecTrk2_pp_truth_e );

    outputTree->Branch("deltapx", &deltapx);
    outputTree->Branch("deltapy", &deltapy);
    outputTree->Branch("deltapz", &deltapz);
    outputTree->Branch("deltae",  &deltae) ;

    outputTree->Branch("index", &index);
    outputTree->Branch("volume_id", &volumeId);
    outputTree->Branch("boundary_id", &boundaryId);
    outputTree->Branch("layer_id", &layerId);
    outputTree->Branch("approach_id", &approachId);
    outputTree->Branch("sensitive_id", &sensitiveId);
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

    geometry_id  = 0;
    particle_id  = 0;
    event_id     = 0;
    index        = -1;

    volumeId = 0;
    boundaryId = 0;
    layerId = 0;
    approachId = 0;
    sensitiveId = 0;

}

void TruthHitProcessor::FillTruth(DTruth *truth_info,
                                  //std::vector<DStep*> *initial_steps,
                                 // std::vector<McParticle *> *raw_mc_ptl,
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
        for (const auto &hit2: raw_rectrk2_hits) {
            auto hit_pcontribs = hit2.getPContribution();
            for (auto pcon2: hit_pcontribs) {
                TParticlePDG* particlePDG = TDatabasePDG::Instance()->GetParticle(pcon2.getPdg());
                if (particlePDG && particlePDG->Charge() != 0 )
                {
                    RecTrk2_truth_hit_x = static_cast<float>(hit2.GetX());
                    RecTrk2_truth_hit_y = static_cast<float>(hit2.GetY());
                    RecTrk2_truth_hit_z = static_cast<float>(hit2.GetZ());
                    RecTrk2_truth_hit_t = static_cast<float>(hit2.getT());
                    geometry_id = static_cast<uint64_t>( 72057731476881664 +
                                                         137438953472 * (hit2.getCellId() - 1));//编号内含物理过程信息, 暂时这么写
                    volumeId = 1;
                    boundaryId = 0;
                    layerId = static_cast<uint64_t>(hit2.getCellId()) * 2;
                    approachId = 0;
                    sensitiveId = 1;

                    //std::cout << "[TRK2]" << "\t" << index << "\t" << pcon2 << std::endl;
                    RecTrk2_pp_truth_x = static_cast<float>(pcon2.getPx() / 1000.); //unit in GeV
                    RecTrk2_pp_truth_y = static_cast<float>(pcon2.getPy() / 1000.);
                    RecTrk2_pp_truth_z = static_cast<float>(pcon2.getPz() / 1000.);
                    RecTrk2_pp_truth_e = static_cast<float>(pcon2.getEnergy() / 1000.);
                    particle_id = static_cast<uint64_t>(pcon2.getId());

                    bool foundMatchingCellId = false;
                    for (const auto &hit1: raw_rectrk1_hits) {
                        if (hit1.getCellId() == hit2.getCellId()) {
                            auto hit1_pcontribs = hit1.getPContribution();
                            for (auto pcon1: hit1_pcontribs) {
                                if (pcon2.getId() == pcon1.getId()) {
                                    //std::cout << "[TRK1]" << "\t" << index << "\t" << pcon1 << std::endl;
                                    deltapx = RecTrk2_pp_truth_x - static_cast<float>(pcon1.getPx() / 1000.);
                                    deltapy = RecTrk2_pp_truth_y - static_cast<float>(pcon1.getPy() / 1000.);
                                    deltapz = RecTrk2_pp_truth_z - static_cast<float>(pcon1.getPz() / 1000.);
                                    deltae = RecTrk2_pp_truth_e - static_cast<float>(pcon1.getEnergy() / 1000.);
                                    index++;
                                    // Add a break here to exit the loop once the first matching cellId is found
                                    foundMatchingCellId = true;
                                    break;
                                }
                            }
                        }
                        // Break the outer loop if a matching cellId is found
                        if (foundMatchingCellId) { break; }
                    }//End loop for hit1

                    outputTree->Fill();
                } ////end if:
            }
        }
    //}// end for: raw mc particle

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
    //const auto &step_collection = evt->getStepCollection();
    //const auto &MCCollection = evt->getMcParticleCollection();
    const auto &simuhit_collection = evt->getSimulatedHitCollection();

//Read
//    std::vector<TrkHit> raw_tagtrk1_hits;
//    std::vector<TrkHit> raw_tagtrk2_hits;
//    for (auto hit : *simuhit_collection.at("TagTrk1")) raw_tagtrk1_hits.emplace_back(*hit);
//    for (auto hit : *simuhit_collection.at("TagTrk2")) raw_tagtrk2_hits.emplace_back(*hit);

    std::vector<TrkHit> raw_rectrk1_hits;
    std::vector<TrkHit> raw_rectrk2_hits;
    for (auto hit : *simuhit_collection.at("RecTrk1")) {
        if (hit->getE() < 0.02) continue;
        raw_rectrk1_hits.emplace_back(*hit);
    }
    for (auto hit : *simuhit_collection.at("RecTrk2")) {
        if (hit->getE() < 0.02) continue;
        raw_rectrk2_hits.emplace_back(*hit);
    }

    event_id = evt->getEventId();
    //const auto &initial_steps = step_collection.at("Initial_Particle_Step");
    //const auto &raw_mc_ptl = MCCollection.at("RawMCParticle");

//Write truth
    this->FillTruth(evt->getTruthInfo(),
                    raw_rectrk1_hits, raw_rectrk2_hits);
}

void TruthHitProcessor::CheckEvt(AnaEvent *evt) {
    if (!evt) cerr << "[Warning] ==> Empty event" << endl;
}

void TruthHitProcessor::End() {
    outputFile->Write();
    outputFile->Close();
    //cout<<"End!"<<endl;
}
