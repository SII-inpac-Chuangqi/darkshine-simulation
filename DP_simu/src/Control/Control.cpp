//
// Created by Zhang Yulei on 12/31/20.
//

#include "Control/Control.h"

// Geant4
#include "G4UnitsTable.hh"

// Required by Singleton
Control *dControl = nullptr;

// Get Instance Class
Control *Control::CreateInstance() {
    if (dControl == nullptr)
        dControl = new Control();

    return dControl;
}

Control::Control() {

    // Construct Material Table First
    //ConstructG4MaterialTable();

    UIManager = G4UImanager::GetUIpointer();
    /* Build All independent Variables */

    //========================================
    /* Global Variables */
    //----------------------------------------
    save_geometry = false;
    check_overlaps = false;
    Memory_Check = true;
    signal_production = false;

    //========================================
    // Magnetic field
    //----------------------------------------
    mag_field_input = "mag_default.root";
    uniform_mag_field = true;
    tag_Tracker_MagField = G4ThreeVector(0, -1.5 * tesla, 0);
    rec_Tracker_MagField = G4ThreeVector(0, -1.5 * tesla, 0);
    mag_verbose = 0;

    //----------------------------------------
    // Signal Option
    signal_mass = 17 * MeV;
    signal_lookup_table = "";

    //----------------------------------------
    // Root Manager Options
    outfile_Name = "dp_out.root";
    tree_Name = "Dark_Photon";

    Run_Number = 0;
    Total_Event_Number = 10000;

    //----------------------------------------
    // Out Collection Options
    save_all_mcp = false;
    save_MC = true;
    save_initial_particle_step = true;

    RawMCCollection_Name = "RawMCParticle";
    InitialParticleStepCollection_Name = "Initial_Particle_Step";

    //========================================
    /* Geometry */
    //----------------------------------------
    // Build Options
    build_target = true;
    build_tag_tracker = true;
    build_rec_tracker = true;
    build_ECAL = true;
    build_HCAL = true;

    build_only_target = false;
    build_only_tag_tracker = false;
    build_only_rec_tracker = false;
    build_only_ECAL = false;
    build_only_HCAL = false;

    //----------------------------------------
    // Target
    MaterialStr["Target_Mat"] = "G4_W";
    Target_Size = G4ThreeVector(10 * cm, 20 * cm, 350 * um);
    Target_Pos = G4ThreeVector(0 * cm, 0 * cm, 0 * cm);

    //----------------------------------------
    // Tracker
    build_silicon_micro_strip = true;
    Trk_Tar_Dis = 7.5 * mm;
    MaterialStr["Tracker_Mat"] = "G4_Si";
    MaterialStr["TrackerRegion_Mat"] = "vacuum";
    Tracker1_Color = G4ThreeVector(0.5, 0.5, 0.);
    Tracker2_Color = G4ThreeVector(0.5, 0.5, 0.);

    // Tagging Tracker
    tag_Tracker_MagField = G4ThreeVector(0, -1.5 * tesla, 0);
    for (int i = 0; i < 7; i++) {
        tag_Size_Tracker.emplace_back(10 * cm, 20 * cm, 0.1 * mm);
        tag_Pos_Tracker.emplace_back(0 * cm, 0 * cm, (-30. + i * 10.) * cm);
        tag_Tracker_Angle_Gap.emplace_back(0 * radian, 0.1 * radian, 1 * um);
        tag_Tracker_Strip_N.emplace_back(20);
    }

    assert(tag_Size_Tracker.size() == tag_Pos_Tracker.size()); // Sanity Check
    assert(tag_Tracker_Angle_Gap.size() == tag_Size_Tracker.size());
    assert(tag_Tracker_Strip_N.size() == tag_Tracker_Angle_Gap.size());

    // Recoil Tracker
    rec_Tracker_MagField = G4ThreeVector(0, -0.5 * tesla, 0);
    for (int i = 0; i < 6; i++) {
        rec_Size_Tracker.emplace_back(10 * cm, 20 * cm, 0.1 * mm);
        rec_Tracker_Angle_Gap.emplace_back(0 * radian, 0.1 * radian, 1 * um);
        rec_Tracker_Strip_N.emplace_back(20);
    }
    rec_Pos_Tracker.emplace_back(0 * cm, 0 * cm, -86.25 * mm);
    rec_Pos_Tracker.emplace_back(0 * cm, 0 * cm, -71.25 * mm);
    rec_Pos_Tracker.emplace_back(0 * cm, 0 * cm, -55.25 * mm);
    rec_Pos_Tracker.emplace_back(0 * cm, 0 * cm, -40.25 * mm);
    rec_Pos_Tracker.emplace_back(0 * cm, 0 * cm, -4.25 * mm);
    rec_Pos_Tracker.emplace_back(0 * cm, 0 * cm, 86.25 * mm);

    assert(rec_Size_Tracker.size() == rec_Pos_Tracker.size());  // Sanity Check
    assert(rec_Tracker_Angle_Gap.size() == rec_Size_Tracker.size());
    assert(rec_Tracker_Strip_N.size() == rec_Tracker_Angle_Gap.size());

    //----------------------------------------
    // Electromagnetic Calorimeter
    ECAL_Name = "ECAL";

    MaterialStr["ECALRegion_Mat"] = "CarbonFiber";
    MaterialStr["ECAL_Center_Mat"] = "LYSO";
    MaterialStr["ECAL_Center_Mat"] = "PWO4"; // X0 = 0.92 cm
    MaterialStr["ECAL_Wrap_Mat"] = "G4_C";

    ECAL_Center_Wrap_Size = G4ThreeVector(0.3 * mm, 0.3 * mm, 0.3 * mm);
    ECAL_Center_Size = G4ThreeVector(2.5 * cm, 2.5 * cm, 2.0 * cm);
    //ECAL_Center_Module_No = G4ThreeVector(20, 20, 18);

    //----------------------------------------
    // Hadronic Calorimeter
    HCAL_Name = "HCAL";

    MaterialStr["HCAL_Absorber_Mat"] = "G4_Fe";
    MaterialStr["HCALRegion_Mat"] = "CarbonFiber";
    MaterialStr["HCAL_Mat"] = "G4_POLYSTYRENE";
    MaterialStr["HCAL_Wrap_Mat"] = "G4_C";

    HCAL_Wrap_Size = G4ThreeVector(0.3 * mm, 0.3 * mm, 0.3 * mm);
    //HCAL_Size_Dir = G4ThreeVector(100 * cm + 19 * HCAL_Wrap_Size.x(), 5 * cm, 1 * cm);
    HCAL_Size_Dir = G4ThreeVector(1 * cm, 5 * cm, 100 * cm + 19 * HCAL_Wrap_Size.x());
    HCAL_Module_No = G4ThreeVector(3, 3, 1);
    HCAL_Absorber_Thickness = 3 * cm;

    //========================================
    /* Biasing */
    //----------------------------------------
    if_bias = false;
    if_bias_target = false;
    if_bias_ECAL = false;

    BiasProcess = "GammaToMuPair"; // or DMProcessDMBrem
    BiasFactor = 1e20;
    BiasEmin = 4.0 * GeV;

    //========================================
    /* Filters */
    //----------------------------------------
    if_filter = false;
    // to filter hardbrem events
    if_HardBrem = false;
    // Example of particle filter
    // select photon with energy < 4 GeV within range from -1 m to 0.2 m
    particle_filters_parameters.emplace_back(22, 6 * GeV, 8 * GeV, -1 * m, 0.2 * m, 1, 1, 1);

    // Example of processor filter
    // exclude event with process of energy < 4 GeV within range from -1 m to 0.2 m
    process_filters_parameters.emplace_back("GammaToMuPair", 2 * GeV, 4 * GeV, -1 * m, 0.2 * m, 0, 1, 1);

    //========================================
    /* Optical */
    //----------------------------------------
    if_optical = true;
    Optical_UseLUT = false; //if disabled, use full optical simulation.
    Optical_YieldFactor = 1e-3; //used to
    Optical_PhysicsVerbose = 0;
    //digitizer
    Optical_digitizerDebug = false;
    Optical_usePositivePolarity = true;// "false" will use negative polarity
    Optical_addClockJitter = true;// "false" will not clock jitters
    Optical_injectNoise = true;// "false" will not add digitization noise
    Optical_sampleInterval = 1.25; // [ns]
    Optical_maxTime = 100.;        // [ns]
    // hadrware parameters:SIPM response
    // 16 channels, 3600 pixels per channel
    Optical_nPixels = 16 * 3600 - 1;       // counting from zero
    Optical_pixelRecoveryTau = 50.;    // [ns]
    Optical_pixelRecoveryCutoff = 10.; // [sigma]
    // hardware parameters: SIPM pulse
    Optical_pulseFilePath = "SiPM_g2_pulse.root";
    Optical_splineName = "pulseShapeSpline";
    Optical_pulseTimeZero = -5.; // appropriate t0 value?
    // hadrware parameters:ADC and FE (now we only save the analog waveform in voltage.)
    Optical_nBits = 4096; // 2^12 ADC bits
    Optical_fullRangeMV = 5000.; //full range voltage in mV
    Optical_range_min = -2047; // -(2^12 - 1)
    Optical_range_max = 2048;  // +(2^12)
    Optical_pedestalLevel = 0;          // [ADC count]
    Optical_noiseSigma = 4;                  // [ADC count]
    Optical_clockJitterSigma = 0.05;      // [ns]
    Optical_apertureJitterSigma = 0.0001; // [ns]
    // scaling factor to allow for two overlapped 3.1 GeV positrons to be in ADC's
    // range,need calibration
    Optical_pulseScaleFactor = 1. /
                               1000; //dummy, can be setted later in DEvent or here. This is for pre-calibration for pulshShape file. output unit is mV

    HCAL_CaloHoleRadius = 1.2 * mm;
    HCAL_FiberRadius = 0.98 * 1.2 * mm;
    //----------------------------------------
    // LUT
    LUT_FilePath = "ECAL_LUT.dat";
    LUT_Name = "ECAL_cube_2.5_2.5_2_v1";

    ECAL_APD_Size = G4ThreeVector(1 * cm, 1 * cm, 1 * mm);
    HCAL_APD_Size = G4ThreeVector(3 * mm, 3 * mm, 1 * mm);
    Glue_Size = G4ThreeVector(1 * cm, 1 * cm, 0.1 * mm);

}

void Control::RebuildVariables() {

    //----------------------------------------
    // Filters
    // if_filter = !particle_filters_parameters.empty() || !process_filters_parameters.empty();
    if (signal_production) {
        save_all_mcp = true;
        save_MC = true;
        save_initial_particle_step = true;
        if_bias = true;
        if_bias_target = true;
        BiasProcess = "DMProcessDMBrem";
        if_filter = true;
        process_filters_parameters.emplace_back("DMProcessDMBrem", 0 * GeV, -1 * GeV, -7.5 * mm, 7.5 * mm, 1, 1, 0);
    }

    //----------------------------------------
    // Build Options
    if (build_only_target || build_only_tag_tracker || build_only_rec_tracker ||
        build_only_ECAL || build_only_HCAL) {
        build_target = !(build_only_tag_tracker || build_only_rec_tracker || build_only_ECAL || build_only_HCAL);
        build_tag_tracker = !(build_only_target || build_only_rec_tracker || build_only_ECAL || build_only_HCAL);
        build_rec_tracker = !(build_only_target || build_only_tag_tracker || build_only_ECAL || build_only_HCAL);
        build_ECAL = !(build_only_target || build_only_tag_tracker || build_only_rec_tracker || build_only_HCAL);
        build_HCAL = !(build_only_target || build_only_tag_tracker || build_only_rec_tracker || build_only_ECAL);
    }
    //----------------------------------------
    // Tagging Tracker
    assert(tag_Size_Tracker.size() == tag_Pos_Tracker.size());
    tag_No_Tracker = tag_Size_Tracker.size();
    /// Size and Position of Recoil Tracker Region
    tag_Size_TrackerRegion = G4ThreeVector(
            2.0 * tag_Size_Tracker[0].x(),
            2.0 * tag_Size_Tracker[0].y(),
            tag_Pos_Tracker[tag_No_Tracker - 1].z() - tag_Pos_Tracker[0].z() +
            2.0 * (tag_Size_Tracker[0].z() ) + eps);

    tag_Pos_TrackerRegion = G4ThreeVector(
            0 * cm, 0 * cm,
            -0.5 * tag_Size_TrackerRegion.z() - Trk_Tar_Dis - 0.5 * Target_Size.z());
    if (build_only_tag_tracker) tag_Pos_TrackerRegion = G4ThreeVector(0, 0, 0);

    //----------------------------------------
    // Recoil Tracker
    assert(rec_Size_Tracker.size() == rec_Pos_Tracker.size());
    rec_No_Tracker = rec_Size_Tracker.size();
    /// Size and Position of Recoil Tracker Region
    rec_Size_TrackerRegion = G4ThreeVector(
            2.0 * rec_Size_Tracker[rec_No_Tracker - 1].x(),
            2.0 * rec_Size_Tracker[rec_No_Tracker - 1].y(),
            rec_Pos_Tracker[rec_No_Tracker - 1].z() - rec_Pos_Tracker[0].z() +
            2.0 * (rec_Size_Tracker[rec_No_Tracker - 1].z()) + eps );

    rec_Pos_TrackerRegion = G4ThreeVector(
            0 * cm, 0 * cm,
            0.5 * rec_Size_TrackerRegion.z() + Trk_Tar_Dis + 0.5 * Target_Size.z());
    if (build_only_rec_tracker) rec_Pos_TrackerRegion = G4ThreeVector(0, 0, 0);

    //----------------------------------------
    // Electromagnetic Calorimeter

    Size_ECALCell.setX(ECAL_Center_Size.x() + ECAL_Center_Wrap_Size.x());
    Size_ECALCell.setY(ECAL_Center_Size.y() + ECAL_Center_Wrap_Size.y());
    Size_ECALCell.setZ(ECAL_Center_Size.z() + ECAL_Center_Wrap_Size.z() + ECAL_APD_Size.z());

    Size_ECALBlock.setX(ECAL_Cell_No.x() * (Size_ECALCell.x() + ECAL_Cell_Gap.x()) );
    Size_ECALBlock.setY(ECAL_Cell_No.y() * (Size_ECALCell.y() + ECAL_Cell_Gap.y()) );
    Size_ECALBlock.setZ(ECAL_Cell_No.z() * (Size_ECALCell.z() + ECAL_Cell_Gap.z()) );

    Size_ECALRegion.setX( ECAL_Block_No.x() * Size_ECALBlock.x() + eps);
    Size_ECALRegion.setY( ECAL_Block_No.y() * Size_ECALBlock.y() + eps);
    Size_ECALRegion.setZ( ECAL_Block_No.z() * Size_ECALBlock.z() + eps );

    Pos_ECALRegion = G4ThreeVector(0, 0,
                                   0.5 * Size_ECALRegion.z() + rec_Pos_TrackerRegion.z() +
                                   0.5 * rec_Size_TrackerRegion.z() + 1.0 * mm);
    if (build_only_ECAL) Pos_ECALRegion = G4ThreeVector(0, 0, 0);

    ECAL_Center_Module_No.setX(ECAL_Cell_No.x() * ECAL_Block_No.x());
    ECAL_Center_Module_No.setY(ECAL_Cell_No.y() * ECAL_Block_No.y());
    ECAL_Center_Module_No.setZ(ECAL_Cell_No.z() * ECAL_Block_No.z());

    //----------------------------------------
    // Hadronic Calorimeter

    /// calculate absorber thickness
    [[maybe_unused]] int prev_endn = 0;
    int startn;
    int endn;
    double thickness;

    HCAL_Absorber_Thickness_Total = 0;
    for (auto thick_i : HCAL_Absorber_Thickness_List) {
        std::tie(startn, endn, thickness) = thick_i;
        /// sanity check
        assert(startn <= endn && "[ERROR] HCAL_Absorber_Thickness_List invalid");
        assert(startn == prev_endn + 1 && "[ERROR] HCAL_Absorber_Thickness_List must be continuous, and start from 1");
        assert(thickness >=0 && "[ERROR] HCAL_Absorber_Thickness_List invalid");
        /// add Thickness
        for (int i = startn; i <= endn; i++) {
            HCAL_Absorber_Thickness_Total += thickness;
        }
        prev_endn = endn;
        HCAL_Layer_N = endn + 1;
    }

    HCAL_Mod_No_Dir.setX(HCAL_Cell_XY_N);
    HCAL_Mod_No_Dir.setY(HCAL_Cell_XY_N);
    HCAL_Mod_No_Dir.setZ(2 * HCAL_Layer_N - 1);

    Size_HCALCell.setX(HCAL_Size_Dir.x() + HCAL_Wrap_Size.x());
    Size_HCALCell.setY(HCAL_Size_Dir.y() + HCAL_Wrap_Size.y());
    Size_HCALCell.setZ(HCAL_Size_Dir.z() + HCAL_Wrap_Size.z() + HCAL_APD_Size.z());

    Size_HCALModule.setX(Size_HCALCell.z());
    Size_HCALModule.setY(Size_HCALCell.z());
    Size_HCALModule.setZ(2 * ( Size_HCALCell.x()) );

    Size_HCALLayer.setX(HCAL_Module_No.x() * (Size_HCALModule.x() + HCAL_Module_Gap.x()) );
    Size_HCALLayer.setY(HCAL_Module_No.y() * (Size_HCALModule.y() + HCAL_Module_Gap.y()) );
    Size_HCALLayer.setZ(HCAL_Module_No.z() * (Size_HCALModule.z() + HCAL_Module_Gap.z()) );

    Size_HCALRegion.setX(Size_HCALLayer.x() + eps);
    Size_HCALRegion.setY(Size_HCALLayer.y() + eps);
    Size_HCALRegion.setZ(HCAL_Layer_N * Size_HCALLayer.z() + HCAL_Absorber_Thickness_Total + eps);

    Pos_HCALRegion = G4ThreeVector(0, 0,
                                   0.5 * Size_HCALRegion.z() + Pos_ECALRegion.z() + 0.5 * Size_ECALRegion.z() + 1 * mm);
    if (build_only_HCAL) Pos_HCALRegion = G4ThreeVector(0, 0, 0);

    //----------------------------------------
    // World
    World_Mat = G4Material::GetMaterial("vacuum");
    //G4double l = 2.0 * (Pos_HCALRegion.z() + Size_HCALRegion.x());
    G4double borderX[5] = {0.2 * m,
                           build_rec_tracker * (fabs(rec_Pos_TrackerRegion.x()) + rec_Size_TrackerRegion.x()),
                           build_tag_tracker * (fabs(tag_Pos_TrackerRegion.x()) + tag_Size_TrackerRegion.x()),
                           build_ECAL * (fabs(Pos_ECALRegion.x()) + Size_ECALRegion.x()),
                           build_HCAL * (fabs(Pos_HCALRegion.x()) + Size_HCALRegion.x())};
    G4double borderY[5] = {0.2 * m,
                           build_rec_tracker * (fabs(rec_Pos_TrackerRegion.y()) + rec_Size_TrackerRegion.y()),
                           build_tag_tracker * (fabs(tag_Pos_TrackerRegion.y()) + tag_Size_TrackerRegion.y()),
                           build_ECAL * (fabs(Pos_ECALRegion.y()) + Size_ECALRegion.y()),
                           build_HCAL * (fabs(Pos_HCALRegion.y()) + Size_HCALRegion.y())};
    G4double borderZ[5] = {0.2 * m,
                           build_rec_tracker * (fabs(rec_Pos_TrackerRegion.z()) + rec_Size_TrackerRegion.z()),
                           build_tag_tracker * (fabs(tag_Pos_TrackerRegion.z()) + tag_Size_TrackerRegion.z()),
                           build_ECAL * (fabs(Pos_ECALRegion.z()) + Size_ECALRegion.z()),
                           build_HCAL * (fabs(Pos_HCALRegion.z()) + Size_HCALRegion.z())};
    G4double lx = *std::max_element(borderX, borderX + 5);
    G4double ly = *std::max_element(borderY, borderY + 5);
    G4double lz = *std::max_element(borderZ, borderZ + 5);
    G4double lzoom = 2;
    Size_World = G4ThreeVector(lzoom * lx, lzoom * ly, lzoom * lz);

    //----------------------------------------
    // Optical

    //----------------------------------------
    // DetectorMap
    dDetectorIDMaps->SetECALCellNo(ECAL_Cell_No.x(),ECAL_Cell_No.y(),ECAL_Cell_No.z());
    dDetectorIDMaps->SetECALBlockNo(ECAL_Block_No.x(),ECAL_Block_No.y(),ECAL_Block_No.z());
    dDetectorIDMaps->SetHCALModuleNo(HCAL_Module_No.x(),HCAL_Module_No.y(),HCAL_Module_No.z());
    dDetectorIDMaps->BuildMap();
}


void Control::ConstructG4MaterialTable() const {

    G4NistManager *nistManager = G4NistManager::Instance();

    // Material definition
    G4Element *elP = nistManager->FindOrBuildElement("P");
    G4Element *elW = nistManager->FindOrBuildElement("W");
    G4Element *elO = nistManager->FindOrBuildElement("O");
    G4Element *elN = nistManager->FindOrBuildElement("N");
    G4Element *elLu = nistManager->FindOrBuildElement("Lu");
    G4Element *elSi = nistManager->FindOrBuildElement("Si");
    G4Element *elH = nistManager->FindOrBuildElement("H");
    G4Element *elC = nistManager->FindOrBuildElement("C");
    G4Element *elY = nistManager->FindOrBuildElement("Y");

    // Vacuum
    auto *Vacuum = new G4Material("vacuum", universe_mean_density, 2);
    Vacuum->AddElement(elN, .7);
    Vacuum->AddElement(elO, .3);

    // Defining LYSO
    auto *LSO = new G4Material("LSO", 7.4 * g / cm3, 3);
    LSO->AddElement(elLu, 2);
    LSO->AddElement(elSi, 1);
    LSO->AddElement(elO, 5);

    auto *YSO = new G4Material("YSO", 4.5 * g / cm3, 3);
    YSO->AddElement(elY, 2);
    YSO->AddElement(elSi, 1);
    YSO->AddElement(elO, 5);

    auto *LYSO = new G4Material("LYSO", 7.1 * g / cm3, 2, kStateSolid);
    LYSO->AddMaterial(LSO, 90 * perCent);
    LYSO->AddMaterial(YSO, 10 * perCent);

    // Defining Polystyrene
    auto *PS = new G4Material("Polystyrene", 1.04 * g / cm3, 2);
    PS->AddElement(elC, 8);
    PS->AddElement(elH, 8);

    // Define PWO4
    auto *PWO4 = new G4Material("PWO4", 5.26 * g / cm3, 3);
    PWO4->AddElement(elP, 1);
    PWO4->AddElement(elW, 1);
    PWO4->AddElement(elO, 4);

    // Define Carbon Fiber
    auto *CarbonFiber = new G4Material("CarbonFiber", 1.93 * g / cm3, 1);
    CarbonFiber->AddElement(elC, 1);

    // Build By Nist
    nistManager->FindOrBuildMaterial("G4_W");
    nistManager->FindOrBuildMaterial("G4_Si");
    nistManager->FindOrBuildMaterial("G4_Al");
    nistManager->FindOrBuildMaterial("G4_Fe");
    nistManager->FindOrBuildMaterial("G4_C");

    /* Define Optical Properties */
    //
    // ------------ Generate & Add Material Properties Table ------------
    //
    std::cout << "[Control] ==> optical enabled. " << std::endl;

    double photonEnergy[] = {0.1 * eV, 2.21 * eV, 2.58 * eV, 2.82 * eV, 2.95 * eV, 3.10 * eV, 4.00 * eV};

    const int nEntries = sizeof(photonEnergy) / sizeof(G4double);

    double RefractionIdx[nEntries] = {1.85, 1.833, 1.821, 1.813, 1.809, 1.804, 1.79};
    double AbsorptionLength[nEntries] = {40. * cm, 40. * cm, 40. * cm, 40. * cm, 40. * cm, 40. * cm, 40. * cm};

    auto *MPT = new G4MaterialPropertiesTable();
    MPT->AddProperty("RINDEX", photonEnergy, RefractionIdx, nEntries);
    MPT->AddProperty("ABSLENGTH", photonEnergy, AbsorptionLength, nEntries);

    double ScintEnergy[nEntries] = {0.1 * eV, 2.21 * eV, 2.58 * eV, 2.82 * eV, 2.95 * eV, 3.10 * eV, 4.00 * eV};
    double ScintFast[nEntries] = {0.0, 0.23, 0.85, 1.93, 2.15, 1.08, 0.0};

    MPT->AddProperty("FASTCOMPONENT", ScintEnergy, ScintFast, nEntries);

    MPT->AddConstProperty("SCINTILLATIONYIELD", 20000. / MeV);
    MPT->AddConstProperty("RESOLUTIONSCALE", 1.0);
    MPT->AddConstProperty("FASTTIMECONSTANT", 40. * ns);
    MPT->AddConstProperty("YIELDRATIO", 1.);

    LYSO->SetMaterialPropertiesTable(MPT);


    // ---------------------------------------------------
    // Add Material and PropertiesTable

    // Calo
    G4double wls_Energy[] = {2.00*eV,2.87*eV,2.90*eV,3.47*eV};
    const G4int wlsnum = sizeof(wls_Energy)/sizeof(G4double);
    G4double rIndexPstyrene[]={ 1.5, 1.5, 1.5, 1.5};
    G4double absorption1[]={2.*cm, 2.*cm, 2.*cm, 2.*cm};
    G4double scintilFast[]={0.00, 0.00, 1.00, 1.00};

    assert(sizeof(rIndexPstyrene) == sizeof(wls_Energy));
    assert(sizeof(absorption1) == sizeof(wls_Energy));
    assert(sizeof(scintilFast) == sizeof(wls_Energy));

    auto *MPTPStyrene = new G4MaterialPropertiesTable();
    MPTPStyrene->AddProperty("RINDEX",wls_Energy,rIndexPstyrene,wlsnum);
    MPTPStyrene->AddProperty("ABSLENGTH",wls_Energy,absorption1,wlsnum);
    MPTPStyrene->AddProperty("FASTCOMPONENT",wls_Energy, scintilFast,wlsnum);
    MPTPStyrene->AddConstProperty("SCINTILLATIONYIELD",Optical_HCAL_Yield); //VIP
    MPTPStyrene->AddConstProperty("RESOLUTIONSCALE",1.0);
    MPTPStyrene->AddConstProperty("FASTTIMECONSTANT", 10.*ns);

    // Fiber Clad
    G4double RefractiveIndexClad1[]={ 1.49, 1.49, 1.49, 1.49};
    G4double AbsFiber[]={9.00*m,9.00*m,0.1*mm,0.1*mm};

    assert(sizeof(RefractiveIndexClad1) == sizeof(wls_Energy));
    assert(sizeof(AbsFiber) == sizeof(wls_Energy));

    auto *clad1Property = new G4MaterialPropertiesTable();
    clad1Property->AddProperty("RINDEX",wls_Energy,RefractiveIndexClad1,wlsnum);
    clad1Property->AddProperty("ABSLENGTH",wls_Energy,AbsFiber,wlsnum);

    // Fiber
    G4double RefractiveIndexFiber[]={ 1.60, 1.60, 1.60, 1.60};
    G4double EmissionFib[]={1.0, 1.0, 0.0, 0.0};

    assert(sizeof(RefractiveIndexFiber) == sizeof(wls_Energy));
    assert(sizeof(EmissionFib) == sizeof(wls_Energy));

    auto *fiberProperty = new G4MaterialPropertiesTable();
    fiberProperty->AddProperty("RINDEX",wls_Energy,RefractiveIndexFiber,wlsnum);
    fiberProperty->AddProperty("WLSABSLENGTH",wls_Energy,AbsFiber,wlsnum);
    fiberProperty->AddProperty("WLSCOMPONENT",wls_Energy,EmissionFib,wlsnum);
    fiberProperty->AddConstProperty("WLSTIMECONSTANT", 0.5*ns);

    // -----------------------------------------------------
    // Add Material
    //

    // Calo
    auto *G4_PS = nistManager->FindOrBuildMaterial("G4_POLYSTYRENE");
    G4_PS->SetMaterialPropertiesTable(MPTPStyrene);
    G4_PS->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

    // Fiber clad
    auto *G4_PE = nistManager->FindOrBuildMaterial("G4_POLYETHYLENE");
    G4_PE->SetMaterialPropertiesTable(clad1Property);

    // Fiber
    auto *PMMA = new G4Material("PMMA",1190*kg/m3,3);
    PMMA->AddElement(elH, 6 + 2 * Optical_polyPMMA);
    PMMA->AddElement(elC,3 + 2 * Optical_polyPMMA);
    PMMA->AddElement(elO, 2);
    PMMA->SetMaterialPropertiesTable(fiberProperty);

    // Print materials
    //G4cout << *(G4Material::GetMaterialTable()) < < G4endl;
}

void Control::AssignG4Material() {
    Target_Mat = G4Material::GetMaterial(MaterialStr.at("Target_Mat"));
    Tracker_Mat = G4Material::GetMaterial(MaterialStr.at("Tracker_Mat"));
    TrackerRegion_Mat = G4Material::GetMaterial(MaterialStr.at("TrackerRegion_Mat"));
    ECALRegion_Mat = G4Material::GetMaterial(MaterialStr.at("ECALRegion_Mat"));
    ECAL_Center_Mat = G4Material::GetMaterial(MaterialStr.at("ECAL_Center_Mat"));
    ECAL_Wrap_Mat = G4Material::GetMaterial(MaterialStr.at("ECAL_Wrap_Mat"));
    HCAL_Absorber_Mat = G4Material::GetMaterial(MaterialStr.at("HCAL_Absorber_Mat"));
    HCALRegion_Mat = G4Material::GetMaterial(MaterialStr.at("HCALRegion_Mat"));
    HCAL_Mat = G4Material::GetMaterial(MaterialStr.at("HCAL_Mat"));
    HCAL_Wrap_Mat = G4Material::GetMaterial(MaterialStr.at("HCAL_Wrap_Mat"));
    HCAL_FiberClad_Mat = G4Material::GetMaterial(MaterialStr.at("HCAL_FiberClad_Mat"));
    HCAL_Fiber_Mat = G4Material::GetMaterial(MaterialStr.at("HCAL_Fiber_Mat"));

    APD_Mat = G4Material::GetMaterial("G4_Si");
    Glue_Mat = G4Material::GetMaterial("G4_W");
    if (if_optical) {
        //----------------------------------------
        // Wrap related
        const G4int cNum = 2;
        G4double ephoton[cNum] = {1 * eV, 7 * eV};//overflow and underflow will take first/last bin
        G4double reflectivity[cNum] = {1.0, 1.0};
        G4double efficiency[cNum] = {0.0, 0.0};
        G4double transmittance[cNum] = {0.0, 0.0};
        Wrap_Surface_Mat = new G4MaterialPropertiesTable();
        Wrap_Surface_Mat->AddProperty("REFLECTIVITY", ephoton, reflectivity, cNum); //reflect fraction, default=1
        Wrap_Surface_Mat->AddProperty("EFFICIENCY", ephoton, efficiency,
                                      cNum); //detection  fraction (abs=1-reflet-trans, then at efficiency, detectition(invoke post-step SD)),default=0
        Wrap_Surface_Mat->AddProperty("TRANSMITTANCE", ephoton, transmittance, cNum); //transmission fraction, default=0

        Wrap_Surface = new G4OpticalSurface("WrapSurfaceOptical");
        Wrap_Surface->SetType(dielectric_LUT);
        Wrap_Surface->SetModel(LUT);
        Wrap_Surface->SetFinish(polishedtyvekair);
        Wrap_Surface->SetMaterialPropertiesTable(Wrap_Surface_Mat);
        //----------------------------------------
        // APD related

        G4double reflectivityAPD[cNum] = {0.0, 0.0};
        G4double efficiencyAPD[cNum] = {0.2, 0.2};
        G4double transmittanceAPD[cNum] = {0.0, 0.0};
        APD_Surface_Mat = new G4MaterialPropertiesTable();
        APD_Surface_Mat->AddProperty("REFLECTIVITY", ephoton, reflectivityAPD, cNum); //reflect fraction, default=1
        APD_Surface_Mat->AddProperty("EFFICIENCY", ephoton, efficiencyAPD,
                                     cNum); //detection  fraction (abs=1-reflet-trans, then at efficiency, detectition(invoke post-step SD)),default=0
        APD_Surface_Mat->AddProperty("TRANSMITTANCE", ephoton, transmittanceAPD, cNum); //transmission fraction, default=0

        APD_Surface = new G4OpticalSurface("APDSurfaceOptical");
        APD_Surface->SetType(dielectric_LUT);
        APD_Surface->SetModel(LUT);
        APD_Surface->SetFinish(polishedvm2000glue);
        APD_Surface->SetMaterialPropertiesTable(APD_Surface_Mat);
    }
}

bool Control::ReadYAML(const G4String &file_in) {
    G4String infile;
    if (file_in.empty()) infile = "default.yaml";
    else infile = file_in;
    Node = YAML::LoadFile(infile);

    try {
        //========================================
        /* Global Variables */
        //----------------------------------------
        save_geometry = Node["save_geometry"].IsDefined() ? Node["save_geometry"].as<bool>() : false;
        check_overlaps = Node["check_overlaps"].IsDefined() ? Node["check_overlaps"].as<bool>() : false;
        signal_production = Node["signal_production"].IsDefined() ? Node["signal_production"].as<bool>() : false;
        if (Node["signal_mass"].IsDefined()) signal_mass = readV2(Node["signal_mass"]);
        signal_lookup_table = (Node["signal_lookup_table"].IsDefined() ? Node["signal_lookup_table"].as<std::string>()
                                                                       : "");

        //std::cout<<Node["signal_use_LUT"].IsDefined()<<" "<<Node["signal_use_LUT"].as<bool>()<<std::endl;
        signal_use_LUT = Node["signal_use_LUT"].IsDefined() && Node["signal_use_LUT"].as<bool>();
        //========================================
        // Magnetic field
        //----------------------------------------
        mag_field_input = Node["MagField"]["mag_field_input"].IsDefined()
                          ? Node["MagField"]["mag_field_input"].as<std::string>() : "mag_default.root";
        uniform_mag_field = Node["MagField"]["uniform_mag_field"].IsDefined()
                            ? Node["MagField"]["uniform_mag_field"].as<bool>() : true;
        tag_Tracker_MagField = Node["MagField"]["tag_Tracker_MagField"].IsDefined() ? readV3(
                Node["MagField"]["tag_Tracker_MagField"], true) : G4ThreeVector(0, -1.5 * tesla, 0);
        rec_Tracker_MagField = Node["MagField"]["rec_Tracker_MagField"].IsDefined() ? readV3(
                Node["MagField"]["rec_Tracker_MagField"], true) : G4ThreeVector(0, -1.5 * tesla, 0);
        mag_verbose = Node["MagField"]["mag_verbose"].IsDefined() ? Node["MagField"]["mag_verbose"].as<int>() : 0;
        //----------------------------------------
        // Root Manager Options
        outfile_Name = Node["RootManager"]["outfile_Name"].as<std::string>();
        tree_Name = Node["RootManager"]["tree_Name"].as<std::string>();
        Run_Number = Node["RootManager"]["Run_Number"].as<int>();
        Total_Event_Number = Node["general_particle_source"]["beam_on"].as<int>();
        //----------------------------------------
        // Out Collection Options
        save_all_mcp = Node["OutCollection"]["save_all_mcp"].as<bool>();
        save_MC = Node["OutCollection"]["save_MC"].as<bool>();
        save_initial_particle_step = Node["OutCollection"]["save_initial_particle_step"].as<bool>();
        save_mcp_helper = Node["OutCollection"]["save_mcp_helper"].as<bool>();
        RawMCCollection_Name = Node["OutCollection"]["RawMCCollection_Name"].as<std::string>();
        InitialParticleStepCollection_Name = Node["OutCollection"]["InitialParticleStepCollection_Name"].as<std::string>();
        MCPHelperCollection_Name = Node["OutCollection"]["MCPHelperCollection_Name"].as<std::string>();
        //----------------------------------------
        // For Memory Leak
        Memory_Check = Node["memory_check"].IsDefined() ? Node["memory_check"].as<bool>() : false;
        //========================================
        /* Biasing */
        //----------------------------------------
        if_bias = Node["Biasing"]["if_bias"].as<bool>();
        if_bias_target = Node["Biasing"]["if_bias_target"].as<bool>();
        if_bias_ECAL = Node["Biasing"]["if_bias_ECAL"].as<bool>();
        BiasProcess = Node["Biasing"]["BiasProcess"].as<std::string>();
        BiasFactor = Node["Biasing"]["BiasFactor"].as<double>();
        BiasEmin = Node["Biasing"]["BiasEmin"][0].as<double>()
                   * G4UnitDefinition::GetValueOf(Node["Biasing"]["BiasEmin"][1].as<std::string>());
        //========================================
        /* Filters */
        //----------------------------------------
        if_filter = Node["Filters"]["if_filter"].as<bool>();
        if_HardBrem = Node["Filters"]["if_HardBrem"].as<bool>();
        particle_filters_parameters.clear();
        for (auto i : Node["Filters"]["particle_filters_parameters"]) {
            particle_filters_parameters.emplace_back(
                    i[0].as<int>(),
                    i[1].as<double>() * G4UnitDefinition::GetValueOf(i[2].as<std::string>()),
                    i[3].as<double>() * G4UnitDefinition::GetValueOf(i[4].as<std::string>()),
                    i[5].as<double>() * G4UnitDefinition::GetValueOf(i[6].as<std::string>()),
                    i[7].as<double>() * G4UnitDefinition::GetValueOf(i[8].as<std::string>()),
                    i[9].as<bool>(),
                    i[10].as<bool>(),
                    i[11].as<bool>());
        }
        process_filters_parameters.clear();
        for (auto i : Node["Filters"]["process_filters_parameters"]) {
            process_filters_parameters.emplace_back(
                    i[0].as<std::string>(),
                    i[1].as<double>() * G4UnitDefinition::GetValueOf(i[2].as<std::string>()),
                    i[3].as<double>() * G4UnitDefinition::GetValueOf(i[4].as<std::string>()),
                    i[5].as<double>() * G4UnitDefinition::GetValueOf(i[6].as<std::string>()),
                    i[7].as<double>() * G4UnitDefinition::GetValueOf(i[8].as<std::string>()),
                    i[9].as<bool>(),
                    i[10].as<bool>(),
                    i[11].as<bool>());
        }
        //========================================
        /* Geometry */
        //----------------------------------------
        // Build Options
        build_target = Node["Geometry"]["build_target"].as<bool>();
        build_tag_tracker = Node["Geometry"]["build_tag_tracker"].as<bool>();
        build_rec_tracker = Node["Geometry"]["build_rec_tracker"].as<bool>();
        build_ECAL = Node["Geometry"]["build_ECAL"].as<bool>();
        build_HCAL = Node["Geometry"]["build_HCAL"].as<bool>();
        build_only_target = Node["Geometry"]["build_only_target"].as<bool>();
        build_only_tag_tracker = Node["Geometry"]["build_only_tag_tracker"].as<bool>();
        build_only_rec_tracker = Node["Geometry"]["build_only_rec_tracker"].as<bool>();
        build_only_ECAL = Node["Geometry"]["build_only_ECAL"].as<bool>();
        build_only_HCAL = Node["Geometry"]["build_only_HCAL"].as<bool>();
        //----------------------------------------
        // Target
        MaterialStr["Target_Mat"] = Node["Geometry"]["Target"]["Target_Mat"].as<std::string>();
        Target_Size = readV3(Node["Geometry"]["Target"]["Target_Size"], true);
        Target_Pos = readV3(Node["Geometry"]["Target"]["Target_Pos"], true);
        //----------------------------------------
        // Tracker
        build_silicon_micro_strip = Node["Geometry"]["Tracker"]["build_silicon_micro_strip"].IsDefined()
                                    ? Node["Geometry"]["Tracker"]["build_silicon_micro_strip"].as<bool>() : false;
        Trk_Tar_Dis = readV2(Node["Geometry"]["Tracker"]["Trk_Tar_Dis"]);
        MaterialStr["Tracker_Mat"] = Node["Geometry"]["Tracker"]["Tracker_Mat"].as<std::string>();
        MaterialStr["TrackerRegion_Mat"] = Node["Geometry"]["Tracker"]["TrackerRegion_Mat"].as<std::string>();
        Tracker1_Color = readV3(Node["Geometry"]["Tracker"]["Tracker1_Color"]);
        Tracker2_Color = readV3(Node["Geometry"]["Tracker"]["Tracker2_Color"]);
        // Tagging Tracker
        tag_Size_Tracker.clear();
        tag_Pos_Tracker.clear();
        tag_Tracker_Angle_Gap.clear();
        tag_Tracker_Strip_N.clear();
        for (auto node : Node["Geometry"]["Tracker"]["tag_Size_Tracker"])
            tag_Size_Tracker.emplace_back(readV3(node, true));
        for (auto node : Node["Geometry"]["Tracker"]["tag_Pos_Tracker"])
            tag_Pos_Tracker.emplace_back(readV3(node, true));
        for (auto node : Node["Geometry"]["Tracker"]["tag_Tracker_Angle_Gap"])
            tag_Tracker_Angle_Gap.emplace_back(readV3(node, true));
        for (auto node : Node["Geometry"]["Tracker"]["tag_Tracker_Strip_N"])
            tag_Tracker_Strip_N.emplace_back(node.as<int>());
        tag_Tracker_Strip_Block_N = Node["Geometry"]["Tracker"]["tag_Tracker_Strip_Block_N"].as<int>();
        assert(tag_Size_Tracker.size() == tag_Pos_Tracker.size()); // Sanity Check
        assert(tag_Tracker_Angle_Gap.size() == tag_Size_Tracker.size());
        assert(tag_Tracker_Strip_N.size() == tag_Tracker_Angle_Gap.size());
        // Recoil Tracker
        rec_Size_Tracker.clear();
        rec_Pos_Tracker.clear();
        rec_Tracker_Angle_Gap.clear();
        rec_Tracker_Strip_N.clear();
        for (auto node : Node["Geometry"]["Tracker"]["rec_Size_Tracker"])
            rec_Size_Tracker.emplace_back(readV3(node, true));
        for (auto node : Node["Geometry"]["Tracker"]["rec_Pos_Tracker"])
            rec_Pos_Tracker.emplace_back(readV3(node, true));
        for (auto node : Node["Geometry"]["Tracker"]["rec_Tracker_Angle_Gap"])
            rec_Tracker_Angle_Gap.emplace_back(readV3(node, true));
        for (auto node : Node["Geometry"]["Tracker"]["rec_Tracker_Strip_N"])
            rec_Tracker_Strip_N.emplace_back(node.as<int>());
        rec_Tracker_Strip_Block_N = Node["Geometry"]["Tracker"]["rec_Tracker_Strip_Block_N"].as<int>();
        assert(rec_Size_Tracker.size() == rec_Pos_Tracker.size()); // Sanity Check
        assert(rec_Tracker_Angle_Gap.size() == rec_Size_Tracker.size());
        assert(rec_Tracker_Strip_N.size() == rec_Tracker_Angle_Gap.size());
        //----------------------------------------
        // Electromagnetic Calorimeter
        ECAL_Name = Node["Geometry"]["ECAL"]["ECAL_Name"].as<std::string>();
        MaterialStr["ECALRegion_Mat"] = Node["Geometry"]["ECAL"]["ECALRegion_Mat"].as<std::string>();
        MaterialStr["ECAL_Center_Mat"] = Node["Geometry"]["ECAL"]["ECAL_Center_Mat"].as<std::string>();
        MaterialStr["ECAL_Wrap_Mat"] = Node["Geometry"]["ECAL"]["ECAL_Wrap_Mat"].as<std::string>();
        ECAL_Center_Wrap_Size = readV3(Node["Geometry"]["ECAL"]["ECAL_Center_Wrap_Size"], true);
        ECAL_APD_Size = readV3(Node["Geometry"]["ECAL"]["ECAL_APD_Size"], true);
        ECAL_Center_Size = readV3(Node["Geometry"]["ECAL"]["ECAL_Center_Size"], true);
        ECAL_Block_No = readV3(Node["Geometry"]["ECAL"]["ECAL_Block_No"]);
        ECAL_Cell_No = readV3(Node["Geometry"]["ECAL"]["ECAL_Cell_No"]);
        ECAL_Cell_Gap = readV3(Node["Geometry"]["ECAL"]["ECAL_Cell_Gap"], true);
        //ECAL_Center_Module_No = readV3(Node["Geometry"]["ECAL"]["ECAL_Center_Module_No"]);
        //----------------------------------------
        // Hadronic Calorimeter
        HCAL_Name = Node["Geometry"]["HCAL"]["HCAL_Name"].as<std::string>();
        MaterialStr["HCAL_Absorber_Mat"] = Node["Geometry"]["HCAL"]["HCAL_Absorber_Mat"].as<std::string>();
        MaterialStr["HCALRegion_Mat"] = Node["Geometry"]["HCAL"]["HCALRegion_Mat"].as<std::string>();
        MaterialStr["HCAL_Mat"] = Node["Geometry"]["HCAL"]["HCAL_Mat"].as<std::string>();
        MaterialStr["HCAL_Wrap_Mat"] = Node["Geometry"]["HCAL"]["HCAL_Wrap_Mat"].as<std::string>();
        MaterialStr["HCAL_FiberClad_Mat"] = Node["Geometry"]["HCAL"]["HCAL_FiberClad_Mat"].IsDefined() ?
                                            Node["Geometry"]["HCAL"]["HCAL_FiberClad_Mat"].as<std::string>() : "G4_POLYSTYRENE";
        MaterialStr["HCAL_Fiber_Mat"] = Node["Geometry"]["HCAL"]["HCAL_Fiber_Mat"].IsDefined() ?
                                        Node["Geometry"]["HCAL"]["HCAL_Fiber_Mat"].as<std::string>() : "PMMA";
        HCAL_Wrap_Size = readV3(Node["Geometry"]["HCAL"]["HCAL_Wrap_Size"], true);
        HCAL_APD_Size = readV3(Node["Geometry"]["HCAL"]["HCAL_APD_Size"], true);
        HCAL_Size_Dir = readV3(Node["Geometry"]["HCAL"]["HCAL_Size_Dir"], true);
        HCAL_Module_No = readV3(Node["Geometry"]["HCAL"]["HCAL_Module_No"]);
        HCAL_Module_Gap = readV3(Node["Geometry"]["HCAL"]["HCAL_Module_Gap"], true);
        HCAL_Cell_XY_N = Node["Geometry"]["HCAL"]["HCAL_Cell_XY_N"].as<int>();
        //HCAL_Absorber_Thickness = readV2(Node["Geometry"]["HCAL"]["HCAL_Absorber_Thickness"]);
        HCAL_Absorber_Thickness_List.clear();
        for (auto i: Node["Geometry"]["HCAL"]["HCAL_Absorber_Thickness_List"]) {
            HCAL_Absorber_Thickness_List.emplace_back(
                    i[0].as<int>(),
                    i[1].as<int>(),
                    i[2].as<double>() * G4UnitDefinition::GetValueOf(i[3].as<std::string>()) );
        }

        HCAL_CaloHoleRadius = Node["Geometry"]["HCAL"]["HCAL_CaloHoleRadius"].IsDefined() ?
                              Node["Geometry"]["HCAL"]["HCAL_CaloHoleRadius"].as<double>() : 1.2 * mm;
        HCAL_FiberRadius = Node["Geometry"]["HCAL"]["HCAL_FiberRadius"].IsDefined() ?
                           Node["Geometry"]["HCAL"]["HCAL_FiberRadius"].as<double>() : 0.98 * 1.2 * mm;

        HCAL_Show_Cell = Node["Geometry"]["HCAL"]["HCAL_Show_Cell"].IsDefined() ?
                         Node["Geometry"]["HCAL"]["HCAL_Show_Cell"].as<bool>() : false;

        //========================================
        /* Optical */
        //----------------------------------------
        if_optical = Node["Optical"]["if_optical"].as<bool>();
        Optical_YieldFactor = Node["Optical"]["Optical_YieldFactor"].as<double>();
        //----------------------------------------
        // HCAL
        //
        Optical_HCAL_Yield = Node["Optical"]["HCAL"]["Yield"].IsDefined() ?
                             Node["Optical"]["HCAL"]["Yield"].as<double>() / keV : 10 / keV;
        //----------------------------------------
        // SiPM
        Optical_pulseFilePath = Node["Optical"]["SiPM"]["Optical_pulseFilePath"].as<std::string>();
        Optical_pulseScaleFactor = Node["Optical"]["SiPM"]["Optical_pulseScaleFactor"].as<double>();
        // Digitizer
        //----------------------------------------
        //now all can be change in analysis level.
        //----------------------------------------
        // LUT
        LUT_FilePath = Node["Optical"]["LUT"]["LUT_FilePath"].as<std::string>();
        LUT_Name = Node["Optical"]["LUT"]["LUT_Name"].as<std::string>();
    }
    catch (YAML::BadConversion &e) {
        std::cerr << "[Read YAML] ==> " << e.msg << std::endl;
        return false;
    }
    catch (YAML::InvalidNode &e) {
        std::cerr << "[Read YAML] ==> " << e.msg << std::endl;
        return false;
    }

    return true;
}

G4ThreeVector Control::readV3(const YAML::Node &n, bool unit) {

    // Sanity Check
    if ((n.size() != 3 && !unit) || (n.size() != 6 && unit)) {
        std::cerr << "[Reading Vector from YAML] ==> vector size is incompatible with unit..." << std::endl;
        return G4ThreeVector();
    }

    double tmp[3] = {0.};
    for (unsigned i = 0; i < 3; ++i) {
        auto extra = unit ? G4UnitDefinition::GetValueOf(n[2 * i + 1].as<std::string>()) : 1;
        tmp[i] = n[unit ? 2 * i : i].as<double>() * extra;
    }

    G4ThreeVector res;
    res.set(tmp[0], tmp[1], tmp[2]);
    return res;
}

/// \brief read value from yaml with unit
double Control::readV2(const YAML::Node &n) {
    if (n.size() != 2) {
        std::cerr << "[Reading Value with Unit from YAML] ==> Value size is incompatible with unit..." << std::endl;
        return -999999;
    }
    return n[0].as<double>() * G4UnitDefinition::GetValueOf(n[1].as<std::string>());
}

//Optical LUT loader//simplified implementation
std::map<G4String, std::pair<G4String, G4String>> Control::Optical_GetLUTDefinations() {
    auto ret = std::map<G4String, std::pair<G4String, G4String>>();
    ret["ECAL"] = std::pair<G4String, G4String>({LUT_FilePath, LUT_Name}); //this id is embeded when making LUT,
    //used to check and match the actual crystal.
    return ret;
}

DetUnitType Control::Optical_GetDetType(const G4String &cIn) {
    if (cIn == "ECAL")
        return ECAL_cube_v1;
    else
        return UnknownDet;
}

DigiScheme Control::Optical_GetDigiScheme(const G4String &cIn) {
    if (cIn == "ECAL")
        return DigiDebug;
        // return SIPM_g2_v1_20210124;
    else
        return UnknownDigi;
}

void Control::ReadAndSetVerbosity() {
    auto node = Node["verbosity"];

    for (auto subnode : node) {
        UIManager->ApplyCommand("/" + subnode.first.as<std::string>() + "/verbose " +
                                subnode.second.as<std::string>());
    }
}

void Control::ReadAndSetGPS() {
    auto node = Node["general_particle_source"]["settings"];
    UIManager = G4UImanager::GetUIpointer();
    for (auto subnode : node) {
        printf("GPS settings: /gps/%s %s \n", subnode.first.as<std::string>().data(),
               subnode.second.as<std::string>().data());
        UIManager->ApplyCommand("/gps/" + subnode.first.as<std::string>() + " " +
                                subnode.second.as<std::string>());
    }
    BeamOnNumber = Node["general_particle_source"]["beam_on"].as<int>();
}

void Control::ReadAndSetRandomSeed() {
    random_seed = Node["random_seed"]["seed"].IsDefined() ? Node["random_seed"]["seed"].as<long int>() : 0;
    random_restore_file = Node["random_seed"]["restore_file"].IsDefined()
                          ? Node["random_seed"]["restore_file"].as<std::string>()
                          : "";
}
