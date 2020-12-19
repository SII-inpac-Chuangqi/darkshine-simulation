//
// Created by Zhang Yulei on 12/18/20.
//

#include "EventDisplay.h"

#include "TFile.h"
#include "TEveGeoNode.h"
#include "TGeoShape.h"
#include "TGeoBBox.h"

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

    _eventID = 0;
}


bool EventDisplay::drawDetector() {
    // draw the geometry, does not really work yet. If it's fixed, the docu in the header file should be changed.
    TGeoNode *top_node = gGeoManager->GetTopNode();
    assert(top_node != nullptr);

    //Set transparency & color of geometry
//    TObjArray* volumes = gGeoManager->GetListOfVolumes();
//    for(int i = 0; i < volumes->GetEntriesFast(); i++) {
//        auto* volume = dynamic_cast<TGeoVolume*>(volumes->At(i));
//        assert(volume != nullptr);
//        std::cout<<"Name: "<< volume->GetName()<<" "<<volume->GetCurrentNodeIndex () <<std::endl;
//        volume->SetLineColor(12);
//        volume->SetTransparency(50);
//    }


    auto *eve_top_node = new TEveGeoTopNode(gGeoManager, top_node);
    eve_top_node->IncDenyDestroy();
    gEve->AddElement(eve_top_node);

    return false;
}

bool EventDisplay::readGeo(const TString &file_in) {
    auto file = std::shared_ptr<TFile>(new TFile(file_in));
    if (!file) {
        std::cerr << "[Display] ==> file: " << file_in << " not existed or broken..." << std::endl;
        return false;
    }
    gGeoManager = (TGeoManager *) file->Get("DetGeoManager");
    if (!gGeoManager) {
        std::cerr << "[Display] ==> No Geometry in the file..." << std::endl;
        return false;
    }

    world_node = dynamic_cast<TGeoNode *>(gGeoManager->GetListOfNodes()->At(0));

    return true;
}

void EventDisplay::Open() {
    makeGUI();
    gApplication->Run(kTRUE);
}

void EventDisplay::makeGUI() {
    TEveBrowser *browser = gEve->GetBrowser();

}
