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
/// \file MagneticField.cc
/// \brief Implementation of the MagneticField class

#include "DP_simu/MagneticField.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MagneticField::MagneticField()
        : G4MagneticField() {
    GetGlobalFieldManager()->SetDetectorField(nullptr);
    if ( ! dControl->uniform_mag_field ) {
        f = new TFile(dControl->mag_field_input);
        if (!f) {
            std::cerr << "[READFILE ERROR] ==> File: " + dControl->mag_field_input + " does not exist." << std::endl;
        }
        BField = std::vector<DMagnet*>({dynamic_cast<DMagnet*>(f->Get("magnet0")),
                                        dynamic_cast<DMagnet*>(f->Get("magnet1")),
                                        dynamic_cast<DMagnet*>(f->Get("magnet2"))});

        dRootMng->getRootFile()->cd();
        for ( auto b : BField ) {
            b->Write();
        }

    }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MagneticField::~MagneticField() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void MagneticField::GetFieldValue(const G4double Point[4],
                                  G4double *Bfield) const {
    if (dControl->uniform_mag_field) {
        Bfield[0] = mag_field_vector.x();
        Bfield[1] = mag_field_vector.y();
        Bfield[2] = mag_field_vector.z();
    } else {
        for ( int i = 0; i < 3; i++ ) {
            Bfield[i] = BField.at(i)->GetField(Point[0], Point[1], Point[2]) * tesla;
        }
    }
    if ( dControl->mag_verbose == 1 )
    {
        std::cout << "Point = " << Point[0] << ", " << Point[1] << ", " << Point[2] << std::endl;
        std::cout << "Bfiled = "<< Bfield[0] << ", " << Bfield[1] << ", " << Bfield[2] << std::endl;
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MagneticField::SetUniformMagFieldVector(G4ThreeVector filedVector) {
    mag_field_vector = filedVector;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4FieldManager *MagneticField::GetGlobalFieldManager() {
    return G4TransportationManager::GetTransportationManager()->GetFieldManager();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
