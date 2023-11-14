//
// Created by Zhang Yulei on 12/20/20.
//

#include "DEventDisplay.h"

#include "TEveStraightLineSet.h"
#include "TEveTrack.h"
#include "TEveTrackPropagator.h"
#include "TEveTrackGL.h"
#include "TEveTrackEditor.h"
#include "TParticle.h"
#include "TEvePathMark.h"
#include "TGLViewer.h"
#include "TEveCaloLegoOverlay.h"
#include "TEveLegoEventHandler.h"
#include "TEveBrowser.h"
#include "TGTab.h"
#include "TEveText.h"
#include "TGLAutoRotator.h"

#include "CaloHitsDisplay.h"

#include <iostream>

bool DEventDisplay::drawEvent(int id, bool resCam) {
    // Draw Events

    // Draw Geometry first
    if (_drawDetector) drawDetector();

    if (!gGeoManager) return false;

    // read event

    if ( !EvtReader->ReadEntry(id) ) {
        std::cerr<<"[Read Event] ==> No event in current file ..."<<std::endl;
        return false;
    }
    evt = EvtReader->getEvt();

    EvtReader->Convert();
    //evtHistory.emplace_back(evt);
    if (!evt) return false;
    std::cout << "[Event Display] ==> Plotting event: " << evt->getEventId() << std::endl;


    // Draw Event collection by collection
    /***************************/
    /*   Display of Raw Hits   */
    /***************************/
    if (_drawMCTracks) {
        drawInitialParticleStep();
        drawMCParticles();
    }
    auto *SimHitsList = new TEveElementList("Simulated Hits");
    gEve->AddElement(SimHitsList);
    // Tracker Hits
    if (_drawSimuTrkHits) {
        drawSimuTrkHits(SimHitsList);
    }
    if (_drawSimuCaloHits) {
        drawSimuCaloHits(SimHitsList);
    }

    // Display Simulated Calorimeter Hits LEGO
//    CaloDisplay = new CaloHitsDisplay();
//    auto CALCols = evt->getSimulatedHitCollection();
//    makeCaloLego(CALCols, CaloDisplay, _drawSimuCaloLego);

    /************************************/
    /*   Reconstruction Visualization   */
    /************************************/
    std::cout << "[Info] ==> Starting Ana processor(s)..." << std::flush;
    RunAnaProcessors();

    /*********************/
    /*   Finalization   */
    /********************/
    gEve->Redraw3D(resCam);
    gEve->FullRedraw3D(resCam);
    gEve->GetDefaultGLViewer()->RequestDraw(TGLRnrCtx::kLODHigh);

    return true;
}

void DEventDisplay::drawInitialParticleStep() { // initial particle will loos energy during passing the tracker... so recorded their steps
    // Step Collection
    auto StepCols = evt->getStepCollection();
    for (const auto &StepCol : StepCols) {
        auto Steps = StepCol.second;
        std::cout << "[Event Display] ==> Draw Collection: " << StepCol.first;
        std::cout << ", with total steps: " << Steps->size() << std::endl;
        if (Steps->size() < 2) continue;
        
        std::vector<int> trkIDs{};
        for(auto s:*Steps)
            trkIDs.push_back(s->getId());
        auto unique_trk = std::unique(trkIDs.begin(),trkIDs.end());
        trkIDs.erase(unique_trk, trkIDs.end()); 
        std::cout<<"[INFO] detected incident MC particles "<<trkIDs.size()<<std::endl;
        MCSteps.clear();
        if(std::find(trkIDs.begin(), trkIDs.end(), 0) != trkIDs.end()){
            // old style 1.5 DStep: ID=0,1,2,... only support same initial trk
            auto lineSet = new TEveStraightLineSet(StepCol.first.data());
            for (unsigned i = 0; i < Steps->size() - 1; ++i) {
                // loop from 0 to n-1 to draw n-1 lines
                auto step = Steps->at(i);
                auto next_step = Steps->at(i + 1);

                TVector3 start(step->getX() / CUNIT, step->getY() / CUNIT, step->getZ() / CUNIT);
                TVector3 end(next_step->getX() / CUNIT, next_step->getY() / CUNIT, next_step->getZ() / CUNIT);

                makeLines(lineSet, start, end, kRed, 1, false, 3, 0);
            }
            gEve->AddElement(lineSet);
            // always copy the vec...
            auto steps = std::make_shared<DStepVec>(*Steps);
            MCSteps.insert({1,std::move(steps)});
        }else{
            // new style 1.5 DStep: ID=trkID:1,2,...support multiple intial trks
            for(auto trkID:trkIDs){
                auto lineSet = new TEveStraightLineSet(Form("%s trk %d",StepCol.first.c_str(),trkID));
                // re-org the steps and keep
                auto steps = std::make_shared<DStepVec>();
                double initialE=-1;
                std::copy_if (Steps->begin(), Steps->end(), std::back_inserter(*steps), [trkID](DStep* s){return s->getId()==trkID;});
                for (unsigned i = 0; i < steps->size() - 1; ++i) {
                    // loop from 0 to n-1 to draw n-1 lines
                    auto step = steps->at(i);
                    auto next_step = steps->at(i + 1);

                    if(i==0)
                        initialE=step->getE();

                    TVector3 start(step->getX() / CUNIT, step->getY() / CUNIT, step->getZ() / CUNIT);
                    TVector3 end(next_step->getX() / CUNIT, next_step->getY() / CUNIT, next_step->getZ() / CUNIT);

                    makeLines(lineSet, start, end, kRed, 1, false, 3, 0);
                }
                lineSet->SetName(Form("%s E %.0fMeV",lineSet->GetName(),initialE));
                gEve->AddElement(lineSet);
                MCSteps.insert({trkID,std::move(steps)});
            }
        }
    }
}

std::string DEventDisplay::easyPDG(int pdg){
    switch(pdg){
        case 11:
            return "e";
        case -11:
            return "ep";
        case 22:
            return "y";
        case 2112:
            return "n";
        case 2212:
            return "p";
        default:
            if(pdg>1000000)
                return "NUC"; // this is Nuclear... then what is DM pdg?
            else
                return "PDG"+std::to_string(pdg)+"E";
    }
}

bool DEventDisplay::isInitialMC(McParticle* mc){
    return mc->getCreateProcess()=="";
}

// TODO: please ensure no loop (DAG)!! or it is NP hard.
int DEventDisplay::recursiveFindTracks(int mother, MCParticleVec* MCs, int* count, std::string path, std::shared_ptr<std::map<int,std::string>> sto){
    // std::cout<<"[DEBUG] scan mother "<<mother<<std::endl;
    if(path=="") path=std::to_string(mother);
    int traced=0;
    for (unsigned i = 0; i < MCs->size(); ++i) {
        auto mc=MCs->at(i);
        if(sto && sto->at(mc->getId())!=""){ // the particle has definate path, no ned to consider again
            continue;
        }
        if(isInitialMC(mc)) continue; //skipinitial particle
        if(!mc->getParents()) continue; // skip particle without parent info
        if(mc->getParents()->getId()!=mother) continue; // mother not match (or initial)
            
        if(!sto){
            //scan mode
            // count all the particle in case some mothers are hide due to PDG.
            traced+=1;
            continue;
        }
        
        // first add this particle
        // std::cout<<"[DEBUG] NOOOO secondary trk of mother "<<mc->getId()<<std::endl;
        (*count)++;
        sto->at(mc->getId())=path;
        // std::cout<<"[DEBUG] Set "<<mc->getId()<<" : "<<path<<std::endl;
        traced+=1;
        // then consider its child
        // std::cout<<"[DEBUG] grand mom "<<mother<<" scan secondary trk of mother "<<mc->getId()<<std::endl;
        int child = recursiveFindTracks(mc->getId(),MCs,count, ""); // scan only
        if(child){
            // std::cout<<"[DEBUG] PROBED (new) secondary trk of mother "<<mc->getId()<<" n="<<child<<std::endl;
            std::string p = std::string(Form("%s->%d(%s%.0f)",path.c_str(),mc->getId(),easyPDG(mc->getPdg()).c_str(),mc->getP()));
            traced+=recursiveFindTracks(mc->getId(),MCs,count, p, sto); //really add; share same propagator
        }
    }
    return traced;
}

void DEventDisplay::drawMCParticles() {
    // MC Collection ( e.g. Particle Track)
    // Initialize Track
    auto *gMCTrackList = new TEveTrackList("MC Tracks");
    gEve->AddElement(gMCTrackList);
    auto *trkProp = gMCTrackList->GetPropagator();
    if (_build_Tracker_BField){
        trkProp->SetMagFieldObj(new DSMagneticField());
        std::cout<<"[INFO] MC track prop: B field enabled"<<std::endl;
        if(dDisData->isMagnets()) std::cout<<"[INFO] B filed: using magnets(high defination B map)..."<<std::endl;
        else std::cout<<"[INFO] B field: no valid magents found. using uniform B..."<<std::endl;
        // trkProp->PrintMagField (0,0,0);
        // trkProp->PrintMagField (0,0,+180 / CUNIT);
        // trkProp->PrintMagField (0,0,+200 / CUNIT);
    } else {
        trkProp->SetMagField(0,0,0);
        std::cout<<"[INFO] MC track prop: NO B field!!"<<std::endl;
    }

    auto MCCols = evt->getMcParticleCollection();
    for (const auto &MCCol : MCCols) {
        auto MCs = MCCol.second;
        std::cout << "[Event Display] ==> Draw Collection: " << MCCol.first;
        std::cout << ", with total particles: " << MCs->size() << std::endl;
        // if (MCs->size() < 2) continue; // ??
        // --------------------------- MC tracking algo (find the MC chain) ---------------
        // modify with CAUTION
        auto trkmap=std::make_shared<std::map<int,std::string>>();
        //determine initial
        std::vector<int> motherIDs;
        for (unsigned i = 0; i < MCs->size(); ++i) {
            auto mc=MCs->at(i);
            if(isInitialMC(mc)){
                motherIDs.push_back(mc->getId());
                trkmap->insert({mc->getId(),"INC"});
            }else{
                trkmap->insert({mc->getId(),""});
            }
        }
        if(motherIDs.size()==0){
            std::cerr<<"[INTERNAL] ERROR: missing MC particle mother info. contact experts"<<std::endl;
            continue;
        }
        // start from initials
        int count=0; // aprticle looped
        int added=0;
        int traced=0;
        for(auto m:motherIDs){
            std::cout<<"MC chain recursively add... initial mother="<<m<<std::endl<<std::flush;
            // use hierarchy structure
            traced += recursiveFindTracks(m, MCs, &count, "", trkmap);
        }
        // format the results
        for (unsigned i = 0; i < MCs->size(); ++i) {
            auto mc=MCs->at(i);
            if(trkmap->at(mc->getId())==""){
                std::cout<<"[DEBUG] Broken MC chain found "<<mc->getId()<<std::endl;
                traced+=recursiveFindTracks(mc->getId(), MCs, &count, "", trkmap);
            }
            else if(trkmap->at(mc->getId())!="INC"){
                trkmap->at(mc->getId())+="->"; // add suffix (better format)
            }
        }
        // then consider those still broken path
        for (unsigned i = 0; i < MCs->size(); ++i) {
            auto mc=MCs->at(i);
            if(trkmap->at(mc->getId())==""){
                trkmap->at(mc->getId())=="???";
            }
        }
        // ok now add the track based on the path
        for (unsigned i = 0; i < MCs->size(); ++i) {
            auto mc=MCs->at(i);
            // if(trkmap->at(mc->getId())=="INC") continue; // skip initial particle since they generated in the tracker and not able to trace
            if (mc->getP() < MC_Emin) continue; // we use momentum since somtime the Nuclear with huge E will spoil it!
            if ( (MC_PDG) && (abs(mc->getPdg()) != MC_PDG)) continue;

            std::string particle=Form("%d(%s%.0f)",mc->getId(),easyPDG(mc->getPdg()).c_str(),mc->getP());
            std::string path=trkmap->at(mc->getId());
            if(path==""){
                std::cerr<<"[INTERNAL] ERROR: missing MC particle path info. contact experts"<<std::endl;
                continue;
            }
            if(path=="INC"){
                //recal steps to make better track
                auto *track = makeMCTrack(trkProp, i, mc, MCSteps.at(mc->getId()));
                track->SetName(Form("[INC] %s",particle.c_str()));
                gMCTrackList->AddElement(track);
            }else{
                auto *track = makeMCTrack(trkProp, i, mc);
                track->SetName(Form("[%s] %s",path.c_str(),particle.c_str()));
                gMCTrackList->AddElement(track);
            }
            added++;
        }
        std::cout<<"Looped MC  "<<count<<" traced "<<traced<<" added(after cut) "<<added<<std::endl<<std::flush;
        // --------------------------- END MC tracking algo (find the MC chain) ---------------
    }
    gMCTrackList->MakeTracks();
    MCSteps.clear();
}

void DEventDisplay::drawSimuTrkHits(TEveElementList *SimHitsList) {
    /***************/
    /*   Tracker   */
    /***************/
    // Ploting raw hits
    // basic idea is to draw a box on the hit point, but the box is scaled with a factor
    auto TrkCols = evt->getSimulatedHitCollection();
    for (const auto &TrkCol : TrkCols) {
        // only count hits in tracker
        if (!TString(TrkCol.first).Contains("Trk")) continue;
        if (TrkCol.second->empty()) continue;
        auto *SimTrkHits = new TEveElementList(TrkCol.first.data());
        SimHitsList->AddElement(SimTrkHits);
        auto Trks = TrkCol.second;
        std::cout << "[Event Display] ==> Draw Collection: " << TrkCol.first;
        std::cout << ", with total hits: " << Trks->size() << std::endl;
        if (Trks->empty()) continue;

        for (auto trk_hit : *Trks) {
            // Skip calo hits under threshold
            if (trk_hit->getE() < Trk_Emin && TString(TrkCol.first).Contains("Trk")) continue;
            auto *box = makeTrackerBox(trk_hit, _scale_factor_SimuTrkHits);
            SimTrkHits->AddElement(reinterpret_cast<TEveElement *>(box));
        }
    }
}

void DEventDisplay::drawSimuCaloHits(TEveElementList *SimHitsList) {
    /************/
    /*   CALO   */
    /************/
    // Ploting raw hits
    // basic idea is to draw a box on the hit point
    auto CALCols = evt->getSimulatedHitCollection();
    for (const auto &CALCol : CALCols) {
        // only count hits in calorimeter
        if (!TString(CALCol.first).Contains("CAL")) continue;
        if (CALCol.second->empty()) continue;
        auto *SimHits = new TEveElementList(CALCol.first.data());
        SimHitsList->AddElement(SimHits);
        auto CALs = CALCol.second;
        std::cout << "[Event Display] ==> Draw Collection: " << CALCol.first;
        std::cout << ", with total hits: " << CALs->size() << std::endl;
        if (CALs->empty()) continue;
        double EMax = 0.;
        for (auto cal_hit : *CALs)
            EMax = (cal_hit->getE() > EMax) ? cal_hit->getE() : EMax;

        for (auto cal_hit : *CALs) {
            // Skip calo hits under threshold
            if (cal_hit->getE() < r_min * EMax) continue;
            if (cal_hit->getE() < ECAL_Emin && TString(CALCol.first).Contains("ECAL")) continue;
            if (cal_hit->getE() < HCAL_Emin && TString(CALCol.first).Contains("HCAL")) continue;
            auto *box = makeSimuCaloBox(cal_hit, EMax);
            SimHits->AddElement(reinterpret_cast<TEveElement *>(box));
        }
    }
}

void DEventDisplay::makeLines(TEveStraightLineSet *lineSet, const TVector3 &start, const TVector3 &end,
                              const Color_t &color, const Style_t &style, bool drawMarkers,
                              double lineWidth, int markerPos) {
    // Make a straight line between two input points
    lineSet->AddLine(start.x(), start.y(), start.z(), end.x(), end.y(), end.z());
    lineSet->SetLineColor(color);
    lineSet->SetLineStyle(style);
    lineSet->SetLineWidth(lineWidth);
    if (drawMarkers) {
        if (markerPos == 0)
            lineSet->AddMarker(start.x(), start.y(), start.z());
        else
            lineSet->AddMarker(end.x(), end.y(), end.z());
    }
}

TEveTrack *DEventDisplay::makeMCTrack(TEveTrackPropagator *trkProp, unsigned /*id*/, McParticle *mc, std::shared_ptr<DStepVec> steps) {
    // refer to https://root.cern.ch/doc/master/classTEvePathMarkT.html
    // get mother id
    int m_id = -999;
    auto p = mc->getParents();
    if (p) m_id = p->getId();

    TEveMCTrack rt;
    rt.SetPdgCode(mc->getPdg());
    rt.SetUniqueID(mc->getId());
    rt.SetMomentum(mc->getPx() * 1e-3, mc->getPy() * 1e-3, mc->getPz() * 1e-3, mc->getEnergy() * 1e-3);
    rt.SetProductionVertex(mc->getVertexX() / CUNIT, mc->getVertexY() / CUNIT, mc->getVertexZ() / CUNIT, 0.);

    auto *track = new TEveTrack(&rt, trkProp);
    track->SetName(Form("Trk %d: PDG %d", rt.GetUniqueID(), rt.GetPdgCode()));
    track->SetPdg(mc->getPdg());
    track->SetLineColor(PDG_Color[mc->getPdg()]);
    // Line Style Setting
    if (mc->getERemain() > 0.) {
        track->SetLineWidth(2);
        track->SetLineStyle(10);
    } else {
        // highlighted Special Particle like muons, hadrons
        if (abs(mc->getPdg()) == 13) {
            track->SetLineWidth(4);
            track->SetLineStyle(2);
        } else if (abs(mc->getPdg()) > 1e2 || abs(mc->getPdg()) < 1e3) {
            track->SetLineWidth(3);
            track->SetLineStyle(3);
        } else if (abs(mc->getPdg()) > 1e3 || mc->getP() > 1e3) { // Require Momentum > 1 GeV
            track->SetLineWidth(4);
            track->SetLineStyle(4);
        }
        if (mc->getP() > 1e3) { // Highlight
            track->SetLineWidth(track->GetLineWidth() * 1.5);
        }
    }

    // add other step points if avalible
    if(steps)
        for(auto s:*steps){
            // std::cout<<"Step added/cm "<<s->getX() / CUNIT << " " <<s->getY() / CUNIT << " " <<s->getZ() / CUNIT<<std::endl;
            // std::cout<<"Step added/GEV "<<s->getPz() / 1e3 << " " <<s->getPy() / 1e3 << " " <<s->getPx() / 1e3<<std::endl;
            track->AddPathMark(TEvePathMarkD(TEvePathMarkD::kReference,
                  TEveVectorD(s->getX() / CUNIT, s->getY() / CUNIT, s->getZ() / CUNIT),
                  TEveVectorD(s->getPx()* 1e-3, s->getPy()* 1e-3, s->getPz()* 1e-3)));
        }

    track->AddPathMark(TEvePathMarkD(TEvePathMarkD::kDecay,
                  TEveVectorD(mc->getEndPointX() / CUNIT, mc->getEndPointY() / CUNIT, mc->getEndPointZ() / CUNIT)));

    track->SetTitle((Form("Index=%d, Pdg=%d\n"
                          "MotherID=%d\n"
                          "E=%.3f, Eremain=%.3f [MeV]\n"
                          "Ek(P)=%.3f (%.3f, %.3f, %.3f) [MeV]\n"
                          "Vertex=(%.3f, %.3f, %.3f) [mm]\n"
                          "End=(%.3f, %.3f, %.3f) [mm]\n"
                          "CreateProcess: %s\n",
                          rt.GetUniqueID(), rt.GetPdgCode(),
                          m_id,
                          mc->getEnergy(), mc->getERemain(),
                          mc->getP(), mc->getPx(), mc->getPy(), mc->getPz(),
                          mc->getVertexX(), mc->getVertexY(), mc->getVertexZ(),
                          mc->getEndPointX(), mc->getEndPointY(), mc->getEndPointZ(),
                          mc->getCreateProcess().data()
    )));

    // debug initial particle
    // if(rt.GetUniqueID()==1){
    //     std::cout<<"PDG ptr "<<rt.GetPDG() << std::endl;
    //     std::cout<<"Px/GeV "<<track->GetEndMomentum()[0] << std::endl;
    //     std::cout<<"Py/GeV "<<track->GetEndMomentum()[1] << std::endl;
    //     std::cout<<"Pz/GeV "<<track->GetEndMomentum()[2] << std::endl;
    //     std::cout<<"chg  "<<track->GetCharge() << std::endl;
    //     std::cout<<"status  "<<track->GetStatus() << std::endl;
    // }

    return track;
}


TEveBox *DEventDisplay::makeBox(const double *abs_pos, const double *half_size) {
    auto *box = new TEveBox();
    double vertex[24] = {0.};
    // (a,b,-c)
    vertex[0] = half_size[0] + abs_pos[0];
    vertex[1] = half_size[1] + abs_pos[1];
    vertex[2] = -half_size[2] + abs_pos[2];
    // (a,-b,-c)
    vertex[3] = half_size[0] + abs_pos[0];
    vertex[4] = -half_size[1] + abs_pos[1];
    vertex[5] = -half_size[2] + abs_pos[2];
    // (-a,-b,-c)
    vertex[6] = -half_size[0] + abs_pos[0];
    vertex[7] = -half_size[1] + abs_pos[1];
    vertex[8] = -half_size[2] + abs_pos[2];
    // (-a,b,-c)
    vertex[9] = -half_size[0] + abs_pos[0];
    vertex[10] = half_size[1] + abs_pos[1];
    vertex[11] = -half_size[2] + abs_pos[2];
    // (a,b,c)
    vertex[12] = half_size[0] + abs_pos[0];
    vertex[13] = half_size[1] + abs_pos[1];
    vertex[14] = half_size[2] + abs_pos[2];
    // (a,-b,c)
    vertex[15] = half_size[0] + abs_pos[0];
    vertex[16] = -half_size[1] + abs_pos[1];
    vertex[17] = half_size[2] + abs_pos[2];
    // (-a,-b,c)
    vertex[18] = -half_size[0] + abs_pos[0];
    vertex[19] = -half_size[1] + abs_pos[1];
    vertex[20] = half_size[2] + abs_pos[2];
    // (-a,b,c)
    vertex[21] = -half_size[0] + abs_pos[0];
    vertex[22] = half_size[1] + abs_pos[1];
    vertex[23] = half_size[2] + abs_pos[2];

    for (int k = 0; k < 24; k += 3) box->SetVertex((k / 3), vertex[k], vertex[k + 1], vertex[k + 2]);

    return box;
}

TEveBox *DEventDisplay::makeRotBox(const double *abs_pos, const double *half_size,  const TRotation& rot) {
    auto *box = new TEveBox();
    double vertex[24] = {0.};
    TVector3 Vec;
    // (a,b,-c)
    Vec.SetXYZ(half_size[0], half_size[1], -half_size[2]);
    Vec.Transform(rot);
    vertex[0] = Vec.x() + abs_pos[0];
    vertex[1] = Vec.y() + abs_pos[1];
    vertex[2] = Vec.z() + abs_pos[2];
    // (a,-b,-c)
    Vec.SetXYZ(half_size[0], -half_size[1], -half_size[2]);
    Vec.Transform(rot);
    vertex[3] = Vec.x() + abs_pos[0];
    vertex[4] = Vec.y() + abs_pos[1];
    vertex[5] = Vec.z() + abs_pos[2];
    // (-a,-b,-c)
    Vec.SetXYZ(-half_size[0], -half_size[1], -half_size[2]);
    Vec.Transform(rot);
    vertex[6] = Vec.x() + abs_pos[0];
    vertex[7] = Vec.y() + abs_pos[1];
    vertex[8] = Vec.z() + abs_pos[2];
    // (-a,b,-c)
    Vec.SetXYZ(-half_size[0], half_size[1], -half_size[2]);
    Vec.Transform(rot);
    vertex[9] = Vec.x() + abs_pos[0];
    vertex[10] = Vec.y() + abs_pos[1];
    vertex[11] = Vec.z() + abs_pos[2];
    // (a,b,c)
    Vec.SetXYZ(half_size[0], half_size[1], half_size[2]);
    Vec.Transform(rot);
    vertex[12] = Vec.x() + abs_pos[0];
    vertex[13] = Vec.y() + abs_pos[1];
    vertex[14] = Vec.z() + abs_pos[2];
    // (a,-b,c)
    Vec.SetXYZ(half_size[0], -half_size[1], half_size[2]);
    Vec.Transform(rot);
    vertex[15] = Vec.x() + abs_pos[0];
    vertex[16] = Vec.y() + abs_pos[1];
    vertex[17] = Vec.z() + abs_pos[2];
    // (-a,-b,c)
    Vec.SetXYZ(-half_size[0], -half_size[1], half_size[2]);
    Vec.Transform(rot);
    vertex[18] = Vec.x() + abs_pos[0];
    vertex[19] = Vec.y() + abs_pos[1];
    vertex[20] = Vec.z() + abs_pos[2];
    // (-a,b,c)
    Vec.SetXYZ(-half_size[0], half_size[1], half_size[2]);
    Vec.Transform(rot);
    vertex[21] = Vec.x() + abs_pos[0];
    vertex[22] = Vec.y() + abs_pos[1];
    vertex[23] = Vec.z() + abs_pos[2];
    for (int k = 0; k < 24; k += 3) box->SetVertex((k / 3), vertex[k], vertex[k + 1], vertex[k + 2]);
    return box;
}


TEveBox *DEventDisplay::makeSimuCaloBox(SimulatedHit *hit, double EMax) const {
    gGeoManager->FindNode(hit->getX() / CUNIT, hit->getY() / CUNIT, hit->getZ() / CUNIT);
//    auto *cur_shape = dynamic_cast<TGeoBBox *>(cur_node->GetVolume()->GetShape());

    auto *mother_node = gGeoManager->GetMother();
    auto *cur_shape = dynamic_cast<TGeoBBox *>(mother_node->GetVolume()->GetShape());
    auto RotationMatrix = mother_node->GetMatrix()->GetRotationMatrix();
    auto *mother2_node = gGeoManager->GetMother(2);
    auto RotationMatrix2 = mother2_node->GetMatrix()->GetRotationMatrix();
    auto *mother3_node = gGeoManager->GetMother(3);
    auto RotationMatrix3 = mother3_node->GetMatrix()->GetRotationMatrix();

    double hx0 = fabs(cur_shape->GetDX() * RotationMatrix[0] + cur_shape->GetDY() * RotationMatrix[1] + cur_shape->GetDZ() * RotationMatrix[2]);
    double hy0 = fabs(cur_shape->GetDX() * RotationMatrix[3] + cur_shape->GetDY() * RotationMatrix[4] + cur_shape->GetDZ() * RotationMatrix[5]);
    double hz0 = fabs(cur_shape->GetDX() * RotationMatrix[6] + cur_shape->GetDY() * RotationMatrix[7] + cur_shape->GetDZ() * RotationMatrix[8]);

    double hx1 = fabs(hx0 * RotationMatrix2[0] + hy0 * RotationMatrix2[1] + hz0 * RotationMatrix2[2]);
    double hy1 = fabs(hx0 * RotationMatrix2[3] + hy0 * RotationMatrix2[4] + hz0 * RotationMatrix2[5]);
    double hz1 = fabs(hx0 * RotationMatrix2[6] + hy0 * RotationMatrix2[7] + hz0 * RotationMatrix2[8]);

    double hx = fabs(hx1 * RotationMatrix3[0] + hy1 * RotationMatrix3[1] + hz1 * RotationMatrix3[2]);
    double hy = fabs(hx1 * RotationMatrix3[3] + hy1 * RotationMatrix3[4] + hz1 * RotationMatrix3[5]);
    double hz = fabs(hx1 * RotationMatrix3[6] + hy1 * RotationMatrix3[7] + hz1 * RotationMatrix3[8]);

    double abs_pos[3] = {hit->getX() / CUNIT, hit->getY() / CUNIT, hit->getZ() / CUNIT};
    double half_size[3] = {hx, hy, hz};

    if (_drawScaleSimuCaloBox) {
        double ratio =
                log(hit->getE() / EMax / _scale_factor_SimuCaloHits + 1) / log(1 / _scale_factor_SimuCaloHits + 1);
        half_size[0] *= ratio;
        half_size[1] *= ratio;
        half_size[2] *= ratio;
    }

    auto *box = makeBox(abs_pos, half_size);
    box->SetName((Form("Cell %d", hit->getCellId())));

    auto color = FindColor(hit->getE(), EMax);
    box->SetLineColor(color);
    box->SetFillColor(color);
    box->SetLineWidth(0.0001);
    if (!_drawScaleSimuCaloBox)
        box->SetMainAlpha(log(log(hit->getE() + 1) / log(EMax + 1) + 1));
    else
        box->SetMainAlpha(0.65);

    box->SetTitle(Form("CellID=%d, ID=(%d, %d, %d)\n"
                       "E = %.3f , E_EM = %.3f, E_Had = %.3f [MeV]\n"
                       "Center=(%.3f, %.3f, %.3f) [mm]\n",
                       hit->getCellId(), hit->getCellIdX(), hit->getCellIdY(), hit->getCellIdZ(),
                       hit->getE(), hit->getEdepEm(), hit->getEdepHad(),
                       hit->getX(), hit->getY(), hit->getZ()
    ));
    return box;
}

TEveBox *DEventDisplay::makeRecCaloBox(CalorimeterHit *hit, double EMax, int copyNo) {
    gGeoManager->FindNode(hit->getX() / CUNIT, hit->getY() / CUNIT, hit->getZ() / CUNIT);
    auto *mother_node = gGeoManager->GetMother();
    auto *cur_shape = dynamic_cast<TGeoBBox *>(mother_node->GetVolume()->GetShape());
    auto RotationMatrix = mother_node->GetMatrix()->GetRotationMatrix();
    auto *mother2_node = gGeoManager->GetMother(2);
    auto RotationMatrix2 = mother2_node->GetMatrix()->GetRotationMatrix();

    double hx0 = fabs(cur_shape->GetDX() * RotationMatrix[0] + cur_shape->GetDY() * RotationMatrix[1] + cur_shape->GetDZ() * RotationMatrix[2]);
    double hy0 = fabs(cur_shape->GetDX() * RotationMatrix[3] + cur_shape->GetDY() * RotationMatrix[4] + cur_shape->GetDZ() * RotationMatrix[5]);
    double hz0 = fabs(cur_shape->GetDX() * RotationMatrix[6] + cur_shape->GetDY() * RotationMatrix[7] + cur_shape->GetDZ() * RotationMatrix[8]);

    double hx = fabs(hx0 * RotationMatrix2[0] + hy0 * RotationMatrix2[1] + hz0 * RotationMatrix2[2]);
    double hy = fabs(hx0 * RotationMatrix2[3] + hy0 * RotationMatrix2[4] + hz0 * RotationMatrix2[5]);
    double hz = fabs(hx0 * RotationMatrix2[6] + hy0 * RotationMatrix2[7] + hz0 * RotationMatrix2[8]);

    double abs_pos[3] = {hit->getX() / CUNIT, hit->getY() / CUNIT, hit->getZ() / CUNIT};
    double half_size[3] = {hx, hy, hz};

    auto *box = makeBox(abs_pos, half_size);
    if(copyNo==-1)
        box->SetName((Form("Cell %d", hit->getCellId())));
    else    
        box->SetName((Form("Cell %d(%d)", hit->getCellId(),copyNo)));

    auto color = FindColor(hit->getE(), EMax);
    box->SetLineColor(color);
    box->SetFillColor(color);

    box->SetTitle(Form("CellID=%d, ID=(%d, %d, %d)\n"
                       "E = %.3f [MeV]\n"
                       "Center=(%.3f, %.3f, %.3f) [mm]\n",
                       hit->getCellId(), hit->getCellIdX(), hit->getCellIdY(), hit->getCellIdZ(),
                       hit->getE(),
                       hit->getX(), hit->getY(), hit->getZ()
    ));
    return box;
}

template<class Hit>
TEveBox *DEventDisplay::makeTrackerBox(Hit *hit, double scale) {
    auto cur_node = gGeoManager->FindNode(hit->getX() / CUNIT, hit->getY() / CUNIT, hit->getZ() / CUNIT );
    auto *cur_shape = dynamic_cast<TGeoBBox *>(cur_node->GetVolume()->GetShape());
    double abs_pos[3] = {hit->getX() / CUNIT, hit->getY() / CUNIT, hit->getZ() / CUNIT};
    double half_size[3] = {cur_shape->GetDX() * scale, cur_shape->GetDY() * scale, cur_shape->GetDZ() * scale};

    auto *mother2_node = gGeoManager->GetMother(2);
    auto RotationMatrix = mother2_node->GetMatrix()->GetRotationMatrix();

    TRotation rot;
    rot.RotateZ(-TMath::ASin(RotationMatrix[1]));
    auto *box = makeRotBox(abs_pos, half_size, rot);

//    auto *box = makeBox(abs_pos, half_size);
    box->SetName((Form("Tracker Hit %d", hit->getCellId())));

    auto color = kMagenta;
    box->SetLineColor(color);
    box->SetFillColor(color);
    box->SetMainAlpha(1.0);

    box->SetTitle(Form("CellID=%d, ID=(%d, %d, %d)\n"
                       "E = %.3f [MeV], t = %.3f [ns]\n"
                       "Center=(%.3f, %.3f, %.3f) [mm]\n",
                       hit->getCellId(), hit->getCellIdX(), hit->getCellIdY(), hit->getCellIdZ(),
                       hit->getE(), hit->getT(),
                       hit->getX(), hit->getY(), hit->getZ()
    ));
    return box;
}

template<class CaloCol>
void DEventDisplay::makeCaloLego(CaloCol col, CaloHitsDisplay *calo_dis, bool if_draw_lego) {
    /*
    // Convert Calo Hits to Eve Calo Data
    // ECAL is split into several slices with respect to Z layer
    //TODO: staggered ECAL is more complex and nontrivial to project
    // HCAL merges together

    int ECAL_slice_number = ECALslice_calo ? (int) ECAL_Cell_Arr[2] : 1;
    std::cout << "[Calo Data Hist] ==> ECAL slice number: " << ECAL_slice_number << std::endl;

    std::vector<std::vector<CaloHit> > tmpCaloHits;

    for (const auto &CALCol : col) {
        // only count hits in calorimeter
        if (!TString(CALCol.first).Contains("CAL")) continue;
        if (CALCol.second->empty()) continue;
        auto CALs = CALCol.second;
        if (TString(CALCol.first).Contains("ECAL_Center") && _drawECAL_calo) {
            // Only Slice for ECAL Center
            for (int i = 0; i < ECAL_slice_number; ++i) {
                tmpCaloHits.emplace_back();
            }

            // Fill Calo hits in to hist 2F vector
            for (auto hits : *CALs) {
                int i = ECALslice_calo ? hits->getCellIdZ() - 1 : 0;
                CaloHit tmp;
                tmp.X = hits->getX() / CUNIT; // cm
                tmp.Y = hits->getY() / CUNIT; // cm
                tmp.Z = hits->getZ() / CUNIT; // cm
                tmp.E = hits->getE();    // MeV
                tmp.Color = kGreen + i;
                tmp.id = hits->getCellId();
                tmp.id_x = hits->getCellIdX();
                tmp.id_y = hits->getCellIdY();
                tmp.id_z = hits->getCellIdZ();

                tmpCaloHits.at(i).push_back(tmp);
            }
        }
        if (TString(CALCol.first).Contains("HCAL") && _drawHCAL_calo) {
            // unused
        }
    }
    for (unsigned i = 0; i < tmpCaloHits.size(); ++i) {
        if (i < (unsigned) ECAL_slice_number) {
            auto name = "ECAL" + to_string(i);
            calo_dis->name.emplace_back(name);
            calo_dis->E_thre.push_back(ECAL_Emin * 1e-3);
            calo_dis->color.push_back(kBlue + i);
        } else {
            calo_dis->name.emplace_back("HCAL");
            calo_dis->E_thre.push_back(HCAL_Emin * 1e-3);
            calo_dis->color.push_back(kMagenta);
        }
    }
    calo_dis->xbin = ECAL_Cell_Arr[0];
    calo_dis->ybin = ECAL_Cell_Arr[1];
    calo_dis->zbin = ECAL_Cell_Arr[2];
    calo_dis->xmin = -ECAL_Size[0];
    calo_dis->xmax = ECAL_Size[0];
    calo_dis->ymin = -ECAL_Size[1];
    calo_dis->ymax = ECAL_Size[1];
    calo_dis->zmin = -ECAL_Size[2] + ECAL_Z_Move;
    calo_dis->zmax = ECAL_Size[2] + ECAL_Z_Move;

    calo_dis->if_drawLego = if_draw_lego;
    calo_dis->if_log = _drawLogSacle;
    calo_dis->scale_factor = _scale_factor_Lego;
    calo_dis->calovec = tmpCaloHits;
    calo_dis->makeLego(win_v.at(0), win_s.at(0), dXY);
    calo_dis->calovec = tmpCaloHits;
    calo_dis->makeLego(win_v.at(1), win_s.at(1), dXZ);
    calo_dis->calovec = tmpCaloHits;
    calo_dis->makeLego(win_v.at(2), win_s.at(2), dYZ);

    // Draw Text For Fun
    win_v.at(3)->SetElementName("Dark SHINE Logo");
    win_s.at(3)->DestroyElements();
    auto marker = new TEvePointSet(1);
    marker->SetName("Origin marker");
    marker->SetMarkerColor(kBlack);
    marker->SetMarkerStyle(3);
    Float_t a = 1;
    marker->SetPoint(0, a,  +a, +a);
    win_s.at(3)->AddElement(marker);
    auto text_ds = new TEveText("Dark SHINE");
    gEve->AddToListTree(text_ds, kFALSE);
    text_ds->PtrMainTrans()->RotateLF(1, 3, TMath::PiOver2());
    text_ds->SetMainColor(kOrange-2);
    text_ds->SetFontSize(64);
    text_ds->SetFontMode(TGLFont::kExtrude);
    text_ds->SetLighting(kTRUE);
    win_s.at(3)->AddElement(text_ds);
    win_v.at(3)->GetGLViewer()->GetAutoRotator()->SetWDolly(0.01);
    win_v.at(3)->GetGLViewer()->GetAutoRotator()->SetADolly(0.01);
    win_v.at(3)->GetGLViewer()->GetAutoRotator()->SetATheta(0.01);
    win_v.at(3)->GetGLViewer()->GetAutoRotator()->SetWTheta(0.01);
    win_v.at(3)->GetGLViewer()->GetAutoRotator()->SetDt(0.001);
    win_v.at(3)->GetGLViewer()->GetAutoRotator()->SetWPhi(0.75);
    win_v.at(3)->GetGLViewer()->GetAutoRotator()->Start();
    */
}


void DEventDisplay::MakeViewerScene(TEveWindowSlot *slot, TEveViewer *&v, TEveScene *&s) {

    // Create a scene and a viewer in the given slot.

    v = new TEveViewer("Viewer");
    v->SpawnGLViewer(reinterpret_cast<TGedEditor *>(gEve->GetEditor()));
    slot->ReplaceWindow(v);
    gEve->GetViewers()->AddElement(v);
    s = gEve->SpawnNewScene("Scene");
    v->AddScene(s);
}
