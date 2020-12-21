//
// Created by Zhang Yulei on 12/18/20.
//

#include "EventDisplay.h"

#include "TFile.h"
#include <TApplication.h>
#include <TEveBrowser.h>
#include <TEveManager.h>
#include <TEveEventManager.h>
#include <TEveGeoNode.h>
#include "TGLViewer.h"
#include "TGPicture.h"
#include "TGLabel.h"
#include "TGButton.h"

#include <TGeoManager.h>
#include <TGeoMatrix.h>
#include <TGeoNode.h>
#include <TGeoTube.h>
#include <TROOT.h>
#include <TSystem.h>

#include <iomanip>
#include <iostream>

void EventDisplay::Initialize() {
    // Need to initialize gApplication and gEve first
    if ((!gApplication) || (gApplication && gApplication->TestBit(TApplication::kDefaultApplication))) {
        std::cout << "In EventDisplay ctor: gApplication not found, creating..." << std::flush;
        new TApplication("ROOT_application", nullptr, nullptr);
        std::cout << "done!" << std::endl;
    }
    if (!gEve) {
        std::cout << "In EventDisplay ctor: gEve not found, creating..." << std::flush;
        TEveManager::Create();
        std::cout << "done!" << std::endl;
    }

    // Set Camera
    auto *v = gEve->GetDefaultGLViewer();
    v->CurrentCamera().SetExternalCenter(kTRUE);
    v->CurrentCamera().SetCenterVec(0,0,0);

    _eventID = 0;

    EvtReader = new EventReader_D();
    evt = new DEvent();
    EvtReader->setEvt(evt);
}

bool EventDisplay::drawDetector() {
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
            volume->SetLineColor(2);
            volume->SetTransparency(10);
        } else if (name.Contains("TagTrk")) {
            volume->SetLineColor(30);
            volume->SetTransparency(55);
        } else if (name.Contains("RecTrk")) {
            volume->SetLineColor(32);
            volume->SetTransparency(55);
        } else if (name == "ECAL") {
            volume->SetLineColor(38);
            volume->SetTransparency(75);
            volume->SetVisibility(kTRUE);
        } else if (name == "HCAL") {
            volume->SetLineColor(39);
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


bool EventDisplay::readFile(const TString &file_in) {
    // Read Geometry from ROOT file
    f = new TFile(file_in);
    if (!f) {
        std::cerr << "[Display] ==> file: " << file_in << " not existed or broken..." << std::endl;
        return false;
    }
    return true;
}

bool EventDisplay::readGeo(const TString &file_in) {
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

    world_node = dynamic_cast<TGeoNode *>(gGeoManager->GetListOfNodes()->At(0));

    return true;
}


bool EventDisplay::readEvt(const TString &file_in) {
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


bool EventDisplay::readEntry(int i) {
    // unused right now
    if (i < 0) return false;
    EvtReader->ReadEntry(i);
    return true;
}


void EventDisplay::Open(EventDisplay* evtDis) {
    makeGUI(evtDis);

    gEve->Redraw3D(kTRUE);
    gApplication->Run(kTRUE);
}

void EventDisplay::makeGUI(EventDisplay* fh) {
    TEveBrowser *browser = gEve->GetBrowser();

    TGLabel* lbl = nullptr;
    TGTextButton* tb = nullptr;

    browser->StartEmbedding(TRootBrowser::kLeft);
    auto * frmMain1 = new TGMainFrame(gClient->GetRoot(), 1000, 600);
    frmMain1->SetWindowName("XX GUI");
    frmMain1->SetCleanup(kDeepCleanup);
    TGHorizontalFrame* hf = nullptr;
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
            guiEvent->Connect("ValueSet(Long_t)", "EventDisplay", fh, "guiGoto()");

            // redraw button
            tb = new TGTextButton(hf, "Redraw Event");
            hf->AddFrame(tb);
            tb->Connect("Clicked()", "EventDisplay", fh, "guiGoto()");
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
    }
    hf = new TGHorizontalFrame(frmMain1); {
        guidrawDetector =  new TGCheckButton(hf, "Draw Detectors");
        if(guidrawDetector) guidrawDetector->Toggle();
        hf->AddFrame(guidrawDetector);
        guidrawDetector->Connect("Toggled(Bool_t)", "EventDisplay", fh, "guiOptions()");
    }
    frmMain1->AddFrame(hf);
    hf = new TGHorizontalFrame(frmMain1); {
        guidrawHits =  new TGCheckButton(hf, "Draw Calo Hits");
        if(guidrawHits) guidrawHits->Toggle();
        hf->AddFrame(guidrawHits);
        guidrawHits->Connect("Toggled(Bool_t)", "EventDisplay", fh, "guiOptions()");
    }
    frmMain1->AddFrame(hf);
    hf = new TGHorizontalFrame(frmMain1); {
        guidrawTracks =  new TGCheckButton(hf, "Draw MC Tracks");
        if(guidrawTracks) guidrawTracks->Toggle();
        hf->AddFrame(guidrawTracks);
        guidrawTracks->Connect("Toggled(Bool_t)", "EventDisplay", fh, "guiOptions()");
    }
    frmMain1->AddFrame(hf);
    // Calo Options
    {
        hf = new TGHorizontalFrame(frmMain1);
        {
            lbl = new TGLabel(hf, "\n Calo Hits Options");
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
            guiR_min->Connect("ValueSet(Long_t)", "EventDisplay", fh, "guiOptions()");
            lbl = new TGLabel(hf, " min. E ratio");
            hf->AddFrame(lbl);
        }
        frmMain1->AddFrame(hf);
        hf = new TGHorizontalFrame(frmMain1);
        {
            guiECAL_Emin = new TGNumberEntry(hf, r_min, 6, 999, TGNumberFormat::kNESReal,
                                             TGNumberFormat::kNEANonNegative,
                                             TGNumberFormat::kNELLimitMinMax,
                                             0.0, 8000);
            hf->AddFrame(guiECAL_Emin);
            guiECAL_Emin->Connect("ValueSet(Long_t)", "EventDisplay", fh, "guiOptions()");
            lbl = new TGLabel(hf, " min. ECAL Cell E [MeV]");
            hf->AddFrame(lbl);
        }
        frmMain1->AddFrame(hf);
        hf = new TGHorizontalFrame(frmMain1);
        {
            guiHCAL_Emin = new TGNumberEntry(hf, r_min, 6, 999, TGNumberFormat::kNESReal,
                                             TGNumberFormat::kNEANonNegative,
                                             TGNumberFormat::kNELLimitMinMax,
                                             0.0, 8000);
            hf->AddFrame(guiHCAL_Emin);
            guiHCAL_Emin->Connect("ValueSet(Long_t)", "EventDisplay", fh, "guiOptions()");
            lbl = new TGLabel(hf, " min. HCAL Cell E [MeV]");
            hf->AddFrame(lbl);
        }
        frmMain1->AddFrame(hf);
    }
    frmMain1->MapSubwindows();
    frmMain1->Resize();
    frmMain1->MapWindow();

    browser->StopEmbedding();
    browser->SetTabTitle("DDis Control", 0);
}


void EventDisplay::guiGoto(){
    Long_t n = guiEvent->GetNumberEntry()->GetIntNumber();
    //guiEvent->SetIntNumber(n);
    gotoEvent(n);
}

void EventDisplay::gotoEvent(unsigned int id) {

    if (EvtReader->GetEntries() == 0)
        return;
    else if(id >= EvtReader->GetEntries())
        id = EvtReader->GetEntries() - 1;

    bool resetCam = true;

    if (id == (unsigned int)_eventID)
        resetCam = false;

    _eventID = (int)id;

    std::cout << "At event " << id << std::endl;
    if (gEve->GetCurrentEvent()) {
        gEve->GetCurrentEvent()->DestroyElements();
    }
    drawEvent(_eventID);

}

void EventDisplay::guiOptions() {
    // Draw Options
    _drawDetector = guidrawDetector->IsOn();
    _drawHits = guidrawHits->IsOn();
    _drawTracks = guidrawTracks->IsOn();

    // Calo Options
    r_min = guiR_min->GetNumberEntry()->GetNumber();
    ECAL_Emin = guiECAL_Emin->GetNumberEntry()->GetNumber();
    HCAL_Emin = guiHCAL_Emin->GetNumberEntry()->GetNumber();


    gotoEvent(_eventID);
}