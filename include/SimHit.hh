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
// $Id$
//
/// \file SimHit.hh
/// \brief Definition of the SimHit class

#ifndef SimHit_h
#define SimHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

/// Calorimeter hit class
///
/// It defines data members to store the the energy deposit and track lengths
/// of charged particles in a selected volume:
/// - fEdep, fTrackLength

class SimHit : public G4VHit
{
  public:
    SimHit();
    SimHit(const SimHit&);
    virtual ~SimHit();

    // operators
    const SimHit& operator=(const SimHit&);
    G4int operator==(const SimHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw() {}
    virtual void Print();

    // methods to handle data
    void SetT(G4double t);
    void SetX(G4double x) { fx = x; };
    void SetY(G4double y) { fy = y; };
    void SetZ(G4double z) { fz = z; };
    void AddEnergyDep(G4double, G4double);
    void SetXYZ(G4double, G4double, G4double);
    void SetParticleID(G4int a) { fID = a; };
    void SetPDG(G4int a) { fPDG = a; };
    void SetDetectorID(G4ThreeVector a) { fDetectorID = a; };
    void SetType(G4int a) { fType = a; }; 
    void SetDetectorRepNo(G4int a) { fDetectorRepNo = a; }; 

    // get methods
    G4double GetEdep() const { return fEdep; };
    G4double GetTime() const { return ft; };
    G4double GetX() const { return fx; };  
    G4double GetY() const { return fy; };  
    G4double GetZ() const { return fz; };  
    G4double GetEdepEM() const  { return fEdepEM; };
    G4double GetEdepHad() const { return fEdepHad; };
    G4int    GetParticleID() const { return fID; };
    G4int    GetPDG() const { return fPDG; };
    G4int    GetType() const { return fType; };
    G4ThreeVector GetDetectorID() const { return fDetectorID; };
    G4int    GetDetectorRepNo() const { return fDetectorRepNo; };

  private:
    G4double fEdep;        ///< Energy deposit in the sensitive volume
    G4double fEdepEM;
    G4double fEdepHad;
    G4double fTrackLength; ///< Track length in the  sensitive volume
    G4double ft;
    G4double fx;
    G4double fy;
    G4double fz;
    G4double fw;

    G4int fType;
    G4int fID;
    G4int fPDG;
    G4ThreeVector fDetectorID;
    G4int fDetectorRepNo;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<SimHit> SimHitsCollection;

extern G4Allocator<SimHit> SimHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* SimHit::operator new(size_t)
{
    void *hit;
    hit = (void *) SimHitAllocator.MallocSingle();
    return hit;
}

inline void SimHit::operator delete(void *hit)
{
    SimHitAllocator.FreeSingle((SimHit*) hit);
}

inline void SimHit::AddEnergyDep(G4double EdepEM, G4double EdepHad) {
    fEdepEM += EdepEM; 
    fEdepHad += EdepHad;

    fw = (fEdep + fEdepEM + fEdepHad <= 0) ? 0. : fEdep/(fEdep + fEdepEM + fEdepHad) ;
    
    fEdep = (fEdepEM + fEdepHad);
}

inline void SimHit::SetXYZ(G4double x, G4double y, G4double z) {
    fx = x;
    fy = y;
    fz = z;
}

inline void SimHit::SetT(G4double t) {

    ft = fw*ft + (1-fw)*t;

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
