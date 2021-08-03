#ifndef _OFF_TRACKING
//................................................................................//
//CPP STL
#include <vector>
#include <map>
#include <malloc.h>
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
#include "Algo/Digitization.h"
#include "Algo/GreedyFinding.h"
#include "Algo/DTrack.h"

TrackingProcessor::TrackingProcessor(string name, shared_ptr<EventStoreAndWriter> evtwrt) : AnaProcessor(std::move(name), std::move(evtwrt))
{
    /*
     *
     *  DEFINE Processor explicitly with NAME in ControlManager.cpp L.37
     *  DEFINE Processor explicitly with NAME in ControlManager.cpp L.37
     *  DEFINE Processor explicitly with NAME in ControlManager.cpp L.37
     *
     */

    RegisterIntParameter   ("clean",    "Clean mode",               &clean,    1);
    RegisterIntParameter   ("Tag_fit_method",
                            "Specify fitting method: 0, no fine fitting; 1, Kalman fitting; 2, Riemann fitting",
                            &Tag_fit_method, 0);
    RegisterIntParameter   ("Rec_fit_method",
                            "Specify fitting method: 0, no fine fitting; 1, Kalman fitting; 2, Riemann fitting",
                            &Rec_fit_method, 0);
    RegisterDoubleParameter("RecTrk_B", "Magnet in recoil tracker", &RecTrk_B, 1.5);

    std::cout << "reading magnet ..." << std::endl;
    std::vector<DMagnet*> magnets = dAnaData->getMagFieldVec();
    genfit::MaterialEffects::getInstance()->init(new genfit::TGeoMaterialInterface());
    genfit::FieldManager::getInstance()->init(new genfit::MapField(*(magnets.at(0)),
                                                                   *(magnets.at(1)),
                                                                   *(magnets.at(2)),
                                                                    genfit::Tesla)); //T->kGs
    std::cout << "end reading magnet ..." << std::endl;
}

void TrackingProcessor::Begin()
{
   
    // Add description for this AnaProcessor
    Description = "Tracking.";

    // Register dp_ana.root
//................................................................................//
//Truth
//................................................................................//
    if(!clean)
    {
        EvtWrt->RegisterIntVariable("TagTrk2_No", &TagTrk2_No, "TagTrk2_No/I");
        EvtWrt->RegisterDoubleVariable("TagTrk2_pp_truth_ini", &TagTrk2_pp_truth_ini, "TagTrk2_pp_truth_ini/D");
        EvtWrt->RegisterDoubleVariable("TagTrk2_pp_truth_fin", &TagTrk2_pp_truth_fin, "TagTrk2_pp_truth_fin/D");
        EvtWrt->RegisterDoubleVariable("TagTrk2_x", TagTrk2_x, "TagTrk2_x[TagTrk2_No]/D");
        EvtWrt->RegisterDoubleVariable("TagTrk2_y", TagTrk2_y, "TagTrk2_y[TagTrk2_No]/D");
        EvtWrt->RegisterDoubleVariable("TagTrk2_z", TagTrk2_z, "TagTrk2_z[TagTrk2_No]/D");
        EvtWrt->RegisterDoubleVariable("TagTrk2_e", TagTrk2_e, "TagTrk2_e[TagTrk2_No]/D");

        EvtWrt->RegisterIntVariable("RecTrk2_No", &RecTrk2_No, "RecTrk2_No/I");
        EvtWrt->RegisterDoubleVariable("RecTrk2_pp_truth_ini", &RecTrk2_pp_truth_ini, "RecTrk2_pp_truth_ini/D");
        EvtWrt->RegisterDoubleVariable("RecTrk2_pp_truth_fin", &RecTrk2_pp_truth_fin, "RecTrk2_pp_truth_fin/D");
        EvtWrt->RegisterDoubleVariable("RecTrk2_x", RecTrk2_x, "RecTrk2_x[RecTrk2_No]/D");
        EvtWrt->RegisterDoubleVariable("RecTrk2_y", RecTrk2_y, "RecTrk2_y[RecTrk2_No]/D");
        EvtWrt->RegisterDoubleVariable("RecTrk2_z", RecTrk2_z, "RecTrk2_z[RecTrk2_No]/D");
        EvtWrt->RegisterDoubleVariable("RecTrk2_e", RecTrk2_e, "RecTrk2_e[RecTrk2_No]/D");
    }
//................................................................................//
//Reconstructed
//................................................................................//
    EvtWrt->RegisterIntVariable("TagTrk2_track_No", &TagTrk2_track_No, "TagTrk2_track_No/I");
    EvtWrt->RegisterDoubleVariable("TagTrk2_pp",    TagTrk2_pp,        "TagTrk2_pp[TagTrk2_track_No]/D");

    if(!clean)
    {
/*
        EvtWrt->RegisterIntVariable("TagTrk2_rechit_No", &TagTrk2_rechit_No,   "TagTrk2_rechit_No/I");
        EvtWrt->RegisterIntVariable("TagTrk2_rectrk_hit_No", TagTrk2_rectrk_hit_No, "TagTrk2_rectrk_hit_No[TagTrk2_track_No]/I");
        EvtWrt->RegisterDoubleVariable("TagTrk2_track_x", TagTrk2_track_x,  "TagTrk2_track_x[TagTrk2_rechit_No]/D");
        EvtWrt->RegisterDoubleVariable("TagTrk2_track_y", TagTrk2_track_y,  "TagTrk2_track_y[TagTrk2_rechit_No]/D");
        EvtWrt->RegisterDoubleVariable("TagTrk2_track_z", TagTrk2_track_z,  "TagTrk2_track_z[TagTrk2_rechit_No]/D");
*/

        EvtWrt->RegisterDoubleVariable("TagTrk2_track_quality", TagTrk2_track_quality, "TagTrk2_track_quality[TagTrk2_track_No]/D");
        EvtWrt->RegisterDoubleVariable("TagTrk2_track_chi2",    TagTrk2_track_chi2,    "TagTrk2_track_chi2[TagTrk2_track_No]/D");
        EvtWrt->RegisterDoubleVariable("TagTrk2_track_x_sigma", TagTrk2_track_x_sigma, "TagTrk2_track_x_sigma[TagTrk2_track_No]/D");
        EvtWrt->RegisterDoubleVariable("TagTrk2_track_y_sigma", TagTrk2_track_y_sigma, "TagTrk2_track_y_sigma[TagTrk2_track_No]/D");
    }

//................................................................................//
    EvtWrt->RegisterIntVariable("RecTrk2_track_No", &RecTrk2_track_No, "RecTrk2_track_No/I");
    EvtWrt->RegisterDoubleVariable("RecTrk2_pp",    RecTrk2_pp,        "RecTrk2_pp[RecTrk2_track_No]/D");

    if(!clean)
    {
/*
        EvtWrt->RegisterIntVariable("RecTrk2_rechit_No", &RecTrk2_rechit_No,   "RecTrk2_rechit_No/I");
        EvtWrt->RegisterIntVariable("RecTrk2_rectrk_hit_No", RecTrk2_rectrk_hit_No, "RecTrk2_rectrk_hit_No[RecTrk2_track_No]/I");
        EvtWrt->RegisterDoubleVariable("RecTrk2_track_x", RecTrk2_track_x,  "RecTrk2_track_x[RecTrk2_rechit_No]/D");
        EvtWrt->RegisterDoubleVariable("RecTrk2_track_y", RecTrk2_track_y,  "RecTrk2_track_y[RecTrk2_rechit_No]/D");
        EvtWrt->RegisterDoubleVariable("RecTrk2_track_z", RecTrk2_track_z,  "RecTrk2_track_z[RecTrk2_rechit_No]/D");
*/

        EvtWrt->RegisterDoubleVariable("RecTrk2_track_quality", RecTrk2_track_quality, "RecTrk2_track_quality[RecTrk2_track_No]/D");
        EvtWrt->RegisterDoubleVariable("RecTrk2_track_chi2",    RecTrk2_track_chi2,    "RecTrk2_track_chi2[RecTrk2_track_No]/D");
        EvtWrt->RegisterDoubleVariable("RecTrk2_track_x_sigma", RecTrk2_track_x_sigma, "RecTrk2_track_x_sigma[RecTrk2_track_No]/D");
        EvtWrt->RegisterDoubleVariable("RecTrk2_track_y_sigma", RecTrk2_track_y_sigma, "RecTrk2_track_y_sigma[RecTrk2_track_No]/D");
    }

    //if(Tag_fit_method == 1 || Rec_fit_method == 1)
}

void TrackingProcessor::ProcessEvt(AnaEvent* evt)
{
    
    const auto &stepCollection = evt->getStepCollection();
    //const auto &MCCollection = evt->getMcParticleCollection();
    const auto &simuHitCollection = evt->getSimulatedHitCollection();

    auto itFindStep = stepCollection.find("Initial_Particle_Step");
    auto itFindRec1 = simuHitCollection.find("RecTrk1");
    auto itFindRec2 = simuHitCollection.find("RecTrk2");
    if(itFindStep != stepCollection.end() &&
       itFindRec1 != simuHitCollection.end() &&
       itFindRec2 != simuHitCollection.end())
    {
//................................................................................//
//Read

        //const auto &mc = MCCollection.at("RawMCParticle");
        const auto &stepIni = stepCollection.at("Initial_Particle_Step");

        vector<TrkHit> rawTagTrkHits;
        auto tag2Hits = simuHitCollection.at("TagTrk2");
        for(auto hit : *tag2Hits) rawTagTrkHits.emplace_back(*hit);

        vector<TrkHit> rawRecTrkHits;
        auto rec2Hits = simuHitCollection.at("RecTrk2");
        for(auto hit : *rec2Hits) rawRecTrkHits.emplace_back(*hit);

        if(rawTagTrkHits.size() < 20 && rawTagTrkHits.size() > 2 &&
           rawRecTrkHits.size() < 20 && rawRecTrkHits.size() > 2)
        //if(rawRecTrkHits.size() < 25 && rawRecTrkHits.size() > 2)
        {

//................................................................................//
//Digitization, depends on further hardware setting
            Digitization(rawTagTrkHits);
            Digitization(rawRecTrkHits); 

            TrkHitPVecMap clusTagTrkHitMap;
            Cluster(rawTagTrkHits, clusTagTrkHitMap);
            TrkHitPVecMap clusRecTrkHitMap;
            Cluster(rawRecTrkHits, clusRecTrkHitMap);

//................................................................................//
//Finding, by pre-fitting
            vector<TrkHitPVec> VecTagTrack;
            GreedyFinding findTag(clusTagTrkHitMap);
            VecTagTrack.assign(findTag.First(), findTag.Last());

            vector<TrkHitPVec> VecRecTrack;
            GreedyFinding findRec(clusRecTrkHitMap);
            VecRecTrack.assign(findRec.First(), findRec.Last());

//................................................................................//
//Fitting, by Genfit, Kalman filter/by Riemann fitting
            TagTrk2_track_No = findTag.GetTrackNo();
            RecTrk2_track_No = findRec.GetTrackNo();

            TagTrk2_rechit_No = 0;
            RecTrk2_rechit_No = 0;

            for (int i = 0; i < findTag.GetTrackNo(); i++)
            {
                TrkHitPVec tagTrack((*(VecTagTrack.begin() + i)).begin(), (*(VecTagTrack.begin() + i)).end());
                DTrack track(tagTrack,
                             findTag.GetR(i),       //used in Kalman filter
                             findTag.GetCenterX(i), //not used in Kalman filter, reserved for future
                             findTag.GetCenterY(i), //not used in Kalman filter, reserved for future
                             1.5);                  //magnetic in the volume where track lies
                track.Fit(Tag_fit_method);          //choose fitting method: Kalman filter
                track.Evaluate();

                TagTrk2_pp[i] = track.GetPp();
                TagTrk2_track_quality[i] = track.GetQuality();
                TagTrk2_track_chi2[i] = track.GetChi2();
                TagTrk2_track_x_sigma[i] = track.GetXSigma();
                TagTrk2_track_y_sigma[i] = track.GetYSigma();

                for(int hitno = 0; hitno < track.GetSize(); hitno++)
                {
                    TagTrk2_track_x[hitno + TagTrk2_rechit_No] = track.At(hitno)->GetX();
                    TagTrk2_track_y[hitno + TagTrk2_rechit_No] = track.At(hitno)->GetY();
                    TagTrk2_track_z[hitno + TagTrk2_rechit_No] = track.At(hitno)->GetZ();
                }
                TagTrk2_rechit_No += track.GetSize();
                TagTrk2_rectrk_hit_No[i] = track.GetSize();
            }

            for(int i = 0; i < findRec.GetTrackNo(); i++)
            {
                TrkHitPVec recTrack((*(VecRecTrack.begin() + i)).begin(), (*(VecRecTrack.begin() + i)).end());
                DTrack track(recTrack,
                             findRec.GetR(i),       //used in Kalman filter
                             findRec.GetCenterX(i), //not used in Kalman filter, reserved for future
                             findRec.GetCenterY(i), //not used in Kalman filter, reserved for future
                             RecTrk_B);             //magnetic in the volume where track lies
                track.Fit(Rec_fit_method);          //choose fitting method: Kalman filter
                track.Evaluate();
               
                RecTrk2_pp[i] = track.GetPp();
                RecTrk2_track_quality[i] = track.GetQuality();
                RecTrk2_track_chi2[i] = track.GetChi2();
                RecTrk2_track_x_sigma[i] = track.GetXSigma();
                RecTrk2_track_y_sigma[i] = track.GetYSigma();

                for(int hitno = 0; hitno < track.GetSize(); hitno++)
                {
                    RecTrk2_track_x[hitno + RecTrk2_rechit_No] = track.At(hitno)->GetX();
                    RecTrk2_track_y[hitno + RecTrk2_rechit_No] = track.At(hitno)->GetY();
                    RecTrk2_track_z[hitno + RecTrk2_rechit_No] = track.At(hitno)->GetZ();
                }
                RecTrk2_rechit_No += track.GetSize();
                RecTrk2_rectrk_hit_No[i] = track.GetSize();
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

            TagTrk2_rechit_No = 0;
            RecTrk2_rechit_No = 0;
        }

        TagTrk2_No = rawTagTrkHits.size();

        bool trackerFlag = false;
        for(auto step : *stepIni)
        {
            if(InTagTrack(step->getX(), step->getY(), step->getZ()) && !trackerFlag)
            {
                TagTrk2_pp_truth_ini = sqrt(step->getPx()*step->getPx() +
                                            step->getPz()*step->getPz()  );
                trackerFlag = true;
            }
            else if(!InTagTrack(step->getX(), step->getY(), step->getZ()) && trackerFlag)
            {
                TagTrk2_pp_truth_fin = sqrt(step->getPx()*step->getPx() +
                                            step->getPz()*step->getPz()  );
                break;
            }
        }
        
        for(int i = 0; i < TagTrk2_No; ++i)
        {
            TagTrk2_x[i] = rawTagTrkHits.at(i).GetX();
            TagTrk2_y[i] = rawTagTrkHits.at(i).GetY();
            TagTrk2_z[i] = rawTagTrkHits.at(i).GetZ();
            TagTrk2_e[i] = rawTagTrkHits.at(i).GetE();
        }

        RecTrk2_No = rawRecTrkHits.size();

        trackerFlag = false;
        for(auto step : *stepIni)
        {
            if(InRecTrack(step->getX(), step->getY(), step->getZ()) && !trackerFlag)
            {
                RecTrk2_pp_truth_ini = sqrt(step->getPx()*step->getPx() +
                                            step->getPz()*step->getPz()  );
                trackerFlag = true;
            }
            else if(!InTagTrack(step->getX(), step->getY(), step->getZ()) && trackerFlag)
            {
                RecTrk2_pp_truth_fin = sqrt(step->getPx()*step->getPx() +
                                            step->getPz()*step->getPz()  );
                break;
            }
        }
        
        for(int i = 0; i < RecTrk2_No; ++i)
        {
            RecTrk2_x[i] = rawRecTrkHits.at(i).GetX();
            RecTrk2_y[i] = rawRecTrkHits.at(i).GetY();
            RecTrk2_z[i] = rawRecTrkHits.at(i).GetZ();
            RecTrk2_e[i] = rawRecTrkHits.at(i).GetE();
        }
//................................................................................//
//Memory management
        //vector<TrkHit>().swap(rawTagTrkHits);
        //vector<TrkHit>().swap(rawRecTrkHits);
        malloc_trim(0);
    }
}

void TrackingProcessor::CheckEvt(AnaEvent* evt)
{
    //cout<<"Check!"<<endl;
    if(!evt) cerr << "null event" << endl;
}

void TrackingProcessor::End()
{
    //for(auto it : ProSum) cout << it << endl;

    //cout<<"End!"<<endl;
}


#endif
