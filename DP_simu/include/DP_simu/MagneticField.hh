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
/// \file MagneticField.hh
/// \brief Definition of the MagneticField class

#ifndef MagneticField_h
#define MagneticField_h 1

#include "TFile.h"

#include "Control/Control.h"
#include "G4MagneticField.hh"
#include "Object/DMagnet.h"
#include "G4ThreeVector.hh"

class G4FieldManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

///  A class for control of the Magnetic Field of the detector.
///  The field is assumed to be uniform.
 
class MagneticField : public G4MagneticField
{
  public:
    MagneticField();
    virtual ~MagneticField();
      
    //Set the field (fieldValue,0,0)
    void SetUniformMagFieldVector(G4ThreeVector);

    void GetFieldValue( const G4double Point[4],
                        G4double* Bfield ) const override;

  protected:
    // Find the global Field Manager
    G4FieldManager* GetGlobalFieldManager(); // static

  private:
    TFile* f;
    std::vector<DMagnet*> BField;
    G4ThreeVector mag_field_vector;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
