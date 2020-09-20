//
// Created by Zhang Yulei on 9/19/20.
//

#include "Core/EventReader.h"

Int_t EventReader::ReadFile(const std::string &filename) {
    std::cout << "[READFILE] ==> Read in file: " + filename << std::endl;
    auto tree = new TTree;
    auto *f = new TFile(TString(filename));
    if (!f) {
        std::cerr << "[READFILE] ==> File: " + filename + " does not exist." << std::endl;
        return -1;
    }
    f->GetObject("Dark_Photon", tree);

    Init(tree);

    Entries = fChain->GetEntriesFast();

    return 1;
}

void EventReader::Convert() {
    // Initialization
    evt->Initialization();

    // Convert Input root to DEvent format

    // Other Variables
    evt->setRunId(RunNumber);
    evt->setEventId(EventNumber);


    // register raw MC collection
    ConvertMC();

    // Register Step Collection
    ConvertStep();

    /*
     * Register Calorimeter Collections
     *      Tagging Tracker 1 & 2
     *      Recoiled Tracker 1 & 2
     *      ECAL_Center
     *      ECAL_Outer 1 & 2 & 3 & 4
     *      HCAL 0 to 8
     */
    ConvertTagTrk();
    ConvertRecTrk();
    ConvertECAL_Center();
    ConvertECAL_Outer();
    ConvertHCAL();
}

/*                     */
/* From ROOT MakeClass */
/*                     */

Int_t EventReader::GetEntry(Long64_t entry) const {
// Read contents of entry.
    if (!fChain) return 0;
    return fChain->GetEntry(entry);
}

void EventReader::Init(TTree *tree) {
    // The Init() function is called when the selector needs to initialize
    // a new tree or chain. Typically here the branch addresses and branch
    // pointers of the tree will be set.
    // It is normally not necessary to make changes to the generated
    // code, but the routine can be extended by the user if needed.
    // Init() will be called many times when running on PROOF
    // (once per file to be processed).

    // Set object pointer
    TRUTH_MC_id = nullptr;
    TRUTH_MC_PDG = nullptr;
    TRUTH_MC_ParentID = nullptr;
    TRUTH_MC_Mom = nullptr;
    TRUTH_MC_E = nullptr;
    TRUTH_MC_Eremain = nullptr;
    TRUTH_MC_VPos = nullptr;
    TRUTH_MC_EPos = nullptr;
    TRUTH_MC_ProcessName = nullptr;
    ip_Pos = nullptr;
    ip_Momentum = nullptr;
    ip_Energy = nullptr;
    ip_PVName = nullptr;
    ip_ProcessName = nullptr;
    TagTrk1_Type = nullptr;
    TagTrk1_DetectorID = nullptr;
    TagTrk1_DetectorID_x = nullptr;
    TagTrk1_DetectorID_y = nullptr;
    TagTrk1_DetectorID_z = nullptr;
    TagTrk1_Time = nullptr;
    TagTrk1_Edep = nullptr;
    TagTrk1_EdepEM = nullptr;
    TagTrk1_EdepHad = nullptr;
    TagTrk1_X = nullptr;
    TagTrk1_Y = nullptr;
    TagTrk1_Z = nullptr;
    TagTrk2_Type = nullptr;
    TagTrk2_DetectorID = nullptr;
    TagTrk2_DetectorID_x = nullptr;
    TagTrk2_DetectorID_y = nullptr;
    TagTrk2_DetectorID_z = nullptr;
    TagTrk2_Time = nullptr;
    TagTrk2_Edep = nullptr;
    TagTrk2_EdepEM = nullptr;
    TagTrk2_EdepHad = nullptr;
    TagTrk2_X = nullptr;
    TagTrk2_Y = nullptr;
    TagTrk2_Z = nullptr;
    RecTrk1_Type = nullptr;
    RecTrk1_DetectorID = nullptr;
    RecTrk1_DetectorID_x = nullptr;
    RecTrk1_DetectorID_y = nullptr;
    RecTrk1_DetectorID_z = nullptr;
    RecTrk1_Time = nullptr;
    RecTrk1_Edep = nullptr;
    RecTrk1_EdepEM = nullptr;
    RecTrk1_EdepHad = nullptr;
    RecTrk1_X = nullptr;
    RecTrk1_Y = nullptr;
    RecTrk1_Z = nullptr;
    RecTrk2_Type = nullptr;
    RecTrk2_DetectorID = nullptr;
    RecTrk2_DetectorID_x = nullptr;
    RecTrk2_DetectorID_y = nullptr;
    RecTrk2_DetectorID_z = nullptr;
    RecTrk2_Time = nullptr;
    RecTrk2_Edep = nullptr;
    RecTrk2_EdepEM = nullptr;
    RecTrk2_EdepHad = nullptr;
    RecTrk2_X = nullptr;
    RecTrk2_Y = nullptr;
    RecTrk2_Z = nullptr;
    ECAL_Center_Type = nullptr;
    ECAL_Center_DetectorID = nullptr;
    ECAL_Center_DetectorID_x = nullptr;
    ECAL_Center_DetectorID_y = nullptr;
    ECAL_Center_DetectorID_z = nullptr;
    ECAL_Center_Time = nullptr;
    ECAL_Center_Edep = nullptr;
    ECAL_Center_EdepEM = nullptr;
    ECAL_Center_EdepHad = nullptr;
    ECAL_Center_X = nullptr;
    ECAL_Center_Y = nullptr;
    ECAL_Center_Z = nullptr;
    ECAL_Outer_1_Type = nullptr;
    ECAL_Outer_1_DetectorID = nullptr;
    ECAL_Outer_1_DetectorID_x = nullptr;
    ECAL_Outer_1_DetectorID_y = nullptr;
    ECAL_Outer_1_DetectorID_z = nullptr;
    ECAL_Outer_1_Time = nullptr;
    ECAL_Outer_1_Edep = nullptr;
    ECAL_Outer_1_EdepEM = nullptr;
    ECAL_Outer_1_EdepHad = nullptr;
    ECAL_Outer_1_X = nullptr;
    ECAL_Outer_1_Y = nullptr;
    ECAL_Outer_1_Z = nullptr;
    ECAL_Outer_2_Type = nullptr;
    ECAL_Outer_2_DetectorID = nullptr;
    ECAL_Outer_2_DetectorID_x = nullptr;
    ECAL_Outer_2_DetectorID_y = nullptr;
    ECAL_Outer_2_DetectorID_z = nullptr;
    ECAL_Outer_2_Time = nullptr;
    ECAL_Outer_2_Edep = nullptr;
    ECAL_Outer_2_EdepEM = nullptr;
    ECAL_Outer_2_EdepHad = nullptr;
    ECAL_Outer_2_X = nullptr;
    ECAL_Outer_2_Y = nullptr;
    ECAL_Outer_2_Z = nullptr;
    ECAL_Outer_3_Type = nullptr;
    ECAL_Outer_3_DetectorID = nullptr;
    ECAL_Outer_3_DetectorID_x = nullptr;
    ECAL_Outer_3_DetectorID_y = nullptr;
    ECAL_Outer_3_DetectorID_z = nullptr;
    ECAL_Outer_3_Time = nullptr;
    ECAL_Outer_3_Edep = nullptr;
    ECAL_Outer_3_EdepEM = nullptr;
    ECAL_Outer_3_EdepHad = nullptr;
    ECAL_Outer_3_X = nullptr;
    ECAL_Outer_3_Y = nullptr;
    ECAL_Outer_3_Z = nullptr;
    ECAL_Outer_4_Type = nullptr;
    ECAL_Outer_4_DetectorID = nullptr;
    ECAL_Outer_4_DetectorID_x = nullptr;
    ECAL_Outer_4_DetectorID_y = nullptr;
    ECAL_Outer_4_DetectorID_z = nullptr;
    ECAL_Outer_4_Time = nullptr;
    ECAL_Outer_4_Edep = nullptr;
    ECAL_Outer_4_EdepEM = nullptr;
    ECAL_Outer_4_EdepHad = nullptr;
    ECAL_Outer_4_X = nullptr;
    ECAL_Outer_4_Y = nullptr;
    ECAL_Outer_4_Z = nullptr;
    HCAL_0_Type = nullptr;
    HCAL_0_DetectorID = nullptr;
    HCAL_0_DetectorID_x = nullptr;
    HCAL_0_DetectorID_y = nullptr;
    HCAL_0_DetectorID_z = nullptr;
    HCAL_0_Time = nullptr;
    HCAL_0_Edep = nullptr;
    HCAL_0_EdepEM = nullptr;
    HCAL_0_EdepHad = nullptr;
    HCAL_0_X = nullptr;
    HCAL_0_Y = nullptr;
    HCAL_0_Z = nullptr;
    HCAL_1_Type = nullptr;
    HCAL_1_DetectorID = nullptr;
    HCAL_1_DetectorID_x = nullptr;
    HCAL_1_DetectorID_y = nullptr;
    HCAL_1_DetectorID_z = nullptr;
    HCAL_1_Time = nullptr;
    HCAL_1_Edep = nullptr;
    HCAL_1_EdepEM = nullptr;
    HCAL_1_EdepHad = nullptr;
    HCAL_1_X = nullptr;
    HCAL_1_Y = nullptr;
    HCAL_1_Z = nullptr;
    HCAL_2_Type = nullptr;
    HCAL_2_DetectorID = nullptr;
    HCAL_2_DetectorID_x = nullptr;
    HCAL_2_DetectorID_y = nullptr;
    HCAL_2_DetectorID_z = nullptr;
    HCAL_2_Time = nullptr;
    HCAL_2_Edep = nullptr;
    HCAL_2_EdepEM = nullptr;
    HCAL_2_EdepHad = nullptr;
    HCAL_2_X = nullptr;
    HCAL_2_Y = nullptr;
    HCAL_2_Z = nullptr;
    HCAL_3_Type = nullptr;
    HCAL_3_DetectorID = nullptr;
    HCAL_3_DetectorID_x = nullptr;
    HCAL_3_DetectorID_y = nullptr;
    HCAL_3_DetectorID_z = nullptr;
    HCAL_3_Time = nullptr;
    HCAL_3_Edep = nullptr;
    HCAL_3_EdepEM = nullptr;
    HCAL_3_EdepHad = nullptr;
    HCAL_3_X = nullptr;
    HCAL_3_Y = nullptr;
    HCAL_3_Z = nullptr;
    HCAL_4_Type = nullptr;
    HCAL_4_DetectorID = nullptr;
    HCAL_4_DetectorID_x = nullptr;
    HCAL_4_DetectorID_y = nullptr;
    HCAL_4_DetectorID_z = nullptr;
    HCAL_4_Time = nullptr;
    HCAL_4_Edep = nullptr;
    HCAL_4_EdepEM = nullptr;
    HCAL_4_EdepHad = nullptr;
    HCAL_4_X = nullptr;
    HCAL_4_Y = nullptr;
    HCAL_4_Z = nullptr;
    HCAL_5_Type = nullptr;
    HCAL_5_DetectorID = nullptr;
    HCAL_5_DetectorID_x = nullptr;
    HCAL_5_DetectorID_y = nullptr;
    HCAL_5_DetectorID_z = nullptr;
    HCAL_5_Time = nullptr;
    HCAL_5_Edep = nullptr;
    HCAL_5_EdepEM = nullptr;
    HCAL_5_EdepHad = nullptr;
    HCAL_5_X = nullptr;
    HCAL_5_Y = nullptr;
    HCAL_5_Z = nullptr;
    HCAL_6_Type = nullptr;
    HCAL_6_DetectorID = nullptr;
    HCAL_6_DetectorID_x = nullptr;
    HCAL_6_DetectorID_y = nullptr;
    HCAL_6_DetectorID_z = nullptr;
    HCAL_6_Time = nullptr;
    HCAL_6_Edep = nullptr;
    HCAL_6_EdepEM = nullptr;
    HCAL_6_EdepHad = nullptr;
    HCAL_6_X = nullptr;
    HCAL_6_Y = nullptr;
    HCAL_6_Z = nullptr;
    HCAL_7_Type = nullptr;
    HCAL_7_DetectorID = nullptr;
    HCAL_7_DetectorID_x = nullptr;
    HCAL_7_DetectorID_y = nullptr;
    HCAL_7_DetectorID_z = nullptr;
    HCAL_7_Time = nullptr;
    HCAL_7_Edep = nullptr;
    HCAL_7_EdepEM = nullptr;
    HCAL_7_EdepHad = nullptr;
    HCAL_7_X = nullptr;
    HCAL_7_Y = nullptr;
    HCAL_7_Z = nullptr;
    HCAL_8_Type = nullptr;
    HCAL_8_DetectorID = nullptr;
    HCAL_8_DetectorID_x = nullptr;
    HCAL_8_DetectorID_y = nullptr;
    HCAL_8_DetectorID_z = nullptr;
    HCAL_8_Time = nullptr;
    HCAL_8_Edep = nullptr;
    HCAL_8_EdepEM = nullptr;
    HCAL_8_EdepHad = nullptr;
    HCAL_8_X = nullptr;
    HCAL_8_Y = nullptr;
    HCAL_8_Z = nullptr;
    // Set branch addresses and branch pointers
    if (!tree) return;
    fChain = tree;
    fCurrent = -1;
    fChain->SetMakeClass(1);

    fChain->SetBranchAddress("RunNumber", &RunNumber, &b_RunNumber);
    fChain->SetBranchAddress("EventNumber", &EventNumber, &b_EventNumber);
    fChain->SetBranchAddress("Rndm", Rndm, &b_Rndm);
    fChain->SetBranchAddress("TRUTH_e1_Momentum", TRUTH_e1_Momentum, &b_t_e1_Momentum);
    fChain->SetBranchAddress("TRUTH_e1_VPos", TRUTH_e1_VPos, &b_t_e1_VPos);
    fChain->SetBranchAddress("TRUTH_e2_Momentum", TRUTH_e2_Momentum, &b_t_e2_Momentum);
    fChain->SetBranchAddress("TRUTH_e2_VPos", TRUTH_e2_VPos, &b_t_e2_VPos);
    fChain->SetBranchAddress("TRUTH_MC_Nb", &TRUTH_MC_Nb, &b_TRUTH_MC_Nb);
    fChain->SetBranchAddress("TRUTH_MC_id", &TRUTH_MC_id, &b_TRUTH_MC_id);
    fChain->SetBranchAddress("TRUTH_MC_PDG", &TRUTH_MC_PDG, &b_TRUTH_MC_PDG);
    fChain->SetBranchAddress("TRUTH_MC_ParentID", &TRUTH_MC_ParentID, &b_TRUTH_MC_ParentID);
    fChain->SetBranchAddress("TRUTH_MC_Mom", &TRUTH_MC_Mom, &b_TRUTH_MC_Mom);
    fChain->SetBranchAddress("TRUTH_MC_E", &TRUTH_MC_E, &b_TRUTH_MC_E);
    fChain->SetBranchAddress("TRUTH_MC_Eremain", &TRUTH_MC_Eremain, &b_TRUTH_MC_Eremain);
    fChain->SetBranchAddress("TRUTH_MC_VPos", &TRUTH_MC_VPos, &b_TRUTH_MC_VPos);
    fChain->SetBranchAddress("TRUTH_MC_EPos", &TRUTH_MC_EPos, &b_TRUTH_MC_EPos);
    fChain->SetBranchAddress("TRUTH_MC_ProcessName", &TRUTH_MC_ProcessName, &b_TRUTH_MC_ProcessName);
    fChain->SetBranchAddress("TRUTH_MC_PNEnergy_Tar", &TRUTH_MC_PNEnergy_Tar, &b_TRUTH_MC_PNEnergy_Tar);
    fChain->SetBranchAddress("TRUTH_MC_PNEnergy_ECal", &TRUTH_MC_PNEnergy_ECal, &b_TRUTH_MC_PNEnergy_ECal);
    fChain->SetBranchAddress("TRUTH_MC_Eleak_ECAL", &TRUTH_MC_Eleak_ECAL, &b_TRUTH_MC_Eleak_ECAL);
    fChain->SetBranchAddress("ip_Pos", &ip_Pos, &b_ip_Pos);
    fChain->SetBranchAddress("ip_Momentum", &ip_Momentum, &b_ip_Momentum);
    fChain->SetBranchAddress("ip_Energy", &ip_Energy, &b_ip_Energy);
    fChain->SetBranchAddress("ip_PVName", &ip_PVName, &b_ip_PVName);
    fChain->SetBranchAddress("ip_ProcessName", &ip_ProcessName, &b_ip_ProcessName);
    fChain->SetBranchAddress("TagTrk1_No", &TagTrk1_No, &b_TagTrk1_No);
    fChain->SetBranchAddress("TagTrk1_Eleak_Wrapper", &TagTrk1_Eleak_Wrapper, &b_TagTrk1_Eleak_Wrapper);
    fChain->SetBranchAddress("TagTrk1_Type", &TagTrk1_Type, &b_TagTrk1_Type);
    fChain->SetBranchAddress("TagTrk1_DetectorID", &TagTrk1_DetectorID, &b_TagTrk1_DetectorID);
    fChain->SetBranchAddress("TagTrk1_DetectorID_x", &TagTrk1_DetectorID_x, &b_TagTrk1_DetectorID_x);
    fChain->SetBranchAddress("TagTrk1_DetectorID_y", &TagTrk1_DetectorID_y, &b_TagTrk1_DetectorID_y);
    fChain->SetBranchAddress("TagTrk1_DetectorID_z", &TagTrk1_DetectorID_z, &b_TagTrk1_DetectorID_z);
    fChain->SetBranchAddress("TagTrk1_Time", &TagTrk1_Time, &b_TagTrk1_Time);
    fChain->SetBranchAddress("TagTrk1_Edep", &TagTrk1_Edep, &b_TagTrk1_Edep);
    fChain->SetBranchAddress("TagTrk1_EdepEM", &TagTrk1_EdepEM, &b_TagTrk1_EdepEM);
    fChain->SetBranchAddress("TagTrk1_EdepHad", &TagTrk1_EdepHad, &b_TagTrk1_EdepHad);
    fChain->SetBranchAddress("TagTrk1_X", &TagTrk1_X, &b_TagTrk1_X);
    fChain->SetBranchAddress("TagTrk1_Y", &TagTrk1_Y, &b_TagTrk1_Y);
    fChain->SetBranchAddress("TagTrk1_Z", &TagTrk1_Z, &b_TagTrk1_Z);
    fChain->SetBranchAddress("TagTrk2_No", &TagTrk2_No, &b_TagTrk2_No);
    fChain->SetBranchAddress("TagTrk2_Eleak_Wrapper", &TagTrk2_Eleak_Wrapper, &b_TagTrk2_Eleak_Wrapper);
    fChain->SetBranchAddress("TagTrk2_Type", &TagTrk2_Type, &b_TagTrk2_Type);
    fChain->SetBranchAddress("TagTrk2_DetectorID", &TagTrk2_DetectorID, &b_TagTrk2_DetectorID);
    fChain->SetBranchAddress("TagTrk2_DetectorID_x", &TagTrk2_DetectorID_x, &b_TagTrk2_DetectorID_x);
    fChain->SetBranchAddress("TagTrk2_DetectorID_y", &TagTrk2_DetectorID_y, &b_TagTrk2_DetectorID_y);
    fChain->SetBranchAddress("TagTrk2_DetectorID_z", &TagTrk2_DetectorID_z, &b_TagTrk2_DetectorID_z);
    fChain->SetBranchAddress("TagTrk2_Time", &TagTrk2_Time, &b_TagTrk2_Time);
    fChain->SetBranchAddress("TagTrk2_Edep", &TagTrk2_Edep, &b_TagTrk2_Edep);
    fChain->SetBranchAddress("TagTrk2_EdepEM", &TagTrk2_EdepEM, &b_TagTrk2_EdepEM);
    fChain->SetBranchAddress("TagTrk2_EdepHad", &TagTrk2_EdepHad, &b_TagTrk2_EdepHad);
    fChain->SetBranchAddress("TagTrk2_X", &TagTrk2_X, &b_TagTrk2_X);
    fChain->SetBranchAddress("TagTrk2_Y", &TagTrk2_Y, &b_TagTrk2_Y);
    fChain->SetBranchAddress("TagTrk2_Z", &TagTrk2_Z, &b_TagTrk2_Z);
    fChain->SetBranchAddress("RecTrk1_No", &RecTrk1_No, &b_RecTrk1_No);
    fChain->SetBranchAddress("RecTrk1_Eleak_Wrapper", &RecTrk1_Eleak_Wrapper, &b_RecTrk1_Eleak_Wrapper);
    fChain->SetBranchAddress("RecTrk1_Type", &RecTrk1_Type, &b_RecTrk1_Type);
    fChain->SetBranchAddress("RecTrk1_DetectorID", &RecTrk1_DetectorID, &b_RecTrk1_DetectorID);
    fChain->SetBranchAddress("RecTrk1_DetectorID_x", &RecTrk1_DetectorID_x, &b_RecTrk1_DetectorID_x);
    fChain->SetBranchAddress("RecTrk1_DetectorID_y", &RecTrk1_DetectorID_y, &b_RecTrk1_DetectorID_y);
    fChain->SetBranchAddress("RecTrk1_DetectorID_z", &RecTrk1_DetectorID_z, &b_RecTrk1_DetectorID_z);
    fChain->SetBranchAddress("RecTrk1_Time", &RecTrk1_Time, &b_RecTrk1_Time);
    fChain->SetBranchAddress("RecTrk1_Edep", &RecTrk1_Edep, &b_RecTrk1_Edep);
    fChain->SetBranchAddress("RecTrk1_EdepEM", &RecTrk1_EdepEM, &b_RecTrk1_EdepEM);
    fChain->SetBranchAddress("RecTrk1_EdepHad", &RecTrk1_EdepHad, &b_RecTrk1_EdepHad);
    fChain->SetBranchAddress("RecTrk1_X", &RecTrk1_X, &b_RecTrk1_X);
    fChain->SetBranchAddress("RecTrk1_Y", &RecTrk1_Y, &b_RecTrk1_Y);
    fChain->SetBranchAddress("RecTrk1_Z", &RecTrk1_Z, &b_RecTrk1_Z);
    fChain->SetBranchAddress("RecTrk2_No", &RecTrk2_No, &b_RecTrk2_No);
    fChain->SetBranchAddress("RecTrk2_Eleak_Wrapper", &RecTrk2_Eleak_Wrapper, &b_RecTrk2_Eleak_Wrapper);
    fChain->SetBranchAddress("RecTrk2_Type", &RecTrk2_Type, &b_RecTrk2_Type);
    fChain->SetBranchAddress("RecTrk2_DetectorID", &RecTrk2_DetectorID, &b_RecTrk2_DetectorID);
    fChain->SetBranchAddress("RecTrk2_DetectorID_x", &RecTrk2_DetectorID_x, &b_RecTrk2_DetectorID_x);
    fChain->SetBranchAddress("RecTrk2_DetectorID_y", &RecTrk2_DetectorID_y, &b_RecTrk2_DetectorID_y);
    fChain->SetBranchAddress("RecTrk2_DetectorID_z", &RecTrk2_DetectorID_z, &b_RecTrk2_DetectorID_z);
    fChain->SetBranchAddress("RecTrk2_Time", &RecTrk2_Time, &b_RecTrk2_Time);
    fChain->SetBranchAddress("RecTrk2_Edep", &RecTrk2_Edep, &b_RecTrk2_Edep);
    fChain->SetBranchAddress("RecTrk2_EdepEM", &RecTrk2_EdepEM, &b_RecTrk2_EdepEM);
    fChain->SetBranchAddress("RecTrk2_EdepHad", &RecTrk2_EdepHad, &b_RecTrk2_EdepHad);
    fChain->SetBranchAddress("RecTrk2_X", &RecTrk2_X, &b_RecTrk2_X);
    fChain->SetBranchAddress("RecTrk2_Y", &RecTrk2_Y, &b_RecTrk2_Y);
    fChain->SetBranchAddress("RecTrk2_Z", &RecTrk2_Z, &b_RecTrk2_Z);
    fChain->SetBranchAddress("ECAL_Center_No", &ECAL_Center_No, &b_ECAL_Center_No);
    fChain->SetBranchAddress("ECAL_Center_Eleak_Wrapper", &ECAL_Center_Eleak_Wrapper, &b_ECAL_Center_Eleak_Wrapper);
    fChain->SetBranchAddress("ECAL_Center_Type", &ECAL_Center_Type, &b_ECAL_Center_Type);
    fChain->SetBranchAddress("ECAL_Center_DetectorID", &ECAL_Center_DetectorID, &b_ECAL_Center_DetectorID);
    fChain->SetBranchAddress("ECAL_Center_DetectorID_x", &ECAL_Center_DetectorID_x, &b_ECAL_Center_DetectorID_x);
    fChain->SetBranchAddress("ECAL_Center_DetectorID_y", &ECAL_Center_DetectorID_y, &b_ECAL_Center_DetectorID_y);
    fChain->SetBranchAddress("ECAL_Center_DetectorID_z", &ECAL_Center_DetectorID_z, &b_ECAL_Center_DetectorID_z);
    fChain->SetBranchAddress("ECAL_Center_Time", &ECAL_Center_Time, &b_ECAL_Center_Time);
    fChain->SetBranchAddress("ECAL_Center_Edep", &ECAL_Center_Edep, &b_ECAL_Center_Edep);
    fChain->SetBranchAddress("ECAL_Center_EdepEM", &ECAL_Center_EdepEM, &b_ECAL_Center_EdepEM);
    fChain->SetBranchAddress("ECAL_Center_EdepHad", &ECAL_Center_EdepHad, &b_ECAL_Center_EdepHad);
    fChain->SetBranchAddress("ECAL_Center_X", &ECAL_Center_X, &b_ECAL_Center_X);
    fChain->SetBranchAddress("ECAL_Center_Y", &ECAL_Center_Y, &b_ECAL_Center_Y);
    fChain->SetBranchAddress("ECAL_Center_Z", &ECAL_Center_Z, &b_ECAL_Center_Z);
    fChain->SetBranchAddress("ECAL_Outer_1_No", &ECAL_Outer_1_No, &b_ECAL_Outer_1_No);
    fChain->SetBranchAddress("ECAL_Outer_1_Eleak_Wrapper", &ECAL_Outer_1_Eleak_Wrapper, &b_ECAL_Outer_1_Eleak_Wrapper);
    fChain->SetBranchAddress("ECAL_Outer_1_Type", &ECAL_Outer_1_Type, &b_ECAL_Outer_1_Type);
    fChain->SetBranchAddress("ECAL_Outer_1_DetectorID", &ECAL_Outer_1_DetectorID, &b_ECAL_Outer_1_DetectorID);
    fChain->SetBranchAddress("ECAL_Outer_1_DetectorID_x", &ECAL_Outer_1_DetectorID_x, &b_ECAL_Outer_1_DetectorID_x);
    fChain->SetBranchAddress("ECAL_Outer_1_DetectorID_y", &ECAL_Outer_1_DetectorID_y, &b_ECAL_Outer_1_DetectorID_y);
    fChain->SetBranchAddress("ECAL_Outer_1_DetectorID_z", &ECAL_Outer_1_DetectorID_z, &b_ECAL_Outer_1_DetectorID_z);
    fChain->SetBranchAddress("ECAL_Outer_1_Time", &ECAL_Outer_1_Time, &b_ECAL_Outer_1_Time);
    fChain->SetBranchAddress("ECAL_Outer_1_Edep", &ECAL_Outer_1_Edep, &b_ECAL_Outer_1_Edep);
    fChain->SetBranchAddress("ECAL_Outer_1_EdepEM", &ECAL_Outer_1_EdepEM, &b_ECAL_Outer_1_EdepEM);
    fChain->SetBranchAddress("ECAL_Outer_1_EdepHad", &ECAL_Outer_1_EdepHad, &b_ECAL_Outer_1_EdepHad);
    fChain->SetBranchAddress("ECAL_Outer_1_X", &ECAL_Outer_1_X, &b_ECAL_Outer_1_X);
    fChain->SetBranchAddress("ECAL_Outer_1_Y", &ECAL_Outer_1_Y, &b_ECAL_Outer_1_Y);
    fChain->SetBranchAddress("ECAL_Outer_1_Z", &ECAL_Outer_1_Z, &b_ECAL_Outer_1_Z);
    fChain->SetBranchAddress("ECAL_Outer_2_No", &ECAL_Outer_2_No, &b_ECAL_Outer_2_No);
    fChain->SetBranchAddress("ECAL_Outer_2_Eleak_Wrapper", &ECAL_Outer_2_Eleak_Wrapper, &b_ECAL_Outer_2_Eleak_Wrapper);
    fChain->SetBranchAddress("ECAL_Outer_2_Type", &ECAL_Outer_2_Type, &b_ECAL_Outer_2_Type);
    fChain->SetBranchAddress("ECAL_Outer_2_DetectorID", &ECAL_Outer_2_DetectorID, &b_ECAL_Outer_2_DetectorID);
    fChain->SetBranchAddress("ECAL_Outer_2_DetectorID_x", &ECAL_Outer_2_DetectorID_x, &b_ECAL_Outer_2_DetectorID_x);
    fChain->SetBranchAddress("ECAL_Outer_2_DetectorID_y", &ECAL_Outer_2_DetectorID_y, &b_ECAL_Outer_2_DetectorID_y);
    fChain->SetBranchAddress("ECAL_Outer_2_DetectorID_z", &ECAL_Outer_2_DetectorID_z, &b_ECAL_Outer_2_DetectorID_z);
    fChain->SetBranchAddress("ECAL_Outer_2_Time", &ECAL_Outer_2_Time, &b_ECAL_Outer_2_Time);
    fChain->SetBranchAddress("ECAL_Outer_2_Edep", &ECAL_Outer_2_Edep, &b_ECAL_Outer_2_Edep);
    fChain->SetBranchAddress("ECAL_Outer_2_EdepEM", &ECAL_Outer_2_EdepEM, &b_ECAL_Outer_2_EdepEM);
    fChain->SetBranchAddress("ECAL_Outer_2_EdepHad", &ECAL_Outer_2_EdepHad, &b_ECAL_Outer_2_EdepHad);
    fChain->SetBranchAddress("ECAL_Outer_2_X", &ECAL_Outer_2_X, &b_ECAL_Outer_2_X);
    fChain->SetBranchAddress("ECAL_Outer_2_Y", &ECAL_Outer_2_Y, &b_ECAL_Outer_2_Y);
    fChain->SetBranchAddress("ECAL_Outer_2_Z", &ECAL_Outer_2_Z, &b_ECAL_Outer_2_Z);
    fChain->SetBranchAddress("ECAL_Outer_3_No", &ECAL_Outer_3_No, &b_ECAL_Outer_3_No);
    fChain->SetBranchAddress("ECAL_Outer_3_Eleak_Wrapper", &ECAL_Outer_3_Eleak_Wrapper, &b_ECAL_Outer_3_Eleak_Wrapper);
    fChain->SetBranchAddress("ECAL_Outer_3_Type", &ECAL_Outer_3_Type, &b_ECAL_Outer_3_Type);
    fChain->SetBranchAddress("ECAL_Outer_3_DetectorID", &ECAL_Outer_3_DetectorID, &b_ECAL_Outer_3_DetectorID);
    fChain->SetBranchAddress("ECAL_Outer_3_DetectorID_x", &ECAL_Outer_3_DetectorID_x, &b_ECAL_Outer_3_DetectorID_x);
    fChain->SetBranchAddress("ECAL_Outer_3_DetectorID_y", &ECAL_Outer_3_DetectorID_y, &b_ECAL_Outer_3_DetectorID_y);
    fChain->SetBranchAddress("ECAL_Outer_3_DetectorID_z", &ECAL_Outer_3_DetectorID_z, &b_ECAL_Outer_3_DetectorID_z);
    fChain->SetBranchAddress("ECAL_Outer_3_Time", &ECAL_Outer_3_Time, &b_ECAL_Outer_3_Time);
    fChain->SetBranchAddress("ECAL_Outer_3_Edep", &ECAL_Outer_3_Edep, &b_ECAL_Outer_3_Edep);
    fChain->SetBranchAddress("ECAL_Outer_3_EdepEM", &ECAL_Outer_3_EdepEM, &b_ECAL_Outer_3_EdepEM);
    fChain->SetBranchAddress("ECAL_Outer_3_EdepHad", &ECAL_Outer_3_EdepHad, &b_ECAL_Outer_3_EdepHad);
    fChain->SetBranchAddress("ECAL_Outer_3_X", &ECAL_Outer_3_X, &b_ECAL_Outer_3_X);
    fChain->SetBranchAddress("ECAL_Outer_3_Y", &ECAL_Outer_3_Y, &b_ECAL_Outer_3_Y);
    fChain->SetBranchAddress("ECAL_Outer_3_Z", &ECAL_Outer_3_Z, &b_ECAL_Outer_3_Z);
    fChain->SetBranchAddress("ECAL_Outer_4_No", &ECAL_Outer_4_No, &b_ECAL_Outer_4_No);
    fChain->SetBranchAddress("ECAL_Outer_4_Eleak_Wrapper", &ECAL_Outer_4_Eleak_Wrapper, &b_ECAL_Outer_4_Eleak_Wrapper);
    fChain->SetBranchAddress("ECAL_Outer_4_Type", &ECAL_Outer_4_Type, &b_ECAL_Outer_4_Type);
    fChain->SetBranchAddress("ECAL_Outer_4_DetectorID", &ECAL_Outer_4_DetectorID, &b_ECAL_Outer_4_DetectorID);
    fChain->SetBranchAddress("ECAL_Outer_4_DetectorID_x", &ECAL_Outer_4_DetectorID_x, &b_ECAL_Outer_4_DetectorID_x);
    fChain->SetBranchAddress("ECAL_Outer_4_DetectorID_y", &ECAL_Outer_4_DetectorID_y, &b_ECAL_Outer_4_DetectorID_y);
    fChain->SetBranchAddress("ECAL_Outer_4_DetectorID_z", &ECAL_Outer_4_DetectorID_z, &b_ECAL_Outer_4_DetectorID_z);
    fChain->SetBranchAddress("ECAL_Outer_4_Time", &ECAL_Outer_4_Time, &b_ECAL_Outer_4_Time);
    fChain->SetBranchAddress("ECAL_Outer_4_Edep", &ECAL_Outer_4_Edep, &b_ECAL_Outer_4_Edep);
    fChain->SetBranchAddress("ECAL_Outer_4_EdepEM", &ECAL_Outer_4_EdepEM, &b_ECAL_Outer_4_EdepEM);
    fChain->SetBranchAddress("ECAL_Outer_4_EdepHad", &ECAL_Outer_4_EdepHad, &b_ECAL_Outer_4_EdepHad);
    fChain->SetBranchAddress("ECAL_Outer_4_X", &ECAL_Outer_4_X, &b_ECAL_Outer_4_X);
    fChain->SetBranchAddress("ECAL_Outer_4_Y", &ECAL_Outer_4_Y, &b_ECAL_Outer_4_Y);
    fChain->SetBranchAddress("ECAL_Outer_4_Z", &ECAL_Outer_4_Z, &b_ECAL_Outer_4_Z);
    fChain->SetBranchAddress("HCAL_0_No", &HCAL_0_No, &b_HCAL_0_No);
    fChain->SetBranchAddress("HCAL_0_Eleak_Wrapper", &HCAL_0_Eleak_Wrapper, &b_HCAL_0_Eleak_Wrapper);
    fChain->SetBranchAddress("HCAL_0_Type", &HCAL_0_Type, &b_HCAL_0_Type);
    fChain->SetBranchAddress("HCAL_0_DetectorID", &HCAL_0_DetectorID, &b_HCAL_0_DetectorID);
    fChain->SetBranchAddress("HCAL_0_DetectorID_x", &HCAL_0_DetectorID_x, &b_HCAL_0_DetectorID_x);
    fChain->SetBranchAddress("HCAL_0_DetectorID_y", &HCAL_0_DetectorID_y, &b_HCAL_0_DetectorID_y);
    fChain->SetBranchAddress("HCAL_0_DetectorID_z", &HCAL_0_DetectorID_z, &b_HCAL_0_DetectorID_z);
    fChain->SetBranchAddress("HCAL_0_Time", &HCAL_0_Time, &b_HCAL_0_Time);
    fChain->SetBranchAddress("HCAL_0_Edep", &HCAL_0_Edep, &b_HCAL_0_Edep);
    fChain->SetBranchAddress("HCAL_0_EdepEM", &HCAL_0_EdepEM, &b_HCAL_0_EdepEM);
    fChain->SetBranchAddress("HCAL_0_EdepHad", &HCAL_0_EdepHad, &b_HCAL_0_EdepHad);
    fChain->SetBranchAddress("HCAL_0_X", &HCAL_0_X, &b_HCAL_0_X);
    fChain->SetBranchAddress("HCAL_0_Y", &HCAL_0_Y, &b_HCAL_0_Y);
    fChain->SetBranchAddress("HCAL_0_Z", &HCAL_0_Z, &b_HCAL_0_Z);
    fChain->SetBranchAddress("HCAL_1_No", &HCAL_1_No, &b_HCAL_1_No);
    fChain->SetBranchAddress("HCAL_1_Eleak_Wrapper", &HCAL_1_Eleak_Wrapper, &b_HCAL_1_Eleak_Wrapper);
    fChain->SetBranchAddress("HCAL_1_Type", &HCAL_1_Type, &b_HCAL_1_Type);
    fChain->SetBranchAddress("HCAL_1_DetectorID", &HCAL_1_DetectorID, &b_HCAL_1_DetectorID);
    fChain->SetBranchAddress("HCAL_1_DetectorID_x", &HCAL_1_DetectorID_x, &b_HCAL_1_DetectorID_x);
    fChain->SetBranchAddress("HCAL_1_DetectorID_y", &HCAL_1_DetectorID_y, &b_HCAL_1_DetectorID_y);
    fChain->SetBranchAddress("HCAL_1_DetectorID_z", &HCAL_1_DetectorID_z, &b_HCAL_1_DetectorID_z);
    fChain->SetBranchAddress("HCAL_1_Time", &HCAL_1_Time, &b_HCAL_1_Time);
    fChain->SetBranchAddress("HCAL_1_Edep", &HCAL_1_Edep, &b_HCAL_1_Edep);
    fChain->SetBranchAddress("HCAL_1_EdepEM", &HCAL_1_EdepEM, &b_HCAL_1_EdepEM);
    fChain->SetBranchAddress("HCAL_1_EdepHad", &HCAL_1_EdepHad, &b_HCAL_1_EdepHad);
    fChain->SetBranchAddress("HCAL_1_X", &HCAL_1_X, &b_HCAL_1_X);
    fChain->SetBranchAddress("HCAL_1_Y", &HCAL_1_Y, &b_HCAL_1_Y);
    fChain->SetBranchAddress("HCAL_1_Z", &HCAL_1_Z, &b_HCAL_1_Z);
    fChain->SetBranchAddress("HCAL_2_No", &HCAL_2_No, &b_HCAL_2_No);
    fChain->SetBranchAddress("HCAL_2_Eleak_Wrapper", &HCAL_2_Eleak_Wrapper, &b_HCAL_2_Eleak_Wrapper);
    fChain->SetBranchAddress("HCAL_2_Type", &HCAL_2_Type, &b_HCAL_2_Type);
    fChain->SetBranchAddress("HCAL_2_DetectorID", &HCAL_2_DetectorID, &b_HCAL_2_DetectorID);
    fChain->SetBranchAddress("HCAL_2_DetectorID_x", &HCAL_2_DetectorID_x, &b_HCAL_2_DetectorID_x);
    fChain->SetBranchAddress("HCAL_2_DetectorID_y", &HCAL_2_DetectorID_y, &b_HCAL_2_DetectorID_y);
    fChain->SetBranchAddress("HCAL_2_DetectorID_z", &HCAL_2_DetectorID_z, &b_HCAL_2_DetectorID_z);
    fChain->SetBranchAddress("HCAL_2_Time", &HCAL_2_Time, &b_HCAL_2_Time);
    fChain->SetBranchAddress("HCAL_2_Edep", &HCAL_2_Edep, &b_HCAL_2_Edep);
    fChain->SetBranchAddress("HCAL_2_EdepEM", &HCAL_2_EdepEM, &b_HCAL_2_EdepEM);
    fChain->SetBranchAddress("HCAL_2_EdepHad", &HCAL_2_EdepHad, &b_HCAL_2_EdepHad);
    fChain->SetBranchAddress("HCAL_2_X", &HCAL_2_X, &b_HCAL_2_X);
    fChain->SetBranchAddress("HCAL_2_Y", &HCAL_2_Y, &b_HCAL_2_Y);
    fChain->SetBranchAddress("HCAL_2_Z", &HCAL_2_Z, &b_HCAL_2_Z);
    fChain->SetBranchAddress("HCAL_3_No", &HCAL_3_No, &b_HCAL_3_No);
    fChain->SetBranchAddress("HCAL_3_Eleak_Wrapper", &HCAL_3_Eleak_Wrapper, &b_HCAL_3_Eleak_Wrapper);
    fChain->SetBranchAddress("HCAL_3_Type", &HCAL_3_Type, &b_HCAL_3_Type);
    fChain->SetBranchAddress("HCAL_3_DetectorID", &HCAL_3_DetectorID, &b_HCAL_3_DetectorID);
    fChain->SetBranchAddress("HCAL_3_DetectorID_x", &HCAL_3_DetectorID_x, &b_HCAL_3_DetectorID_x);
    fChain->SetBranchAddress("HCAL_3_DetectorID_y", &HCAL_3_DetectorID_y, &b_HCAL_3_DetectorID_y);
    fChain->SetBranchAddress("HCAL_3_DetectorID_z", &HCAL_3_DetectorID_z, &b_HCAL_3_DetectorID_z);
    fChain->SetBranchAddress("HCAL_3_Time", &HCAL_3_Time, &b_HCAL_3_Time);
    fChain->SetBranchAddress("HCAL_3_Edep", &HCAL_3_Edep, &b_HCAL_3_Edep);
    fChain->SetBranchAddress("HCAL_3_EdepEM", &HCAL_3_EdepEM, &b_HCAL_3_EdepEM);
    fChain->SetBranchAddress("HCAL_3_EdepHad", &HCAL_3_EdepHad, &b_HCAL_3_EdepHad);
    fChain->SetBranchAddress("HCAL_3_X", &HCAL_3_X, &b_HCAL_3_X);
    fChain->SetBranchAddress("HCAL_3_Y", &HCAL_3_Y, &b_HCAL_3_Y);
    fChain->SetBranchAddress("HCAL_3_Z", &HCAL_3_Z, &b_HCAL_3_Z);
    fChain->SetBranchAddress("HCAL_4_No", &HCAL_4_No, &b_HCAL_4_No);
    fChain->SetBranchAddress("HCAL_4_Eleak_Wrapper", &HCAL_4_Eleak_Wrapper, &b_HCAL_4_Eleak_Wrapper);
    fChain->SetBranchAddress("HCAL_4_Type", &HCAL_4_Type, &b_HCAL_4_Type);
    fChain->SetBranchAddress("HCAL_4_DetectorID", &HCAL_4_DetectorID, &b_HCAL_4_DetectorID);
    fChain->SetBranchAddress("HCAL_4_DetectorID_x", &HCAL_4_DetectorID_x, &b_HCAL_4_DetectorID_x);
    fChain->SetBranchAddress("HCAL_4_DetectorID_y", &HCAL_4_DetectorID_y, &b_HCAL_4_DetectorID_y);
    fChain->SetBranchAddress("HCAL_4_DetectorID_z", &HCAL_4_DetectorID_z, &b_HCAL_4_DetectorID_z);
    fChain->SetBranchAddress("HCAL_4_Time", &HCAL_4_Time, &b_HCAL_4_Time);
    fChain->SetBranchAddress("HCAL_4_Edep", &HCAL_4_Edep, &b_HCAL_4_Edep);
    fChain->SetBranchAddress("HCAL_4_EdepEM", &HCAL_4_EdepEM, &b_HCAL_4_EdepEM);
    fChain->SetBranchAddress("HCAL_4_EdepHad", &HCAL_4_EdepHad, &b_HCAL_4_EdepHad);
    fChain->SetBranchAddress("HCAL_4_X", &HCAL_4_X, &b_HCAL_4_X);
    fChain->SetBranchAddress("HCAL_4_Y", &HCAL_4_Y, &b_HCAL_4_Y);
    fChain->SetBranchAddress("HCAL_4_Z", &HCAL_4_Z, &b_HCAL_4_Z);
    fChain->SetBranchAddress("HCAL_5_No", &HCAL_5_No, &b_HCAL_5_No);
    fChain->SetBranchAddress("HCAL_5_Eleak_Wrapper", &HCAL_5_Eleak_Wrapper, &b_HCAL_5_Eleak_Wrapper);
    fChain->SetBranchAddress("HCAL_5_Type", &HCAL_5_Type, &b_HCAL_5_Type);
    fChain->SetBranchAddress("HCAL_5_DetectorID", &HCAL_5_DetectorID, &b_HCAL_5_DetectorID);
    fChain->SetBranchAddress("HCAL_5_DetectorID_x", &HCAL_5_DetectorID_x, &b_HCAL_5_DetectorID_x);
    fChain->SetBranchAddress("HCAL_5_DetectorID_y", &HCAL_5_DetectorID_y, &b_HCAL_5_DetectorID_y);
    fChain->SetBranchAddress("HCAL_5_DetectorID_z", &HCAL_5_DetectorID_z, &b_HCAL_5_DetectorID_z);
    fChain->SetBranchAddress("HCAL_5_Time", &HCAL_5_Time, &b_HCAL_5_Time);
    fChain->SetBranchAddress("HCAL_5_Edep", &HCAL_5_Edep, &b_HCAL_5_Edep);
    fChain->SetBranchAddress("HCAL_5_EdepEM", &HCAL_5_EdepEM, &b_HCAL_5_EdepEM);
    fChain->SetBranchAddress("HCAL_5_EdepHad", &HCAL_5_EdepHad, &b_HCAL_5_EdepHad);
    fChain->SetBranchAddress("HCAL_5_X", &HCAL_5_X, &b_HCAL_5_X);
    fChain->SetBranchAddress("HCAL_5_Y", &HCAL_5_Y, &b_HCAL_5_Y);
    fChain->SetBranchAddress("HCAL_5_Z", &HCAL_5_Z, &b_HCAL_5_Z);
    fChain->SetBranchAddress("HCAL_6_No", &HCAL_6_No, &b_HCAL_6_No);
    fChain->SetBranchAddress("HCAL_6_Eleak_Wrapper", &HCAL_6_Eleak_Wrapper, &b_HCAL_6_Eleak_Wrapper);
    fChain->SetBranchAddress("HCAL_6_Type", &HCAL_6_Type, &b_HCAL_6_Type);
    fChain->SetBranchAddress("HCAL_6_DetectorID", &HCAL_6_DetectorID, &b_HCAL_6_DetectorID);
    fChain->SetBranchAddress("HCAL_6_DetectorID_x", &HCAL_6_DetectorID_x, &b_HCAL_6_DetectorID_x);
    fChain->SetBranchAddress("HCAL_6_DetectorID_y", &HCAL_6_DetectorID_y, &b_HCAL_6_DetectorID_y);
    fChain->SetBranchAddress("HCAL_6_DetectorID_z", &HCAL_6_DetectorID_z, &b_HCAL_6_DetectorID_z);
    fChain->SetBranchAddress("HCAL_6_Time", &HCAL_6_Time, &b_HCAL_6_Time);
    fChain->SetBranchAddress("HCAL_6_Edep", &HCAL_6_Edep, &b_HCAL_6_Edep);
    fChain->SetBranchAddress("HCAL_6_EdepEM", &HCAL_6_EdepEM, &b_HCAL_6_EdepEM);
    fChain->SetBranchAddress("HCAL_6_EdepHad", &HCAL_6_EdepHad, &b_HCAL_6_EdepHad);
    fChain->SetBranchAddress("HCAL_6_X", &HCAL_6_X, &b_HCAL_6_X);
    fChain->SetBranchAddress("HCAL_6_Y", &HCAL_6_Y, &b_HCAL_6_Y);
    fChain->SetBranchAddress("HCAL_6_Z", &HCAL_6_Z, &b_HCAL_6_Z);
    fChain->SetBranchAddress("HCAL_7_No", &HCAL_7_No, &b_HCAL_7_No);
    fChain->SetBranchAddress("HCAL_7_Eleak_Wrapper", &HCAL_7_Eleak_Wrapper, &b_HCAL_7_Eleak_Wrapper);
    fChain->SetBranchAddress("HCAL_7_Type", &HCAL_7_Type, &b_HCAL_7_Type);
    fChain->SetBranchAddress("HCAL_7_DetectorID", &HCAL_7_DetectorID, &b_HCAL_7_DetectorID);
    fChain->SetBranchAddress("HCAL_7_DetectorID_x", &HCAL_7_DetectorID_x, &b_HCAL_7_DetectorID_x);
    fChain->SetBranchAddress("HCAL_7_DetectorID_y", &HCAL_7_DetectorID_y, &b_HCAL_7_DetectorID_y);
    fChain->SetBranchAddress("HCAL_7_DetectorID_z", &HCAL_7_DetectorID_z, &b_HCAL_7_DetectorID_z);
    fChain->SetBranchAddress("HCAL_7_Time", &HCAL_7_Time, &b_HCAL_7_Time);
    fChain->SetBranchAddress("HCAL_7_Edep", &HCAL_7_Edep, &b_HCAL_7_Edep);
    fChain->SetBranchAddress("HCAL_7_EdepEM", &HCAL_7_EdepEM, &b_HCAL_7_EdepEM);
    fChain->SetBranchAddress("HCAL_7_EdepHad", &HCAL_7_EdepHad, &b_HCAL_7_EdepHad);
    fChain->SetBranchAddress("HCAL_7_X", &HCAL_7_X, &b_HCAL_7_X);
    fChain->SetBranchAddress("HCAL_7_Y", &HCAL_7_Y, &b_HCAL_7_Y);
    fChain->SetBranchAddress("HCAL_7_Z", &HCAL_7_Z, &b_HCAL_7_Z);
    fChain->SetBranchAddress("HCAL_8_No", &HCAL_8_No, &b_HCAL_8_No);
    fChain->SetBranchAddress("HCAL_8_Eleak_Wrapper", &HCAL_8_Eleak_Wrapper, &b_HCAL_8_Eleak_Wrapper);
    fChain->SetBranchAddress("HCAL_8_Type", &HCAL_8_Type, &b_HCAL_8_Type);
    fChain->SetBranchAddress("HCAL_8_DetectorID", &HCAL_8_DetectorID, &b_HCAL_8_DetectorID);
    fChain->SetBranchAddress("HCAL_8_DetectorID_x", &HCAL_8_DetectorID_x, &b_HCAL_8_DetectorID_x);
    fChain->SetBranchAddress("HCAL_8_DetectorID_y", &HCAL_8_DetectorID_y, &b_HCAL_8_DetectorID_y);
    fChain->SetBranchAddress("HCAL_8_DetectorID_z", &HCAL_8_DetectorID_z, &b_HCAL_8_DetectorID_z);
    fChain->SetBranchAddress("HCAL_8_Time", &HCAL_8_Time, &b_HCAL_8_Time);
    fChain->SetBranchAddress("HCAL_8_Edep", &HCAL_8_Edep, &b_HCAL_8_Edep);
    fChain->SetBranchAddress("HCAL_8_EdepEM", &HCAL_8_EdepEM, &b_HCAL_8_EdepEM);
    fChain->SetBranchAddress("HCAL_8_EdepHad", &HCAL_8_EdepHad, &b_HCAL_8_EdepHad);
    fChain->SetBranchAddress("HCAL_8_X", &HCAL_8_X, &b_HCAL_8_X);
    fChain->SetBranchAddress("HCAL_8_Y", &HCAL_8_Y, &b_HCAL_8_Y);
    fChain->SetBranchAddress("HCAL_8_Z", &HCAL_8_Z, &b_HCAL_8_Z);
}


