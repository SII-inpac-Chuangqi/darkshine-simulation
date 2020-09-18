/**
 * @file GammaPhysics.cxx
 * @brief Class used to enhanced the gamma physics list.
 * @author Jeremy McCormick, SLAC National Accelerator Laboratory
 * @author Omar Moreno, SLAC National Accelerator Laboratory
 */

#include "DP_simu/GammaPhysics.h"


GammaPhysics::GammaPhysics(const G4String& name) :
        G4VPhysicsConstructor(name) {
}

GammaPhysics::~GammaPhysics() {
}

// needed for GEANT4 10.3.0 and later
#ifndef aParticleIterator
#define aParticleIterator ((subInstanceManager.offset[g4vpcInstanceID])._aParticleIterator)
#endif

void GammaPhysics::ConstructProcess() {

    aParticleIterator->reset();

    // Loop through all of the particles and find the "gamma".
    while ((*aParticleIterator)()) {
        
        G4ParticleDefinition* particle = aParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        const G4String& particleName = particle->GetParticleName();

        pmanager->SetVerboseLevel(2);
        
        if (particleName == "gamma") {

            // Add the gamma -> mumu to the physics list.
            pmanager->AddDiscreteProcess(&gammaConvProcess);

            // Get the process list associated with the gamma.
            G4ProcessVector* vProcess = pmanager->GetProcessList(); 
            
            // Search the list for the process "photoNuclear".  When found, 
            // change the calling order so photonNuclear is called before 
            // EM processes. The biasing operator needs the photonNuclear
            // process to be called first because the cross-section is
            // needed to bias down other process.
            for (int iProcess = 0; iProcess < vProcess->size(); ++iProcess) { 
                G4String processName = (*vProcess)[iProcess]->GetProcessName();
                G4cout<<"[Gamma Phyiscs] ==> "<<processName<<", with Type "<<(*vProcess)[iProcess]->GetProcessType()<<", sub Type "<<(*vProcess)[iProcess]->GetProcessSubType()<<G4endl;
                //if (processName == "photonNuclear") { 
                //    pmanager->SetProcessOrderingToFirst((*vProcess)[iProcess], 
                //            //G4ProcessVectorDoItIndex::idxAlongStep);
                //            G4ProcessVectorDoItIndex::idxAll); 
                //}
            }

        }

        if (particleName == "e-") {
            // Get the process list associated with the gamma.
            G4ProcessVector* vProcess = pmanager->GetProcessList(); 

            for (int iProcess = 0; iProcess < vProcess->size(); ++iProcess) { 
                G4String processName = (*vProcess)[iProcess]->GetProcessName();
                G4cout<<"[Electron Phyiscs] ==> "<<processName<<", with Type "<<(*vProcess)[iProcess]->GetProcessType()<<", sub Type "<<(*vProcess)[iProcess]->GetProcessSubType()<<G4endl;
            }

        }
    }
}
