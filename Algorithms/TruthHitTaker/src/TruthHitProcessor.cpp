#include "Algo/TruthHitProcessor.h"

//................................................................................//
//C++
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <algorithm>
#include <cmath>

//................................................................................//
//ROOT
#include "TString.h"
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
#include "Algo/TrackingProcessor.h"

TruthHitProcessor::TruthHitProcessor(string name, shared_ptr<EventStoreAndWriter> evtwrt) : AnaProcessor(
        std::move(name), std::move(evtwrt)) {
    /*
     *
     *  DEFINE Processor explicitly with NAME in ControlManager.cpp L.37
     *  DEFINE Processor explicitly with NAME in ControlManager.cpp L.37
     *  DEFINE Processor explicitly with NAME in ControlManager.cpp L.37
     *
     */
    // Add description for this AnaProcessor
    Description = "Truth hit taker by Joseph Junhua Zhang";

    RegisterIntParameter("verbose", "Verbose", &Verbose, 0);
    RegisterIntParameter("if_strip", "If use strip structures in trackers", &if_strip, 1);
    RegisterIntParameter("if_smear", "If smear hits in strip structure", &if_smear, 1);

}

void TruthHitProcessor::Begin() {
//................................................................................//
//Load geometry
//................................................................................//
    digitizer.ReadTrackerInfo(if_strip);
    digitizer.SetIfSmear(if_smear);
    digitizer.SetClusterWidth(0.03);

    DTrack::SetResolutions(0.03, 0.03 / 0.05, 0.);

    EvtWrt->RegisterOutVariable("TagTrk2_simu_hit_x", &TagTrk2_simu_hit_x);
    EvtWrt->RegisterOutVariable("TagTrk2_simu_hit_y", &TagTrk2_simu_hit_y);
    EvtWrt->RegisterOutVariable("TagTrk2_simu_hit_z", &TagTrk2_simu_hit_z);
    EvtWrt->RegisterOutVariable("RecTrk2_simu_hit_x", &RecTrk2_simu_hit_x);
    EvtWrt->RegisterOutVariable("RecTrk2_simu_hit_y", &RecTrk2_simu_hit_y);
    EvtWrt->RegisterOutVariable("RecTrk2_simu_hit_z", &RecTrk2_simu_hit_z);

    EvtWrt->RegisterOutVariable("TagTrk2_smeared_hit_x", &TagTrk2_smeared_hit_x);
    EvtWrt->RegisterOutVariable("TagTrk2_smeared_hit_y", &TagTrk2_smeared_hit_y);
    EvtWrt->RegisterOutVariable("TagTrk2_smeared_hit_z", &TagTrk2_smeared_hit_z);
    EvtWrt->RegisterOutVariable("RecTrk2_smeared_hit_x", &RecTrk2_smeared_hit_x);
    EvtWrt->RegisterOutVariable("RecTrk2_smeared_hit_y", &RecTrk2_smeared_hit_y);
    EvtWrt->RegisterOutVariable("RecTrk2_smeared_hit_z", &RecTrk2_smeared_hit_z);

}

void TruthHitProcessor::InitEvt() {

    std::vector<double>().swap(TagTrk2_simu_hit_x);
    std::vector<double>().swap(TagTrk2_simu_hit_y);
    std::vector<double>().swap(TagTrk2_simu_hit_z);
    std::vector<double>().swap(RecTrk2_simu_hit_x);
    std::vector<double>().swap(RecTrk2_simu_hit_y);
    std::vector<double>().swap(RecTrk2_simu_hit_z);

    std::vector<double>().swap(TagTrk2_smeared_hit_x);
    std::vector<double>().swap(TagTrk2_smeared_hit_y);
    std::vector<double>().swap(TagTrk2_smeared_hit_z);
    std::vector<double>().swap(RecTrk2_smeared_hit_x);
    std::vector<double>().swap(RecTrk2_smeared_hit_y);
    std::vector<double>().swap(RecTrk2_smeared_hit_z);

}

void TruthHitProcessor::FillTruth(DTruth *truth_info,
                                  std::vector<TrkHit> raw_tagtrk2_hits,
                                  std::vector<TrkHit> raw_rectrk2_hits) {
    dAnaData->LoadTruthInfo(truth_info);

    int TagTrk2_No = raw_tagtrk2_hits.size();
    for (int i = 0; i < TagTrk2_No; ++i) {
        TagTrk2_simu_hit_x.push_back(raw_tagtrk2_hits.at(i).GetX());
        TagTrk2_simu_hit_y.push_back(raw_tagtrk2_hits.at(i).GetY());
        TagTrk2_simu_hit_z.push_back(raw_tagtrk2_hits.at(i).GetZ());
    }

    int RecTrk2_No = raw_rectrk2_hits.size();
    for (int i = 0; i < RecTrk2_No; ++i) {
        RecTrk2_simu_hit_x.push_back(raw_rectrk2_hits.at(i).GetX());
        RecTrk2_simu_hit_y.push_back(raw_rectrk2_hits.at(i).GetY());
        RecTrk2_simu_hit_z.push_back(raw_rectrk2_hits.at(i).GetZ());
    }
}

void TruthHitProcessor::ProcessEvt(AnaEvent *evt) {
    [[maybe_unused]] bool if_raw_tag_hits(false);
    [[maybe_unused]] bool if_raw_rec_hits(false);
//................................................................................//
//Initialize vars
    this->InitEvt();

    const auto &simuhit_collection = evt->getSimulatedHitCollection();

    auto it_find_tag1 = simuhit_collection.find("TagTrk1");
    auto it_find_tag2 = simuhit_collection.find("TagTrk2");
    auto it_find_rec1 = simuhit_collection.find("RecTrk1");
    auto it_find_rec2 = simuhit_collection.find("RecTrk2");

    if (it_find_tag1 != simuhit_collection.end() &&
        it_find_tag2 != simuhit_collection.end() &&
        it_find_rec1 != simuhit_collection.end() &&
        it_find_rec2 != simuhit_collection.end())
    {
        if_raw_tag_hits = true; if_raw_rec_hits = true;

//................................................................................//
//Read
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
        if (raw_tagtrk2_hits.size() > 2)
        {
        //Digitization
            digitizer.Layering(raw_tagtrk1_hits, raw_tagtrk2_hits, clus_tag_trkhit_map, tracking::tag);

            for (auto &tag_hm : clus_tag_trkhit_map) {
                for (auto &tag_sm_hits: tag_hm.second) {
                    TagTrk2_smeared_hit_x.push_back( tag_sm_hits->GetX() );
                    TagTrk2_smeared_hit_y.push_back( tag_sm_hits->GetY() );
                    TagTrk2_smeared_hit_z.push_back( tag_sm_hits->GetZ() );
                }
            }

        }

//................................................................................//
//Recoil tracker
        TrkHitPVecMap clus_rec_trkhit_map;
        if (raw_rectrk2_hits.size() > 2)
        {
            digitizer.Layering(raw_rectrk1_hits, raw_rectrk2_hits, clus_rec_trkhit_map, tracking::rec);

            for (auto &rec_hm : clus_rec_trkhit_map) {
                for (auto &rec_sm_hits: rec_hm.second) {
                    RecTrk2_smeared_hit_x.push_back( rec_sm_hits->GetX() );
                    RecTrk2_smeared_hit_y.push_back( rec_sm_hits->GetY() );
                    RecTrk2_smeared_hit_z.push_back( rec_sm_hits->GetZ() );
                }
            }
        }

//................................................................................//
//Filltruth
        this->FillTruth(evt->getTruthInfo(), raw_tagtrk2_hits, raw_rectrk2_hits);
    }
}

void TruthHitProcessor::CheckEvt(AnaEvent *evt) {
    //cout << "check" << endl;
    if (!evt) cerr << "[Warning] ==> Empty event" << endl;
}

void TruthHitProcessor::End() {
    //cout<<"End!"<<endl;
}
