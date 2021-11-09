#ifndef _OFF_TRACKING
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

    RegisterIntParameter("clean_mode", "Clean mode: no truth information", &clean, 1);
    RegisterIntParameter("if_strip", "If strip structures in trackers", &if_strip, 1);
    RegisterIntParameter("Tag_fit_method",
                         "Specify fitting method: 0, no fine fitting; 1, Kalman fitting",
                         &Tag_fit_method, 0);
    RegisterIntParameter("Rec_fit_method",
                         "Specify fitting method: 0, no fine fitting; 1, Kalman fitting",
                         &Rec_fit_method, 0);
}

void TrackingProcessor::Begin() {
//................................................................................//
//Load Geometry
//................................................................................//
    digitizer.GetTrackerInfo(if_strip);

//................................................................................//
//Load magnet
//................................................................................//
    if(Tag_fit_method == dKalman || Rec_fit_method == dKalman)
    {
        genfit::MaterialEffects::getInstance()->init(new genfit::TGeoMaterialInterface());
        magnets = dAnaData->getMagFieldVec();
        if (magnets.size() == 3 && magnets.at(0) && magnets.at(1) && magnets.at(2)) {
            genfit::FieldManager::getInstance()->init(new genfit::MapField(*(magnets.at(0)),
                                                                           *(magnets.at(1)),
                                                                           *(magnets.at(2)),
                                                                           genfit::Tesla)); //T->kGs
        } else
            genfit::FieldManager::getInstance()->init(new genfit::ConstField(0., -1.5*10., 0.));
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

        EvtWrt->RegisterIntVariable("RecTrk2_No", &RecTrk2_No, "RecTrk2_No/I");
        EvtWrt->RegisterDoubleVariable("RecTrk2_pp_truth_ini", &RecTrk2_pp_truth_ini, "RecTrk2_pp_truth_ini/D");
        EvtWrt->RegisterDoubleVariable("RecTrk2_pp_truth_fin", &RecTrk2_pp_truth_fin, "RecTrk2_pp_truth_fin/D");
        EvtWrt->RegisterOutVariable("RecTrk2_x", &RecTrk2_x);
        EvtWrt->RegisterOutVariable("RecTrk2_y", &RecTrk2_y);
        EvtWrt->RegisterOutVariable("RecTrk2_z", &RecTrk2_z);
        EvtWrt->RegisterOutVariable("RecTrk2_e", &RecTrk2_e);
    }
//................................................................................//
//Reconstructed
//................................................................................//
    EvtWrt->RegisterIntVariable("TagTrk2_track_No", &TagTrk2_track_No, "TagTrk2_track_No/I");
    EvtWrt->RegisterOutVariable("TagTrk2_pp", &TagTrk2_pp);
    EvtWrt->RegisterOutVariable("TagTrk2_track_chi2", &TagTrk2_track_chi2);

    if (!clean) {
/*
        EvtWrt->RegisterIntVariable("TagTrk2_rechit_No", &TagTrk2_rechit_No,   "TagTrk2_rechit_No/I");
        EvtWrt->RegisterIntVariable("TagTrk2_rectrk_hit_No", &TagTrk2_rectrk_hit_No);
        EvtWrt->RegisterDoubleVariable("TagTrk2_track_x", &TagTrk2_track_x);
        EvtWrt->RegisterDoubleVariable("TagTrk2_track_y", &TagTrk2_track_y);
        EvtWrt->RegisterDoubleVariable("TagTrk2_track_z", &TagTrk2_track_z);
*/

        EvtWrt->RegisterOutVariable("TagTrk2_track_quality", &TagTrk2_track_quality);
        EvtWrt->RegisterOutVariable("TagTrk2_track_x_sigma", &TagTrk2_track_x_sigma);
        EvtWrt->RegisterOutVariable("TagTrk2_track_y_sigma", &TagTrk2_track_y_sigma);
    }

//................................................................................//
    EvtWrt->RegisterIntVariable("RecTrk2_track_No", &RecTrk2_track_No, "RecTrk2_track_No/I");
    EvtWrt->RegisterOutVariable("RecTrk2_pp", &RecTrk2_pp);
    EvtWrt->RegisterOutVariable("RecTrk2_track_chi2", &RecTrk2_track_chi2);

    if (!clean) {
/*
        EvtWrt->RegisterIntVariable("RecTrk2_rechit_No", &RecTrk2_rechit_No,   "RecTrk2_rechit_No/I");
        EvtWrt->RegisterIntVariable("RecTrk2_rectrk_hit_No", &RecTrk2_rectrk_hit_No);
        EvtWrt->RegisterDoubleVariable("RecTrk2_track_x", &RecTrk2_track_x);
        EvtWrt->RegisterDoubleVariable("RecTrk2_track_y", &RecTrk2_track_y);
        EvtWrt->RegisterDoubleVariable("RecTrk2_track_z", &RecTrk2_track_z);
*/

        EvtWrt->RegisterOutVariable("RecTrk2_track_quality", &RecTrk2_track_quality);
        EvtWrt->RegisterOutVariable("RecTrk2_track_x_sigma", &RecTrk2_track_x_sigma);
        EvtWrt->RegisterOutVariable("RecTrk2_track_y_sigma", &RecTrk2_track_y_sigma);
    }

    //if(Tag_fit_method == 1 || Rec_fit_method == 1)
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
/*
        std::vector<int>().swap(TagTrk2_rectrk_hit_No);    
        std::vector<double>().swap(TagTrk2_rectrk_x);
        std::vector<double>().swap(TagTrk2_rectrk_y);
        std::vector<double>().swap(TagTrk2_rectrk_z);

        std::vector<int>().swap(RecTrk2_rectrk_hit_No);
        std::vector<double>().swap(RecTrk2_rectrk_x);
        std::vector<double>().swap(RecTrk2_rectrk_y);
        std::vector<double>().swap(RecTrk2_rectrk_z);
*/
    }

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
}

void TrackingProcessor::FillTruth(std::vector<DStep*> *stepIni,
                                  std::vector<TrkHit> rawTagTrk2Hits,
                                  std::vector<TrkHit> rawRecTrk2Hits) {
    if (!clean) {
        TagTrk2_No = rawTagTrk2Hits.size();
 
        bool trackerFlag = false;
        for (auto step : *stepIni) {
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
            TagTrk2_x.push_back(rawTagTrk2Hits.at(i).GetX());
            TagTrk2_y.push_back(rawTagTrk2Hits.at(i).GetY());
            TagTrk2_z.push_back(rawTagTrk2Hits.at(i).GetZ());
            TagTrk2_e.push_back(rawTagTrk2Hits.at(i).GetE());
        }
 
        RecTrk2_No = rawRecTrk2Hits.size();
 
        trackerFlag = false;
        for (auto step : *stepIni) {
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
            RecTrk2_x.push_back(rawRecTrk2Hits.at(i).GetX());
            RecTrk2_y.push_back(rawRecTrk2Hits.at(i).GetY());
            RecTrk2_z.push_back(rawRecTrk2Hits.at(i).GetZ());
            RecTrk2_e.push_back(rawRecTrk2Hits.at(i).GetE());
        }
    }
}

void TrackingProcessor::ProcessEvt(AnaEvent *evt) {
    this->CleanEvt();

    const auto &stepCollection = evt->getStepCollection();
    //const auto &MCCollection = evt->getMcParticleCollection();
    const auto &simuHitCollection = evt->getSimulatedHitCollection();

    auto itFindStep = stepCollection.find("Initial_Particle_Step");
    auto itFindTag1 = simuHitCollection.find("TagTrk1");
    auto itFindTag2 = simuHitCollection.find("TagTrk2");
    auto itFindRec1 = simuHitCollection.find("RecTrk1");
    auto itFindRec2 = simuHitCollection.find("RecTrk2");
    if (itFindStep != stepCollection.end() &&
        itFindTag1 != simuHitCollection.end() &&
        itFindTag2 != simuHitCollection.end() &&
        itFindRec1 != simuHitCollection.end() &&
        itFindRec2 != simuHitCollection.end()) {
//................................................................................//
//Read
        //const auto &mc = MCCollection.at("RawMCParticle");
        const auto &stepIni = stepCollection.at("Initial_Particle_Step");

        std::vector<TrkHit> rawTagTrk1Hits;
        std::vector<TrkHit> rawTagTrk2Hits;
        for (auto hit : *simuHitCollection.at("TagTrk1")) rawTagTrk1Hits.emplace_back(*hit);
        for (auto hit : *simuHitCollection.at("TagTrk2")) rawTagTrk2Hits.emplace_back(*hit);

        std::vector<TrkHit> rawRecTrk1Hits;
        std::vector<TrkHit> rawRecTrk2Hits;
        for (auto hit : *simuHitCollection.at("RecTrk1")) rawRecTrk1Hits.emplace_back(*hit);
        for (auto hit : *simuHitCollection.at("RecTrk2")) rawRecTrk2Hits.emplace_back(*hit);

        if (rawTagTrk2Hits.size() < 20 && rawTagTrk2Hits.size() > 2 &&
            rawRecTrk2Hits.size() < 20 && rawRecTrk2Hits.size() > 2)
        {
//................................................................................//
//Digitization, depends on further hardware setting
            TrkHitPVecMap clusTagTrkHitMap;
            digitizer.Layering(rawTagTrk1Hits, rawTagTrk2Hits, clusTagTrkHitMap, tag);
            TrkHitPVecMap clusRecTrkHitMap;
            digitizer.Layering(rawRecTrk1Hits, rawRecTrk2Hits, clusRecTrkHitMap, rec);

            if(clusTagTrkHitMap.size() && clusRecTrkHitMap.size())
            {
//................................................................................//
//Finding, by pre-fitting
                std::vector<TrkHitPVec> VecTagTrack;
                GreedyFinding findTag(clusTagTrkHitMap);
                VecTagTrack.assign(findTag.First(), findTag.Last());
    
                std::vector<TrkHitPVec> VecRecTrack;
                GreedyFinding findRec(clusRecTrkHitMap);
                VecRecTrack.assign(findRec.First(), findRec.Last());

//................................................................................//
//Fitting, by Genfit, Kalman filter/by Riemann fitting
                TagTrk2_track_No = findTag.GetTrackNo();
                RecTrk2_track_No = findRec.GetTrackNo();
    
                TagTrk2_rechit_No = 0;
                RecTrk2_rechit_No = 0;

                for (int i = 0; i < findTag.GetTrackNo(); i++) {
                    TrkHitPVec tagTrack((*(VecTagTrack.begin() + i)).begin(), (*(VecTagTrack.begin() + i)).end());
                    DTrack track(tagTrack,
                                 findTag.GetR(i),       //used in Kalman filter
                                 findTag.GetCenterX(i), //not used in Kalman filter, reserved for future
                                 findTag.GetCenterY(i), //not used in Kalman filter, reserved for future
                                 magnets);              //magnetic in the volume where track lies
                    track.Fit(Tag_fit_method);          //choose fitting method: Kalman filter
                    track.Evaluate();
    
                    TagTrk2_pp.push_back(track.GetPp());
                    TagTrk2_track_chi2.push_back(track.GetChi2());

                    if (!clean) {
                        TagTrk2_track_quality.push_back(track.GetQuality());
                        TagTrk2_track_x_sigma.push_back(track.GetXSigma());
                        TagTrk2_track_y_sigma.push_back(track.GetYSigma());
/*    
                        for (int hitno = 0; hitno < track.GetSize(); hitno++) {
                            TagTrk2_track_x.push_back(track.At(hitno)->GetX());
                            TagTrk2_track_y.push_back(track.At(hitno)->GetY());
                            TagTrk2_track_z.push_back(track.At(hitno)->GetZ());
                        }
                        TagTrk2_rechit_No += track.GetSize();
                        TagTrk2_rectrk_hit_No.push_back(track.GetSize());
*/
                    }
                }
    
                for (int i = 0; i < findRec.GetTrackNo(); i++) {
                    TrkHitPVec recTrack((*(VecRecTrack.begin() + i)).begin(), (*(VecRecTrack.begin() + i)).end());
                    DTrack track(recTrack,
                                 findRec.GetR(i),       //used in Kalman filter
                                 findRec.GetCenterX(i), //not used in Kalman filter, reserved for future
                                 findRec.GetCenterY(i), //not used in Kalman filter, reserved for future
                                 magnets);              //magnetic in the volume where track lies
                    track.Fit(Rec_fit_method);          //choose fitting method: Kalman filter
                    track.Evaluate();
    
                    RecTrk2_pp.push_back(track.GetPp());
                    RecTrk2_track_chi2.push_back(track.GetChi2());

                    if (!clean) {
                        RecTrk2_track_quality.push_back(track.GetQuality());
                        RecTrk2_track_x_sigma.push_back(track.GetXSigma());
                        RecTrk2_track_y_sigma.push_back(track.GetYSigma());
/*    
                        for (int hitno = 0; hitno < track.GetSize(); hitno++) {
                            RecTrk2_track_x.push_back(track.At(hitno)->GetX());
                            RecTrk2_track_y.push_back(track.At(hitno)->GetY());
                            RecTrk2_track_z.push_back(track.At(hitno)->GetZ());
                        }
                        RecTrk2_rechit_No += track.GetSize();
                        RecTrk2_rectrk_hit_No.push_back(track.GetSize());
*/
                    }
                }
            }
            else
            {
                TagTrk2_track_No = 0;
                RecTrk2_track_No = 0;

                if (!clean) {
                    TagTrk2_rechit_No = 0;
                    RecTrk2_rechit_No = 0;
                }
            }

//................................................................................//
//Write root
//................................................................................//
//Truth
        }
        else
        {
            TagTrk2_track_No = 0;
            RecTrk2_track_No = 0;

            if (!clean) {
                TagTrk2_rechit_No = 0;
                RecTrk2_rechit_No = 0;
            }
        }

        this->FillTruth(stepIni, rawTagTrk2Hits, rawRecTrk2Hits);
    }
}

void TrackingProcessor::CheckEvt(AnaEvent *evt) {
   //cout << "check" << endl;
    if (!evt) cerr << "null event" << endl;
}

void TrackingProcessor::End() {
    //cout<<"End!"<<endl;
}


#endif
