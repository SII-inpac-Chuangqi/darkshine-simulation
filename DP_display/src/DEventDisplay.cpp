//
// Created by Zhang Yulei on 12/18/20.
//

#include "DEventDisplay.h"

#include "TFile.h"
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

#include <TGeoManager.h>
#include <TGeoMatrix.h>
#include <TGeoNode.h>
#include <TROOT.h>
#include <TSystem.h>

#include <iomanip>
#include <iostream>

void DEventDisplay::Initialize() {
    // Need to initialize gApplication and gEve first
    if ((!gApplication) || (gApplication && gApplication->TestBit(TApplication::kDefaultApplication))) {
        std::cout << "In DEventDisplay ctor: gApplication not found, creating..." << std::flush;
        new TApplication("ROOT_application", nullptr, nullptr);
        std::cout << "done!" << std::endl;

    }
    if (!gEve) {
        std::cout << "In DEventDisplay ctor: gEve not found, creating..." << std::flush;
        TEveManager::Create();
        std::cout << "done!" << std::endl;

        gEve->GetWindowManager()->SetName("Dark SHINE Event Display");
        gEve->GetWindowManager()->SetTitle("Dark SHINE Event Display");
        gEve->GetBrowser()->SetTabTitle("Main 3D Event Display");
    }

    // Set Camera
    auto *v = gEve->GetDefaultGLViewer();
    v->CurrentCamera().SetExternalCenter(kTRUE);
    v->CurrentCamera().SetCenterVec(0, 0, 30.);

    // Set TApplication Termination
    gEve->GetBrowser()->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");

    _eventID = 0;
    EvtReader = shared_ptr<EventReader_D>(new EventReader_D());
}

bool DEventDisplay::drawDetector() {
    // draw the geometry.
    TGeoNode *top_node = gGeoManager->GetTopNode();
    assert(top_node != nullptr);

    //Set transparency & color of geometry
    TObjArray *volumes = gGeoManager->GetListOfVolumes();
    for (int i = 0; i < volumes->GetEntriesFast(); i++) {
        auto *volume = dynamic_cast<TGeoVolume *>(volumes->At(i));
        auto name = TString(volume->GetName());
        assert(volume != nullptr);
        if (name == "Target_LV") {
            volume->SetLineColor(46);
            volume->SetTransparency(20);
        } else if (name.Contains("TagTrk")) {
            volume->SetLineColor(30);
            volume->SetTransparency(70);
        } else if (name.Contains("RecTrk")) {
            volume->SetLineColor(32);
            volume->SetTransparency(70);
        } else if (name == "ECAL") {
            volume->SetLineColor(38);
            volume->SetTransparency(75);
            volume->SetVisibility(kTRUE);
        } else if (name == "HCAL") {
            volume->SetLineColor(40);
            volume->SetTransparency(75);
            volume->SetVisibility(kTRUE);
        }
    }

    for (int i = 0; i < world_node->GetNdaughters(); ++i) {
        auto *current_node = dynamic_cast<TGeoNode *>(world_node->GetDaughter(i));
        auto cur_Name = TString(current_node->GetVolume()->GetName());

        if (cur_Name.Contains("ECAL") || cur_Name.Contains("HCAL")) {
            current_node->SetVisDaughters(kFALSE);
            current_node->SetVisibility(kTRUE);
        }
    }

    auto *eve_top_node = new TEveGeoTopNode(gGeoManager, top_node);
    eve_top_node->IncDenyDestroy();
    gEve->AddElement(eve_top_node);

    return true;
}


bool DEventDisplay::readFile(const TString &file_in) {
    // Read Geometry from ROOT file
    f = new TFile(file_in);
    if (!f) {
        std::cerr << "[Display] ==> file: " << file_in << " not existed or broken..." << std::endl;
        return false;
    }
    return true;
}

bool DEventDisplay::readGeo(const TString &file_in) {
    // Read Geometry from ROOT file
//    auto file = std::shared_ptr<TFile>(new TFile(file_in));
//    if (!file) {
//        std::cerr << "[Display] ==> file: " << file_in << " not existed or broken..." << std::endl;
//        return false;
//    }
    auto file = f;
    gGeoManager = (TGeoManager *) file->Get("DetGeoManager");
    if (!gGeoManager) {
        std::cerr << "[Display] ==> No Geometry in the file..." << std::endl;
        return false;
    }

    world_node = shared_ptr<TGeoNode>(dynamic_cast<TGeoNode *>(gGeoManager->GetListOfNodes()->At(0)));

    return true;
}


bool DEventDisplay::readEvt(const TString &file_in) {
    // load Event from ROOT file
//    auto file = std::shared_ptr<TFile>(new TFile(file_in));
//    if (!file) {
//        std::cerr << "[Display] ==> file: " << file_in << " not existed or broken..." << std::endl;
//        return false;
//    }

    auto file = f;
    EvtReader->ReadTree("Dark_Photon", file);

    return true;
}


bool DEventDisplay::readEntry(int i) {
    // unused right now
    if (i < 0) return false;
    EvtReader->ReadEntry(i);
    return true;
}


void DEventDisplay::Open(DEventDisplay *evtDis) {
    makeGUIRaw(evtDis);
    makeGUIProcessor(evtDis);

    gEve->Redraw3D(kTRUE);
    gApplication->Run(kFALSE);
}

void DEventDisplay::makeGUIRaw(DEventDisplay *fh) {
    TEveBrowser *browser = gEve->GetBrowser();

    TGLabel *lbl = nullptr;
    TGTextButton *tb = nullptr;

    browser->StartEmbedding(TRootBrowser::kLeft);
    auto *frmMain1 = new TGMainFrame(gClient->GetRoot(), 1200, 800);
    frmMain1->SetWindowName("XX GUI");
    frmMain1->SetCleanup(kDeepCleanup);
    TGHorizontalFrame *hf = nullptr;
    // Event Options
    {
        hf = new TGHorizontalFrame(frmMain1);
        {
            // evt number entry
            lbl = new TGLabel(hf, "Go to event: ");
            hf->AddFrame(lbl);
            guiEvent = new TGNumberEntry(hf, 0, 9, 999, TGNumberFormat::kNESInteger,
                                         TGNumberFormat::kNEANonNegative,
                                         TGNumberFormat::kNELLimitMinMax,
                                         0, 99999);
            hf->AddFrame(guiEvent);
            guiEvent->Connect("ValueSet(Long_t)", "DEventDisplay", fh, "guiGoto()");

            // redraw button
            tb = new TGTextButton(hf, "Redraw Event");
            hf->AddFrame(tb);
            tb->Connect("Clicked()", "DEventDisplay", fh, "guiGoto()");
        }
        frmMain1->AddFrame(hf);
    }
    // Draw Options
    {
        hf = new TGHorizontalFrame(frmMain1);
        {
            lbl = new TGLabel(hf, "\n Draw Options");
            hf->AddFrame(lbl);
        }
        frmMain1->AddFrame(hf);

        hf = new TGHorizontalFrame(frmMain1);
        {
            guidrawDetector = new TGCheckButton(hf, "Draw Detectors");
            if (guidrawDetector) guidrawDetector->Toggle();
            hf->AddFrame(guidrawDetector);
            guidrawDetector->Connect("Toggled(Bool_t)", "DEventDisplay", fh, "guiOptions()");
        }
        frmMain1->AddFrame(hf);
        hf = new TGHorizontalFrame(frmMain1);
        {
            guidrawMCTracks = new TGCheckButton(hf, "Draw MC Tracks");
            if (guidrawMCTracks) guidrawMCTracks->Toggle();
            hf->AddFrame(guidrawMCTracks);
            guidrawMCTracks->Connect("Toggled(Bool_t)", "DEventDisplay", fh, "guiOptions()");
        }
        frmMain1->AddFrame(hf);
        hf = new TGHorizontalFrame(frmMain1);
        {
            guidrawSimuTrkHits = new TGCheckButton(hf, "Draw Simulated Tracker Hits");
            if (guidrawSimuTrkHits) guidrawSimuTrkHits->Toggle();
            hf->AddFrame(guidrawSimuTrkHits);
            guidrawSimuTrkHits->Connect("Toggled(Bool_t)", "DEventDisplay", fh, "guiOptions()");
        }
        frmMain1->AddFrame(hf);
        hf = new TGHorizontalFrame(frmMain1);
        {
            guidrawSimuCaloHits = new TGCheckButton(hf, "Draw Simulated Calo Hits");
            if (guidrawSimuCaloHits) guidrawSimuCaloHits->Toggle();
            hf->AddFrame(guidrawSimuCaloHits);
            guidrawSimuCaloHits->Connect("Toggled(Bool_t)", "DEventDisplay", fh, "guiOptions()");
        }
        frmMain1->AddFrame(hf);
        hf = new TGHorizontalFrame(frmMain1);
        {
            guidrawCaloHitsLego = new TGCheckButton(hf, "Draw Simulated CaloHits Lego");
            if (guidrawCaloHitsLego) guidrawCaloHitsLego->Toggle();
            hf->AddFrame(guidrawCaloHitsLego);
            guidrawCaloHitsLego->Connect("Toggled(Bool_t)", "DEventDisplay", fh, "guiOptions()");
        }
        frmMain1->AddFrame(hf);
    }// Draw Options

    // MC Tracks Options
    {
        hf = new TGHorizontalFrame(frmMain1);
        {
            lbl = new TGLabel(hf, "\n MC Tracks Options");
            hf->AddFrame(lbl);
        }
        frmMain1->AddFrame(hf);
        hf = new TGHorizontalFrame(frmMain1);
        {
            guiMC_Emin = new TGNumberEntry(hf, MC_Emin, 6, 999,
                                                          TGNumberFormat::kNESReal,
                                                          TGNumberFormat::kNEANonNegative,
                                                          TGNumberFormat::kNELLimitMin,
                                                          0.);
            hf->AddFrame(guiMC_Emin);
            guiMC_Emin->Connect("ValueSet(Long_t)", "DEventDisplay", fh, "guiOptions()");
            lbl = new TGLabel(hf, " min. E for MC Tracks [MeV]");
            hf->AddFrame(lbl);
        }
        frmMain1->AddFrame(hf);
        hf = new TGHorizontalFrame(frmMain1);
        {
            guiMC_PDG = new TGNumberEntry(hf, MC_PDG, 6, 999, TGNumberFormat::kNESInteger,
                                         TGNumberFormat::kNEANonNegative,
                                         TGNumberFormat::kNELLimitMin,
                                         0);
            hf->AddFrame(guiMC_PDG);
            guiMC_PDG->Connect("ValueSet(Long_t)", "DEventDisplay", fh, "guiOptions()");
            lbl = new TGLabel(hf, " Abs of PDG of MC Tracks");
            hf->AddFrame(lbl);
        }
        frmMain1->AddFrame(hf);
    }// MC Tracks Options

    // Calo Options
    {
        hf = new TGHorizontalFrame(frmMain1);
        {
            lbl = new TGLabel(hf, "\n Simulated Hits Options");
            hf->AddFrame(lbl);
        }
        frmMain1->AddFrame(hf);
        hf = new TGHorizontalFrame(frmMain1);
        {
            guiScaleFactorSimuTrkHits = new TGNumberEntry(hf, _scale_factor_SimuTrkHits, 6, 999,
                                                          TGNumberFormat::kNESReal,
                                                          TGNumberFormat::kNEANonNegative,
                                                          TGNumberFormat::kNELLimitMinMax,
                                                          1e-5, 1.);
            hf->AddFrame(guiScaleFactorSimuTrkHits);
            guiScaleFactorSimuTrkHits->Connect("ValueSet(Long_t)", "DEventDisplay", fh, "guiOptions()");
            lbl = new TGLabel(hf, " Display scale factor for Trk SimuHit");
            hf->AddFrame(lbl);
        }
        frmMain1->AddFrame(hf);
        hf = new TGHorizontalFrame(frmMain1);
        {
            guiR_min = new TGNumberEntry(hf, r_min, 6, 999, TGNumberFormat::kNESReal,
                                         TGNumberFormat::kNEANonNegative,
                                         TGNumberFormat::kNELLimitMinMax,
                                         0.0, 1.);
            hf->AddFrame(guiR_min);
            guiR_min->Connect("ValueSet(Long_t)", "DEventDisplay", fh, "guiOptions()");
            lbl = new TGLabel(hf, " min. E ratio for Calo Hits");
            hf->AddFrame(lbl);
        }
        frmMain1->AddFrame(hf);
        hf = new TGHorizontalFrame(frmMain1);
        {
            guiECAL_Emin = new TGNumberEntry(hf, ECAL_Emin, 6, 999, TGNumberFormat::kNESReal,
                                             TGNumberFormat::kNEANonNegative,
                                             TGNumberFormat::kNELLimitMinMax,
                                             0.0, 8000);
            hf->AddFrame(guiECAL_Emin);
            guiECAL_Emin->Connect("ValueSet(Long_t)", "DEventDisplay", fh, "guiOptions()");
            lbl = new TGLabel(hf, " min. ECAL Cell E [MeV]");
            hf->AddFrame(lbl);
        }
        frmMain1->AddFrame(hf);
        hf = new TGHorizontalFrame(frmMain1);
        {
            guiHCAL_Emin = new TGNumberEntry(hf, HCAL_Emin, 6, 999, TGNumberFormat::kNESReal,
                                             TGNumberFormat::kNEANonNegative,
                                             TGNumberFormat::kNELLimitMinMax,
                                             0.0, 8000);
            hf->AddFrame(guiHCAL_Emin);
            guiHCAL_Emin->Connect("ValueSet(Long_t)", "DEventDisplay", fh, "guiOptions()");
            lbl = new TGLabel(hf, " min. HCAL Cell E [MeV]");
            hf->AddFrame(lbl);
        }
        frmMain1->AddFrame(hf);
        hf = new TGHorizontalFrame(frmMain1);
        {
            guiScaleSimuCaloBox = new TGCheckButton(hf, "Scale Simulated CaloHits Box by Energy");
            //if (guiScaleSimuCaloBox) guiScaleSimuCaloBox->Toggle();
            hf->AddFrame(guiScaleSimuCaloBox);
            guiScaleSimuCaloBox->Connect("Toggled(Bool_t)", "DEventDisplay", fh, "guiOptions()");
        }
        frmMain1->AddFrame(hf);
        hf = new TGHorizontalFrame(frmMain1);
        {
            guiScaleFactorSimuCaloBox = new TGNumberEntry(hf, _scale_factor_SimuCaloHits, 6, 999, TGNumberFormat::kNESReal,
                                             TGNumberFormat::kNEANonNegative,
                                             TGNumberFormat::kNELLimitMinMax,
                                             0.0001, 1.0);
            hf->AddFrame(guiScaleFactorSimuCaloBox);
            guiScaleFactorSimuCaloBox->Connect("ValueSet(Long_t)", "DEventDisplay", fh, "guiOptions()");
            lbl = new TGLabel(hf, " Scale Factor for Calo Box");
            hf->AddFrame(lbl);
        }
        frmMain1->AddFrame(hf);
    }// Calo Options

    // CaloHit Display Options
    {
        hf = new TGHorizontalFrame(frmMain1);
        {
            lbl = new TGLabel(hf, "\n CaloHit Lego Options");
            hf->AddFrame(lbl);
        }
        frmMain1->AddFrame(hf);

        hf = new TGHorizontalFrame(frmMain1);
        {
            guiLogCaloHitsLego = new TGCheckButton(hf, "Log Scale");
            if (guiLogCaloHitsLego) guiLogCaloHitsLego->Toggle();
            hf->AddFrame(guiLogCaloHitsLego);
            guiLogCaloHitsLego->Connect("Toggled(Bool_t)", "DEventDisplay", fh, "guiOptions()");
        }
        frmMain1->AddFrame(hf);

        hf = new TGHorizontalFrame(frmMain1);
        {
            guiScaleFactorLego = new TGNumberEntry(hf, _scale_factor_Lego, 6, 999, TGNumberFormat::kNESReal,
                                                   TGNumberFormat::kNEANonNegative,
                                                   TGNumberFormat::kNELLimitMinMax,
                                                   0.01, 10.);
            hf->AddFrame(guiScaleFactorLego);
            guiScaleFactorLego->Connect("ValueSet(Long_t)", "DEventDisplay", fh, "guiOptions()");
            lbl = new TGLabel(hf, " Scale Factor of Height");
            hf->AddFrame(lbl);
        }
        frmMain1->AddFrame(hf);
    }// CaloHit Display Options


    frmMain1->MapSubwindows();
    frmMain1->Resize();
    frmMain1->MapWindow();

    browser->StopEmbedding();
    browser->SetTabTitle("DDis Control", 0);

}


[[maybe_unused]] void DEventDisplay::guiGoto() {
    Long_t n = guiEvent->GetNumberEntry()->GetIntNumber();
    //guiEvent->SetIntNumber(n);
    gotoEvent(n);
}

void DEventDisplay::gotoEvent(unsigned int id) {

    if (EvtReader->GetEntries() == 0 || id < 0)
        return;
    else if (id >= EvtReader->GetEntries())
        id = EvtReader->GetEntries() - 1;

    bool resetCam = true;

    if (id == (unsigned int) _eventID)
        resetCam = false;

    _eventID = (int) id;

    std::cout << "At event " << id << std::endl;
    // Clear Event
    if (gEve->GetCurrentEvent()) {
        gEve->GetCurrentEvent()->DestroyElements();
    }
    // Clear List Tree
    if (CaloDisplay->getLegoListTree())
        gEve->RemoveFromListTree(CaloDisplay->getCaloHitsDisplayList(), gEve->GetListTree(),
                                 CaloDisplay->getLegoListTree());

    // Redraw Event
    drawEvent(_eventID);

}

[[maybe_unused]] void DEventDisplay::guiOptions() {
    // Draw Options
    _drawDetector = guidrawDetector->IsOn();
    _drawSimuCaloHits = guidrawSimuCaloHits->IsOn();
    _drawMCTracks = guidrawMCTracks->IsOn();
    _drawSimuTrkHits = guidrawSimuTrkHits->IsOn();
    _drawSimuCaloLego = guidrawCaloHitsLego->IsOn();

    // MC Track Options
    MC_Emin = guiMC_Emin->GetNumberEntry()->GetNumber();
    MC_PDG = guiMC_PDG->GetNumberEntry()->GetNumber();

    // Calo Options
    r_min = guiR_min->GetNumberEntry()->GetNumber();
    ECAL_Emin = guiECAL_Emin->GetNumberEntry()->GetNumber();
    HCAL_Emin = guiHCAL_Emin->GetNumberEntry()->GetNumber();
    _scale_factor_SimuTrkHits = guiScaleFactorSimuTrkHits->GetNumberEntry()->GetNumber();
    _drawScaleSimuCaloBox =  guiScaleSimuCaloBox->IsOn();
    _scale_factor_SimuCaloHits = guiScaleFactorSimuCaloBox->GetNumberEntry()->GetNumber();

    // CaloHits Lego Options
    _drawLogSacle = guiLogCaloHitsLego->IsOn();
    _scale_factor_Lego = guiScaleFactorLego->GetNumberEntry()->GetNumber();

    // Ana Processors
    RecECAL_ = guiRecECAL->IsOn();
    RecECAL_W0 = guiRecECAL_W0->GetNumberEntry()->GetNumber();
    RecECAL_r_cut = guiRecECAL_r_cut->GetNumberEntry()->GetNumber();

    gotoEvent(_eventID);
}


void DEventDisplay::bookSlot() {
    // frames
    auto slot = TEveWindow::CreateWindowInTab(gEve->GetBrowser()->GetTabRight());
    auto packH = slot->MakePack();
    packH->SetElementName("CaloHits Details");
    packH->SetHorizontal();
    packH->SetShowTitleBar(kFALSE);

    slot = packH->NewSlot();
    auto pack0 = slot->MakePack();
    pack0->SetShowTitleBar(kFALSE);
    win_slots.push_back(pack0->NewSlot());
    win_slots.push_back(pack0->NewSlot());

    slot = packH->NewSlot();
    auto pack1 = slot->MakePack();
    pack1->SetShowTitleBar(kFALSE);
    win_slots.push_back(pack1->NewSlot());
    win_slots.push_back(pack1->NewSlot());

    for (unsigned i = 0; i < win_slots.size(); ++i) {
        win_v.push_back(new TEveViewer);
        win_s.push_back(new TEveScene);
        MakeViewerScene(win_slots.at(i), win_v.at(i), win_s.at(i));
    }

    gEve->GetBrowser()->GetTabLeft()->SetTab(1);

}