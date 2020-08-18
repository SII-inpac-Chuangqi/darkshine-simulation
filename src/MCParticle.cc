
#include "MCParticle.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4Allocator<MCParticle> MCParticleAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MCParticle::MCParticle() 
{
  _id               = 0 ;
  _PDG              = 0 ;
  _ParentID         = 0 ;
  _Energy           = 0 ;
  _Momentum         = G4ThreeVector() ;
  _VPos             = G4ThreeVector() ;
  _EPos             = G4ThreeVector() ;
  _CreateProcess    = "BLANK";
  _ProcessType      = 0 ;
  _ProcessSubType   = 0 ;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MCParticle::~MCParticle() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MCParticle::Initialize() 
{
  _id               = 0 ;
  _PDG              = 0 ;
  _ParentID         = 0 ;
  _Energy           = 0 ;
  _Momentum         = G4ThreeVector() ;
  _VPos             = G4ThreeVector() ;
  _EPos             = G4ThreeVector() ;
  _CreateProcess    = "BLANK";
  _ProcessType      = 0 ;
  _ProcessSubType   = 0 ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MCParticle::MCParticle(const MCParticle& right)
{
  _id               = right._id             ;
  _PDG              = right._PDG            ;
  _ParentID         = right._ParentID       ;
  _Energy           = right._Energy         ;
  _Momentum         = right._Momentum       ;
  _VPos             = right._VPos           ;
  _EPos             = right._EPos           ;
  _CreateProcess    = right._CreateProcess  ;
  _ProcessType      = right._ProcessType    ;
  _ProcessSubType   = right._ProcessSubType ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const MCParticle& MCParticle::operator=(const MCParticle& right)
{
  _id               = right._id             ;
  _PDG              = right._PDG            ;
  _ParentID         = right._ParentID       ;
  _Energy           = right._Energy         ;
  _Momentum         = right._Momentum       ;
  _VPos             = right._VPos           ;
  _EPos             = right._EPos           ;
  _CreateProcess    = right._CreateProcess  ;
  _ProcessType      = right._ProcessType    ;
  _ProcessSubType   = right._ProcessSubType ;

  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int MCParticle::operator==(const MCParticle& right) const
{
  return ( this == &right ) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


