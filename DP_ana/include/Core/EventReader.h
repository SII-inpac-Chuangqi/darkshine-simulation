//
// Created by Zhang Yulei on 9/19/20.
//

#ifndef DSIMU_EVENTREADER_H
#define DSIMU_EVENTREADER_H

#include "TROOT.h"
#include <TChain.h>
#include <TFile.h>

#include "Object/DEvent.h"

using namespace std;

class EventReader {
public:
    EventReader() : evt(nullptr), fChain(nullptr) {};
    ~EventReader();

    bool ReadEvt(int evtNumber, int SkipNumber) const;
    void Convert();

    DEvent *getEvt() const {
        return evt;
    }

    void setEvt(DEvent *in) {
        evt = in;
    }

    Long64_t getEntries() const {
        return Entries;
    }

private:
    DEvent* evt;
    Long64_t Entries;
    /*                     */
    /* From ROOT MakeClass */
    /*                     */
public:
    Int_t GetEntry(Long64_t entry) const;
    Int_t ReadFile(const std::string& filename);

    virtual Long64_t LoadTree(Long64_t entry);
    virtual void     Init(TTree *tree);

public :
    TTree          *fChain;   //!pointer to the analyzed TTree or TChain
    Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

    // Declaration of leaf types
    Int_t           RunNumber;
    Int_t           EventNumber;
    Double_t        Rndm[4];
    Double_t        TRUTH_e1_Momentum[3];
    Double_t        TRUTH_e1_VPos[3];
    Double_t        TRUTH_e2_Momentum[3];
    Double_t        TRUTH_e2_VPos[3];
    Int_t           TRUTH_MC_Nb;
    vector<int>     *TRUTH_MC_id;
    vector<int>     *TRUTH_MC_PDG;
    vector<int>     *TRUTH_MC_ParentID;
    vector<TArrayD> *TRUTH_MC_Mom;
    vector<double>  *TRUTH_MC_E;
    vector<double>  *TRUTH_MC_Eremain;
    vector<TArrayD> *TRUTH_MC_VPos;
    vector<TArrayD> *TRUTH_MC_EPos;
    vector<TString> *TRUTH_MC_ProcessName;
    Double_t        TRUTH_MC_PNEnergy_Tar;
    Double_t        TRUTH_MC_PNEnergy_ECal;
    Double_t        TRUTH_MC_Eleak_ECAL;
    vector<TArrayD> *ip_Pos;
    vector<TArrayD> *ip_Momentum;
    vector<double>  *ip_Energy;
    vector<TString> *ip_PVName;
    vector<TString> *ip_ProcessName;
    Int_t           TagTrk1_No;
    Double_t        TagTrk1_Eleak_Wrapper;
    vector<int>     *TagTrk1_Type;
    vector<int>     *TagTrk1_DetectorID;
    vector<int>     *TagTrk1_DetectorID_x;
    vector<int>     *TagTrk1_DetectorID_y;
    vector<int>     *TagTrk1_DetectorID_z;
    vector<double>  *TagTrk1_Time;
    vector<double>  *TagTrk1_Edep;
    vector<double>  *TagTrk1_EdepEM;
    vector<double>  *TagTrk1_EdepHad;
    vector<double>  *TagTrk1_X;
    vector<double>  *TagTrk1_Y;
    vector<double>  *TagTrk1_Z;
    Int_t           TagTrk2_No;
    Double_t        TagTrk2_Eleak_Wrapper;
    vector<int>     *TagTrk2_Type;
    vector<int>     *TagTrk2_DetectorID;
    vector<int>     *TagTrk2_DetectorID_x;
    vector<int>     *TagTrk2_DetectorID_y;
    vector<int>     *TagTrk2_DetectorID_z;
    vector<double>  *TagTrk2_Time;
    vector<double>  *TagTrk2_Edep;
    vector<double>  *TagTrk2_EdepEM;
    vector<double>  *TagTrk2_EdepHad;
    vector<double>  *TagTrk2_X;
    vector<double>  *TagTrk2_Y;
    vector<double>  *TagTrk2_Z;
    Int_t           RecTrk1_No;
    Double_t        RecTrk1_Eleak_Wrapper;
    vector<int>     *RecTrk1_Type;
    vector<int>     *RecTrk1_DetectorID;
    vector<int>     *RecTrk1_DetectorID_x;
    vector<int>     *RecTrk1_DetectorID_y;
    vector<int>     *RecTrk1_DetectorID_z;
    vector<double>  *RecTrk1_Time;
    vector<double>  *RecTrk1_Edep;
    vector<double>  *RecTrk1_EdepEM;
    vector<double>  *RecTrk1_EdepHad;
    vector<double>  *RecTrk1_X;
    vector<double>  *RecTrk1_Y;
    vector<double>  *RecTrk1_Z;
    Int_t           RecTrk2_No;
    Double_t        RecTrk2_Eleak_Wrapper;
    vector<int>     *RecTrk2_Type;
    vector<int>     *RecTrk2_DetectorID;
    vector<int>     *RecTrk2_DetectorID_x;
    vector<int>     *RecTrk2_DetectorID_y;
    vector<int>     *RecTrk2_DetectorID_z;
    vector<double>  *RecTrk2_Time;
    vector<double>  *RecTrk2_Edep;
    vector<double>  *RecTrk2_EdepEM;
    vector<double>  *RecTrk2_EdepHad;
    vector<double>  *RecTrk2_X;
    vector<double>  *RecTrk2_Y;
    vector<double>  *RecTrk2_Z;
    Int_t           ECAL_Center_No;
    Double_t        ECAL_Center_Eleak_Wrapper;
    vector<int>     *ECAL_Center_Type;
    vector<int>     *ECAL_Center_DetectorID;
    vector<int>     *ECAL_Center_DetectorID_x;
    vector<int>     *ECAL_Center_DetectorID_y;
    vector<int>     *ECAL_Center_DetectorID_z;
    vector<double>  *ECAL_Center_Time;
    vector<double>  *ECAL_Center_Edep;
    vector<double>  *ECAL_Center_EdepEM;
    vector<double>  *ECAL_Center_EdepHad;
    vector<double>  *ECAL_Center_X;
    vector<double>  *ECAL_Center_Y;
    vector<double>  *ECAL_Center_Z;
    Int_t           ECAL_Outer_1_No;
    Double_t        ECAL_Outer_1_Eleak_Wrapper;
    vector<int>     *ECAL_Outer_1_Type;
    vector<int>     *ECAL_Outer_1_DetectorID;
    vector<int>     *ECAL_Outer_1_DetectorID_x;
    vector<int>     *ECAL_Outer_1_DetectorID_y;
    vector<int>     *ECAL_Outer_1_DetectorID_z;
    vector<double>  *ECAL_Outer_1_Time;
    vector<double>  *ECAL_Outer_1_Edep;
    vector<double>  *ECAL_Outer_1_EdepEM;
    vector<double>  *ECAL_Outer_1_EdepHad;
    vector<double>  *ECAL_Outer_1_X;
    vector<double>  *ECAL_Outer_1_Y;
    vector<double>  *ECAL_Outer_1_Z;
    Int_t           ECAL_Outer_2_No;
    Double_t        ECAL_Outer_2_Eleak_Wrapper;
    vector<int>     *ECAL_Outer_2_Type;
    vector<int>     *ECAL_Outer_2_DetectorID;
    vector<int>     *ECAL_Outer_2_DetectorID_x;
    vector<int>     *ECAL_Outer_2_DetectorID_y;
    vector<int>     *ECAL_Outer_2_DetectorID_z;
    vector<double>  *ECAL_Outer_2_Time;
    vector<double>  *ECAL_Outer_2_Edep;
    vector<double>  *ECAL_Outer_2_EdepEM;
    vector<double>  *ECAL_Outer_2_EdepHad;
    vector<double>  *ECAL_Outer_2_X;
    vector<double>  *ECAL_Outer_2_Y;
    vector<double>  *ECAL_Outer_2_Z;
    Int_t           ECAL_Outer_3_No;
    Double_t        ECAL_Outer_3_Eleak_Wrapper;
    vector<int>     *ECAL_Outer_3_Type;
    vector<int>     *ECAL_Outer_3_DetectorID;
    vector<int>     *ECAL_Outer_3_DetectorID_x;
    vector<int>     *ECAL_Outer_3_DetectorID_y;
    vector<int>     *ECAL_Outer_3_DetectorID_z;
    vector<double>  *ECAL_Outer_3_Time;
    vector<double>  *ECAL_Outer_3_Edep;
    vector<double>  *ECAL_Outer_3_EdepEM;
    vector<double>  *ECAL_Outer_3_EdepHad;
    vector<double>  *ECAL_Outer_3_X;
    vector<double>  *ECAL_Outer_3_Y;
    vector<double>  *ECAL_Outer_3_Z;
    Int_t           ECAL_Outer_4_No;
    Double_t        ECAL_Outer_4_Eleak_Wrapper;
    vector<int>     *ECAL_Outer_4_Type;
    vector<int>     *ECAL_Outer_4_DetectorID;
    vector<int>     *ECAL_Outer_4_DetectorID_x;
    vector<int>     *ECAL_Outer_4_DetectorID_y;
    vector<int>     *ECAL_Outer_4_DetectorID_z;
    vector<double>  *ECAL_Outer_4_Time;
    vector<double>  *ECAL_Outer_4_Edep;
    vector<double>  *ECAL_Outer_4_EdepEM;
    vector<double>  *ECAL_Outer_4_EdepHad;
    vector<double>  *ECAL_Outer_4_X;
    vector<double>  *ECAL_Outer_4_Y;
    vector<double>  *ECAL_Outer_4_Z;
    Int_t           HCAL_0_No;
    Double_t        HCAL_0_Eleak_Wrapper;
    vector<int>     *HCAL_0_Type;
    vector<int>     *HCAL_0_DetectorID;
    vector<int>     *HCAL_0_DetectorID_x;
    vector<int>     *HCAL_0_DetectorID_y;
    vector<int>     *HCAL_0_DetectorID_z;
    vector<double>  *HCAL_0_Time;
    vector<double>  *HCAL_0_Edep;
    vector<double>  *HCAL_0_EdepEM;
    vector<double>  *HCAL_0_EdepHad;
    vector<double>  *HCAL_0_X;
    vector<double>  *HCAL_0_Y;
    vector<double>  *HCAL_0_Z;
    Int_t           HCAL_1_No;
    Double_t        HCAL_1_Eleak_Wrapper;
    vector<int>     *HCAL_1_Type;
    vector<int>     *HCAL_1_DetectorID;
    vector<int>     *HCAL_1_DetectorID_x;
    vector<int>     *HCAL_1_DetectorID_y;
    vector<int>     *HCAL_1_DetectorID_z;
    vector<double>  *HCAL_1_Time;
    vector<double>  *HCAL_1_Edep;
    vector<double>  *HCAL_1_EdepEM;
    vector<double>  *HCAL_1_EdepHad;
    vector<double>  *HCAL_1_X;
    vector<double>  *HCAL_1_Y;
    vector<double>  *HCAL_1_Z;
    Int_t           HCAL_2_No;
    Double_t        HCAL_2_Eleak_Wrapper;
    vector<int>     *HCAL_2_Type;
    vector<int>     *HCAL_2_DetectorID;
    vector<int>     *HCAL_2_DetectorID_x;
    vector<int>     *HCAL_2_DetectorID_y;
    vector<int>     *HCAL_2_DetectorID_z;
    vector<double>  *HCAL_2_Time;
    vector<double>  *HCAL_2_Edep;
    vector<double>  *HCAL_2_EdepEM;
    vector<double>  *HCAL_2_EdepHad;
    vector<double>  *HCAL_2_X;
    vector<double>  *HCAL_2_Y;
    vector<double>  *HCAL_2_Z;
    Int_t           HCAL_3_No;
    Double_t        HCAL_3_Eleak_Wrapper;
    vector<int>     *HCAL_3_Type;
    vector<int>     *HCAL_3_DetectorID;
    vector<int>     *HCAL_3_DetectorID_x;
    vector<int>     *HCAL_3_DetectorID_y;
    vector<int>     *HCAL_3_DetectorID_z;
    vector<double>  *HCAL_3_Time;
    vector<double>  *HCAL_3_Edep;
    vector<double>  *HCAL_3_EdepEM;
    vector<double>  *HCAL_3_EdepHad;
    vector<double>  *HCAL_3_X;
    vector<double>  *HCAL_3_Y;
    vector<double>  *HCAL_3_Z;
    Int_t           HCAL_4_No;
    Double_t        HCAL_4_Eleak_Wrapper;
    vector<int>     *HCAL_4_Type;
    vector<int>     *HCAL_4_DetectorID;
    vector<int>     *HCAL_4_DetectorID_x;
    vector<int>     *HCAL_4_DetectorID_y;
    vector<int>     *HCAL_4_DetectorID_z;
    vector<double>  *HCAL_4_Time;
    vector<double>  *HCAL_4_Edep;
    vector<double>  *HCAL_4_EdepEM;
    vector<double>  *HCAL_4_EdepHad;
    vector<double>  *HCAL_4_X;
    vector<double>  *HCAL_4_Y;
    vector<double>  *HCAL_4_Z;
    Int_t           HCAL_5_No;
    Double_t        HCAL_5_Eleak_Wrapper;
    vector<int>     *HCAL_5_Type;
    vector<int>     *HCAL_5_DetectorID;
    vector<int>     *HCAL_5_DetectorID_x;
    vector<int>     *HCAL_5_DetectorID_y;
    vector<int>     *HCAL_5_DetectorID_z;
    vector<double>  *HCAL_5_Time;
    vector<double>  *HCAL_5_Edep;
    vector<double>  *HCAL_5_EdepEM;
    vector<double>  *HCAL_5_EdepHad;
    vector<double>  *HCAL_5_X;
    vector<double>  *HCAL_5_Y;
    vector<double>  *HCAL_5_Z;
    Int_t           HCAL_6_No;
    Double_t        HCAL_6_Eleak_Wrapper;
    vector<int>     *HCAL_6_Type;
    vector<int>     *HCAL_6_DetectorID;
    vector<int>     *HCAL_6_DetectorID_x;
    vector<int>     *HCAL_6_DetectorID_y;
    vector<int>     *HCAL_6_DetectorID_z;
    vector<double>  *HCAL_6_Time;
    vector<double>  *HCAL_6_Edep;
    vector<double>  *HCAL_6_EdepEM;
    vector<double>  *HCAL_6_EdepHad;
    vector<double>  *HCAL_6_X;
    vector<double>  *HCAL_6_Y;
    vector<double>  *HCAL_6_Z;
    Int_t           HCAL_7_No;
    Double_t        HCAL_7_Eleak_Wrapper;
    vector<int>     *HCAL_7_Type;
    vector<int>     *HCAL_7_DetectorID;
    vector<int>     *HCAL_7_DetectorID_x;
    vector<int>     *HCAL_7_DetectorID_y;
    vector<int>     *HCAL_7_DetectorID_z;
    vector<double>  *HCAL_7_Time;
    vector<double>  *HCAL_7_Edep;
    vector<double>  *HCAL_7_EdepEM;
    vector<double>  *HCAL_7_EdepHad;
    vector<double>  *HCAL_7_X;
    vector<double>  *HCAL_7_Y;
    vector<double>  *HCAL_7_Z;
    Int_t           HCAL_8_No;
    Double_t        HCAL_8_Eleak_Wrapper;
    vector<int>     *HCAL_8_Type;
    vector<int>     *HCAL_8_DetectorID;
    vector<int>     *HCAL_8_DetectorID_x;
    vector<int>     *HCAL_8_DetectorID_y;
    vector<int>     *HCAL_8_DetectorID_z;
    vector<double>  *HCAL_8_Time;
    vector<double>  *HCAL_8_Edep;
    vector<double>  *HCAL_8_EdepEM;
    vector<double>  *HCAL_8_EdepHad;
    vector<double>  *HCAL_8_X;
    vector<double>  *HCAL_8_Y;
    vector<double>  *HCAL_8_Z;

    // List of branches
    TBranch        *b_RunNumber;   //!
    TBranch        *b_EventNumber;   //!
    TBranch        *b_Rndm;   //!
    TBranch        *b_t_e1_Momentum;   //!
    TBranch        *b_t_e1_VPos;   //!
    TBranch        *b_t_e2_Momentum;   //!
    TBranch        *b_t_e2_VPos;   //!
    TBranch        *b_TRUTH_MC_Nb;   //!
    TBranch        *b_TRUTH_MC_id;   //!
    TBranch        *b_TRUTH_MC_PDG;   //!
    TBranch        *b_TRUTH_MC_ParentID;   //!
    TBranch        *b_TRUTH_MC_Mom;   //!
    TBranch        *b_TRUTH_MC_E;   //!
    TBranch        *b_TRUTH_MC_Eremain;   //!
    TBranch        *b_TRUTH_MC_VPos;   //!
    TBranch        *b_TRUTH_MC_EPos;   //!
    TBranch        *b_TRUTH_MC_ProcessName;   //!
    TBranch        *b_TRUTH_MC_PNEnergy_Tar;   //!
    TBranch        *b_TRUTH_MC_PNEnergy_ECal;   //!
    TBranch        *b_TRUTH_MC_Eleak_ECAL;   //!
    TBranch        *b_ip_Pos;   //!
    TBranch        *b_ip_Momentum;   //!
    TBranch        *b_ip_Energy;   //!
    TBranch        *b_ip_PVName;   //!
    TBranch        *b_ip_ProcessName;   //!
    TBranch        *b_TagTrk1_No;   //!
    TBranch        *b_TagTrk1_Eleak_Wrapper;   //!
    TBranch        *b_TagTrk1_Type;   //!
    TBranch        *b_TagTrk1_DetectorID;   //!
    TBranch        *b_TagTrk1_DetectorID_x;   //!
    TBranch        *b_TagTrk1_DetectorID_y;   //!
    TBranch        *b_TagTrk1_DetectorID_z;   //!
    TBranch        *b_TagTrk1_Time;   //!
    TBranch        *b_TagTrk1_Edep;   //!
    TBranch        *b_TagTrk1_EdepEM;   //!
    TBranch        *b_TagTrk1_EdepHad;   //!
    TBranch        *b_TagTrk1_X;   //!
    TBranch        *b_TagTrk1_Y;   //!
    TBranch        *b_TagTrk1_Z;   //!
    TBranch        *b_TagTrk2_No;   //!
    TBranch        *b_TagTrk2_Eleak_Wrapper;   //!
    TBranch        *b_TagTrk2_Type;   //!
    TBranch        *b_TagTrk2_DetectorID;   //!
    TBranch        *b_TagTrk2_DetectorID_x;   //!
    TBranch        *b_TagTrk2_DetectorID_y;   //!
    TBranch        *b_TagTrk2_DetectorID_z;   //!
    TBranch        *b_TagTrk2_Time;   //!
    TBranch        *b_TagTrk2_Edep;   //!
    TBranch        *b_TagTrk2_EdepEM;   //!
    TBranch        *b_TagTrk2_EdepHad;   //!
    TBranch        *b_TagTrk2_X;   //!
    TBranch        *b_TagTrk2_Y;   //!
    TBranch        *b_TagTrk2_Z;   //!
    TBranch        *b_RecTrk1_No;   //!
    TBranch        *b_RecTrk1_Eleak_Wrapper;   //!
    TBranch        *b_RecTrk1_Type;   //!
    TBranch        *b_RecTrk1_DetectorID;   //!
    TBranch        *b_RecTrk1_DetectorID_x;   //!
    TBranch        *b_RecTrk1_DetectorID_y;   //!
    TBranch        *b_RecTrk1_DetectorID_z;   //!
    TBranch        *b_RecTrk1_Time;   //!
    TBranch        *b_RecTrk1_Edep;   //!
    TBranch        *b_RecTrk1_EdepEM;   //!
    TBranch        *b_RecTrk1_EdepHad;   //!
    TBranch        *b_RecTrk1_X;   //!
    TBranch        *b_RecTrk1_Y;   //!
    TBranch        *b_RecTrk1_Z;   //!
    TBranch        *b_RecTrk2_No;   //!
    TBranch        *b_RecTrk2_Eleak_Wrapper;   //!
    TBranch        *b_RecTrk2_Type;   //!
    TBranch        *b_RecTrk2_DetectorID;   //!
    TBranch        *b_RecTrk2_DetectorID_x;   //!
    TBranch        *b_RecTrk2_DetectorID_y;   //!
    TBranch        *b_RecTrk2_DetectorID_z;   //!
    TBranch        *b_RecTrk2_Time;   //!
    TBranch        *b_RecTrk2_Edep;   //!
    TBranch        *b_RecTrk2_EdepEM;   //!
    TBranch        *b_RecTrk2_EdepHad;   //!
    TBranch        *b_RecTrk2_X;   //!
    TBranch        *b_RecTrk2_Y;   //!
    TBranch        *b_RecTrk2_Z;   //!
    TBranch        *b_ECAL_Center_No;   //!
    TBranch        *b_ECAL_Center_Eleak_Wrapper;   //!
    TBranch        *b_ECAL_Center_Type;   //!
    TBranch        *b_ECAL_Center_DetectorID;   //!
    TBranch        *b_ECAL_Center_DetectorID_x;   //!
    TBranch        *b_ECAL_Center_DetectorID_y;   //!
    TBranch        *b_ECAL_Center_DetectorID_z;   //!
    TBranch        *b_ECAL_Center_Time;   //!
    TBranch        *b_ECAL_Center_Edep;   //!
    TBranch        *b_ECAL_Center_EdepEM;   //!
    TBranch        *b_ECAL_Center_EdepHad;   //!
    TBranch        *b_ECAL_Center_X;   //!
    TBranch        *b_ECAL_Center_Y;   //!
    TBranch        *b_ECAL_Center_Z;   //!
    TBranch        *b_ECAL_Outer_1_No;   //!
    TBranch        *b_ECAL_Outer_1_Eleak_Wrapper;   //!
    TBranch        *b_ECAL_Outer_1_Type;   //!
    TBranch        *b_ECAL_Outer_1_DetectorID;   //!
    TBranch        *b_ECAL_Outer_1_DetectorID_x;   //!
    TBranch        *b_ECAL_Outer_1_DetectorID_y;   //!
    TBranch        *b_ECAL_Outer_1_DetectorID_z;   //!
    TBranch        *b_ECAL_Outer_1_Time;   //!
    TBranch        *b_ECAL_Outer_1_Edep;   //!
    TBranch        *b_ECAL_Outer_1_EdepEM;   //!
    TBranch        *b_ECAL_Outer_1_EdepHad;   //!
    TBranch        *b_ECAL_Outer_1_X;   //!
    TBranch        *b_ECAL_Outer_1_Y;   //!
    TBranch        *b_ECAL_Outer_1_Z;   //!
    TBranch        *b_ECAL_Outer_2_No;   //!
    TBranch        *b_ECAL_Outer_2_Eleak_Wrapper;   //!
    TBranch        *b_ECAL_Outer_2_Type;   //!
    TBranch        *b_ECAL_Outer_2_DetectorID;   //!
    TBranch        *b_ECAL_Outer_2_DetectorID_x;   //!
    TBranch        *b_ECAL_Outer_2_DetectorID_y;   //!
    TBranch        *b_ECAL_Outer_2_DetectorID_z;   //!
    TBranch        *b_ECAL_Outer_2_Time;   //!
    TBranch        *b_ECAL_Outer_2_Edep;   //!
    TBranch        *b_ECAL_Outer_2_EdepEM;   //!
    TBranch        *b_ECAL_Outer_2_EdepHad;   //!
    TBranch        *b_ECAL_Outer_2_X;   //!
    TBranch        *b_ECAL_Outer_2_Y;   //!
    TBranch        *b_ECAL_Outer_2_Z;   //!
    TBranch        *b_ECAL_Outer_3_No;   //!
    TBranch        *b_ECAL_Outer_3_Eleak_Wrapper;   //!
    TBranch        *b_ECAL_Outer_3_Type;   //!
    TBranch        *b_ECAL_Outer_3_DetectorID;   //!
    TBranch        *b_ECAL_Outer_3_DetectorID_x;   //!
    TBranch        *b_ECAL_Outer_3_DetectorID_y;   //!
    TBranch        *b_ECAL_Outer_3_DetectorID_z;   //!
    TBranch        *b_ECAL_Outer_3_Time;   //!
    TBranch        *b_ECAL_Outer_3_Edep;   //!
    TBranch        *b_ECAL_Outer_3_EdepEM;   //!
    TBranch        *b_ECAL_Outer_3_EdepHad;   //!
    TBranch        *b_ECAL_Outer_3_X;   //!
    TBranch        *b_ECAL_Outer_3_Y;   //!
    TBranch        *b_ECAL_Outer_3_Z;   //!
    TBranch        *b_ECAL_Outer_4_No;   //!
    TBranch        *b_ECAL_Outer_4_Eleak_Wrapper;   //!
    TBranch        *b_ECAL_Outer_4_Type;   //!
    TBranch        *b_ECAL_Outer_4_DetectorID;   //!
    TBranch        *b_ECAL_Outer_4_DetectorID_x;   //!
    TBranch        *b_ECAL_Outer_4_DetectorID_y;   //!
    TBranch        *b_ECAL_Outer_4_DetectorID_z;   //!
    TBranch        *b_ECAL_Outer_4_Time;   //!
    TBranch        *b_ECAL_Outer_4_Edep;   //!
    TBranch        *b_ECAL_Outer_4_EdepEM;   //!
    TBranch        *b_ECAL_Outer_4_EdepHad;   //!
    TBranch        *b_ECAL_Outer_4_X;   //!
    TBranch        *b_ECAL_Outer_4_Y;   //!
    TBranch        *b_ECAL_Outer_4_Z;   //!
    TBranch        *b_HCAL_0_No;   //!
    TBranch        *b_HCAL_0_Eleak_Wrapper;   //!
    TBranch        *b_HCAL_0_Type;   //!
    TBranch        *b_HCAL_0_DetectorID;   //!
    TBranch        *b_HCAL_0_DetectorID_x;   //!
    TBranch        *b_HCAL_0_DetectorID_y;   //!
    TBranch        *b_HCAL_0_DetectorID_z;   //!
    TBranch        *b_HCAL_0_Time;   //!
    TBranch        *b_HCAL_0_Edep;   //!
    TBranch        *b_HCAL_0_EdepEM;   //!
    TBranch        *b_HCAL_0_EdepHad;   //!
    TBranch        *b_HCAL_0_X;   //!
    TBranch        *b_HCAL_0_Y;   //!
    TBranch        *b_HCAL_0_Z;   //!
    TBranch        *b_HCAL_1_No;   //!
    TBranch        *b_HCAL_1_Eleak_Wrapper;   //!
    TBranch        *b_HCAL_1_Type;   //!
    TBranch        *b_HCAL_1_DetectorID;   //!
    TBranch        *b_HCAL_1_DetectorID_x;   //!
    TBranch        *b_HCAL_1_DetectorID_y;   //!
    TBranch        *b_HCAL_1_DetectorID_z;   //!
    TBranch        *b_HCAL_1_Time;   //!
    TBranch        *b_HCAL_1_Edep;   //!
    TBranch        *b_HCAL_1_EdepEM;   //!
    TBranch        *b_HCAL_1_EdepHad;   //!
    TBranch        *b_HCAL_1_X;   //!
    TBranch        *b_HCAL_1_Y;   //!
    TBranch        *b_HCAL_1_Z;   //!
    TBranch        *b_HCAL_2_No;   //!
    TBranch        *b_HCAL_2_Eleak_Wrapper;   //!
    TBranch        *b_HCAL_2_Type;   //!
    TBranch        *b_HCAL_2_DetectorID;   //!
    TBranch        *b_HCAL_2_DetectorID_x;   //!
    TBranch        *b_HCAL_2_DetectorID_y;   //!
    TBranch        *b_HCAL_2_DetectorID_z;   //!
    TBranch        *b_HCAL_2_Time;   //!
    TBranch        *b_HCAL_2_Edep;   //!
    TBranch        *b_HCAL_2_EdepEM;   //!
    TBranch        *b_HCAL_2_EdepHad;   //!
    TBranch        *b_HCAL_2_X;   //!
    TBranch        *b_HCAL_2_Y;   //!
    TBranch        *b_HCAL_2_Z;   //!
    TBranch        *b_HCAL_3_No;   //!
    TBranch        *b_HCAL_3_Eleak_Wrapper;   //!
    TBranch        *b_HCAL_3_Type;   //!
    TBranch        *b_HCAL_3_DetectorID;   //!
    TBranch        *b_HCAL_3_DetectorID_x;   //!
    TBranch        *b_HCAL_3_DetectorID_y;   //!
    TBranch        *b_HCAL_3_DetectorID_z;   //!
    TBranch        *b_HCAL_3_Time;   //!
    TBranch        *b_HCAL_3_Edep;   //!
    TBranch        *b_HCAL_3_EdepEM;   //!
    TBranch        *b_HCAL_3_EdepHad;   //!
    TBranch        *b_HCAL_3_X;   //!
    TBranch        *b_HCAL_3_Y;   //!
    TBranch        *b_HCAL_3_Z;   //!
    TBranch        *b_HCAL_4_No;   //!
    TBranch        *b_HCAL_4_Eleak_Wrapper;   //!
    TBranch        *b_HCAL_4_Type;   //!
    TBranch        *b_HCAL_4_DetectorID;   //!
    TBranch        *b_HCAL_4_DetectorID_x;   //!
    TBranch        *b_HCAL_4_DetectorID_y;   //!
    TBranch        *b_HCAL_4_DetectorID_z;   //!
    TBranch        *b_HCAL_4_Time;   //!
    TBranch        *b_HCAL_4_Edep;   //!
    TBranch        *b_HCAL_4_EdepEM;   //!
    TBranch        *b_HCAL_4_EdepHad;   //!
    TBranch        *b_HCAL_4_X;   //!
    TBranch        *b_HCAL_4_Y;   //!
    TBranch        *b_HCAL_4_Z;   //!
    TBranch        *b_HCAL_5_No;   //!
    TBranch        *b_HCAL_5_Eleak_Wrapper;   //!
    TBranch        *b_HCAL_5_Type;   //!
    TBranch        *b_HCAL_5_DetectorID;   //!
    TBranch        *b_HCAL_5_DetectorID_x;   //!
    TBranch        *b_HCAL_5_DetectorID_y;   //!
    TBranch        *b_HCAL_5_DetectorID_z;   //!
    TBranch        *b_HCAL_5_Time;   //!
    TBranch        *b_HCAL_5_Edep;   //!
    TBranch        *b_HCAL_5_EdepEM;   //!
    TBranch        *b_HCAL_5_EdepHad;   //!
    TBranch        *b_HCAL_5_X;   //!
    TBranch        *b_HCAL_5_Y;   //!
    TBranch        *b_HCAL_5_Z;   //!
    TBranch        *b_HCAL_6_No;   //!
    TBranch        *b_HCAL_6_Eleak_Wrapper;   //!
    TBranch        *b_HCAL_6_Type;   //!
    TBranch        *b_HCAL_6_DetectorID;   //!
    TBranch        *b_HCAL_6_DetectorID_x;   //!
    TBranch        *b_HCAL_6_DetectorID_y;   //!
    TBranch        *b_HCAL_6_DetectorID_z;   //!
    TBranch        *b_HCAL_6_Time;   //!
    TBranch        *b_HCAL_6_Edep;   //!
    TBranch        *b_HCAL_6_EdepEM;   //!
    TBranch        *b_HCAL_6_EdepHad;   //!
    TBranch        *b_HCAL_6_X;   //!
    TBranch        *b_HCAL_6_Y;   //!
    TBranch        *b_HCAL_6_Z;   //!
    TBranch        *b_HCAL_7_No;   //!
    TBranch        *b_HCAL_7_Eleak_Wrapper;   //!
    TBranch        *b_HCAL_7_Type;   //!
    TBranch        *b_HCAL_7_DetectorID;   //!
    TBranch        *b_HCAL_7_DetectorID_x;   //!
    TBranch        *b_HCAL_7_DetectorID_y;   //!
    TBranch        *b_HCAL_7_DetectorID_z;   //!
    TBranch        *b_HCAL_7_Time;   //!
    TBranch        *b_HCAL_7_Edep;   //!
    TBranch        *b_HCAL_7_EdepEM;   //!
    TBranch        *b_HCAL_7_EdepHad;   //!
    TBranch        *b_HCAL_7_X;   //!
    TBranch        *b_HCAL_7_Y;   //!
    TBranch        *b_HCAL_7_Z;   //!
    TBranch        *b_HCAL_8_No;   //!
    TBranch        *b_HCAL_8_Eleak_Wrapper;   //!
    TBranch        *b_HCAL_8_Type;   //!
    TBranch        *b_HCAL_8_DetectorID;   //!
    TBranch        *b_HCAL_8_DetectorID_x;   //!
    TBranch        *b_HCAL_8_DetectorID_y;   //!
    TBranch        *b_HCAL_8_DetectorID_z;   //!
    TBranch        *b_HCAL_8_Time;   //!
    TBranch        *b_HCAL_8_Edep;   //!
    TBranch        *b_HCAL_8_EdepEM;   //!
    TBranch        *b_HCAL_8_EdepHad;   //!
    TBranch        *b_HCAL_8_X;   //!
    TBranch        *b_HCAL_8_Y;   //!
    TBranch        *b_HCAL_8_Z;   //!

};

#endif //DSIMU_EVENTREADER_H
