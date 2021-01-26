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
#include <utility>

#include "Optical/OpticalLUTManager.hh"

std::map<G4String, std::shared_ptr<OpticalLUT>> OpticalLUTManager::fLUTs;
// G4String OpticalLUTManager::baseDir="./";

OpticalLUTManager::OpticalLUTManager() { ; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

OpticalLUTManager::~OpticalLUTManager() { ; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//now this is not used since no good way to validate LUT
std::shared_ptr<OpticalLUT> OpticalLUTManager::getLUT(const G4String &unitName, G4String path, const G4String &unitID) {
    if (fLUTs.find(unitName) != fLUTs.end()) {
        if (fLUTs[unitName]->isReady())
            return fLUTs[unitName];
        else {
            G4cerr << "Error: LUT invalid! require unit ID= " << unitID << G4endl;
            fLUTs[unitName]->Print();
            return nullptr;
        }
    } else {
        //Lets load one
        // OpticalLUT &LUT = *(new OpticalLUT());
        auto LUT = std::shared_ptr<OpticalLUT>(new OpticalLUT());
        LUT->initialize(std::move(path), unitID); //hard-coded convention: unitName_LUT.dat
        if (LUT->isReady()) {
            fLUTs[unitName] = LUT;
            G4cout << "Load LUT ok! validated ok: request ID= " << unitID << G4endl;
            fLUTs[unitName]->Print();
            return fLUTs[unitName];
        } else {
            G4cerr << "Error: LUT not validated, request ID=" << unitID << G4endl;
            fLUTs[unitName] = LUT;
            fLUTs[unitName]->Print();
            return nullptr;
        }
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
std::shared_ptr<OpticalLUT> OpticalLUTManager::getLUT(const G4String &unitName) // no validation
{
    if (fLUTs.find(unitName) != fLUTs.end()) {
        return fLUTs[unitName];
    } else {
        G4cerr << "Error: LUT not loaded! cIn=" << unitName << G4endl;
        return nullptr;
    }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....