#ifndef RootManager_h
#define RootManager_h 1

#include "Object/McParticle.h"
#include "Object/McPHelper.h"
#include "Object/SimulatedHit.h"
#include "RootMessenger.hh"
#include "DP_simu/DetectorConstruction.hh"
#include "Control/Control.h"

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TRandom3.h"
#include "TArrayD.h"
#include "TParameter.h"
#include "TH1D.h"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
#include <stdexcept>
#include <sstream>
#include <utility>
#include <vector>
#include <map>

#include "RootGlobal.hh"

#include "Object/DEvent.h"
#include "Object/DDetectorIDMaps.h"
#include "Bias_Filter/FilterManager.hh"


#include "Optical/OpticalDigitizer.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class TFile;

class TTree;

class TRandom3;

class RootMessenger;

class DetectorConstruction;

class OpticalDigitizer;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// class description:
//   This class

class RootManager {
public:

    RootManager(const RootManager &) = delete;

    RootManager &operator=(RootManager const &) = delete;

    static RootManager *CreateInstance();

    void book();

    void bookCollection(const G4String &);

    Long64_t save();

    void initialize();

    /* get methods */
    [[nodiscard]] int GetNbEvent() const { return fEvtNb; };

    [[nodiscard]] DEvent *GetEvt() const { return Evt; }

    [[nodiscard]] TFile *getRootFile() const {
        return rootFile;
    }

    [[nodiscard]] Long64_t getFEvtNRecorded() const {
        return fEvtNRecorded;
    }

    [[nodiscard]] Int_t getFEvtNbKilledByTruthFilter() const {
        return fEvtNbKilledByTruthFilter;
    }

    [[nodiscard]] Int_t getFEvtNbKillByFilter() const {
        return fEvtNbKilledByFilter;
    }

    [[nodiscard]] Bool_t getIsBiasPool() const {
        return is_bias_pool;
    }

    [[nodiscard]] Bool_t getKilledByTruthFilter() const {
        return killed_by_truth_filter;
    }


    /* fill methods */
    void FillSim(Int_t EventID, const Double_t *Rndm);

    void FillSimHit(const G4String &, SimulatedHit *);

    McParticle * FillMC(McParticle *, int);

    void FillMCPHelper(McPHelper *, int);

    void FillPNE(G4double E1, G4double E2, G4double Z);

    void FillENE(G4double E1, G4double E2, G4double Z);

    void FillEleak(const G4Step *, const G4String &);

    // bool FillOptical(const G4Step *, G4String);
    bool SetOpticalTimeZero(G4double T0, const G4String &cIn);

    bool FillOpticalLUTs(std::vector<OpticalHit *> *hits, G4int GenNo, const G4String &cIn, int copyNum);

    bool FinalizeOptical();

    void FillParticleStep(const G4Step *);

    void FillGeometry(const G4String &filename);

    void FillWeight(double w) {
        weight = w;
    }

    void FillHistBin(TH1* hist, int nbin, TString label, Double_t content) {
        hist->GetXaxis()->SetBinLabel(nbin, label);
        hist->SetBinContent(nbin, content);
    }

    void AddFEvtNbKilledByTruthFilter() {
        fEvtNbKilledByTruthFilter++;
    }

    void AddFEvtNbKilledByFilter() {
        fEvtNbKilledByFilter++;
    }

    void AddBiasNPool() {
        fBiasNPool++;
    }

    void AddBiasNPoolPassTruthFilter() {
        fBiasNPoolPassTruthFilter++;
    }

    void SetIsBiasPool(Bool_t in) {
        is_bias_pool = in;
    }

    void SetKilledByTruthFilter(Bool_t in) {
        killed_by_truth_filter = in;
    }


private:

    RootMessenger *fMessenger;
    std::map<G4String, OpticalDigitizer *> fDigitizers;


    /*              */
    /* Root Outputs */
    /*              */

    G4String outfile_name;
    TFile *rootFile;
    TTree *tr;

    Int_t fStart; // Run Number, Initialized to 0. Set method: RootManager::SetStartID(int id)
    Int_t fEvtNb; // Event Number, Initialized to 100000. Set method: RootManager::SetNbEvent(int id)
    Int_t fEvtN{};
    Int_t fEvtNbKilledByTruthFilter{0};
    Int_t fEvtNbKilledByFilter{0};
    Int_t fEvtNRecorded{0};
    Int_t fBiasNPool{0};
    Int_t fBiasNPoolPassTruthFilter{0};
    Double_t weight{0.};
    Double_t OpticalHCALYield{0.};

    Int_t EventID{};
    Double_t Rndm[4]{}; //Random double array, size=4

    // Optical Photon
    std::map<G4String, int> Optical_No;
    std::map<G4String, std::vector<double> *> Optical_Time;
    std::map<G4String, std::vector<double> *> Optical_E;
    std::map<G4String, std::vector<int> *> Optical_DetID;
    //std::map<G4String, int* > Optical_DetID_x;
    //std::map<G4String, int* > Optical_DetID_y;
    //std::map<G4String, int* > Optical_DetID_z;

    // Clean Mode
    TRandom3 rnd;
    Bool_t if_clean{false}; // Flag of Clean Mode. Initialized to false

    // DEvent Collection
    DEvent *Evt;

    // Change during event
    Bool_t is_bias_pool{false}; // for biasing, count if mother particle can perform bias
    Bool_t killed_by_truth_filter{false};

private:
    RootManager();
};

extern RootManager *dRootMng;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
