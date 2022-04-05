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

#include "DP_simu/RootManager.hh"
#include "DP_simu/DetectorSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorSD::DetectorSD(G4int Type,
                       const G4String &name,
                       const G4ThreeVector &CellID
) : G4VSensitiveDetector(name) {
    fCellID = CellID;
    fType = Type;
    fname = name;

    dRootMng->bookCollection(name);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorSD::~DetectorSD() {
    for (auto simhit : fSimHitVec) {
        delete simhit;
    }

    fSimHitVec.clear();
    fSimHitVec.shrink_to_fit();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorSD::Initialize(G4HCofThisEvent *) {
    if (fType != nTracker) {
        for (int i = 0; i < fCellID.x() * fCellID.y() * fCellID.z(); i++)
            fSimHitVec.push_back(new SimulatedHit());

    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool DetectorSD::ProcessHits(G4Step *step,
                               G4TouchableHistory *) {
    // energy deposit
    G4double edep = step->GetTotalEnergyDeposit();

    // step length
    G4double stepLength = 0.;
    if (step->GetTrack()->GetDefinition()->GetPDGCharge() != 0.) {
        stepLength = step->GetStepLength();
    }

    if (edep == 0. && stepLength == 0.) return false;

    auto *touchable = (G4TouchableHistory *) (step->GetPreStepPoint()->GetTouchable());

    // Get calorimeter cell id
    reNumber1 = touchable->GetReplicaNumber(1); // for calo, depth=1

    // Get hit accounting data for this cell
    SimulatedHit *hit;
    if (!fType) hit = new SimulatedHit();
    else hit = fSimHitVec[reNumber1];

    auto xID = (int) fCellID.x();
    auto yID = (int) fCellID.y();
    //G4int zID = (int)fCellID.z();
    G4ThreeVector CellID(0, 0, 0);
    if (fType == nTracker) {
        CellID.setX(touchable->GetReplicaNumber(0) + 1);
        CellID.setY(1);
        CellID.setZ(reNumber1 + 1);
    } else if (fType == nECAL) {
        CellID.setZ((int) (reNumber1 / (xID * yID)) + 1);
        CellID.setX((reNumber1 % (xID * yID)) % xID + 1);
        CellID.setY((int) ((reNumber1 % (xID * yID)) / yID) + 1);
    } else if (fType == nHCAL || nHCAL_APD) {
        if ((int) CellID.z() % 2 == 0) {
            CellID.setX(1);
            CellID.setY(((reNumber1 % (xID * yID)) % yID) + 1);
        } else {
            CellID.setY(1);
            CellID.setX(((reNumber1 % (xID * yID)) % yID) + 1);
        }
    } else {
        std::cerr << "[ERROR] DetectorSD ==> Wrong Detecotr Type" << std::endl;
        exit(EXIT_FAILURE);
    }

    /// Save Photon
    if (fType == nHCAL_APD) {
        particleName = step->GetTrack()->GetDefinition()->GetParticleName();
        if (particleName == "opticalphoton") {
            hit->addPhoton();
        }
    }

    // Calculate the center position of this cell
    G4ThreeVector origin(0., 0., 0.);
    G4ThreeVector CellPosition = step->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->
            GetTopTransform().Inverse().TransformPoint(origin);
    G4ThreeVector HitPoint = step->GetPreStepPoint()->GetPosition();

    // Calculate Energy Deposition from EM or Hadron
    G4int PDG = step->GetTrack()->GetDefinition()->GetPDGEncoding();
    G4double E_EM = 0;
    G4double E_Had = 0;
    // electron, positron, gamma
    if (PDG == 11 || PDG == -11 || PDG == 22) E_EM = edep;
    else E_Had = edep;

    // Add values
    hit->addEdep(E_EM, E_Had);
    hit->setT(step->GetPostStepPoint()->GetGlobalTime());
    hit->setCellIdX(static_cast<int>(CellID.x()));
    hit->setCellIdY(static_cast<int>(CellID.y()));
    hit->setCellIdZ(static_cast<int>(CellID.z()));

    // Add MC particle contribution
    auto fMC = new McParticle();
    fMC->setPdg(step->GetTrack()->GetParticleDefinition()->GetPDGEncoding());
    fMC->setId(step->GetTrack()->GetTrackID());
    fMC->setEnergy(step->GetTrack()->GetKineticEnergy());
    fMC->setPx(step->GetTrack()->GetMomentum()[0]);
    fMC->setPy(step->GetTrack()->GetMomentum()[1]);
    fMC->setPz(step->GetTrack()->GetMomentum()[2]);
    if (step->GetTrack()->GetCreatorProcess())
        fMC->setCreateProcess(step->GetTrack()->GetCreatorProcess()->GetProcessName());
    hit->addParticleContribution(*fMC, edep, !fType);
    delete fMC;

    hit->setCellId(reNumber1 + 1); // replica start from 0 in DetectorConstruction
    if (fType == nTracker) {
        if (dControl->build_silicon_micro_strip) {
            hit->setX(CellPosition.x());
            hit->setY(CellPosition.y());
            hit->setZ(CellPosition.z());
            dRootMng->FillSimHit(fname, hit);

            delete hit;
        } else {
            hit->setX(HitPoint.x());
            hit->setY(HitPoint.y());
            hit->setZ(CellPosition.z());
            dRootMng->FillSimHit(fname, hit);

            delete hit;
        }
    } else {
        hit->setX(CellPosition.x());
        hit->setY(CellPosition.y());
        hit->setZ(CellPosition.z());
    }

    //G4cout<<fname<<", "<<reNumber<<", "<<hit->GetEdep()<<", Edep "<<edep<<G4endl;

    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorSD::EndOfEvent(G4HCofThisEvent *) {
    if (fType != nTracker) {
        for (auto simhit : fSimHitVec) {
            if (simhit->getE() >= 1e-10) dRootMng->FillSimHit(fname, simhit);
            delete simhit;
        }
    }
    fSimHitVec.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
