#ifndef RootManager_h
#define RootManager_h 1

#include "Object/McParticle.h"
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

    void save();

    void initialize();

    /* get methods */
    [[nodiscard]] int GetNbEvent() const { return fEvtNb; };

    [[nodiscard]] DEvent *GetEvt() const { return Evt; }

    /* fill methods */
    void FillSim(Int_t EventID, const Double_t *Rndm);

    void FillSimHit(const G4String &, SimulatedHit *);

    void FillMC(McParticle *, int);

    void FillPNE(G4double E1, G4double E2);

    void FillEleak(const G4Step *, const G4String&);

    // bool FillOptical(const G4Step *, G4String);
    bool SetOpticalTimeZero(G4double T0, const G4String& cIn);
    bool FillOpticalLUTs(std::vector<OpticalHit *>* hits,   G4int GenNo, const G4String& cIn, int copyNum);
    bool FinalizeOptical();


    void FillParticleStep(const G4Step *);

    void FillGeometry(const G4String &filename);

public:

    RootMessenger *fMessenger;
    std::map<G4String, OpticalDigitizer*> fDigitizers;


    /*              */
    /* Root Outputs */
    /*              */

    G4String outfile_name;
    TFile *rootFile;
    TTree *tr;

    Int_t fStart; // Run Number, Initialized to 0. Set method: RootManager::SetStartID(int id)
    Int_t fEvtNb; // Event Number, Initialized to 100000. Set method: RootManager::SetNbEvent(int id)
    Int_t fEvtN{};

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

private:
    RootManager();

};

extern RootManager *dRootMng;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
