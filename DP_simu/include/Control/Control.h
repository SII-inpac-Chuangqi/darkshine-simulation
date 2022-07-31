//
// Created by Zhang Yulei on 12/31/20.
//
#ifndef DSIMU_CONTROL_H
#define DSIMU_CONTROL_H

// Geant4 Class
#include <G4ThreeVector.hh>
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4OpticalSurface.hh"
#include "G4UImanager.hh"

// yaml-cpp
#include "yaml-cpp/yaml.h"

#include <vector>
#include <tuple>
#include <map>

#include "Object/DigiForm.hh"
#include "Object/DDetectorIDMaps.h"

/// \brief The central control class which should store all the data

class Control {
public:
    Control(const Control &) = delete;

    Control &operator=(Control const &) = delete;

    static Control *CreateInstance();

    // build geant4 material table
    void ConstructG4MaterialTable() const;

    // Assign Materials using GetMaterial, and define OpticalSurface
    void AssignG4Material();

    // Rebuild all dependent variables
    void RebuildVariables();

    // Read data from yaml
    bool ReadYAML(const G4String &file_in);

    void ReadAndSetVerbosity();

    void ReadAndSetGPS();

    void ReadAndSetRandomSeed();

public:
    /*************************************/
    /*  Define all the variables needed  */
    /*************************************/

    G4String DSimu_version = "1.0.0"; // DSimu version

    //========================================
    /* Geant4 settings */
    //----------------------------------------
    int BeamOnNumber = 0;
    long random_seed = 0;
    G4String random_restore_file = "non";
    //========================================
    /* Global Variables */
    //----------------------------------------
    bool save_geometry; // save the geometry in root
    bool check_overlaps; // check the geometry overlap, may be very slow and verbose
    bool signal_production; // production of signal Dark Photon Process

    //----------------------------------------
    // Signal Option
    double signal_mass;
    G4String signal_lookup_table;
    bool signal_use_LUT{};
    bool visible_decay;
    G4String dp_decay_channel;
    double dp_eplsion;

    //----------------------------------------
    // Magnetic field
    G4String mag_field_input; // file name of magnetic field
    G4bool uniform_mag_field;
    G4ThreeVector tag_Tracker_MagField;
    G4ThreeVector rec_Tracker_MagField;
    G4int mag_verbose;


    //----------------------------------------
    // Root Manager Options
    G4String outfile_Name;
    G4String tree_Name;

    /* event_id = id + Run_Number * Total_Event_Number */
    int Run_Number; // run number
    int Total_Event_Number; // event number

    //----------------------------------------
    // Out Collection Options
    bool save_all_mcp; // save all the mc particles
    bool save_MC;
    bool save_initial_particle_step;
    bool save_mcp_helper;

    G4String RawMCCollection_Name;
    G4String InitialParticleStepCollection_Name;
    G4String MCPHelperCollection_Name;

    //----------------------------------------
    // For Memory Leak
    bool Memory_Check;

    //========================================
    /* Geometry */
    //----------------------------------------
    static constexpr double eps = 10 * um;

    //----------------------------------------
    // Build Options
    bool build_target;
    bool build_MagnetShield;
    bool build_tag_tracker;
    bool build_rec_tracker;
    bool build_ECAL;
    bool build_HCAL;
    bool build_SideHCAL;

    bool build_only_target;
    bool build_only_tag_tracker;
    bool build_only_rec_tracker;
    bool build_only_ECAL;
    bool build_only_HCAL;


    //----------------------------------------
    // Material Stirng
    std::map<G4String, G4String> MaterialStr{};

    //----------------------------------------
    // World
    G4Material *World_Mat{};
    G4ThreeVector Size_World;  // dependent

    //----------------------------------------
    // Magnet Shield
    G4double MagnetShield_Thickness{};

    //----------------------------------------
    // Target
    G4Material *Target_Mat;
    G4ThreeVector Target_Size;
    G4ThreeVector Target_Pos;

    //----------------------------------------
    // Tracker
    bool build_silicon_micro_strip;
    double Trk_Tar_Dis;
    G4Material *Tracker_Mat;
    G4Material *TrackerRegion_Mat;
    G4ThreeVector Tracker1_Color;
    G4ThreeVector Tracker2_Color;

    // Tagging Tracker
    std::vector<G4ThreeVector> tag_Size_Tracker{};
    std::vector<G4ThreeVector> tag_Pos_Tracker{};
    std::vector<G4ThreeVector> tag_Tracker_Angle_Gap{};
    std::vector<G4int> tag_Tracker_Strip_N{};
    G4int tag_Tracker_Strip_Block_N{};
    int tag_No_Tracker{};  // dependent
    G4ThreeVector tag_Size_TrackerRegion;  // dependent
    G4ThreeVector tag_Pos_TrackerRegion;  // dependent

    // Recoil Tracker
    std::vector<G4ThreeVector> rec_Size_Tracker{};
    std::vector<G4ThreeVector> rec_Pos_Tracker{};
    std::vector<G4ThreeVector> rec_Tracker_Angle_Gap{};
    std::vector<G4int> rec_Tracker_Strip_N{};
    G4int rec_Tracker_Strip_Block_N{};
    int rec_No_Tracker{};  // dependent
    G4ThreeVector rec_Size_TrackerRegion;  // dependent
    G4ThreeVector rec_Pos_TrackerRegion;  // dependent

    //----------------------------------------
    // Electromagnetic Calorimeter
    G4String ECAL_Name;

    G4Material *ECALRegion_Mat{};
    G4Material *ECAL_Center_Mat{};
    G4Material *ECAL_Wrap_Mat{};
    G4ThreeVector Size_ECALCell; // dependent
    G4ThreeVector Size_ECALBlock; // dependent
    G4ThreeVector Size_ECALRegion;  // dependent
    G4ThreeVector Pos_ECALRegion;  // dependent
    G4ThreeVector ECAL_Center_Wrap_Size;
    G4ThreeVector ECAL_Center_Size;
    G4ThreeVector ECAL_Block_No;
    G4ThreeVector ECAL_Cell_No;
    G4ThreeVector ECAL_Center_Module_No; // dependent
    G4ThreeVector ECAL_Cell_Gap;

    //----------------------------------------
    // Hadronic Calorimeter
    G4String HCAL_Name;

    G4Material *HCALRegion_Mat{};
    G4Material *HCAL_Mat{};
    G4Material *HCAL_Wrap_Mat{};
    G4Material *HCAL_Absorber_Mat{};
    G4Material *HCAL_FiberClad_Mat{};
    G4Material *HCAL_Fiber_Mat{};
    G4ThreeVector Size_HCALCell; // dependent
    G4ThreeVector Size_HCALModule; // dependent
    G4ThreeVector Size_HCALLayer; // dependent
    G4double HCAL_Absorber_Thickness_Total; // dependent
    G4ThreeVector Size_HCALRegion;  // dependent
    G4ThreeVector Pos_HCALRegion;  // dependent
    G4ThreeVector HCAL_Wrap_Size;
    G4ThreeVector HCAL_Size_Dir;
    G4ThreeVector HCAL_XYUnit_Group_No;
    G4ThreeVector HCAL_Absorber_Z_No;
    G4ThreeVector HCAL_Mod_No_Dir; // dependent
    G4ThreeVector HCAL_Module_No;
    G4int HCAL_Cell_XY_N;
    G4int HCAL_Layer_N; // dependent
    std::vector<std::tuple<int, int, double> > HCAL_Absorber_Thickness_List;
    G4ThreeVector HCAL_Module_Gap;
    G4double HCAL_Absorber_Thickness;
    G4bool HCAL_Show_Cell{false};


    //----------------------------------------
    // Side HCAL
    G4String SideHCAL_Name;
    G4ThreeVector Pos_SideHCALRegion_0;
    std::vector<G4ThreeVector> Pos_SideHCALRegion; // dependent
    G4ThreeVector Size_SideHCALRegion;
    G4ThreeVector SideHCAL_Size_Dir;
    std::vector<std::tuple<int, int, double> > SideHCAL_Absorber_Thickness_List;
    G4double SideHCAL_Absorber_Thickness_Total;
    G4int SideHCAL_Layer_N; // dependent
    G4ThreeVector Size_SideHCALCell; // dependent

    //========================================
    /* Biasing */
    //----------------------------------------
    // if to open bias
    bool if_bias;
    bool if_bias_target;
    bool if_bias_ECAL;


    G4String BiasProcess;
    double BiasFactor;
    double BiasEmin;

    //========================================
    /* Filters */
    //----------------------------------------
    bool if_filter;
    bool if_HardBrem;

    std::vector<std::tuple<int, double, double, double, double, bool, bool, bool> > particle_filters_parameters;
    std::vector<std::tuple<G4String, double, double, double, double, bool, bool, bool> > process_filters_parameters;

    G4int fStage{};
    //========================================
    /* Optical */
    //----------------------------------------
    // if to open optical process
    bool if_optical = true;
    bool Optical_UseLUT;
    double Optical_YieldFactor;
    double Optical_HCAL_Yield;
    int Optical_PhysicsVerbose = 0;
    //LUT loader
    G4String LUT_FilePath;
    G4String LUT_Name;

    std::map<G4String, std::pair<G4String, G4String>> Optical_GetLUTDefinations();

    DigiScheme Optical_GetDigiScheme(const G4String &cIn);

    DetUnitType Optical_GetDetType(const G4String &cIn);

    //Digitizer
    bool Optical_digitizerDebug;
    bool Optical_usePositivePolarity;
    bool Optical_addClockJitter;
    bool Optical_injectNoise;
    double Optical_sampleInterval;
    double Optical_maxTime;
    int Optical_nPixels;
    double Optical_pixelRecoveryTau;
    double Optical_pixelRecoveryCutoff;
    G4String Optical_pulseFilePath;
    G4String Optical_splineName;
    double Optical_pulseTimeZero;
    int Optical_nBits;
    double Optical_fullRangeMV;

    double Optical_voltageToADC() { return Optical_fullRangeMV / Optical_nBits; };
    // double Optical_voltageToADC;
    int Optical_range_min;
    int Optical_range_max;
    int Optical_pedestalLevel;
    int Optical_noiseSigma;
    double Optical_clockJitterSigma;
    double Optical_apertureJitterSigma;
    double Optical_pulseScaleFactor;
    double HCAL_CaloHoleRadius;
    double HCAL_FiberRadius;

    //----------------------------------------
    // Wrap related
    G4MaterialPropertiesTable *Wrap_Surface_Mat;
    G4OpticalSurface *Wrap_Surface;
    //----------------------------------------
    // APD related
    G4Material *APD_Mat;
    G4ThreeVector ECAL_APD_Size;
    G4ThreeVector HCAL_APD_Size;

    G4Material *Glue_Mat;
    G4ThreeVector Glue_Size;
    G4MaterialPropertiesTable *APD_Surface_Mat;
    G4OpticalSurface *APD_Surface;
    //----------------------------------------
    // Optical Fiber related
    int Optical_polyPMMA = 1;

    //========================================
    /* Truth Analysis Parameters */
    //----------------------------------------

    /** For Truth Particle */
    double E_kin_min_record = 0;
    double E_kin_min_step = 0;
    double E_leak_min = 0;
    double E_remain_min = 0;
    /** For Truth Process */
    double E_min_process = 0.;
    double E_process_ratio = 0.;

private:
    Control();

    YAML::Node Node;

    // for geant4 internal settings
    G4UImanager *UIManager;

    static G4ThreeVector readV3(const YAML::Node &n, bool unit = false);

    static double readV2(const YAML::Node &n);
};

extern Control *dControl;
#endif //DSIMU_CONTROL_H
