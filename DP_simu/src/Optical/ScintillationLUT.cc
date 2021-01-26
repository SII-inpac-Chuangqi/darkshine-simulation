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
//
////////////////////////////////////////////////////////////////////////
// Scintillation Light Class Implementation
////////////////////////////////////////////////////////////////////////
//
// File:        ScintillationLUT.cc
// Description: RestDiscrete Process - Generation of Scintillation Photons
// Version:     0.1
// Created:     2020-12-16
// Author:      Q. Liu
// Updated:     2020-12-16 Initial commit. Thanks for Geant4 provide the
//              original G4Scintillation and g2 experiment providing
//              the LUT codes.
//
// mail:        qibin.liu@sjtu.edu.cn
//
////////////////////////////////////////////////////////////////////////

#include "G4ios.hh"
#include "globals.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTypes.hh"
#include "G4EmProcessSubType.hh"
#include "G4ScintillationTrackInformation.hh" //still remains. Used to save original trackinfo like scint. type.

#include "Optical/ScintillationLUT.hh"

ScintillationLUT::ScintillationLUT(RootManager *rootMgr,
                                   const G4String &processName,
                                   G4ProcessType type)
        : G4VRestDiscreteProcess(processName, type),
          fRootMgr(rootMgr),
          fTrackSecondariesFirst(false),
          fFiniteRiseTime(false),
          fYieldFactor(1.0),
          fExcitationRatio(1.0),
          fScintillationByParticleType(false),
          fScintillationTrackInfo(false),
          fStackingFlag(true),
          fNumPhotons(0),
          fEmSaturation(nullptr),
          fUseLUT(true),
          lookupTable(nullptr)
//now we will not create real photons anymore in the DSimu
//since impossible to simu this in the full detector
//instead, we use standalone FW to study and make LUT for only one unit
// fCreatePhotons(true),
// fAddAUX(true),
// fSaveGen(true),
// fSaveUndLUT(false)
{
    SetProcessSubType(fScintillation);

#ifdef DEBUG_SCINTILLATION_LUT
    ScintTrackEDep = 0.;
    ScintTrackYield = 0.;
#endif

    fFastIntegralTable = nullptr;
    fSlowIntegralTable = nullptr;

    if (verboseLevel > 0) {
        G4cout << GetProcessName() << " is created " << G4endl;
    }
    fUseLUT = dControl->Optical_UseLUT;
    if (fUseLUT) {
        G4cout << "Scint LUT is enabled: " << G4endl;
        //pre-load LUT, and validate with configured name
        for (auto &imap : dControl->Optical_GetLUTDefinations()) {
            //map<detName,<path, ID>>, ID is used to validate the design version
            //and to prevent using wrong LUT files.
            OpticalLUTManager::getLUT(imap.first, imap.second.first, imap.second.second); //validation mode
        }
    }
}

ScintillationLUT::~ScintillationLUT() {
    if (fFastIntegralTable != nullptr) {
        fFastIntegralTable->clearAndDestroy();
        delete fFastIntegralTable;
    }
    if (fSlowIntegralTable != nullptr) {
        fSlowIntegralTable->clearAndDestroy();
        delete fSlowIntegralTable;
    }
}

G4bool ScintillationLUT::IsApplicable(const G4ParticleDefinition &aParticleType) {
    return !(aParticleType.GetPDGCharge() == 0.0 ||
             aParticleType.IsShortLived());
}

void ScintillationLUT::BuildPhysicsTable(const G4ParticleDefinition &) {
    if (fFastIntegralTable != nullptr) {
        fFastIntegralTable->clearAndDestroy();
        delete fFastIntegralTable;
        fFastIntegralTable = nullptr;
    }
    if (fSlowIntegralTable != nullptr) {
        fSlowIntegralTable->clearAndDestroy();
        delete fSlowIntegralTable;
        fSlowIntegralTable = nullptr;
    }
    BuildThePhysicsTable();
}

// AtRestDoIt
// ----------
//
G4VParticleChange *
ScintillationLUT::AtRestDoIt(const G4Track &aTrack, const G4Step &aStep)

// This routine simply calls the equivalent PostStepDoIt since all the
// necessary information resides in aStep.GetTotalEnergyDeposit()

{
    return ScintillationLUT::PostStepDoIt(aTrack, aStep);
}

// PostStepDoIt
// -------------
//
G4VParticleChange *
ScintillationLUT::PostStepDoIt(const G4Track &aTrack, const G4Step &aStep)

// This routine is called for each tracking step of a charged particle
// in a scintillator. A Poisson/Gauss-distributed number of photons is
// generated according to the scintillation yield formula, distributed
// evenly along the track segment and uniformly into 4pi.
{

    // Start process
    aParticleChange.Initialize(aTrack);
    fNumPhotons = 0;

    const G4DynamicParticle *aParticle = aTrack.GetDynamicParticle();
    const G4Material *aMaterial = aTrack.GetMaterial();

    G4StepPoint *pPreStepPoint = aStep.GetPreStepPoint();
    G4StepPoint *pPostStepPoint = aStep.GetPostStepPoint();

    const G4ThreeVector &x0 = pPreStepPoint->GetPosition();
    G4ThreeVector p0 = aStep.GetDeltaPosition().unit();
    G4double t0 = pPreStepPoint->GetGlobalTime();

    G4double TotalEnergyDeposit = aStep.GetTotalEnergyDeposit();

    G4MaterialPropertiesTable *aMaterialPropertiesTable =
            aMaterial->GetMaterialPropertiesTable();
    if (!aMaterialPropertiesTable) {
        if (verboseLevel > 0) G4cout << "No MPT " << aMaterial->GetName() << G4endl;
        return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
    }

    G4MaterialPropertyVector *Fast_Intensity =
            aMaterialPropertiesTable->GetProperty(kFASTCOMPONENT);
    G4MaterialPropertyVector *Slow_Intensity =
            aMaterialPropertiesTable->GetProperty(kSLOWCOMPONENT);

    if (!Fast_Intensity && !Slow_Intensity) {//no scitilation
        if (verboseLevel > 0) {
            G4cout << "No Parameter of Scint " << G4endl;
            aMaterialPropertiesTable->DumpTable();
        }
        return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
    }

    G4int nscnt = 1;
    if (Fast_Intensity && Slow_Intensity)
        nscnt = 2;

    G4double ScintillationYield = 0.;
    fYieldFactor = dControl->Optical_YieldFactor;

    // Scintillation depends on particle type, energy deposited
    if (fScintillationByParticleType) {

        ScintillationYield =
                GetScintillationYieldByParticleType(aTrack, aStep);

        // The default linear scintillation process
    } else {

        ScintillationYield = aMaterialPropertiesTable->GetConstProperty(kSCINTILLATIONYIELD);

        // Units: [# scintillation photons / MeV]
        // ScintillationYield *= fYieldFactor; //yield factor is set by messenger
    }
    ScintillationYield *= fYieldFactor; //yield factor is set by messenger

    G4double ResolutionScale = aMaterialPropertiesTable->GetConstProperty(kRESOLUTIONSCALE);

    // Birks law saturation:

    //G4double constBirks = 0.0;

    //constBirks = aMaterial->GetIonisation()->GetBirksConstant();

    G4double MeanNumberOfPhotons;

    // Birk's correction via fEmSaturation and specifying scintillation by
    // by particle type are physically mutually exclusive

    if (fScintillationByParticleType)
        MeanNumberOfPhotons = ScintillationYield;
    else if (fEmSaturation)
        MeanNumberOfPhotons = ScintillationYield *
                              (fEmSaturation->VisibleEnergyDepositionAtAStep(&aStep));
    else
        MeanNumberOfPhotons = ScintillationYield * TotalEnergyDeposit;

    // CALCULATE real ph number
    // if (MeanNumberOfPhotons > 10.) //we no need to generate this real photon number?
    // {
    //   G4double sigma = ResolutionScale * std::sqrt(MeanNumberOfPhotons);
    //   fNumPhotons = G4int(G4RandGauss::shoot(MeanNumberOfPhotons, sigma) + 0.5);
    // }
    // else
    // {
    //   fNumPhotons = G4int(G4Poisson(MeanNumberOfPhotons));
    // }
    // if (fNumPhotons <= 0 || !fStackingFlag)
    // {
    //   // return unchanged particle and no secondaries
    //   aParticleChange.SetNumberOfSecondaries(0);
    //   return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
    // }

    ////////////////////////////////////////////////////////////////
    // SET TRACK STATUS
    // only suspend primary when create real photon

    // aParticleChange.SetNumberOfSecondaries(fNumPhotons); //need this??
    // if (fCreatePhotons && fTrackSecondariesFirst) //no need for suspend track
    // {
    //   if (aTrack.GetTrackStatus() == fAlive)
    //     aParticleChange.ProposeTrackStatus(fSuspend);
    // }
    ////////////////////////////////////////////////////////////////


    // #############################################################
    // calculations required information for lookup hits (part1: Geometry naming)
    // #############################################################
    // extract more information using touchable and touchable history method
    G4VTouchable const *touchable = aStep.GetPreStepPoint()->GetTouchable();
    G4TouchableHandle const touchable_counted = aStep.GetPreStepPoint()->GetTouchableHandle();
    G4NavigationHistory const *navHistory = touchable_counted->GetHistory();

    G4String cIn = touchable_counted->GetVolume(1)->GetLogicalVolume()->GetName(); //like, ECAL_LVW
    cIn = cIn.remove(cIn.index("_LVW"));
    G4int copyNum = touchable_counted->GetCopyNumber(1);
    G4bool calobound_good = true; //now dummy. used to filter non-optical region where optical property setted

    // INTRODUCE LUT PHOTONS NUMBER
    G4double MeanNumberOfPhotonsDecimated = 0; //scaled photons with max eff in LUT
    //////////////////////////////////////////////////////////
    G4bool skipLookupHit = ((!calobound_good) || !fUseLUT);
    if (!skipLookupHit) {
        lookupTable = OpticalLUTManager::getLUT(cIn); // no validation needed since in pre-load is validated.
        MeanNumberOfPhotonsDecimated = MeanNumberOfPhotons / (lookupTable->getFactor());
    }

    G4int fNumPhotonsLUT = 0; //scaled LUT photons

    // LUT number calculate.
    // FOLLOW same maner to generate decimated photons
    if (MeanNumberOfPhotonsDecimated > 10.) {
        G4double sigma = ResolutionScale * std::sqrt(MeanNumberOfPhotonsDecimated);
        fNumPhotonsLUT = lround(G4RandGauss::shoot(MeanNumberOfPhotonsDecimated, sigma) + 0.5);
    } else if (MeanNumberOfPhotonsDecimated > 0.) {
        fNumPhotonsLUT = G4int(G4Poisson(MeanNumberOfPhotonsDecimated));
    } else {
        skipLookupHit = true;
        //   fNumPhotonsLUT = 0; //for safe
    }

    aParticleChange.SetNumberOfSecondaries(0); //cancel all real secondaries

    if (fNumPhotonsLUT <= 0)
        skipLookupHit = true;

    fNumPhotons = fNumPhotonsLUT; //overwrite numPhoton with LUT scaled value; note, no real photon anymore.
    G4int fNumPhotonsLUTGen = MeanNumberOfPhotons; //we record this to final result
    // #############################################################
    // calculations required information for lookup hits (part2: Geometry transformation)
    // #############################################################
    // Now, extract detailed geometry information
    //information needed for calculation of LUT input.
    G4VSolid *caloBound;
    G4int calobound_depth = 0;
    G4double offset_z = 0.;
    G4RotationMatrix const *rot = touchable->GetRotation();
    if (!skipLookupHit) {
        // Shift z is the distance from local origin to surface along local z
        calobound_depth = navHistory->GetDepth() - 1;
        caloBound = touchable_counted->GetVolume(1)->GetLogicalVolume()->GetSolid(); //LV="ECAL_LVW"
        //use general method to extrive LV, or for the current LV, eaiser way is just ->LV xxx.
        //             calobound_depth = navHistory->GetDepth();
        //             caloBound = touchable_counted->GetVolume(0)->GetLogicalVolume()->GetSolid(); //LV="outlineLV"
        offset_z = caloBound->DistanceToOut(G4ThreeVector(0, 0, 0), G4ThreeVector(0, 0, 1));
        //NB: our LUT origin is the left boundary of the *Whole Unit*, include the wrapper, not the crystal!
    } else //in other part which optical properties enabled but no need for optical
    {
        if (verboseLevel > 0) G4cout << "ScintLUT: skipLUT" << G4endl;
        return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
        // return pParticleChange; //?
    }
    // #############################################################

    //save T0
    fRootMgr->SetOpticalTimeZero(t0, cIn); //global for one unit / event
    //used for trig simulation. Bu now not implemented. So global timeZero=0 for now.

    // MAIN LOOP
    //  here scnt means scitinlation type:
    std::vector<OpticalHit *> Hits; //used to buffer hits
    G4int materialIndex = aMaterial->GetIndex();
    Long64_t LUT_counter = 0;
    Long64_t LUT_saved_counter = 0;
    G4int Num = fNumPhotons;
    for (G4int scnt = 1; scnt <= nscnt; scnt++) {
        // Retrieve the Scintillation Integral for this material
        // new G4PhysicsOrderedFreeVector allocated to hold CII's
        G4double ScintillationTime = 0. * ns;
        G4double ScintillationRiseTime = 0. * ns;
        G4PhysicsOrderedFreeVector *ScintillationIntegral = nullptr;
        G4ScintillationType ScintillationType = Slow;

        if (scnt == 1) { //
            if (nscnt == 1) { // only faat or slow component of sci. light
                if (Fast_Intensity) {
                    ScintillationTime = aMaterialPropertiesTable->GetConstProperty(kFASTTIMECONSTANT);
                    if (fFiniteRiseTime) {
                        ScintillationRiseTime = aMaterialPropertiesTable->GetConstProperty(kFASTSCINTILLATIONRISETIME);
                    }
                    ScintillationType = Fast;
                    ScintillationIntegral =
                            (G4PhysicsOrderedFreeVector *) ((*fFastIntegralTable)(materialIndex));
                }
                if (Slow_Intensity) {
                    ScintillationTime = aMaterialPropertiesTable->GetConstProperty(kSLOWTIMECONSTANT);
                    if (fFiniteRiseTime) {
                        ScintillationRiseTime = aMaterialPropertiesTable->GetConstProperty(kSLOWSCINTILLATIONRISETIME);
                    }
                    ScintillationType = Slow;
                    ScintillationIntegral =
                            (G4PhysicsOrderedFreeVector *) ((*fSlowIntegralTable)(materialIndex));
                }
            } else { //fast + slow components: add fast
                G4double yieldRatio = aMaterialPropertiesTable->GetConstProperty(kYIELDRATIO);
                if (fExcitationRatio == 1.0 ||
                    fExcitationRatio == 0.0) //by default, this ratio is not set then taken yield ratio from material
                {
                    Num = G4int(std::min(yieldRatio, 1.0) * fNumPhotons);
                } else {
                    Num = G4int(std::min(fExcitationRatio, 1.0) *
                                fNumPhotons); //if set exctition ratio, use this (for both )
                }
                ScintillationTime = aMaterialPropertiesTable->GetConstProperty(kFASTTIMECONSTANT);
                if (fFiniteRiseTime) {
                    ScintillationRiseTime = aMaterialPropertiesTable->GetConstProperty(kFASTSCINTILLATIONRISETIME);
                }
                ScintillationType = Fast;
                ScintillationIntegral =
                        (G4PhysicsOrderedFreeVector *) ((*fFastIntegralTable)(materialIndex));
            }
        } else { //fast + slow components: add slow
            Num = fNumPhotons - Num;
            ScintillationTime = aMaterialPropertiesTable->GetConstProperty(kSLOWTIMECONSTANT);
            if (fFiniteRiseTime) {
                ScintillationRiseTime = aMaterialPropertiesTable->GetConstProperty(kSLOWSCINTILLATIONRISETIME);
            }
            ScintillationType = Slow;
            ScintillationIntegral =
                    (G4PhysicsOrderedFreeVector *) ((*fSlowIntegralTable)(materialIndex));
        }
        //SETTED: ScintillationTime, ScintillationRiseTime, ScintillationIntegral, ScintillationType

        if (!ScintillationIntegral)
            continue;

        // Max Scintillation Integral

        G4double CIImax = ScintillationIntegral->GetMaxValue();

        for (G4int i = 0; i < Num; i++) { //generate photons

            // Determine photon energy

            G4double CIIvalue = G4UniformRand() * CIImax;
            G4double sampledEnergy =
                    ScintillationIntegral->GetEnergy(CIIvalue);

            if (verboseLevel > 1) {
                G4cout << "sampledEnergy = " << sampledEnergy << G4endl;
                G4cout << "CIIvalue =        " << CIIvalue << G4endl;
            }

            // Generate random photon direction

            G4double cost = 1. - 2. * G4UniformRand();
            G4double sint = std::sqrt((1. - cost) * (1. + cost));

            G4double phi = twopi * G4UniformRand();
            G4double sinp = std::sin(phi);
            G4double cosp = std::cos(phi);

            G4double px = sint * cosp;
            G4double py = sint * sinp;
            G4double pz = cost;

            // Create photon momentum direction vector

            G4ParticleMomentum photonMomentum(px, py, pz);

            // Determine polarization of new photon

            G4double sx = cost * cosp;
            G4double sy = cost * sinp;
            G4double sz = -sint;

            G4ThreeVector photonPolarization(sx, sy, sz);

            G4ThreeVector perp = photonMomentum.cross(photonPolarization);

            phi = twopi * G4UniformRand();
            sinp = std::sin(phi);
            cosp = std::cos(phi);

            photonPolarization = cosp * photonPolarization + sinp * perp;

            photonPolarization = photonPolarization.unit();

            // Calculate track information ahead, then generate LUT or find from table.

            G4double rand;

            if (aParticle->GetDefinition()->GetPDGCharge() != 0) {
                rand = G4UniformRand();
            } else {
                rand = 1.0;
            }

            G4double delta = rand * aStep.GetStepLength();
            G4double deltaTime = delta / (pPreStepPoint->GetVelocity() +
                                          rand * (pPostStepPoint->GetVelocity() - pPreStepPoint->GetVelocity()) / 2.);

            // emission time distribution
            if (ScintillationRiseTime == 0.0) {
                deltaTime = deltaTime -
                            ScintillationTime * std::log(G4UniformRand());
            } else {
                deltaTime = deltaTime +
                            sample_time(ScintillationRiseTime, ScintillationTime);
            }

            G4double aSecondaryTime = t0 + deltaTime;

            G4ThreeVector aSecondaryPosition =
                    x0 + rand * aStep.GetDeltaPosition(); //might have bug?? e.g. direct beyond the boundary.

            // #############################################################
            // calculations required information for lookup hits (part3: LUT input)
            // #############################################################
            G4double depth = -1024; //default value for not caloboun_good region
            G4double local_cosTheta = -1025;
            // calculate depth into crystal for this Cerenkov photon:
            // get photon position in calobound coordinates
            G4ThreeVector depth_pos = navHistory->GetTransform(calobound_depth).TransformPoint(aSecondaryPosition);
            // finally set the depth based on the depth of caloBound
            // depth = depth_pos.z() + offset; //for all z center it is ok.
            depth = depth_pos.z() + offset_z; //shift to the surface of calo unit
            // calculate cos(theta), relative to crystal long direction, for this Cerenkov photon
            G4ThreeVector local_mom = photonMomentum;
            local_mom.transform(*rot); //all z has no rotation
            local_cosTheta = local_mom.cosTheta();
            //                    G4cout<<"  Abs location: " << aSecondaryPosition;
            //                    G4cout<<"  Rel location: " << depth_pos; //relative to outlineLV
            //                    G4cout<<"  Crystal shift(half length): " << offset_z; //relative to outlineLV
            //                    G4cout<<"  => Depth/cm: "<<depth/cm<<G4endl;

            //////////////////////////////////////////////////////
            // Now lets do the LUT
            // We need: NumPhotonsDecimated, navHistory, calobound_depth, aSecondaryPosition
            // and offset, photonMomentum, rot, lookupTable, aSecondaryTime, copyNum
            // We put: LUT hit
            // material howto copy?

            if (!skipLookupHit && lookupTable != nullptr && lookupTable->isReady()) //for safe, check LUT
            {
                // check if running a lookup-table / full-tracking comparison test
                if (i >= fNumPhotonsLUT)
                    continue;

                //We calculate the needed information before.

                // generate random numbers for lookup hit
                G4double rand1 = G4UniformRand();
                G4double rand2 = G4UniformRand();

                //Now lets create hit and push to collection
                auto *newHit = new OpticalHit(copyNum);
                LUT_counter++;
                newHit->BuildLUTHit(*lookupTable, depth, local_cosTheta, aSecondaryTime, sampledEnergy,
                                    aSecondaryPosition, rand1, rand2, fScintillation);
                // newHit->SetT0(t0);
                if (newHit->isDetected()) {
                    // fRootMgr->FillOpticalLUT(newHit, cIn); //save the hit one-by-one
                    Hits.emplace_back(newHit); //buffered
                    LUT_saved_counter++;
                }
                // delete newHit; // delete the hit one-by-one
            }
            //////////////////////////////////////////////////////
        }
    }

    if (verboseLevel > 0) {
        G4cout << "\n Exiting from ScintillationLUT::DoIt -- NumberOfSecondaries = "
               << aParticleChange.GetNumberOfSecondaries() << " (LUT mode)" << G4endl;
        G4cout << "\n LUT hits mean yields(no scaling): " << fNumPhotonsLUTGen << G4endl;
        G4cout << "\n LUT hits generated: " << LUT_counter << G4endl;
        G4cout << "\n LUT hits saved: " << LUT_saved_counter << G4endl;
        G4cout << "\n LUT hits buffered: " << Hits.size() << G4endl;
    }

    //Now save the Hits// now we save it one-by-one, not using this for memory
    if (!Hits.empty())
        fRootMgr->FillOpticalLUTs(Hits, fNumPhotonsLUTGen, cIn, copyNum);
    //now clear all hits inside digitizer since, we need all sorted hits for digitizer!!
    // for(auto *h: Hits)
    //   delete h;

    return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
}

// BuildThePhysicsTable for the scintillation process
// --------------------------------------------------
//

void ScintillationLUT::BuildThePhysicsTable() {
    if (fFastIntegralTable && fSlowIntegralTable)
        return;

    const G4MaterialTable *theMaterialTable =
            G4Material::GetMaterialTable();
    G4int numOfMaterials = G4Material::GetNumberOfMaterials();

    // create new physics table

    if (!fFastIntegralTable)
        fFastIntegralTable =
                new G4PhysicsTable(numOfMaterials);
    if (!fSlowIntegralTable)
        fSlowIntegralTable =
                new G4PhysicsTable(numOfMaterials);

    // loop for materials

    for (G4int i = 0; i < numOfMaterials; i++) {
        auto *aPhysicsOrderedFreeVector = new G4PhysicsOrderedFreeVector();
        auto *bPhysicsOrderedFreeVector = new G4PhysicsOrderedFreeVector();

        // Retrieve vector of scintillation wavelength intensity for
        // the material from the material's optical properties table.

        G4Material *aMaterial = (*theMaterialTable)[i];

        G4MaterialPropertiesTable *aMaterialPropertiesTable =
                aMaterial->GetMaterialPropertiesTable();

        if (aMaterialPropertiesTable) {

            G4MaterialPropertyVector *theFastLightVector =
                    aMaterialPropertiesTable->GetProperty(kFASTCOMPONENT);

            if (theFastLightVector) {

                // Retrieve the first intensity point in vector
                // of (photon energy, intensity) pairs

                G4double currentIN = (*theFastLightVector)[0];

                if (currentIN >= 0.0) {

                    // Create first (photon energy, Scintillation
                    // Integral pair

                    G4double currentPM = theFastLightVector->Energy(0);

                    G4double currentCII = 0.0;

                    aPhysicsOrderedFreeVector->InsertValues(currentPM, currentCII);

                    // Set previous values to current ones prior to loop

                    G4double prevPM = currentPM;
                    G4double prevCII = currentCII;
                    G4double prevIN = currentIN;

                    // loop over all (photon energy, intensity)
                    // pairs stored for this material

                    for (size_t ii = 1;
                         ii < theFastLightVector->GetVectorLength();
                         ++ii) {
                        currentPM = theFastLightVector->Energy(ii);
                        currentIN = (*theFastLightVector)[ii];

                        currentCII = 0.5 * (prevIN + currentIN);

                        currentCII = prevCII +
                                     (currentPM - prevPM) * currentCII;

                        aPhysicsOrderedFreeVector->InsertValues(currentPM, currentCII);

                        prevPM = currentPM;
                        prevCII = currentCII;
                        prevIN = currentIN;
                    }
                }
            }

            G4MaterialPropertyVector *theSlowLightVector =
                    aMaterialPropertiesTable->GetProperty(kSLOWCOMPONENT);

            if (theSlowLightVector) {

                // Retrieve the first intensity point in vector
                // of (photon energy, intensity) pairs

                G4double currentIN = (*theSlowLightVector)[0];

                if (currentIN >= 0.0) {

                    // Create first (photon energy, Scintillation
                    // Integral pair

                    G4double currentPM = theSlowLightVector->Energy(0);

                    G4double currentCII = 0.0;

                    bPhysicsOrderedFreeVector->InsertValues(currentPM, currentCII);

                    // Set previous values to current ones prior to loop

                    G4double prevPM = currentPM;
                    G4double prevCII = currentCII;
                    G4double prevIN = currentIN;

                    // loop over all (photon energy, intensity)
                    // pairs stored for this material

                    for (size_t ii = 1;
                         ii < theSlowLightVector->GetVectorLength();
                         ++ii) {
                        currentPM = theSlowLightVector->Energy(ii);
                        currentIN = (*theSlowLightVector)[ii];

                        currentCII = 0.5 * (prevIN + currentIN);

                        currentCII = prevCII +
                                     (currentPM - prevPM) * currentCII;

                        bPhysicsOrderedFreeVector->InsertValues(currentPM, currentCII);

                        prevPM = currentPM;
                        prevCII = currentCII;
                        prevIN = currentIN;
                    }
                }
            }
        }

        // The scintillation integral(s) for a given material
        // will be inserted in the table(s) according to the
        // position of the material in the material table.

        fFastIntegralTable->insertAt(i, aPhysicsOrderedFreeVector);
        fSlowIntegralTable->insertAt(i, bPhysicsOrderedFreeVector);
    }
}

void ScintillationLUT::SetScintillationByParticleType(const G4bool scintType) {
    if (fEmSaturation) {
        G4Exception("ScintillationLUT::SetScintillationByParticleType", "Scint02",
                    JustWarning, "Redefinition: Birks Saturation is replaced by ScintillationByParticleType!");
        RemoveSaturation();
    }
    fScintillationByParticleType = scintType;
}

// GetMeanFreePath
// ---------------
//

G4double ScintillationLUT::GetMeanFreePath(const G4Track &,
                                           G4double,
                                           G4ForceCondition *condition) {
    *condition = StronglyForced;

    return DBL_MAX;
}

// GetMeanLifeTime
// ---------------
//

G4double ScintillationLUT::GetMeanLifeTime(const G4Track &,
                                           G4ForceCondition *condition) {
    *condition = Forced;

    return DBL_MAX;
}

G4double ScintillationLUT::sample_time(G4double tau1, G4double tau2) {
    // tau1: rise time and tau2: decay time

    // Loop checking, 07-Aug-2015, Vladimir Ivanchenko
    while (1) {
        // two random numbers
        G4double ran1 = G4UniformRand();
        G4double ran2 = G4UniformRand();
        //
        // exponential distribution as envelope function: very efficient
        //
        G4double d = (tau1 + tau2) / tau2;
        // make sure the envelope function is
        // always larger than the bi-exponential
        G4double t = -1.0 * tau2 * std::log(1 - ran1);
        G4double gg = d * single_exp(t, tau2);
        if (ran2 <= bi_exp(t, tau1, tau2) / gg)
            return t;
    }
    return -1.0;
}

G4double ScintillationLUT::
GetScintillationYieldByParticleType(const G4Track &aTrack, const G4Step &aStep) {
    ////////////////////////////////////////
    // Get the scintillation yield vector //
    ////////////////////////////////////////

    G4ParticleDefinition *pDef = aTrack.GetDynamicParticle()->GetDefinition();

    G4MaterialPropertyVector *Scint_Yield_Vector = nullptr;

    G4MaterialPropertiesTable *aMaterialPropertiesTable = aTrack.GetMaterial()->GetMaterialPropertiesTable();

    // Get the G4MaterialPropertyVector containing the scintillation
    // yield as a function of the energy deposited and particle type

    // Protons
    if (pDef == G4Proton::ProtonDefinition())
        Scint_Yield_Vector = aMaterialPropertiesTable->GetProperty(kPROTONSCINTILLATIONYIELD);

        // Deuterons
    else if (pDef == G4Deuteron::DeuteronDefinition())
        Scint_Yield_Vector = aMaterialPropertiesTable->GetProperty(kDEUTERONSCINTILLATIONYIELD);

        // Tritons
    else if (pDef == G4Triton::TritonDefinition())
        Scint_Yield_Vector = aMaterialPropertiesTable->GetProperty(kTRITONSCINTILLATIONYIELD);

        // Alphas
    else if (pDef == G4Alpha::AlphaDefinition())
        Scint_Yield_Vector = aMaterialPropertiesTable->GetProperty(kALPHASCINTILLATIONYIELD);

        // Ions (particles derived from G4VIon and G4Ions) and recoil ions
        // below the production cut from neutrons after hElastic
    else if (pDef->GetParticleType() == "nucleus" ||
             pDef == G4Neutron::NeutronDefinition())
        Scint_Yield_Vector = aMaterialPropertiesTable->GetProperty(kIONSCINTILLATIONYIELD);

        // Electrons (must also account for shell-binding energy
        // attributed to gamma from standard photoelectric effect)
    else if (pDef == G4Electron::ElectronDefinition() ||
             pDef == G4Gamma::GammaDefinition())
        Scint_Yield_Vector = aMaterialPropertiesTable->GetProperty(kELECTRONSCINTILLATIONYIELD);

        // Default for particles not enumerated/listed above
    else
        Scint_Yield_Vector = aMaterialPropertiesTable->GetProperty(kELECTRONSCINTILLATIONYIELD);

    // If the user has specified none of the above particles then the
    // default is the electron scintillation yield
    if (!Scint_Yield_Vector)
        Scint_Yield_Vector = aMaterialPropertiesTable->GetProperty(kELECTRONSCINTILLATIONYIELD);

    // Throw an exception if no scintillation yield vector is found
    if (!Scint_Yield_Vector) {
        G4ExceptionDescription ed;
        ed << "\nG4Scintillation::PostStepDoIt(): "
           << "Request for scintillation yield for energy deposit and particle\n"
           << "type without correct entry in MaterialPropertiesTable.\n"
           << "ScintillationByParticleType requires at minimum that \n"
           << "ELECTRONSCINTILLATIONYIELD is set by the user\n"
           << G4endl;
        G4String comments = "Missing MaterialPropertiesTable entry - No correct entry in MaterialPropertiesTable";
        G4Exception("ScintillationLUT::PostStepDoIt", "Scint01",
                    FatalException, ed, comments);
    }

    ///////////////////////////////////////
    // Calculate the scintillation light //
    ///////////////////////////////////////
    // To account for potential nonlinearity and scintillation photon
    // density along the track, light (L) is produced according to:
    //
    // L_currentStep = L(PreStepKE) - L(PreStepKE - EDep)

    G4double ScintillationYield = 0.;

    G4double StepEnergyDeposit = aStep.GetTotalEnergyDeposit();
    G4double PreStepKineticEnergy = aStep.GetPreStepPoint()->GetKineticEnergy();

    if (PreStepKineticEnergy <= Scint_Yield_Vector->GetMaxEnergy()) {
        G4double Yield1 = Scint_Yield_Vector->Value(PreStepKineticEnergy);
        G4double Yield2 = Scint_Yield_Vector->Value(PreStepKineticEnergy - StepEnergyDeposit);
        ScintillationYield = Yield1 - Yield2;
    } else {
        G4ExceptionDescription ed;
        ed << "\nG4Scintillation::GetScintillationYieldByParticleType(): Request\n"
           << "for scintillation light yield above the available energy range\n"
           << "specifed in G4MaterialPropertiesTable. A linear interpolation\n"
           << "will be performed to compute the scintillation light yield using\n"
           << "(L_max / E_max) as the photon yield per unit energy."
           << G4endl;
        G4String cmt = "\nScintillation yield may be unphysical!\n";
        G4Exception("ScintillationLUT::GetScintillationYieldByParticleType()",
                    "Scint03", JustWarning, ed, cmt);

        G4double LinearYield = Scint_Yield_Vector->GetMaxValue() / Scint_Yield_Vector->GetMaxEnergy();

        // Units: [# scintillation photons]
        ScintillationYield = LinearYield * StepEnergyDeposit;
    }

#ifdef DEBUG_SCINTILLATION_LUT

    // Increment track aggregators
    ScintTrackYield += ScintillationYield;
    ScintTrackEDep += StepEnergyDeposit;

    G4cout << "\n--- ScintillationLUT::GetScintillationYieldByParticleType() ---\n"
           << "--\n"
           << "--  Name         =  " << aTrack.GetParticleDefinition()->GetParticleName() << "\n"
           << "--  TrackID      =  " << aTrack.GetTrackID() << "\n"
           << "--  ParentID     =  " << aTrack.GetParentID() << "\n"
           << "--  Current KE   =  " << aTrack.GetKineticEnergy() / MeV << " MeV\n"
           << "--  Step EDep    =  " << aStep.GetTotalEnergyDeposit() / MeV << " MeV\n"
           << "--  Track EDep   =  " << ScintTrackEDep / MeV << " MeV\n"
           << "--  Vertex KE    =  " << aTrack.GetVertexKineticEnergy() / MeV << " MeV\n"
           << "--  Step yield   =  " << ScintillationYield << " photons\n"
           << "--  Track yield  =  " << ScintTrackYield << " photons\n"
           << G4endl;

    // The track has terminated within or has left the scintillator volume
    if ((aTrack.GetTrackStatus() == fStopButAlive) or
        (aStep.GetPostStepPoint()->GetStepStatus() == fGeomBoundary))
    {

      // Reset aggregators for the next track
      ScintTrackEDep = 0.;
      ScintTrackYield = 0.;
    }

#endif

    return ScintillationYield;
}

void ScintillationLUT::DumpPhysicsTable() const {
    if (fFastIntegralTable) {
        G4int PhysicsTableSize = fFastIntegralTable->entries();
        G4PhysicsOrderedFreeVector *v;

        for (G4int i = 0; i < PhysicsTableSize; i++) {
            v = (G4PhysicsOrderedFreeVector *) (*fFastIntegralTable)[i];
            v->DumpValues();
        }
    }

    if (fSlowIntegralTable) {
        G4int PhysicsTableSize = fSlowIntegralTable->entries();
        G4PhysicsOrderedFreeVector *v;

        for (G4int i = 0; i < PhysicsTableSize; i++) {
            v = (G4PhysicsOrderedFreeVector *) (*fSlowIntegralTable)[i];
            v->DumpValues();
        }
    }
}
