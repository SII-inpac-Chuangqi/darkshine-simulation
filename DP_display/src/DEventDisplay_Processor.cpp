//
// Created by Zhang Yulei on 12/22/20.
// Modified by Q.L. on 6/20/23.
//
#include "DEventDisplay.h"

#include <TApplication.h>
#include <TEveBrowser.h>
#include "TEveWindowManager.h"
#include <TEveManager.h>
#include <TEveEventManager.h>
#include <TEveGeoNode.h>
#include "TGLViewer.h"
#include "TGPicture.h"
#include "TGLabel.h"
#include "TGButton.h"
#include "TGTab.h"
#include "TString.h"
#include <TGeoManager.h>
#include <TGeoMatrix.h>
#include <TGeoNode.h>
#include <TROOT.h>
#include <TSystem.h>
//-----------
// Processor
#include "Core/AnaData.h"
#include "Core/ConfigManager.h"
#include "Algo/RecECAL.h"
#include "Algo/Digitizer.h"
#include "Algo/TrackingProcessor.h"

void DEventDisplay::RunAnaProcessors() { 
   if(!RunAna_) return;
   auto *AnaList = new TEveElementList("AnaProcessor Outputs");
   gEve->AddElement(AnaList);

    /************************************/
    /*       Init ALgo processor        */
    /************************************/
   // init dummy evt writter (to share information between algo processor)
   // non-persistent: refresh per event (?)
   std::cout << "[AnaProcessor] ==> Load dummy EvtWriter..." << std::endl;
   auto EvtWrt = make_shared<EventStoreAndWriter>("_DDis_temp.root");
   EvtWrt->setIsDisplay(true);
   EvtWrt->RegisterTree();
   //TODO: now all the branch in mem and no fill; may need regular fill/flush for mem or keep the track of history.
   std::cout << "[AnaProcessor] ==> Load AnaData..." << std::endl;
   AnaData::CreateInstance();  
   std::cout << "[AnaProcessor] ==> Load AnaData Geometry..." << std::endl;
   dAnaData->setRootFile(dDisData->GetGeoFile()); //TODO: unify DisData and AnaData
   dAnaData->readGeometryDetails();
   dAnaData->printGeometryDetails(); // print analysis level geometry (better for human read)
   // Initialize the Processors // e.g. Digitizer Tracking RecECAL // all use default parameter
   //TODO: make algo reusable. note the init of each run.
   std::cout << "[AnaProcessor] ==> Load Processors..." << std::endl;
   auto digitizer = make_shared<Digitizer>("Digitizer", EvtWrt);
   digitizer->setIsDisplay(true);
   auto tracking = make_shared<TrackingProcessor>("Tracking", EvtWrt);
   tracking->setIsDisplay(true);
   auto recECAL = make_shared<RecECAL>("RecECAL", EvtWrt);
   recECAL->setIsDisplay(true);
   // preload the ana config
   if(anaConfig!=""){ 
    //TODO: actually the list could be read... but not just hardcoded -- you have to define what you wantto display!!
    //or we can define a common display_output, when algo send sth there, DDis will automatically display it! That will be cool-er!
        std::cout << "[AnaProcessor] ==> Init Processors with config..." << std::endl;
        auto config = make_shared<Config>(anaConfig.Data());
        ConfigManager::SetupAnaParameters(config.get(),"Digitizer",digitizer.get());
        ConfigManager::SetupAnaParameters(config.get(),"Tracking",tracking.get());
        ConfigManager::SetupAnaParameters(config.get(),"RecECAL",recECAL.get());
   }
    std::cout << "[AnaProcessor] ==> Init ECAL Processors with UI setup..." << std::endl;
    recECAL->setIntValue("Advance",3); //enable cluster + spliting
    recECAL->setIntValue("SaveTrackInfo",1); //enable cluster (no need for return, only use the dump hits
    recECAL->setIntValue("SaveTruthInfo",1); //enable cluster (no need for return, only use the dump hits
    // clustering scheme
    recECAL->setDoubleValue("Enoise",Enoise);
    recECAL->setDoubleValue("EThres_S",EThres_S);
    recECAL->setDoubleValue("EThres_N",EThres_N);
    recECAL->setDoubleValue("EThres_P",EThres_P);
    recECAL->setDoubleValue("Critical_E",Critical_E);
    recECAL->setDoubleValue("Critical_N",Critical_N);
    recECAL->setDoubleValue("EM_ENERGY_SCALE_MeV",EM_ENERGY_SCALE_MeV);
    recECAL->setDoubleValue("ENERGY_SHIFT_MeV",ENERGY_SHIFT_MeV);
    recECAL->setIntValue("weight_type",weight_type);
    recECAL->setDoubleValue("EM_SCALE_LENGTH_mm",EM_SCALE_LENGTH_mm);
    std::cout << "[AnaProcessor] ==> Start Processors..." << std::endl;
    //*((recECAL->getDoubleParameters()).at("r_cut").second) = RecECAL_r_cut;
    digitizer->Begin();
    tracking->Begin();
    recECAL->Begin();

    /************************************/
    /*        Run and collect results   */
    /************************************/
    digitizer->ProcessEvt(evt);
    digitizer->CheckEvt(evt);
    tracking->ProcessEvt(evt);
    tracking->CheckEvt(evt);
    recECAL->ProcessEvt(evt);
    recECAL->CheckEvt(evt);
    //collect output
    std::cout << "[AnaProcessor] ==> Collect Results..." << std::endl;
    // method1: access internal variable or collection (remember to unique_ptr or delete)
    auto& clustered_hits = recECAL->clustered_hits; // this collection will not be saved as contain too many internal information
    // auto CaloCols = evt->getCalorimeterHitCollection(); // this is to access the digitized hit

    //method2: access the EventStoreAndWriter (no need really FillTree)
    auto ECAL_Cluster_N = EvtWrt->FindOutVariable<int>("ECAL_Cluster_N");
    auto ECAL_Cluster_E_total = EvtWrt->FindOutVariable<double>("ECAL_Cluster_E_total");
    auto ECAL_Cluster_E = EvtWrt->FindOutVariable<vector<double>>("ECAL_Cluster_E");
    auto ECAL_Cluster_NSub = EvtWrt->FindOutVariable<vector<int>>("ECAL_Cluster_NSub");
    auto ECAL_ClusterSub_E = EvtWrt->FindOutVariable<vector<double>>("ECAL_ClusterSub_E");
    auto ECAL_ClusterSub_P0 = EvtWrt->FindOutVariable<vector<int>>("ECAL_ClusterSub_P0");
    auto ECAL_ClusterSub_P1 = EvtWrt->FindOutVariable<vector<int>>("ECAL_ClusterSub_P1");

    /************************************/
    /*    Visualization Out Collection  */
    /************************************/
    /*
    - Primary cluster (list) (only draw line, alpha==0)
      - Sub cluster1 (same color but just selectable)
       - box (splitted box use thicker line width)
      - Sub cluster2
    
    */
    // create cluster and sub cluster folder
    std::cout << "[AnaProcessor] ==> Visualizing..." << std::endl;
    for(int P0=0;P0<*ECAL_Cluster_N;P0++){
        if(AnaDisClusLeadingOnly && P0>0) break;
        double priE=ECAL_Cluster_E->at(P0);
        auto *Primary = new TEveElementList(Form("PCLUS_%02d_E%.0fMeV",P0,priE));
        AnaList->AddElement(Primary);
        for(int Psub=-1;Psub<ECAL_Cluster_NSub->at(P0);Psub++){ //loop possible sub clusters
            // P1==P2==-1: no splitting
            // P1==P2>=0: local maximum
            // P1>=0, P2==-1: belong to one cluster
            // P1>=0, P2>=0: belong to two clusters
            double subE=ECAL_Cluster_E->at(P0); 
            if(ECAL_Cluster_NSub->at(P0)==0){ // no splitting
                if(!AnaDisCellPri) break;
                auto *Sub = new TEveElementList(Form("Main_E%.0fMeV",subE));
                // P1/P2=-1 means non splitting
                Primary->AddElement(Sub);
                for (auto const &h : clustered_hits) {
                    if(h->P0()!=P0 || !h->isValid()) continue;
                    if (h->hit->getE() < r_min * subE) continue; // reuse the fractional filter
                    if (h->hit->getE() < ECAL_Emin) continue; // apply universal filter
                    auto *box = makeRecCaloBox(h->hit, -1);
                    auto color=FindColor(priE,*ECAL_Cluster_E_total); //same primary cluster share one color
                    box->SetLineColor(color);
                    // box->SetFillColor(color);
                    // assert P1==P2==-1
                    box->SetMainAlpha(0);
                    box->SetLineWidth(2);
                    box->SetLineWidth(2);
                    box->SetName(Form("%s P%02d(%.0fMeV)",box->GetName(),P0,h->E()));
                    Sub->AddElement(reinterpret_cast<TEveElement *>(box));
                }
                break;
            }else if(Psub>=0){ // splitting
                for(int k=0;k<ECAL_ClusterSub_P0->size();k++)
                    if(ECAL_ClusterSub_P0->at(k)==P0 && ECAL_ClusterSub_P1->at(k)==Psub){
                        subE=ECAL_ClusterSub_E->at(k); // find correct subE
                        break;
                    }
                auto *Sub = new TEveElementList(Form("Sub_%02d_E%.0fMeV",Psub,subE));
                // P1/P2=-1 means non splitting
                Primary->AddElement(Sub);
                for (auto const &h : clustered_hits) {
                    bool cellDraw=true;
                    if(h->P0()!=P0 || !h->isValid()) continue;
                    if (h->hit->getE() < r_min * subE) continue; // reuse the fractional filter
                    if (h->hit->getE() < ECAL_Emin) continue; // apply universal filter
                    auto *box = makeRecCaloBox(h->hit, -1);
                    auto color=FindColor(priE,*ECAL_Cluster_E_total); //same primary cluster share one color
                    box->SetLineColor(color);
                    // box->SetFillColor(color);
                    box->SetMainAlpha(0);
                    box->SetLineWidth(1);

                    if (AnaDisCellMax && h->P1()==h->P2() && h->P2()==Psub && Psub>=0){ //local  maximum -- line width+1
                        box->SetLineWidth(5);
                        box->SetName(Form("P%02d MAX S%02d(%.0fMeV) %s",P0,Psub,h->E(),box->GetName()));
                    }
                    else if(AnaDisCellSub && h->P1()==Psub && h->P2()==-1){ //non splitted cell
                        box->SetName(Form("P%02d S%02d(%.0fMeV) %s",P0,Psub,h->E(),box->GetName()));
                    }else if(AnaDisCellSp1 && h->P1()==Psub && h->P2()>=0){ //splitted cell line width-1, belong to P1 but split to P2
                        box->SetLineWidth(0.5);
                        box->SetName(Form("P%02d sp1 S%02d(%.0fMeV) S%02d(%.0fMeV) %s",P0,Psub,h->E_sub1(),h->P2(),h->E_sub2(),box->GetName()));
                    }else if(AnaDisCellSp2 && h->P2()==Psub && h->P2()>=0 && h->P1()>=0){ //splitted cell line width-1, belong to P1 but split to P2
                        box=makeRecCaloBox(h->hit, -1, 1);
                        box->SetLineWidth(0.5);
                        box->SetName(Form("P%02d sp2 S%02d(%.0fMeV) S%02d(%.0fMeV) %s",P0,Psub,h->E_sub2(),h->P1(),h->E_sub1(),box->GetName()));
                    }
                    else{
                        cellDraw=false;
                    }
                    if(cellDraw)
                        Sub->AddElement(reinterpret_cast<TEveElement *>(box));
                }
            }
        }
    }
    /************************************/
    /*    Clean context                 */
    /************************************/
    std::cout << "[AnaProcessor] ==> Cleaning..." << std::endl;
    // TODO: make algo reusable and only clean the storage, or save the display history?
    EvtWrt->CloseFile();
    // since they are unique_ptr, will be deleted automatically.
}

void DEventDisplay::makeGUIProcessor(DEventDisplay *fh) {
   TEveBrowser *browser = gEve->GetBrowser();

   TGLabel *lbl = nullptr;

   browser->StartEmbedding(TRootBrowser::kLeft);
   auto *frmMain1 = new TGMainFrame(gClient->GetRoot(), 1200, 800);
   frmMain1->SetWindowName("XX GUI");
   frmMain1->SetCleanup(kDeepCleanup);
   TGHorizontalFrame *hf = nullptr;
   // RecECAL
   {
       // Label
       hf = new TGHorizontalFrame(frmMain1);
       {
           lbl = new TGLabel(hf, "RecECAL");
           hf->AddFrame(lbl);
       }
       frmMain1->AddFrame(hf);
       //Universal toggle
       hf = new TGHorizontalFrame(frmMain1);
       {
           guiRunAna = new TGCheckButton(hf, "Run Ana");
           if (guiRunAna) guiRunAna->Toggle(); // default on
           hf->AddFrame(guiRunAna);
           guiRunAna->Connect("Toggled(Bool_t)", "DEventDisplay", fh, "guiOptionsAna()");
       }
       frmMain1->AddFrame(hf);
       //-----------------------------------------------------------------------------------------
        hf = new TGHorizontalFrame(frmMain1);
        {
        gui_Enoise = new TGNumberEntry(hf, Enoise, 9, 999, TGNumberFormat::kNESReal,
                                            TGNumberFormat::kNEAPositive ,
                                            TGNumberFormat::kNELLimitMinMax,
                                            0.001,999);
        hf->AddFrame(gui_Enoise);
        gui_Enoise->Connect("ValueSet(Long_t)", "DEventDisplay", fh, "guiOptionsAna()");
        lbl = new TGLabel(hf, "E_noise/MeV");
        hf->AddFrame(lbl);
        }
        frmMain1->AddFrame(hf);
        //-----------------------------------------------------------------------------------------
        hf = new TGHorizontalFrame(frmMain1);
        {
        gui_EThres_S = new TGNumberEntry(hf, EThres_S, 9, 999, TGNumberFormat::kNESInteger,
                                            TGNumberFormat::kNEAPositive ,
                                            TGNumberFormat::kNELLimitMinMax,
                                            -1,999);
        hf->AddFrame(gui_EThres_S);
        gui_EThres_S->Connect("ValueSet(Long_t)", "DEventDisplay", fh, "guiOptionsAna()");
        lbl = new TGLabel(hf, "S");
        hf->AddFrame(lbl);
        }
        frmMain1->AddFrame(hf);
        //-----------------------------------------------------------------------------------------
        hf = new TGHorizontalFrame(frmMain1);
        {
        gui_EThres_N = new TGNumberEntry(hf, EThres_N, 9, 999, TGNumberFormat::kNESInteger,
                                            TGNumberFormat::kNEAPositive ,
                                            TGNumberFormat::kNELLimitMinMax,
                                            -1,999);
        hf->AddFrame(gui_EThres_N);
        gui_EThres_N->Connect("ValueSet(Long_t)", "DEventDisplay", fh, "guiOptionsAna()");
        lbl = new TGLabel(hf, "N");
        hf->AddFrame(lbl);
        }
        frmMain1->AddFrame(hf);
        //-----------------------------------------------------------------------------------------
        hf = new TGHorizontalFrame(frmMain1);
        {
        gui_EThres_P = new TGNumberEntry(hf, EThres_P, 9, 999, TGNumberFormat::kNESInteger,
                                            TGNumberFormat::kNEAPositive ,
                                            TGNumberFormat::kNELLimitMinMax,
                                            -1,999);
        hf->AddFrame(gui_EThres_P);
        gui_EThres_P->Connect("ValueSet(Long_t)", "DEventDisplay", fh, "guiOptionsAna()");
        lbl = new TGLabel(hf, "P");
        hf->AddFrame(lbl);
        }
        frmMain1->AddFrame(hf);
        //-----------------------------------------------------------------------------------------
        hf = new TGHorizontalFrame(frmMain1);
        {
        gui_Critical_E = new TGNumberEntry(hf, Critical_E, 9, 999, TGNumberFormat::kNESReal,
                                            TGNumberFormat::kNEAPositive ,
                                            TGNumberFormat::kNELLimitMinMax,
                                            0.001,999);
        hf->AddFrame(gui_Critical_E);
        gui_Critical_E->Connect("ValueSet(Long_t)", "DEventDisplay", fh, "guiOptionsAna()");
        lbl = new TGLabel(hf, "Critical E/MeV");
        hf->AddFrame(lbl);
        }
        frmMain1->AddFrame(hf);
        //-----------------------------------------------------------------------------------------
        hf = new TGHorizontalFrame(frmMain1);
        {
        gui_Critical_N = new TGNumberEntry(hf, Critical_N, 9, 999, TGNumberFormat::kNESInteger,
                                            TGNumberFormat::kNEAPositive ,
                                            TGNumberFormat::kNELLimitMinMax,
                                            0,999);
        hf->AddFrame(gui_Critical_N);
        gui_Critical_N->Connect("ValueSet(Long_t)", "DEventDisplay", fh, "guiOptionsAna()");
        lbl = new TGLabel(hf, "Critical N");
        hf->AddFrame(lbl);
        }
        frmMain1->AddFrame(hf);
        //-----------------------------------------------------------------------------------------
        hf = new TGHorizontalFrame(frmMain1);
        {
        gui_EM_ENERGY_SCALE_MeV = new TGNumberEntry(hf, EM_ENERGY_SCALE_MeV, 9, 999, TGNumberFormat::kNESReal,
                                            TGNumberFormat::kNEAPositive ,
                                            TGNumberFormat::kNELLimitMinMax,
                                            0.001,999);
        hf->AddFrame(gui_EM_ENERGY_SCALE_MeV);
        gui_EM_ENERGY_SCALE_MeV->Connect("ValueSet(Long_t)", "DEventDisplay", fh, "guiOptionsAna()");
        lbl = new TGLabel(hf, "EM_ENERGY_SCALE_MeV");
        hf->AddFrame(lbl);
        }
        frmMain1->AddFrame(hf);
        //-----------------------------------------------------------------------------------------
        hf = new TGHorizontalFrame(frmMain1);
        {
        gui_ENERGY_SHIFT_MeV = new TGNumberEntry(hf, ENERGY_SHIFT_MeV, 9, 999, TGNumberFormat::kNESReal,
                                            TGNumberFormat::kNEAPositive ,
                                            TGNumberFormat::kNELLimitMinMax,
                                            0.001,999);
        hf->AddFrame(gui_ENERGY_SHIFT_MeV);
        gui_ENERGY_SHIFT_MeV->Connect("ValueSet(Long_t)", "DEventDisplay", fh, "guiOptionsAna()");
        lbl = new TGLabel(hf, "ENERGY_SHIFT_MeV");
        hf->AddFrame(lbl);
        }
        frmMain1->AddFrame(hf);
        //-----------------------------------------------------------------------------------------
        hf = new TGHorizontalFrame(frmMain1);
        {
        gui_EM_SCALE_LENGTH_mm = new TGNumberEntry(hf, EM_SCALE_LENGTH_mm, 9, 999, TGNumberFormat::kNESReal,
                                            TGNumberFormat::kNEAPositive ,
                                            TGNumberFormat::kNELLimitMinMax,
                                            0.001,999);
        hf->AddFrame(gui_EM_SCALE_LENGTH_mm);
        gui_EM_SCALE_LENGTH_mm->Connect("ValueSet(Long_t)", "DEventDisplay", fh, "guiOptionsAna()");
        lbl = new TGLabel(hf, "EM_SCALE_LENGTH_mm");
        hf->AddFrame(lbl);
        }
        frmMain1->AddFrame(hf);
        //-----------------------------------------------------------------------------------------
        hf = new TGHorizontalFrame(frmMain1);
        {
        gui_weight_type = new TGNumberEntry(hf, weight_type, 9, 999, TGNumberFormat::kNESInteger,
                                            TGNumberFormat::kNEAPositive ,
                                            TGNumberFormat::kNELLimitMinMax,
                                            -1,999);
        hf->AddFrame(gui_weight_type);
        gui_weight_type->Connect("ValueSet(Long_t)", "DEventDisplay", fh, "guiOptionsAna()");
        lbl = new TGLabel(hf, "weight_type(0-linear,1-log)");
        hf->AddFrame(lbl);
        }
        frmMain1->AddFrame(hf);
        // Label
       hf = new TGHorizontalFrame(frmMain1);
       {
           lbl = new TGLabel(hf, "Display Options");
           hf->AddFrame(lbl);
       }
       frmMain1->AddFrame(hf);
       //Universal toggle
       hf = new TGHorizontalFrame(frmMain1);
       {
           gui_AnaDisClusLeadingOnly = new TGCheckButton(hf, "Hide Non-Leading Cluster");
           hf->AddFrame(gui_AnaDisClusLeadingOnly);
           gui_AnaDisClusLeadingOnly->Connect("Toggled(Bool_t)", "DEventDisplay", fh, "guiOptionsAna()");
       }
       frmMain1->AddFrame(hf);
       //Universal toggle
       hf = new TGHorizontalFrame(frmMain1);
       {
           gui_AnaDisCellMax = new TGCheckButton(hf, "Show localMax");
           if (gui_AnaDisCellMax) gui_AnaDisCellMax->Toggle(); // default on
           hf->AddFrame(gui_AnaDisCellMax);
           gui_AnaDisCellMax->Connect("Toggled(Bool_t)", "DEventDisplay", fh, "guiOptionsAna()");
       }
       frmMain1->AddFrame(hf);
       //Universal toggle
       hf = new TGHorizontalFrame(frmMain1);
       {
           gui_AnaDisCellSp1 = new TGCheckButton(hf, "Show split cell(main)");
           if (gui_AnaDisCellSp1) gui_AnaDisCellSp1->Toggle(); // default on
           hf->AddFrame(gui_AnaDisCellSp1);
           gui_AnaDisCellSp1->Connect("Toggled(Bool_t)", "DEventDisplay", fh, "guiOptionsAna()");
       }
       frmMain1->AddFrame(hf);
       //Universal toggle
       hf = new TGHorizontalFrame(frmMain1);
       {
           gui_AnaDisCellSp2 = new TGCheckButton(hf, "Show split cell(share)");
           if (gui_AnaDisCellSp2) gui_AnaDisCellSp2->Toggle(); // default on
           hf->AddFrame(gui_AnaDisCellSp2);
           gui_AnaDisCellSp2->Connect("Toggled(Bool_t)", "DEventDisplay", fh, "guiOptionsAna()");
       }
       frmMain1->AddFrame(hf);
       //Universal toggle
       hf = new TGHorizontalFrame(frmMain1);
       {
           gui_AnaDisCellPri = new TGCheckButton(hf, "Show PriClus Cell");
           if (gui_AnaDisCellPri) gui_AnaDisCellPri->Toggle(); // default on
           hf->AddFrame(gui_AnaDisCellPri);
           gui_AnaDisCellPri->Connect("Toggled(Bool_t)", "DEventDisplay", fh, "guiOptionsAna()");
       }
       frmMain1->AddFrame(hf);
       //Universal toggle
       hf = new TGHorizontalFrame(frmMain1);
       {
           gui_AnaDisCellSub = new TGCheckButton(hf, "Show SubClus Cell");
           if (gui_AnaDisCellSub) gui_AnaDisCellSub->Toggle(); // default on
           hf->AddFrame(gui_AnaDisCellSub);
           gui_AnaDisCellSub->Connect("Toggled(Bool_t)", "DEventDisplay", fh, "guiOptionsAna()");
       }
       frmMain1->AddFrame(hf);
   }// end of RecECAL

   frmMain1->MapSubwindows();
   frmMain1->Resize();
   frmMain1->MapWindow();

   browser->StopEmbedding();
   browser->SetTabTitle("Ana Processors", 0);
}

[[maybe_unused]] void DEventDisplay::guiOptionsAna() {
   // Ana Processors
    RunAna_ = guiRunAna->IsOn();
    Enoise=gui_Enoise->GetNumber();
    EThres_S=gui_EThres_S->GetNumber();
    EThres_N=gui_EThres_N->GetNumber();
    EThres_P=gui_EThres_P->GetNumber();
    Critical_E=gui_Critical_E->GetNumber();
    Critical_N=gui_Critical_N->GetNumber();
    EM_ENERGY_SCALE_MeV=gui_EM_ENERGY_SCALE_MeV->GetNumber();
    ENERGY_SHIFT_MeV=gui_ENERGY_SHIFT_MeV->GetNumber();
    weight_type=gui_weight_type->GetIntNumber();
    EM_SCALE_LENGTH_mm=gui_EM_SCALE_LENGTH_mm->GetNumber();

    AnaDisClusLeadingOnly = gui_AnaDisClusLeadingOnly->IsOn();
    AnaDisCellMax = gui_AnaDisCellMax->IsOn();
    AnaDisCellSp1 = gui_AnaDisCellSp1->IsOn();
    AnaDisCellSp2 = gui_AnaDisCellSp2->IsOn();
    AnaDisCellPri = gui_AnaDisCellPri->IsOn();
    AnaDisCellSub = gui_AnaDisCellSub->IsOn();

   gotoEvent(_eventID);
}
