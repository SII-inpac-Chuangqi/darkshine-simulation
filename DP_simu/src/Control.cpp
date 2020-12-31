//
// Created by Zhang Yulei on 12/31/20.
//

#include "Control.h"


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
    ConstructG4MaterialTable();

    /* Build All independent Variables */

    //========================================
    /* Global Variables */
    //----------------------------------------
    save_all_mcp = false;
    save_geometry = false;
    check_overlaps = false;

    outfile_Name = "dp_out.root";
    tree_Name = "Dark_Photon";

    Run_Number = 0;
    Total_Event_Number = 10000;

    //----------------------------------------
    // Out Collection Options
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
    Target_Mat = G4Material::GetMaterial("G4_W");
    Target_Size = G4ThreeVector(10 * cm, 20 * cm, 350 * um);
    Target_Pos = G4ThreeVector(0 * cm, 0 * cm, 0 * cm);

    //----------------------------------------
    // Tracker
    Trk_Tar_Dis = 7.5 * mm;
    Tracker_Mat = G4Material::GetMaterial("G4_Si");
    TrackerRegion_Mat = G4Material::GetMaterial("vacuum");
    Tracker1_Rotation = 0. * radian;
    Tracker2_Rotation = 0.1 * radian;
    Tracker1_Color = G4ThreeVector(0.5, 0.5, 0.);
    Tracker2_Color = G4ThreeVector(0.5, 0.5, 0.);

    // Tagging Tracker
    tag_Tracker_MagField = G4ThreeVector(0, -1.5 * tesla, 0);
    for (int i = 0; i < 7; i++) {
        tag_Size_Tracker.emplace_back(10 * cm, 20 * cm, 0.1 * mm);
        tag_Pos_Tracker.emplace_back(0 * cm, 0 * cm, (-30. + i * 10.) * cm);
    }

    assert(tag_Size_Tracker.size() == tag_Pos_Tracker.size());

    // Recoil Tracker
    rec_Tracker_MagField = G4ThreeVector(0, -0.5 * tesla, 0);
    for (int i = 0; i < 6; i++) {
        rec_Size_Tracker.emplace_back(10 * cm, 20 * cm, 0.1 * mm);
    }
    rec_Pos_Tracker.emplace_back(0 * cm, 0 * cm, -86.25 * mm);
    rec_Pos_Tracker.emplace_back(0 * cm, 0 * cm, -71.25 * mm);
    rec_Pos_Tracker.emplace_back(0 * cm, 0 * cm, -55.25 * mm);
    rec_Pos_Tracker.emplace_back(0 * cm, 0 * cm, -40.25 * mm);
    rec_Pos_Tracker.emplace_back(0 * cm, 0 * cm, -4.25 * mm);
    rec_Pos_Tracker.emplace_back(0 * cm, 0 * cm, 86.25 * mm);

    assert(rec_Size_Tracker.size() == rec_Pos_Tracker.size());

    //----------------------------------------
    // Electromagnetic Calorimeter
    ECAL_Name = "ECAL_Center";

    ECALRegion_Mat = G4Material::GetMaterial("CarbonFiber");
    //ECAL_Center_Mat = G4Material::GetMaterial("LYSO");
    ECAL_Center_Mat = G4Material::GetMaterial("PWO4"); // X0 = 0.92 cm
    ECAL_Wrap_Mat = G4Material::GetMaterial("G4_Al");

    ECAL_Center_Wrap_Size = G4ThreeVector(0.3 * mm, 0.3 * mm, 0.3 * mm);
    ECAL_Center_Size = G4ThreeVector(2.5 * cm, 2.5 * cm, 2.0 * cm);
    ECAL_Center_Module_No = G4ThreeVector(20, 20, 18);

    //----------------------------------------
    // Hadronic Calorimeter
    HCAL_Name = "HCAL";

    HCAL_Absorber_Mat = G4Material::GetMaterial("G4_Fe");
    HCALRegion_Mat = G4Material::GetMaterial("CarbonFiber");
    HCAL_Mat = G4Material::GetMaterial("Polystyrene");
    HCAL_Wrap_Mat = G4Material::GetMaterial("G4_Al");

    HCAL_Wrap_Size = G4ThreeVector(0.3 * mm, 0.3 * mm, 0.3 * mm);
    HCAL_Size_Dir = G4ThreeVector(100 * cm + 19 * HCAL_Wrap_Size.x(), 5 * cm, 1 * cm);
    HCAL_Mod_No_Dir = G4ThreeVector(1, 20, 120);
    HCAL_Module_No = G4ThreeVector(3, 3, 1);
    HCAL_Module_Gap = 0.5 * mm;
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
    /* Optical */
    //----------------------------------------
    if_optical = false;

    //----------------------------------------
    // APD related
    APD_Mat = G4Material::GetMaterial("G4_Si");
    APD_Size = G4ThreeVector(1 * cm, 1 * cm, 1 * mm);

    Glue_Mat = G4Material::GetMaterial("G4_W");
    Glue_Size = G4ThreeVector(1 * cm, 1 * cm, 0.1 * mm);
    //----------------------------------------

}

void Control::RebuildVariables() {

    //----------------------------------------
    // Build Options
    build_target = !(build_only_tag_tracker || build_only_rec_tracker || build_only_ECAL || build_only_HCAL);
    build_tag_tracker = !(build_only_target || build_only_rec_tracker || build_only_ECAL || build_only_HCAL);
    build_rec_tracker = !(build_only_target || build_only_tag_tracker || build_only_ECAL || build_only_HCAL);
    build_ECAL = !(build_only_target || build_only_tag_tracker || build_only_rec_tracker || build_only_HCAL);
    build_HCAL = !(build_only_target || build_only_tag_tracker || build_only_rec_tracker || build_only_ECAL);

    //----------------------------------------
    // Tagging Tracker
    assert(tag_Size_Tracker.size() == tag_Pos_Tracker.size());
    tag_No_Tracker = tag_Size_Tracker.size();
    /// Size and Position of Recoil Tracker Region
    tag_Size_TrackerRegion = G4ThreeVector(
            2.0 * tag_Size_Tracker[0].x(),
            2.0 * tag_Size_Tracker[0].y(),
            tag_Pos_Tracker[tag_No_Tracker - 1].z() - tag_Pos_Tracker[0].z() + 2.0 * tag_Size_Tracker[0].z() +
            2 * eps * (tag_No_Tracker + 1));

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
            2.0 * rec_Size_Tracker[rec_No_Tracker - 1].z() + 2 * eps * (rec_No_Tracker + 1));

    rec_Pos_TrackerRegion = G4ThreeVector(
            0 * cm, 0 * cm,
            0.5 * rec_Size_TrackerRegion.z() + Trk_Tar_Dis + 0.5 * Target_Size.z());
    if (build_only_rec_tracker) rec_Pos_TrackerRegion = G4ThreeVector(0, 0, 0);

    //----------------------------------------
    // Electromagnetic Calorimeter
    Size_ECALRegion.setX((ECAL_Center_Size.x() + ECAL_Center_Wrap_Size.x()) * ECAL_Center_Module_No.x() +
                         ECAL_Center_Module_No.x() * 2 * eps);
    Size_ECALRegion.setY((ECAL_Center_Size.y() + ECAL_Center_Wrap_Size.y()) * ECAL_Center_Module_No.y() +
                         ECAL_Center_Module_No.y() * 2 * eps);
    Size_ECALRegion.setZ((ECAL_Center_Size.z() + ECAL_Center_Wrap_Size.z()) * ECAL_Center_Module_No.z() +
                         ECAL_Center_Module_No.z() * 2 * eps);

    Pos_ECALRegion = G4ThreeVector(0, 0,
                                   0.5 * Size_ECALRegion.z() + rec_Pos_TrackerRegion.z() +
                                   0.5 * rec_Size_TrackerRegion.z() + 1.0 * mm);
    if (build_only_ECAL) Pos_ECALRegion = G4ThreeVector(0, 0, 0);

    //----------------------------------------
    // Hadronic Calorimeter
    Size_HCALRegion.setX(
            HCAL_Module_No.x() * (HCAL_Size_Dir.x() + HCAL_Wrap_Size.x())
            + HCAL_Module_Gap * (HCAL_Module_No.x() - 1)
            + HCAL_Module_No.x() * 2 * eps);
    Size_HCALRegion.setY(
            HCAL_Module_No.y() * HCAL_Mod_No_Dir.y() * (HCAL_Size_Dir.y() + HCAL_Wrap_Size.y())
            + HCAL_Module_Gap * (HCAL_Module_No.y() - 1) + HCAL_Module_No.x() * 2 * eps);
    Size_HCALRegion.setZ(
            HCAL_Mod_No_Dir.z() / 2 * (2 * (HCAL_Size_Dir.z() + HCAL_Wrap_Size.z()) + HCAL_Absorber_Thickness)
            + HCAL_Module_Gap * (HCAL_Module_No.z() - 1) + HCAL_Module_No.x() * 2 * eps);

    Pos_HCALRegion = G4ThreeVector(0, 0,
                                   0.5 * Size_HCALRegion.z() + Pos_ECALRegion.z() + 0.5 * Size_ECALRegion.z() + 1 * mm);
    if (build_only_HCAL) Pos_HCALRegion = G4ThreeVector(0, 0, 0);

    //----------------------------------------
    // World
    World_Mat = G4Material::GetMaterial("vacuum");
    G4double l = 2.0 * (Pos_HCALRegion.z() + Size_HCALRegion.x());;
    Size_World = G4ThreeVector(l, l, l);
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

    /* Define Optical Properties */
    if (if_optical) {
        //
        // ------------ Generate & Add Material Properties Table ------------
        //

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

        MPT->AddConstProperty("SCINTILLATIONYIELD", 200. / MeV);
        MPT->AddConstProperty("RESOLUTIONSCALE", 1.0);
        MPT->AddConstProperty("FASTTIMECONSTANT", 40. * ns);
        MPT->AddConstProperty("YIELDRATIO", 1.);

        LYSO->SetMaterialPropertiesTable(MPT);
    }

    // Print materials
    //G4cout << *(G4Material::GetMaterialTable()) < < G4endl;
}
