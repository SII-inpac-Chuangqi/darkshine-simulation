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

// yaml-cpp
#include "yaml-cpp/yaml.h"

#include <vector>
#include <tuple>

#include "Object/DigiForm.hh"

/// \brief The central control class which should store all the data

class Control {
public:
    Control(const Control &) = delete;

    Control &operator=(Control const &) = delete;

    static Control *CreateInstance();

    // build geant4 material table
    void ConstructG4MaterialTable() const;

    // Rebuild all dependent variables
    void RebuildVariables();

    // Read data from yaml
    bool ReadYAML(const G4String& file_in);

public:
    /*************************************/
    /*  Define all the variables needed  */
    /*************************************/

    //========================================
    /* Global Variables */
    //----------------------------------------
    bool save_geometry; // save the geometry in root
    bool check_overlaps; // check the geometry overlap, may be very slow and verbose
    bool signal_production; // production of signal Dark Photon Process

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

    G4String RawMCCollection_Name;
    G4String InitialParticleStepCollection_Name;

    //========================================
    /* Geometry */
    //----------------------------------------
    double eps = 1 * um;

    //----------------------------------------
    // Build Options
    bool build_target;
    bool build_tag_tracker;
    bool build_rec_tracker;
    bool build_ECAL;
    bool build_HCAL;

    bool build_only_target;
    bool build_only_tag_tracker;
    bool build_only_rec_tracker;
    bool build_only_ECAL;
    bool build_only_HCAL;
    //----------------------------------------
    // World
    G4Material *World_Mat{};
    G4ThreeVector Size_World;  // dependent

    //----------------------------------------
    // Target
    G4Material *Target_Mat;
    G4ThreeVector Target_Size;
    G4ThreeVector Target_Pos;

    //----------------------------------------
    // Tracker
    double Trk_Tar_Dis;
    G4Material *Tracker_Mat;
    G4Material *TrackerRegion_Mat;
    double Tracker1_Rotation;
    double Tracker2_Rotation;
    G4ThreeVector Tracker1_Color;
    G4ThreeVector Tracker2_Color;

    // Tagging Tracker
    std::vector<G4ThreeVector> tag_Size_Tracker{};
    std::vector<G4ThreeVector> tag_Pos_Tracker{};
    int tag_No_Tracker{};  // dependent
    G4ThreeVector tag_Size_TrackerRegion;  // dependent
    G4ThreeVector tag_Pos_TrackerRegion;  // dependent
    G4ThreeVector tag_Tracker_MagField;

    // Recoil Tracker
    std::vector<G4ThreeVector> rec_Size_Tracker{};
    std::vector<G4ThreeVector> rec_Pos_Tracker{};
    int rec_No_Tracker{};  // dependent
    G4ThreeVector rec_Size_TrackerRegion;  // dependent
    G4ThreeVector rec_Pos_TrackerRegion;  // dependent
    G4ThreeVector rec_Tracker_MagField;

    //----------------------------------------
    // Electromagnetic Calorimeter
    G4String ECAL_Name;

    G4Material *ECALRegion_Mat{};
    G4Material *ECAL_Center_Mat{};
    G4Material *ECAL_Wrap_Mat{};
    G4ThreeVector Size_ECALRegion;  // dependent
    G4ThreeVector Pos_ECALRegion;  // dependent
    G4ThreeVector ECAL_Center_Wrap_Size;
    G4ThreeVector ECAL_Center_Size;
    G4ThreeVector ECAL_Center_Module_No;

    //----------------------------------------
    // Hadronic Calorimeter
    G4String HCAL_Name;

    G4Material *HCALRegion_Mat{};
    G4Material *HCAL_Mat{};
    G4Material *HCAL_Wrap_Mat{};
    G4Material *HCAL_Absorber_Mat{};
    G4ThreeVector Size_HCALRegion;  // dependent
    G4ThreeVector Pos_HCALRegion;  // dependent
    G4ThreeVector HCAL_Wrap_Size;
    G4ThreeVector HCAL_Size_Dir;
    G4ThreeVector HCAL_Mod_No_Dir;
    G4ThreeVector HCAL_Module_No;
    G4double HCAL_Module_Gap;
    G4double HCAL_Absorber_Thickness;

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

    std::vector<std::tuple<int, double, double, double, double, bool, bool, bool> > particle_filters_parameters;
    std::vector<std::tuple<G4String, double, double, double, double, bool, bool, bool> > process_filters_parameters;

    G4int fStage;
    //========================================
    /* Optical */
    //----------------------------------------
    // if to open optical process
    bool if_optical=true;
    bool Optical_UseLUT;
    double Optical_YieldFactor;
    int Optical_PhysicsVerbose = 0;
    //LUT loader
    G4String LUT_FilePath;
    G4String LUT_Name;
    std::map<G4String,std::pair<G4String, G4String>> Optical_GetLUTDefinations();
    DigiScheme Optical_GetDigiScheme(const G4String& cIn);
    DetUnitType Optical_GetDetType(const G4String& cIn);
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
    double Optical_voltageToADC(){return Optical_fullRangeMV/Optical_nBits;};
    // double Optical_voltageToADC;
    int Optical_range_min;
    int Optical_range_max; 
    int Optical_pedestalLevel;          
    int Optical_noiseSigma;
    double Optical_clockJitterSigma;
    double Optical_apertureJitterSigma;
    double Optical_pulseScaleFactor;

    //----------------------------------------
    // Wrap related
    G4MaterialPropertiesTable *Wrap_Surface_Mat;
    G4OpticalSurface *Wrap_Surface;
    //----------------------------------------
    // APD related
    G4Material *APD_Mat;
    G4ThreeVector APD_Size;

    G4Material *Glue_Mat;
    G4ThreeVector Glue_Size;
    G4MaterialPropertiesTable *APD_Surface_Mat;
    G4OpticalSurface *APD_Surface;
    //----------------------------------------


private:
    Control();

    static G4ThreeVector readV3(const YAML::Node &n, bool unit=false);
    static double readV2(const YAML::Node &n);
};

extern Control *dControl;
#endif //DSIMU_CONTROL_H
