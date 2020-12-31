#ifndef RootManager_h
#define RootManager_h 1

#include "Object/McParticle.h"
#include "Object/SimulatedHit.h"
#include "RootMessenger.hh"
#include "DP_simu/DetectorConstruction.hh"
#include "Control.h"

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TRandom3.h"
#include "TArrayD.h"

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
#include "Bias_Filter/FilterManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class TFile;

class TTree;

class TRandom3;

class RootMessenger;

class DetectorConstruction;

// Some Global String
// static TString InitialParticleStepCollection = "Initial_Particle_Step";
// static TString RawMCCollection = "RawMCParticle";

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// class description:
//   This class

class RootManager {
public:
    RootManager();

    ~RootManager();

    void book();

    void bookCollection(const G4String&);

    void save();

    void initialize();

    /* set methods */
    void SetOutFileName(G4String in) { outfilename = std::move(in); };

    void SetStartID(int id) { fStart = id; };

    void SetNbEvent(int id) { fEvtNb = id; };

    void SetClean(bool id) { if_clean = id; };

    void SetFilter(bool id) { if_filter = id; };

    void SetEndEvt(bool id) { if_EndEvt = id; };

    void SetOptical(bool id) {
        if_Optical = id;
        G4cout << "Optical Process: " << if_Optical << G4endl;
    };

    void SetRecordStep(bool id) { if_record_ip = id; };

    void SetifBias(G4bool in) { ifBias = in; };

    void SetBiasProcess(G4String in) { BiasProcess = std::move(in); };

    void SetBiasFactor(G4double in) { BiasFactor = in; };

    void SetBiasEmin(G4double in) { BiasEmin = in; };

    void SetifBiasTarget(G4bool in) { ifBiasTarget = in; };

    void SetifBiasECAL(G4bool in) { ifBiasECAL = in; };

    /* get methods */
    bool GetFilter() const { return if_filter; };

    bool GetOptical() const { return if_Optical; };

    bool GetRecordStep() const { return if_record_ip; };

    int GetNbEvent() const { return fEvtNb; };

    G4String GetOutFileName() { return outfilename; };

    G4bool GetifBias() const { return ifBias; };

    G4String GetBiasProcess() { return BiasProcess; };

    G4double GetBiasFactor() const { return BiasFactor; };

    G4double GetBiasEmin() const { return BiasEmin; };

    G4bool GetifBiasTarget() const { return ifBiasTarget; };

    G4bool GetifBiasECAL() const { return ifBiasECAL; };

    G4bool GetifFilter_Particle() { return fFilterMng->GetifFilter_Particle(); };

    G4bool GetifFilter_Process() { return fFilterMng->GetifFilter_Process(); };

    [[nodiscard]] DEvent *GetEvt() const {
        return Evt;
    }

    /* filter methods */
    void Filter_Track_Initialize() { fFilterMng->Filter_Track_Initialize(); };

    void Filter_Event_Initialize() { fFilterMng->Filter_Event_Initialize(); };

    void SetnewTrack(G4bool in) { newTrack = in; };

    void SetNew_Particle_Filter(G4int pdg, G4double minEnergy, G4double maxEnergy,
                                G4double minScanDistance, G4double maxScanDistance, G4bool flag)
    {
        fFilterMng->SetNew_Particle_Filter(pdg, minEnergy, maxEnergy, minScanDistance, maxScanDistance, flag);
    };

    void SetNew_Process_Filter(G4String processName, G4double minEnergy, G4double maxEnergy,
                               G4double minScanDistance, G4double maxScanDistance, G4bool flag)
    {
        fFilterMng->SetNew_Process_Filter(std::move(processName), minEnergy, maxEnergy, minScanDistance, maxScanDistance, flag);
    };

    G4bool Filter_Particle(const G4Step *aStep) { return fFilterMng->Filter_Particle(aStep); };

    void Filter_Process(const G4Step *aStep) { fFilterMng->Filter_Process(aStep); };

    G4bool Filter_Particle_Found_Result() { return fFilterMng->Filter_Particle_Found_Result(); };

    G4bool Filter_Process_Found_Result() { return fFilterMng->Filter_Process_Found_Result(); };

    G4bool GetFilter_Process_Result() { return fFilterMng->GetFilter_Process_Result(); };

    /* fill methods */
    void FillSim(Int_t EventID, const Double_t *Rndm);

    void FillSimHit(const G4String &, SimulatedHit *);

    void FillMC(McParticle *, int);

    void FillPNE(G4double E1, G4double E2);

    void FillEleak(const G4Step *, G4String);

    bool FillOptical(const G4Step *, G4String);

    void FillParticleStep(const G4Step *);

    void FillGeometry(const G4String& filename);

private:
    /*                   */
    /* Control Variables */
    /*                   */

    Bool_t if_filter{};
    Bool_t if_EndEvt{};
    Bool_t if_Optical; // flag of Optical Process. 

    RootMessenger *fMessenger;

    /* Biasing Variables */
    G4bool ifBias{};

    G4String BiasProcess;
    G4double BiasFactor{};
    G4double BiasEmin{};
    G4bool ifBiasTarget{};
    G4bool ifBiasECAL{};

    /* Filter Variables */
    std::shared_ptr<FilterManager> fFilterMng;
    G4bool newTrack{};


    /*              */
    /* Root Outputs */
    /*              */

    G4String outfilename;
    TFile *rootFile;
    TTree *tr;

    Int_t fStart; // Run Number, Initialized to 0. Set method: RootManager::SetStartID(int id)
    Int_t fEvtNb; // Event Number, Initialized to 100000. Set method: RootManager::SetNbEvent(int id)
    Int_t fEvtN{};

    Int_t EventID{};
    Double_t Rndm[4]{}; //Random double array, size=4


    // Initial Particle Movement
    bool if_record_ip{true};

    // Optical Photon
    std::map<G4String, int> Optical_No;
    std::map<G4String, std::vector<double> *> Optical_Time;
    std::map<G4String, std::vector<double> *> Optical_E;
    std::map<G4String, std::vector<int> *> Optical_DetID;
    //std::map<G4String, int* > Optical_DetID_x;
    //std::map<G4String, int* > Optical_DetID_y;
    //std::map<G4String, int* > Optical_DetID_z;

    // iterators
    std::map<G4String, int>::iterator itr_i;
    std::map<G4String, double *>::iterator itr_double;
    std::map<G4String, std::vector<int> *>::iterator itrvec_int;
    std::map<G4String, std::vector<double> *>::iterator itrvec_double;

    // Clean Mode
    TRandom3 rnd;
    Bool_t if_clean{false}; // Flag of Clean Mode. Initialized to false

    // DEvent Collection
    DEvent *Evt;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
