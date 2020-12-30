#include "DarkPhysics/DarkMatterPhysics.hh"

#include "DarkPhysics/DarkMatter.hh"
#include "DarkPhysics/DMParticleAPrime.hh"
#include "DarkPhysics/DMBremProcess.hh"
#include "DarkPhysics/DarkPhotons.hh"

#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Gamma.hh"
#include "G4MuonMinus.hh"
#include "G4MuonPlus.hh"
#include "G4StepLimiter.hh"
#include "G4BuilderType.hh"
#include "G4SystemOfUnits.hh"
#include "G4ProcessManager.hh"
#include "G4VPhysicsConstructor.hh"
#include "G4VProcess.hh"

#include <G4ParticleTable.hh>

#include <iostream>

// needed for GEANT4 10.3.0 and later
#ifndef aParticleIterator
#define aParticleIterator ((subInstanceManager.offset[g4vpcInstanceID])._aParticleIterator)
#endif

DarkMatterPhysics::DarkMatterPhysics()
        : G4VPhysicsConstructor("DarkMatterPhysics") {
    SetPhysicsType(bUnknown);
    //SetPhysicsType(bElectromagnetic);
    //G4double BiasSigmaFactor0 = 8.e10;//here is a estimation from calchep results
    G4double BiasSigmaFactor0 = 1;//here is a estimation from calchep results
    G4double Etresh = 0.5;//the sebsivity calculation for the invisible mode, need to be checked
    double dummy_mass = 0.0167;
    myDarkMatter = new DarkPhotons(dummy_mass, Etresh);
    BiasSigmaFactor = BiasSigmaFactor0 * 0.0001 * 0.0001 / (myDarkMatter->Getepsil() * myDarkMatter->Getepsil());

}

DarkMatterPhysics::~DarkMatterPhysics() {
    delete myDarkMatter;
}

void DarkMatterPhysics::ConstructParticle() {
    // This call to particle definition must be first or at least go before
    // Physics::ConstructProcess()

    DMParticleAPrime::Definition();

    std::cout << "Add A_prime" << std::endl;

}

void DarkMatterPhysics::ConstructProcess() {
    //here to define which process should be add to the physicslist
    G4ParticleDefinition *theDMParticlePtr = nullptr;
    G4ProcessManager *pmanager = nullptr;

    auto *electron = G4ParticleTable::fgParticleTable->FindParticle("e-");

    if (myDarkMatter->GetParentPDGID() == 11) {
        theDMParticlePtr = DMParticleAPrime::Definition();
        pmanager = electron->GetProcessManager();
    }

    //if(!theDMParticlePtr) {G4cout << "DarkMatterPhysics::ConstructProcess: did not manage to determine the DM particle type, exiting" << G4endl; exit(1);}

    myDarkMatter->SetMA(theDMParticlePtr->GetPDGMass() / GeV);
    myDarkMatter->PrepareTable();

    G4PhysicsListHelper *phLHelper = G4PhysicsListHelper::GetPhysicsListHelper();

    std::cout << pmanager->AddDiscreteProcess(new DMBremProcess(myDarkMatter, theDMParticlePtr, BiasSigmaFactor),
                                              G4ProcessVectorDoItIndex::idxPostStep) << std::endl;
    //phLHelper->RegisterProcess(new G4StepLimiter(), theDMParticlePtr);

    //phLHelper->DumpOrdingParameterTable();

    // Get the process list associated with the gamma.
    G4ProcessVector *vProcess = pmanager->GetProcessList();

    for (unsigned iProcess = 0; iProcess < vProcess->size(); ++iProcess) {
        G4String processName = (*vProcess)[iProcess]->GetProcessName();
        G4cout << "[Electron Phyiscs 2] ==> " << processName << ", with Type "
               << (*vProcess)[iProcess]->GetProcessType()
               << ", sub Type " << (*vProcess)[iProcess]->GetProcessSubType() << G4endl;

//        if (processName == "DMProcessDMBrem") {
//            pmanager->SetProcessOrderingToFirst((*vProcess)[iProcess],
//                                                G4ProcessVectorDoItIndex::idxPostStep);
//        }
    }


}
