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
/// \file DetectorSD.cc
/// \brief Implementation of the DetectorSD class

#include "RootManager.hh"
#include "DetectorSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorSD::DetectorSD(     G4int Type,
                            const G4String& name, 
                            G4ThreeVector CellID,
                            RootManager* rootMng
                            )
 : G4VSensitiveDetector(name)
{
  fRootMng = rootMng;
  fCellID  = CellID;
  fType    = Type;
  fname    = name;

  fRootMng->bookCollection(name);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorSD::~DetectorSD() 
{ 
    //delete fHitsCollection;
    //delete vHitsCollection;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorSD::Initialize(G4HCofThisEvent*)
{
    if ( fType != 0 ) {
        for(int i=0; i<fCellID.x()*fCellID.y()*fCellID.z(); i++)
            fSimHitVec.push_back( new SimHit() );

    std::cout<<fCellID.x()*fCellID.y()*fCellID.z()<<std::endl;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool DetectorSD::ProcessHits(G4Step* step, 
                                     G4TouchableHistory*)
{  
    // energy deposit
    G4double edep = step->GetTotalEnergyDeposit();
    
    // step length
    G4double stepLength = 0.;
    if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. ) {
      stepLength = step->GetStepLength();
    }
    
    if ( edep==0. && stepLength == 0. ) return false;      
    
    G4TouchableHistory* touchable
      = (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());
      
    // Get calorimeter cell id 
    G4int reNumber = touchable->GetReplicaNumber();
    G4int xID = (int)fCellID.x();
    G4int yID = (int)fCellID.y();
    //G4int zID = (int)fCellID.z();
    G4ThreeVector CellID(0,0,0);
    CellID.setZ( (int)(reNumber/(xID*yID)) + 1);
    CellID.setX( (reNumber%(xID*yID)) % xID + 1  ); 
    CellID.setY( (int)((reNumber%(xID*yID))/yID) + 1 );
    if ( fType == 2) {
        if ( (int)CellID.z() % 2 == 0) {
            CellID.setX( 1 ); 
            CellID.setY( ((reNumber%(xID*yID)) % yID) + 1 );
        } else {
            CellID.setY( 1 ); 
            CellID.setX( ((reNumber%(xID*yID)) % yID) + 1 );
        }
    }
    
    // Get hit accounting data for this cell
    SimHit* hit;
    if ( !fType ) hit = new SimHit();
    else          hit = fSimHitVec[reNumber];
    
    // Calculate the center position of this cell
    G4ThreeVector origin(0.,0.,0.);
    G4ThreeVector CellPosition = step->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->
                                 GetTopTransform().Inverse().TransformPoint(origin);
    G4ThreeVector HitPoint      = step->GetPreStepPoint()->GetPosition();
    
    // Calculate Energy Deposition from EM or Hadron
    G4int PDG = step->GetTrack()->GetDefinition()->GetPDGEncoding();
    G4double E_EM = 0;
    G4double E_Had= 0;
    // electron, positron, gamma
    if ( PDG == 11 || PDG == -11 || PDG == 22 ) E_EM  = edep;
    else                                        E_Had = edep;


    // Add values
    hit->AddEnergyDep( E_EM, E_Had );
    hit->SetT( step->GetPostStepPoint()->GetGlobalTime() );
    hit->SetDetectorID( CellID );
    hit->SetParticleID( step->GetTrack()->GetTrackID() );
    hit->SetPDG( reNumber );
    hit->SetType( fType );
    hit->SetDetectorRepNo( reNumber+1 ); // replica start from 0 in DetectorConstruction
    if ( !fType ) { 
        hit->SetXYZ( HitPoint.x(), HitPoint.y(), HitPoint.z() ); 
        fRootMng->FillSimHit(fname, hit);

        delete hit;
    }
    else 
        hit->SetXYZ( CellPosition.x(), CellPosition.y(), CellPosition.z() );
    
    //G4cout<<fname<<", "<<reNumber<<", "<<hit->GetEdep()<<", Edep "<<edep<<G4endl;

    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorSD::EndOfEvent(G4HCofThisEvent* )
{
    if ( fType != 0 ) {
        for ( itr = fSimHitVec.begin(); itr != fSimHitVec.end(); itr++ ) { 
            if( (*itr)->GetEdep() >= 1e-10 ) fRootMng->FillSimHit(fname, (*itr));
            delete (*itr);
        }
    }
    fSimHitVec.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
