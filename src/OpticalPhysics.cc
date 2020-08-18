#include "G4OpBoundaryProcess.hh"
#include "G4BuilderType.hh"
#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpticalPhoton.hh"
#include "G4PhysicsListHelper.hh"
#include "G4ProcessManager.hh"

#include "OpticalPhysics.h"


OpticalPhysics::OpticalPhysics(RootManager* rootMng): G4VPhysicsConstructor("OpticalPhysics")
{
    //SetPhysicsType(G4BuilderType::bElectromagnetic);
    frootMng = rootMng;
}

OpticalPhysics::~OpticalPhysics()
{}

void OpticalPhysics::ConstructParticle()
{
    G4OpticalPhoton::OpticalPhotonDefinition();
}

void OpticalPhysics::ConstructProcess()
{
        
    G4cout<<"[Optical Phyiscs] ==> Initiated..."<<G4endl;

    if ( frootMng->GetOptical() ) {
        G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
        //G4Cerenkov* cerenkov = new G4Cerenkov();
        G4Scintillation* scintillation = new G4Scintillation();

        auto aParticleIterator = GetParticleIterator();
        aParticleIterator->reset();

        while ((*aParticleIterator)()) {
            G4ParticleDefinition* particle = aParticleIterator->value();

            if (scintillation->IsApplicable(*particle)) {
                ph->RegisterProcess(scintillation, particle);
            }
        }
        G4ProcessManager* pm = G4OpticalPhoton::OpticalPhoton()->GetProcessManager();
        pm->AddDiscreteProcess(new G4OpAbsorption());
        auto opBo = new G4OpBoundaryProcess();
        
        //opBo->SetVerboseLevel(2);
        pm->AddDiscreteProcess(opBo);

        G4cout<<"[Optical Phyiscs] ==> Scintillation optical photon simulated. "<<G4endl;
    } else {

        G4cout<<"[Optical Phyiscs] ==> No optical photon simulation. "<<G4endl;

    }

}
