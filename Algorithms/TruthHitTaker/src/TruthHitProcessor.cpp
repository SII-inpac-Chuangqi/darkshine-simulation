//
// Created by Joseph Zhang on 10/26/23.
//

#include "Algorithms/TruthHitTaker/include/Algo/TruthHitProcessor.h"

//................................................................................//
//C++
#include <vector>
#include <memory>
#include <iostream>
#include <tuple>
#include <algorithm>

//................................................................................//
//ROOT
#include "TGeoManager.h"
#include <Math/Vector4D.h>
#include <TParticlePDG.h>
#include "TFile.h"

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
    RegisterIntParameter("if_tagging", "If write truth info in tagging trackers", &if_tagging, 1);
    RegisterIntParameter("if_recoil", "If write truth info in recoil trackers", &if_recoil, 0);
}


TTree* TruthHitProcessor::MakeTree() {
// Create a new TTree
    TTree* outputTree = new TTree("hits", "hits");
//................................................................................//
//Truth
//................................................................................//
    outputTree->Branch("event_id", &event_id);
    outputTree->Branch("geometry_id", &geometry_id, "geometry_id/l");
    outputTree->Branch("particle_id", &particle_id, "particle_id/l");

    outputTree->Branch("tx", &truth_hit_x);
    outputTree->Branch("ty", &truth_hit_y);
    outputTree->Branch("tz", &truth_hit_z);
    outputTree->Branch("tt", &truth_hit_t);

    outputTree->Branch("tpx", &pp_truth_x);
    outputTree->Branch("tpy", &pp_truth_y);
    outputTree->Branch("tpz", &pp_truth_z);
    outputTree->Branch("te", &pp_truth_e );

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

    return outputTree;
}

void TruthHitProcessor::Begin() {

// Open a ROOT file for writing
//    TString outputFileName = "dp_hits.root"; // 指定输出文件名
//    TString outputFileNameT;
//    TString outputFileNameR;
    if (if_tagging) {
        fileT = new TFile(TString("dp_tag_hits.root"), "RECREATE");
        outputTreeT = MakeTree();
    }
    if (if_recoil) {
        fileR = new TFile(TString("dp_rcl_hits.root"), "RECREATE");
        outputTreeR = MakeTree();
    }
//    outputFile = new TFile(outputFileNameT, "RECREATE");
//    outputFile = new TFile(outputFileNameR, "RECREATE");

//................................................................................//
//Load geometry
//................................................................................//
    digitizer.ReadTrackerInfo(if_strip);
    digitizer.SetClusterWidth(0.03);
    DTrack::SetResolutions(0.03, 0.03 / 0.05, 0.);
}

void TruthHitProcessor::InitEvt() {

    truth_hit_x = RETURN;
    truth_hit_y = RETURN;
    truth_hit_z = RETURN;
    truth_hit_t = RETURN;

    pp_truth_x = RETURN;
    pp_truth_y = RETURN;
    pp_truth_z = RETURN;
    pp_truth_e = RETURN;

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
                                TFile *outputFile,
                                TTree *outputTree,
                                std::vector<TrkHit> raw_trk1_hits,
                                std::vector<TrkHit> raw_trk2_hits) {
    dAnaData->LoadTruthInfo(truth_info);
//................................................................................//

        for (const auto &hit2: raw_trk2_hits) {
            auto hit_pcontribs = hit2.getPContribution();
            for (auto pcon2: hit_pcontribs) {
                TParticlePDG* particlePDG = TDatabasePDG::Instance()->GetParticle(pcon2.getPdg());
                if (particlePDG && particlePDG->Charge() != 0 )
                {
                    truth_hit_z = static_cast<float>(hit2.GetZ());
                    truth_hit_t = static_cast<float>(hit2.getT());
                    geometry_id = static_cast<uint64_t>( 72057731476881664 +
                                                         137438953472 * (hit2.getCellId() - 1));//编号内含物理过程信息, 暂时这么写
                    volumeId = 1;
                    boundaryId = 0;
                    layerId = static_cast<uint64_t>(hit2.getCellId()) * 2;
                    approachId = 0;
                    sensitiveId = 1;

                    //std::cout << "[TRK2]" << "\t" << index << "\t" << pcon2 << std::endl;
                    pp_truth_x = static_cast<float>(pcon2.getPx() / 1000.); //unit in GeV
                    pp_truth_y = static_cast<float>(pcon2.getPy() / 1000.);
                    pp_truth_z = static_cast<float>(pcon2.getPz() / 1000.);
                    pp_truth_e = static_cast<float>(pcon2.getEnergy() / 1000.);
                    particle_id = static_cast<uint64_t>(pcon2.getId());

                    bool foundMatchingCellId = false;
                    for (const auto &hit1: raw_trk1_hits) {
                        if (hit1.getCellId() == hit2.getCellId()) {
                            auto hit1_pcontribs = hit1.getPContribution();
                            for (auto pcon1: hit1_pcontribs) {
                                //std::cout << "[TRK1HIT]" << "\t" << hit1 << std::endl;
                                truth_hit_x = static_cast<float>(hit1.GetX());
                                double yc = hit2.GetY() + ( ( hit1.GetX() - hit2.GetX() ) / tan( 0.05 * std::pow(-1.0, hit2.getCellId()) ) );
                                truth_hit_y = static_cast<float>(yc);

                                if (pcon2.getId() == pcon1.getId()) {
                                    //std::cout << "[TRK1]" << "\t" << index << "\t" << pcon1 << std::endl;
                                    deltapx = pp_truth_x - static_cast<float>(pcon1.getPx() / 1000.);
                                    deltapy = pp_truth_y - static_cast<float>(pcon1.getPy() / 1000.);
                                    deltapz = pp_truth_z - static_cast<float>(pcon1.getPz() / 1000.);
                                    deltae = pp_truth_e - static_cast<float>(pcon1.getEnergy() / 1000.);
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


                    if (outputTree) {
                        outputTree->Fill();
                    }
                } ////end if:
            }
        }

}

void TruthHitProcessor::ProcessEvt(AnaEvent *evt) {
//Initialize vars
    this->InitEvt();
    event_id = evt->getEventId();
    //const auto &step_collection = evt->getStepCollection();
    //const auto &MCCollection = evt->getMcParticleCollection();
    const auto &simuhit_collection = evt->getSimulatedHitCollection();

//Read
    std::vector<TrkHit> raw_trk1_hits;
    std::vector<TrkHit> raw_trk2_hits;

    if (if_tagging) {
        raw_trk1_hits.clear();
        raw_trk2_hits.clear();
        for (auto hit : *simuhit_collection.at("TagTrk1")) {
            if (hit->getE() < 0.005) continue;
            raw_trk1_hits.emplace_back(*hit);
        }
        for (auto hit : *simuhit_collection.at("TagTrk2")) {
            if (hit->getE() < 0.01) continue;
            raw_trk2_hits.emplace_back(*hit);
        }
        this->FillTruth(evt->getTruthInfo(), fileT, outputTreeT,
                        raw_trk1_hits, raw_trk2_hits);
    }

    if (if_recoil) {
        raw_trk1_hits.clear();
        raw_trk2_hits.clear();
        for (auto hit : *simuhit_collection.at("RecTrk1")) {
            if (hit->getE() < 0.005) continue;
            raw_trk1_hits.emplace_back(*hit);
        }
        for (auto hit : *simuhit_collection.at("RecTrk2")) {
            if (hit->getE() < 0.01) continue;
            raw_trk2_hits.emplace_back(*hit);
        }
        this->FillTruth(evt->getTruthInfo(), fileR, outputTreeR,
                        raw_trk1_hits, raw_trk2_hits);
    }

    //const auto &initial_steps = step_collection.at("Initial_Particle_Step");
    //const auto &raw_mc_ptl = MCCollection.at("RawMCParticle");
}

void TruthHitProcessor::CheckEvt(AnaEvent *evt) {
    if (!evt) cerr << "[Warning] ==> Empty event" << endl;
}

void TruthHitProcessor::End() {
    if (if_tagging) {
        fileT->Write();
        fileT->Close();
    }
    if (if_recoil) {
        fileR->Write();
        fileR->Close();
    }
    //cout<<"End!"<<endl;
}

