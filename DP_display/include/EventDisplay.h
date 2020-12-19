//
// Created by Zhang Yulei on 12/18/20.
//

#ifndef DSIMU_EVENTDISPLAY_H
#define DSIMU_EVENTDISPLAY_H

#include "TEveManager.h"
#include "TGeoManager.h"
#include "TApplication.h"
#include "TString.h"
#include "TNamed.h"

#include "TGeoNode.h"

enum Det_Type {DNone, DTarget, DTracker, DECAL, DHCAL};

class EventDisplay : public TNamed {
public:
    EventDisplay() = default;

    ~EventDisplay() override = default;

    // Initialize gApplication and gEve
    void Initialize();

    // Read Detector Geometry from root file
    bool readGeo(const TString& file_in);

    // Draw Detector Geometry
    bool drawDetector();

    // Open Application
    void makeGUI();
    void Open();

    /**************************/
    /* Detector Geometry Info */
    /**************************/
    void inspectMainRegion();
    void inspectSubRegion(int id, Det_Type dt);

private:
    // Core Manager from ROOT
    // gEve
    // gGeoManager
    // gApplication

    TGeoNode* world_node{nullptr};

    int _eventID{0};


ClassDefOverride(EventDisplay,100);
};


#endif //DSIMU_EVENTDISPLAY_H
