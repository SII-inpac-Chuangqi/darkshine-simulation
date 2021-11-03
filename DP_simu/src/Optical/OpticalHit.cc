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
/// \file OpticalHit.cc
/// \brief Implementation of the OpticalHit class

#include "Optical/OpticalHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

//G4ThreadLocal G4Allocator<OpticalHit> *OpticalHitAllocator = nullptr;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpticalHit::OpticalHit(G4int detID) : fDetID(detID) { ; }
//        : G4VHit(), fDetID(detID) { ; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpticalHit::~OpticalHit() = default;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpticalHit::OpticalHit(const OpticalHit &right) {
//        : G4VHit() {
    fDetID = right.fDetID;
//    fCellID=right.fCellID;
    // fPos=right.fPos;
    fDetectedFlag = right.fDetectedFlag;
    fIsLUT = right.fIsLUT;
    // fDepth=right.fDepth;
    // fCosTheta=right.fCosTheta;
    fE2 = right.fE2;
    // fE1=right.fE1;
    fT2 = right.fT2;
    // fT1=right.fT1;
    // fT0=right.fT0;
    fType = right.fType;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpticalHit &OpticalHit::operator=(const OpticalHit &right) = default;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool OpticalHit::operator==(const OpticalHit &right) const {
    return (this == &right);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

bool OpticalHit::BuildLUTHit(OpticalLUT &LUT, G4double depth, G4double cosTheta, G4double creationT, G4double creationE,
                             const G4ThreeVector& /*creationP*/, G4double rand1, G4double rand2, G4EmProcessSubType Type) {
    if (!LUT.isReady()) {
        G4cerr << "Error: Not valid LUT, should not happen." << G4endl;
        return false;
    }
    //basic info
    fType = Type;
    // Find efficiency and then isdetected
    double efficiency = LUT.efficiency(depth, cosTheta);
    fDetectedFlag = (rand1 < efficiency);
    fIsLUT = true;

    // fDepth = depth;
    // fCosTheta = cosTheta;
    // Find deltaT and arrival time
    // fCreationT = creationT;
    // fT1=creationT;
    fT2 = creationT + LUT.transitTime(depth, cosTheta, rand2);

    //Deal with energy change (shift?)
    // Dummy now
    // fE1 = creationE;
    fE2 = creationE + 0;

    //Pos set to center?
    // fPos=creationP;

    return fDetectedFlag;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//for minimum storagr, we removed the Pos storage. Sorry we couldnot draw you!

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpticalHit::Print() const {
    G4cout
            << " detID: " << fDetID
            << " isLUT:" << fIsLUT
            << " IsDetected:" << fDetectedFlag
            //     << " CellID:" << fCellID
            //  << " Pos:" << G4BestUnit(fPos,"Length")
            //  << " Depth:" << G4BestUnit(fDepth,"Length")
            //  << " CosTheta:" << fCosTheta
            << " ArrivalE:" << G4BestUnit(fE2, "Energy")
            //  << " DeltaE:" << G4BestUnit(GetDeltaE(),"Energy")
            << " ArrivalT:" << G4BestUnit(fT2, "Length")
            //  << " DeltaT:" << G4BestUnit(GetDeltaT(),"Time")
            << " Type:" << GetType()
            << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
