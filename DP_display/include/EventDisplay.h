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
#include "TFile.h"

#include "TEveViewer.h"
#include "TEveScene.h"
#include "TEveWindow.h"

#include "TGNumberEntry.h"
#include "TGButton.h"
#include "TGeoNode.h"
#include "TEveProjectionManager.h"
#include "TEveStraightLineSet.h"
#include "TEveTrack.h"
#include "TGeoVolume.h"
#include "TGeoBBox.h"
#include "TEveBox.h"

#include "TEveCaloData.h"
#include "TEveCaloLegoGL.h"

#include <TVector3.h>

#include <map>

#include "MultiView.h"
#include "Object/DEvent.h"
#include "EventReader_dis.h"
#include "DSMagneticField.h"
#include "CaloHitsDisplay.h"

typedef vector<TH2F *> CaloHitsVec;

class CaloHitsDisplay;

namespace {
    enum Det_Type {
        DNone, DTarget, DTracker, DECAL, DHCAL
    };

    std::map<int, Color_t> PDG_Color = {
            // Leptons
            {11,    kGreen},
            {-11,   kBlue},
            {12,    kCyan},
            {-12,   kCyan},
            {13,    kGreen - 10},
            {-13,   kGreen - 10},
            {14,    kCyan + 2},
            {-14,   kCyan + 2},
            {22,    kPink + 9},
            // Hadrons
            {2212,  kYellow},
            {-2212, kYellow}, // proton
            {2112,  kYellow - 7},
            {-2112, kYellow - 7}, // neutron
            {111,   kMagenta},
            {-111,  kMagenta}, // pion0
            {321,   kYellow + 2},
            {-321,  kYellow + 2}, // kaon
            {211,   kMagenta - 9},
            {-211,  kMagenta - 9}  // pion +-
    };

    std::map<double, Color_t> Energy_Color = {
            // Energy Color Represent [MeV]
            {0.001, kBlue},
            {0.005, kBlue - 4},
            {0.01,  kBlue - 7},
            {0.05,  kBlue - 9},
            {0.10,  kBlue - 10},
            {0.25,  kRed - 10},
            {0.50,  kRed - 7},
            {0.75,  kRed - 4},
            {1.00,  kRed},
    };

    Color_t FindColor(double E, double EMax) {
        double r = E / EMax;
        double prev = 1e-7;
        for (auto c_map : Energy_Color) {
            if (r < c_map.first && r >= prev)
                return c_map.second;
        }
        return kRed + 2;
    }
}
/*
 * Importance: Unit in TEve is [cm], [GeV]
 */
class EventDisplay : public TNamed {
public:
    EventDisplay() = default;

    ~EventDisplay() override = default;

    // Initialize gApplication and gEve
    void Initialize();

    // Read Data from root file
    bool readFile(const TString &file_in);

    bool readGeo(const TString &file_in = "");

    bool readEvt(const TString &file_in = "");

    // Draw Detector Geometry
    bool drawDetector();

    // Load Event
    bool readEntry(int i);

    bool drawEvent(int id, bool resCam = false);

    // Draw Class
    static void makeLines(TEveStraightLineSet *lineSet, const TVector3 &start, const TVector3 &end,
                          const Color_t &color, const Style_t &style, bool drawMarkers, double lineWidth,
                          int markerPos);

    TEveTrack *makeMCTrack(TEveTrackPropagator *trkProp, unsigned id, McParticle *mc);

    static TEveBox *makeCaloBox(SimulatedHit *hit, double EMax);
    static TEveBox *makeBox(const double *abs_pos, const double *half_size);

    template<class CaloCol>
    void makeCaloLego(CaloCol col, CaloHitsDisplay* calo_dis);

    static void MakeViewerScene(TEveWindowSlot* slot, TEveViewer*& v, TEveScene*& s);

    // Open Application
    void makeGUI(EventDisplay *);

    void Open(EventDisplay *);

    // Navigator && GUI commands
    void gotoEvent(unsigned int id);

    void guiGoto();

    void guiOptions();

    void bookSlot();

    /**************************/
    /* Detector Geometry Info */
    /**************************/
    void inspectMainRegion();

    void inspectSubRegion(int id, Det_Type dt);


private:
    TGNumberEntry *guiEvent{nullptr};
    TGNumberEntry *guiR_min{nullptr};
    TGNumberEntry *guiECAL_Emin{nullptr};
    TGNumberEntry *guiHCAL_Emin{nullptr};
    TGCheckButton *guidrawDetector{nullptr};
    TGCheckButton *guidrawHits{nullptr};
    TGCheckButton *guidrawTracks{nullptr};
    TGCheckButton *guiLogCaloHitsLego{nullptr};
    TGNumberEntry *guiScaleFactorLego{nullptr};


    // Core Manager from ROOT
    // gEve
    // gGeoManager
    // gApplication
    TFile *f{nullptr};

    TGeoNode *world_node{nullptr};

    MultiView *gMultiView{nullptr};

    EventReader_D *EvtReader{nullptr};

    int _eventID{0};

    DEvent *evt{nullptr};


    // Detector Geometry Info
    // size is half, length = 2*size [cm]
    double ECAL_Z_Move{0.};
    TVector3 ECAL_Size{TVector3(-1, -1, -1)};
    TVector3 ECAL_Cell_Size{TVector3(-1, -1, -1)};
    TVector3 ECAL_Cell_Arr{TVector3(-1, -1, -1)};

    // Draw Options
    bool _drawDetector = true;
    bool _drawHits = true;
    bool _drawTracks = true;
    bool _drawCaloHist = true;

    // Calo Options
    double r_min = 0.;
    double ECAL_Emin = 0.;
    double HCAL_Emin = 0.;

    // CaloHits Lego Options
    bool ECALslice_calo = false; // if to slice ECAL hits w.r.t Z layer
    bool _drawECAL_calo = true;
    bool _drawHCAL_calo = false;
    bool _drawLogSacle = true;
    double _scale_factor = 1.0;

    // Window Slots
    vector<TEveWindowSlot* > win_slots;
    vector<TEveViewer* > win_v;
    vector<TEveScene* > win_s;

ClassDefOverride(EventDisplay, 0);
};

#endif //DSIMU_EVENTDISPLAY_H
