#ifndef RootManager_h
#define RootManager_h 1

#include "Object/McParticle.h"
#include "Object/SimulatedHit.h"
#include "RootMessenger.hh"

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
#include <vector>
#include <map>

#include "RootGlobal.hh"

#include "Object/DEvent.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class TFile;

class TTree;

class TRandom3;

class RootMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class RootManager {
public:
    RootManager();

    ~RootManager();

    void book();

    void bookCollection(const G4String&);

    void save();

    void initialize();

    /* set methods */
    void SetOutFileName(G4String in) { outfilename = in; };

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

    void SetBiasProcess(G4String in) { BiasProcess = in; };

    void SetBiasFactor(G4double in) { BiasFactor = in; };

    void SetBiasEmin(G4double in) { BiasEmin = in; };

    void SetifBiasTarget(G4bool in) { ifBiasTarget = in; };

    void SetifBiasECAL(G4bool in) { ifBiasECAL = in; };

    void SetifFilter_HardBrem(G4bool in) { ifFilter_HardBrem = in; };

    void SetifFilter_Process(G4bool in) { ifFilter_Process = in; };


    /* get methods */
    bool GetFilter() { return if_filter; };

    bool GetOptical() { return if_Optical; };

    bool GetRecordStep() { return if_record_ip; };

    int GetNbEvent() { return fEvtNb; };

    G4String GetOutFileName() { return outfilename; };

    G4bool GetifBias() { return ifBias; };

    G4String GetBiasProcess() { return BiasProcess; };

    G4double GetBiasFactor() { return BiasFactor; };

    G4double GetBiasEmin() { return BiasEmin; };

    G4bool GetifBiasTarget() { return ifBiasTarget; };

    G4bool GetifBiasECAL() { return ifBiasECAL; };

    G4bool GetifFilter_HardBrem() { return ifFilter_HardBrem; };

    G4bool GetifFilter_Process() { return ifFilter_Process; };

    DEvent *GetEvt() const {
        return Evt;
    }

    /* filter methods */
    void Filter_Track_Initialize();

    void Filter_Event_Initialize();

    void SetnewTrack(G4bool in) { newTrack = in; };

    void SetGammaEmin(G4double in) { GammaEmin = in; };

    void SetHardBrem_ScanDistance(G4double in) { HardBrem_ScanDistance = in; };

    void SetProcessName(G4String in) { ProcessName = in; };

    void SetProcessEmin(G4double in) { ProcessEmin = in; };

    void SetProcess_MinScanDistance(G4double in) { Process_MinScanDistance = in; };

    void SetProcess_MaxScanDistance(G4double in) { Process_MaxScanDistance = in; };

    G4bool Filter_HardBrem(const G4Step *);

    void Filter_Process(const G4Step *);

    G4bool GetFilter_Process_Result() { return Filter_Process_Result; };

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

    Bool_t if_filter;
    Bool_t if_EndEvt;
    Bool_t if_Optical;

    RootMessenger *fMessenger;

    /* Biasing Variables */
    G4bool ifBias;

    G4String BiasProcess;
    G4double BiasFactor;
    G4double BiasEmin;
    G4bool ifBiasTarget;
    G4bool ifBiasECAL;

    /* Filter Variables */
    G4bool ifFilter_HardBrem;
    G4bool ifFilter_Process;
    G4bool newTrack;

    G4double Filter_HardBrem_Result;
    G4double GammaEmin;
    G4double HardBrem_ScanDistance;

    G4String ProcessName;
    G4double Filter_Process_Result;
    G4double ProcessEmin;
    G4double Process_MinScanDistance;
    G4double Process_MaxScanDistance;

    /*              */
    /* Root Outputs */
    /*              */

    G4String outfilename;
    TFile *rootFile;
    TTree *tr;

    Int_t fStart;
    Int_t fEvtNb;
    Int_t fEvtN;

    Int_t EventID;
    Double_t Rndm[4];


    // Initial Particle Movement
    bool if_record_ip;

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
    Bool_t if_clean;

    // DEvent Collection
    DEvent *Evt;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
