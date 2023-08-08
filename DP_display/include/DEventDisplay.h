//
// Created by Zhang Yulei on 12/18/20.
//

#ifndef DSIMU_DEVENTDISPLAY_H
#define DSIMU_DEVENTDISPLAY_H

#include "DisData.h"

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

#include "Object/DEvent.h"
#include "Event/AnaEvent.h"
#include "Event/EventReader.h"
//#include "DEventReader_dis.h"
#include "DSMagneticField.h"
#include "CaloHitsDisplay.h"
#include "TDatabasePDG.h"

class CaloHitsDisplay;

/*
 * Importance: Unit in TEve is [cm], [GeV]
 */

/// \brief
class DEventDisplay : public TNamed {
public:

    enum Det_Type {
        DNone, DTarget, DTracker, DECAL, DHCAL, DSideHCAL
    };

    static inline std::map<int, Color_t> PDG_Color{
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
            {2112,  kOrange + 7},
            {-2112, kOrange - 7}, // neutron
            {111,   kMagenta},
            {-111,  kMagenta}, // pion0
            {321,   kViolet - 9},
            {-321,  kViolet - 8}, // kaon
            {211,   kMagenta - 9},
            {-211,  kMagenta - 9}  // pion +-
    };

    static inline std::map<double, Color_t> Energy_Color{
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

    [[nodiscard]] static Color_t FindColor(double E, double EMax) {
        double r = E / EMax;
        double prev = 1e-7;
        for (auto c_map : Energy_Color) {
            if (r < c_map.first && r >= prev)
                return c_map.second;
        }
        return kRed + 2;
    }

    DEventDisplay() {  EvtReader = shared_ptr<EventReader>(new EventReader()); };

    ~DEventDisplay() override = default;

    // Initialize gApplication and gEve
    void Initialize();

    // Read Data from root file
    bool readFile(const TString &file_in);

    //bool readGeo(const TString &file_in = "");
    bool readGeo(TFile* geo_file);

    bool readEvt(const TString &file_in = "");

    TFile* getGeo() {return EvtReader->getDataFile();}

    // Draw Detector Geometry
    bool drawDetector();

    /* Draw Event Components */
    void drawInitialParticleStep();

    void drawMCParticles();

    void drawSimuTrkHits(TEveElementList *SimHitsList);

    void drawSimuCaloHits(TEveElementList *SimHitsList);

    // Load Event
    bool readEntry(int i);

    bool drawEvent(int id, bool resCam = false);

    // Draw Utility Class
    static void makeLines(TEveStraightLineSet *lineSet, const TVector3 &start, const TVector3 &end,
                          const Color_t &color, const Style_t &style, bool drawMarkers, double lineWidth,
                          int markerPos);

    static TEveTrack *makeMCTrack(TEveTrackPropagator *trkProp, unsigned id, McParticle *mc, std::shared_ptr<DStepVec> steps=nullptr);
    int recursiveFindTracks(int mother, MCParticleVec* MCs, int* count, std::string path, std::shared_ptr<std::map<int,std::string>> sto=nullptr);
    std::string easyPDG(int pdg);
    bool isInitialMC(McParticle* mc);

    template<class Hit>
    static TEveBox *makeTrackerBox(Hit *hit, double scale);

    // I dont know why not to make it template function
    // get Error if not
    TEveBox *makeSimuCaloBox(SimulatedHit *hit, double EMax) const;

    static TEveBox *makeRecCaloBox(CalorimeterHit *hit, double EMax, int copyNo=-1);

    static TEveBox *makeBox(const double *abs_pos, const double *half_size);

    static TEveBox *makeRotBox(const double *abs_pos, const double *half_size, const TRotation& rot);

    template<class CaloCol>
    void makeCaloLego(CaloCol col, CaloHitsDisplay *calo_dis, bool if_draw_lego = true);

    static void MakeViewerScene(TEveWindowSlot *slot, TEveViewer *&v, TEveScene *&s);

    // Ana Processors
    void RunAnaProcessors();

    // Open Application
    void makeGUIRaw(DEventDisplay *fh);

    void makeGUIProcessor(DEventDisplay *fh);

    void Open(DEventDisplay *);

    // Navigator && GUI commands
    void gotoEvent(unsigned int id);

    [[maybe_unused]] void guiGoto();

    [[maybe_unused]] void guiOptions();

    [[maybe_unused]] void guiOptionsAna();

    void bookSlot();

    /**************************/
    /* Detector Geometry Info */
    /**************************/
    void inspectMainRegion();

    void inspectSubRegion(int id, Det_Type dt);

    bool loadAnaConfig(TString &in){
        anaConfig=in;
        return anaConfig=="";
    };
    void setNoStripMode(bool no_strip = true) {
        if (no_strip) {
            _scale_factor_SimuTrkHits = 0.01;
        }
    }


private:
    // Common GUI
    TGNumberEntry *guiEvent{nullptr};
    TGNumberEntry *guiR_min{nullptr};
    TGNumberEntry *guiECAL_Emin{nullptr};
    TGNumberEntry *guiHCAL_Emin{nullptr};
    TGCheckButton *guidrawDetector{nullptr};
    TGCheckButton *guidrawSimuCaloHits{nullptr};
    TGCheckButton *guidrawSimuTrkHits{nullptr};
    TGCheckButton *guidrawMCTracks{nullptr};
    TGCheckButton *guidrawCaloHitsLego{nullptr};
    TGCheckButton *guiLogCaloHitsLego{nullptr};
    TGNumberEntry *guiScaleFactorLego{nullptr};
    TGNumberEntry *guiScaleFactorSimuTrkHits{nullptr};
    TGNumberEntry *guiScaleFactorSimuCaloBox{nullptr};
    TGCheckButton *guiScaleSimuCaloBox{nullptr};

    // MC Tracker GUI
    TGNumberEntry *guiMC_Emin{nullptr};
    TGNumberEntry *guiMC_PDG{nullptr};


    // Core Manager from ROOT
    // gEve
    // gGeoManager
    // gApplication

    TFile *f{nullptr};

    shared_ptr<TGeoNode> world_node;

    shared_ptr<EventReader> EvtReader;

    int _eventID{0};

    AnaEvent* evt{};

    // Detector Geometry Info
    // size is half, length = 2*size [cm]
    double ECAL_Z_Move{0.};
    TVector3 ECAL_Size{TVector3(-1, -1, -1)};
    TVector3 ECAL_Cell_Size{TVector3(-1, -1, -1)};
    TVector3 ECAL_Cell_Arr{TVector3(-1, -1, -1)};

    // Draw Options
    bool _drawDetector = true;
    bool _drawSimuCaloHits = true;
    bool _drawSimuTrkHits = true;
    bool _drawMCTracks = true;
    bool _drawScaleSimuCaloBox = false;
    bool _drawSimuCaloLego = false;

    // MC Tracks Option
    double MC_Emin = 1; // MeV
    int MC_PDG = 0;
    // steps saved for plotting
    std::map<int,std::shared_ptr<DStepVec>> MCSteps;

    // Calo Options
    double r_min = 0.01;
    double ECAL_Emin = 1; // MeV
    double HCAL_Emin = 0.;
    double Trk_Emin = 0.; // track hit (e.g. strip)
    double _scale_factor_SimuTrkHits = 1.0;
    double _scale_factor_SimuCaloHits = 1.0;
    bool _build_Tracker_BField = false;

    // CaloHits Lego Options
    CaloHitsDisplay *CaloDisplay{nullptr};
    bool ECALslice_calo = false; // if to slice ECAL hits w.r.t Z layer
    bool _drawECAL_calo = true;
    bool _drawHCAL_calo = false;
    bool _drawLogSacle = true;
    double _scale_factor_Lego = 1.0;

    // Window Slots
    vector<TEveWindowSlot *> win_slots;
    vector<TEveViewer *> win_v;
    vector<TEveScene *> win_s;

    /***************************/
    /* Ana Processor Variables */
    /***************************/
    // Format:
    //   ProcessorName_Variable
    TString anaConfig;
    bool RunAna_{true};
    TGCheckButton* guiRunAna{nullptr};

    double Enoise{1};
    double EThres_S{4};
    double EThres_N{2};
    double EThres_P{0};
    double Critical_E{500};
    double Critical_N{4};
    double EM_ENERGY_SCALE_MeV{1};
    double ENERGY_SHIFT_MeV{0};
    int weight_type{0};
    double EM_SCALE_LENGTH_mm{50};

    TGNumberEntry* gui_Enoise{nullptr};
    TGNumberEntry* gui_EThres_S{nullptr};
    TGNumberEntry* gui_EThres_N{nullptr};
    TGNumberEntry* gui_EThres_P{nullptr};
    TGNumberEntry* gui_Critical_E{nullptr};
    TGNumberEntry* gui_Critical_N{nullptr};
    TGNumberEntry* gui_EM_ENERGY_SCALE_MeV{nullptr};
    TGNumberEntry* gui_ENERGY_SHIFT_MeV{nullptr};
    TGNumberEntry* gui_weight_type{nullptr};
    TGNumberEntry* gui_EM_SCALE_LENGTH_mm{nullptr};

    bool AnaDisClusLeadingOnly{false};
    bool AnaDisCellMax{true};
    bool AnaDisCellSp1{true};
    bool AnaDisCellSp2{true};
    bool AnaDisCellPri{true};
    bool AnaDisCellSub{true};
    TGCheckButton* gui_AnaDisClusLeadingOnly{nullptr};
    TGCheckButton* gui_AnaDisCellMax{nullptr};
    TGCheckButton* gui_AnaDisCellSp1{nullptr};
    TGCheckButton* gui_AnaDisCellSp2{nullptr};
    TGCheckButton* gui_AnaDisCellPri{nullptr};
    TGCheckButton* gui_AnaDisCellSub{nullptr};

    ClassDefOverride(DEventDisplay, 0);
};


#endif //DSIMU_DEVENTDISPLAY_H
