//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file OpticalHit.hh
/// \brief Definition of the OpticalHit class

#ifndef OpticalHit_h
#define OpticalHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"
#include "G4ios.hh"

#include "OpticalLUT.hh"
// #include "OpticalTrackData.hh"

#include "G4EmProcessSubType.hh"

/// Optical hit class (slimmed)
///
/// It defines the optical photon hit, 
// which the optical track should carry aux information.
/// - fDepth, fcosTheta, fE, fT, 
//TODO: consider which variables really need and simplify the storage!!
//TODO: and determine which use constructor, which use setter??
class OpticalHit : public G4VHit
{
  public:
    OpticalHit(G4int detID);
    OpticalHit(const OpticalHit&);

    // OpticalHit(G4int fDetId, const G4ThreeVector &fCellId, const G4ThreeVector &fPos, int fDetectedFlag, bool fIsLUT,
    //            G4double fDepth, G4double fCosTheta, G4double fE2, G4double fE1, G4double fT2, G4double fT1,
    //            G4EmProcessSubType fType);

    virtual ~OpticalHit();

    // operators
    const OpticalHit& operator=(const OpticalHit&);
    G4bool operator==(const OpticalHit&) const;
    // bool operator < (const OpticalHit h) const
    // {
    //     return (fT2 < h.fT2);
    // }
    // bool operator > (const OpticalHit h) const
    // {
    //     return (fT2 > h.fT2);
    // }

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw();
    virtual void Print();

    // Build methods
    bool BuildLUTHit(OpticalLUT& LUT, G4double depth, G4double cosTheta,
            G4double T1, G4double E1,G4ThreeVector P1,
            G4double rand1, G4double rand2,
            G4EmProcessSubType Type=fGammaGeneralProcess); //LUT photon, fGammaGeneralProcess is dummy
    //In production DSimu, no need for real ph and gen ph
    // bool BuildREALHit(OpticalTrackData *AUX,
    //         G4double T2, G4double E2,G4ThreeVector P2,
    //         G4EmProcessSubType Type=fGammaGeneralProcess); //REAL (recieved photons) from SD
    // bool BuildGENHit(G4double depth, G4double cosTheta,
    //         G4double T1, G4double E1,G4ThreeVector P1,
    //         G4EmProcessSubType Type=fGammaGeneralProcess); //GEN (generated photons,
             // note we here not used track ending method anymore, just record at scint process)

    // Set methods
//    void SetCellID     (G4ThreeVector cID)      { fCellID=cID; };  //now is dummy
    // void SetPos      (G4ThreeVector xyz){ fPos = xyz; }; 
    // void SetArrivalE     (G4double e)      { fArrivalE = e; };
    // void SetDeltaE     (G4double de)      { fDeltaE = de; };
    // void SetDeltaT     (G4double dt)      { fDeltaT=dt; };
    // void SetDpeth     (G4double d)      { fDepth=d; };
    // void SetCosTheta     (G4double ct)      { fCosTheta=ct; };
    
    
    // Get methods: basic
    G4int GetDetID() const { return fDetID; };
//    G4ThreeVector GetCellID() const { return fCellID; };
//    G4ThreeVector GetPos() const { return fPos; };
//    G4double GetT0() const { return fT0; };
    // G4double GetT1() const { return fT1; }; //T0 not saved here, saved in scint process
    G4double GetT2() const { return fT2; };
    // G4double GetE1() const { return fE1; };
    G4double GetE2() const { return fE2; };
    // G4double GetDepth() const { return fDepth; };
    // G4double GetCosTheta() const { return fCosTheta; };
    //Get: composed
    G4double GetArrivalE() const { return fE2; };
    // G4double GetDeltaE() const { return fE2-fE1; };
    // G4double GetCreateE() const { return fE1; };
    G4double GetArrivalT() const { return fT2; };
    // G4double GetCreateT() const { return fT1; };
//    G4double GetInitialT() const { return fT0; };
//    G4double GetTotalDT() const { return fT2-fT0; };
//    G4double GetScintT() const { return fT1-fT0; };
    // G4double GetDeltaT() const { return fT2-fT1; };

    bool isLUT() const {return fIsLUT;};
    bool isDetected() const {return fDetectedFlag==1;};
    bool isGen() const {return fDetectedFlag==-1;};
    int GetDetectedStatus() const {return fDetectedFlag==1;};
    bool isValid() const {return fType>=fCerenkov;};
    bool isScint() const {return fType==fScintillation;};
    bool isCerekov() const {return fType==fCerenkov;};
    G4int GetType() const {return fType;};

    // G4double GetT0(){return fT0;};
    // void SetT0(G4double v){fT0=v;};

  private:
     //slimmed for minimum storage
      // G4int         fTrackID;
      // G4int         fChamberNb;
      // G4double      fEdep;

      G4int fDetID{-1}; // 1-D No.
//      G4ThreeVector fCellID{-1,-1,-1}; // 3-D No., or we may store real hit position
      // G4ThreeVector fPos{-999,-999,-999}; // real position (P1 or P2)

      int fDetectedFlag{-1};
      bool fIsLUT{false};

      // G4double fDepth{-999}; //taken from track aux data
      // G4double fCosTheta{-2};

//save nominal value, do not convert unit here!. Only convert when get.!
      G4double fE2{-2}; //arrival photon energy
      // G4double fE1{-1}; //photon generation energy
      G4double fT2{-3}; //arrival T
      // G4double fT1{-2}; //photonstart time
      // G4double fT0{-1};
//      G4double fT0{0}; //scint happen time
      G4EmProcessSubType  fType{fGammaGeneralProcess};

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<OpticalHit> OpticalHitsCollection;

extern G4ThreadLocal G4Allocator<OpticalHit>* OpticalHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* OpticalHit::operator new(size_t)
{
  if(!OpticalHitAllocator)
      OpticalHitAllocator = new G4Allocator<OpticalHit>;
  return (void *) OpticalHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void OpticalHit::operator delete(void *hit)
{
  OpticalHitAllocator->FreeSingle((OpticalHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
