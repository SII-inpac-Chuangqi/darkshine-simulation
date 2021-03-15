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

//................................................................................//
//TRACKING
#include "Algo/TypeDef.h"
#include "Algo/Util.h"
#include "Algo/TrkHit.h"
#include "Algo/Digitization.h"
#include "Algo/GreedyFinding.h"
#include "Algo/KalmanFitting.h"

void TrackingProcessor::Begin()
{
    /*
     *
     *  DEFINE Processor explicitly with NAME in ControlManager.cpp L.37
     *  DEFINE Processor explicitly with NAME in ControlManager.cpp L.37
     *  DEFINE Processor explicitly with NAME in ControlManager.cpp L.37
     *
     */

    // Add description for this AnaProcessor
    Description = "Tracking.";

    // Register Int parameter
    //RegisterIntParameter("intVar", "Int Variable", &intVar, 0);

    // Register Double parameter
    //RegisterDoubleParameter("DoubleVar", "Double Var", &doubleVar, 0.);

    // Register String Parameter
    //RegisterStringParameter("StrVar", "String Variable", &strVar, "test");
    
    // Register dp_ana.root
//................................................................................//
//Truth
//................................................................................//
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

//................................................................................//
//Reconstructed
//................................................................................//
    EvtWrt->RegisterIntVariable("TagTrk2_trackNo",          &TagTrk2_trackNo,       "TagTrk2_trackNo/I");
    EvtWrt->RegisterDoubleVariable("TagTrk2_pp",            TagTrk2_pp,             "TagTrk2_pp[TagTrk2_trackNo]/D");

    EvtWrt->RegisterIntVariable("TagTrk2_recNo",      &TagTrk2_recNo,   "TagTrk2_recNo/I");
    EvtWrt->RegisterIntVariable("TagTrk2_rechitNo",   TagTrk2_rechitNo, "TagTrk2_rechitNo[TagTrk2_trackNo]/I");
    EvtWrt->RegisterDoubleVariable("TagTrk2_track_x", TagTrk2_track_x,  "TagTrk2_track_x[TagTrk2_recNo]/D");
    EvtWrt->RegisterDoubleVariable("TagTrk2_track_y", TagTrk2_track_y,  "TagTrk2_track_y[TagTrk2_recNo]/D");
    EvtWrt->RegisterDoubleVariable("TagTrk2_track_z", TagTrk2_track_z,  "TagTrk2_track_z[TagTrk2_recNo]/D");

    EvtWrt->RegisterDoubleVariable("TagTrk2_track_chi2",    TagTrk2_track_chi2,    "TagTrk2_track_chi2[TagTrk2_trackNo]/D");
    EvtWrt->RegisterDoubleVariable("TagTrk2_track_x_sigma", TagTrk2_track_x_sigma, "TagTrk2_track_x_sigma[TagTrk2_trackNo]/D");
    EvtWrt->RegisterDoubleVariable("TagTrk2_track_y_sigma", TagTrk2_track_y_sigma, "TagTrk2_track_y_sigma[TagTrk2_trackNo]/D");

    //EvtWrt->RegisterDoubleVariable("TagTrk2_pp_pre",  TagTrk2_pp_pre,   "TagTrk2_pp_pre[TagTrk2_trackNo]/D");

//................................................................................//
    EvtWrt->RegisterIntVariable("RecTrk2_trackNo",          &RecTrk2_trackNo,       "RecTrk2_trackNo/I");
    EvtWrt->RegisterDoubleVariable("RecTrk2_pp",            RecTrk2_pp,             "RecTrk2_pp[RecTrk2_trackNo]/D");

    EvtWrt->RegisterIntVariable("RecTrk2_recNo",      &RecTrk2_recNo,   "RecTrk2_recNo/I");
    EvtWrt->RegisterIntVariable("RecTrk2_rechitNo",   RecTrk2_rechitNo, "RecTrk2_rechitNo[RecTrk2_trackNo]/I");
    EvtWrt->RegisterDoubleVariable("RecTrk2_track_x", RecTrk2_track_x,  "RecTrk2_track_x[RecTrk2_recNo]/D");
    EvtWrt->RegisterDoubleVariable("RecTrk2_track_y", RecTrk2_track_y,  "RecTrk2_track_y[RecTrk2_recNo]/D");
    EvtWrt->RegisterDoubleVariable("RecTrk2_track_z", RecTrk2_track_z,  "RecTrk2_track_z[RecTrk2_recNo]/D");

    EvtWrt->RegisterDoubleVariable("RecTrk2_track_chi2",    RecTrk2_track_chi2,     "RecTrk2_track_chi2[RecTrk2_trackNo]/D");
    EvtWrt->RegisterDoubleVariable("RecTrk2_track_x_sigma", RecTrk2_track_x_sigma, "RecTrk2_track_x_sigma[RecTrk2_trackNo]/D");
    EvtWrt->RegisterDoubleVariable("RecTrk2_track_y_sigma", RecTrk2_track_y_sigma, "RecTrk2_track_y_sigma[RecTrk2_trackNo]/D");

    //EvtWrt->RegisterDoubleVariable("RecTrk2_pp_pre",  RecTrk2_pp_pre,   "RecTrk2_pp_pre[RecTrk2_trackNo]/D");
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
            TagTrk2_trackNo = findTag.GetTrackNo();
            RecTrk2_trackNo = findRec.GetTrackNo();

            TagTrk2_recNo = 0;
            RecTrk2_recNo = 0;

            for (int i = 0; i < findTag.GetTrackNo(); i++)
            {
                vector<shared_ptr<TrkHit>> tagTrack;
                tagTrack.assign((*(VecTagTrack.begin() + i)).begin(), (*(VecTagTrack.begin() + i)).end());
                KalmanFitting fitTag(tagTrack, findTag.GetR(i), 1.5);
                
                TagTrk2_pp[i] = fitTag.GetPp();
                TagTrk2_track_chi2[i] = fitTag.GetChi2();
                TagTrk2_track_x_sigma[i] = fitTag.GetXSigma();
                TagTrk2_track_y_sigma[i] = fitTag.GetYSigma();

                for(int hitno = 0; hitno < static_cast<int>(tagTrack.size()); hitno++)
                {
                    TagTrk2_track_x[hitno + TagTrk2_recNo] = tagTrack.at(hitno)->GetX();
                    TagTrk2_track_y[hitno + TagTrk2_recNo] = tagTrack.at(hitno)->GetY();
                    TagTrk2_track_z[hitno + TagTrk2_recNo] = tagTrack.at(hitno)->GetZ();
                }
                TagTrk2_recNo += tagTrack.size();
                TagTrk2_rechitNo[i] = tagTrack.size();
            }

            for(int i = 0; i < findRec.GetTrackNo(); i++)
            {
                vector<shared_ptr<TrkHit>> recTrack;
                recTrack.assign((*(VecRecTrack.begin() + i)).begin(), (*(VecRecTrack.begin() + i)).end());
                KalmanFitting fitRec(recTrack, findRec.GetR(i), 1.5);
                
                RecTrk2_pp[i] = fitRec.GetPp();
                RecTrk2_track_chi2[i] = fitRec.GetChi2();
                RecTrk2_track_x_sigma[i] = fitRec.GetXSigma();
                RecTrk2_track_y_sigma[i] = fitRec.GetYSigma();

                for(int hitno = 0; hitno < static_cast<int>(recTrack.size()); hitno++)
                {
                    RecTrk2_track_x[hitno + RecTrk2_recNo] = recTrack.at(hitno)->GetX();
                    RecTrk2_track_y[hitno + RecTrk2_recNo] = recTrack.at(hitno)->GetY();
                    RecTrk2_track_z[hitno + RecTrk2_recNo] = recTrack.at(hitno)->GetZ();
                }
                RecTrk2_recNo += recTrack.size();
                RecTrk2_rechitNo[i] = recTrack.size();
            }

//................................................................................//
//Write root
//................................................................................//
//Truth
        }
        else
        {
            TagTrk2_trackNo = 0;
            RecTrk2_trackNo = 0;

            TagTrk2_recNo = 0;
            RecTrk2_recNo = 0;
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
        //malloc_trim(0);
    }
}

void TrackingProcessor::CheckEvt(AnaEvent* evt)
{
    //cout<<"Check!"<<endl;
}

void TrackingProcessor::End()
{
    //for(auto it : ProSum) cout << it << endl;

    //cout<<"End!"<<endl;
}


