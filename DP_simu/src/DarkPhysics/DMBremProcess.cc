#include "DarkPhysics//DMBremProcess.hh"
#include "DarkPhysics/DarkMatter.hh"
#include "G4ParticleChange.hh"
#include "DarkPhysics/DMParticleAPrime.hh"
#include "G4ProcessType.hh"
#include "G4EmProcessSubType.hh"
#include "G4SystemOfUnits.hh"

DMBremProcess::DMBremProcess(DarkPhotons *DarkMatterPointerIn, G4ParticleDefinition *theDMParticlePtrIn,
                             G4double BiasSigmaFactorIn)
        : G4VDiscreteProcess("DMProcessDMBrem", fElectromagnetic),  // fElectromagnetic
          myDarkMatter(DarkMatterPointerIn),
          theDMParticlePtr(theDMParticlePtrIn),
          BiasSigmaFactor(BiasSigmaFactorIn) {
//here I don't think what to define
    SetProcessSubType(500);
    theDMParticlePtr->SetPDGLifeTime(CLHEP::hbar_Planck / (myDarkMatter->Width() * GeV));
}

G4bool DMBremProcess::IsApplicable(const G4ParticleDefinition &pDef) {
    return ("e-" == pDef.GetParticleName());
    //return fModelPtr->IsApplicableToParticle(pDef);
}

//Here is not need for invisible decay, will be added in future 
G4double DMBremProcess::GetMeanFreePath(const G4Track &aTrack, G4double, /*previousStepSize*/
                                        G4ForceCondition * /*condition*/ ) {

    G4double DensityMat = aTrack.GetMaterial()->GetDensity() / (g / cm3);
    G4double ekin = aTrack.GetKineticEnergy() / GeV;

    //std::cout << "[Dark Process] E_kin: " << ekin << ", DensityMat: " << DensityMat << std::endl;

    //Define if we can emission of Dark Matter/Dark Photon
    if (myDarkMatter->EmissionAllowed(ekin, DensityMat)) {
        G4double XMeanFreePath = myDarkMatter->GetMeanFreePathFactor() / myDarkMatter->GetSigmaTot(ekin);
        XMeanFreePath /= BiasSigmaFactor;
        std::cout << "DMMeanFreePath = " << myDarkMatter->GetMeanFreePathFactor() << std::endl;
        std::cout << "SigmaTotal = " << myDarkMatter->GetSigmaTot(ekin) << " [pb]" << std::endl;
        std::cout << "XMeanFreePath = " << XMeanFreePath << std::endl;
        return XMeanFreePath;
    }
    //if not emit, nothing
    return DBL_MAX;
}


G4VParticleChange *DMBremProcess::PostStepDoIt(const G4Track &aTrack, const G4Step &aStep) {
//    std::cout << "Dark Process!!" << std::endl;

    const G4double incidentE = aTrack.GetKineticEnergy();
    //const G4double DMMass = theDMParticleAPrimePtr->GetPDGMass();
    const G4ThreeVector &incidentDir = aTrack.GetMomentumDirection();

    G4double XAcc, angles[2];
    XAcc = myDarkMatter->SimulateEmissionWithAngle(incidentE / GeV, angles);

    // Check if it failed? In this case XAcc = 0

    if (XAcc > 0.00) myDarkMatter->EmissionSimulated();

    const G4double recoilE = incidentE * (1. - XAcc), recoilTheta = 0.; // this is not simulated
    G4double DMTheta = angles[0], DMPhi = angles[1];
    G4double DME = incidentE * XAcc;

    // Initialize DM direction vector:
    G4ThreeVector DMDirection(0., 0., .1);
    {
        DMDirection.setMag(1.);
        DMDirection.setTheta(DMTheta);
        DMDirection.setPhi(DMPhi);
        DMDirection.rotateUz(incidentDir);
    }
    // Initialize new projectile particle direction vector:
    G4ThreeVector projDirection(0., 0., 1.);
    {
        projDirection.setMag(1.);
        projDirection.setTheta(recoilTheta);
        projDirection.setPhi(-DMPhi);
        projDirection.rotateUz(incidentDir);
    }

    auto *movingDM = new G4DynamicParticle(theDMParticlePtr, DMDirection, DME);
    aParticleChange.Initialize(aTrack);

    // Set DM:
    aParticleChange.SetNumberOfSecondaries(1);
    aParticleChange.AddSecondary(movingDM);
    // Set projectile changes:
    aParticleChange.ProposeEnergy(recoilE);
    aParticleChange.ProposeMomentumDirection(projDirection);

    std::cout << "DM PDG ID = " << theDMParticlePtr->GetPDGEncoding()
              << " emitted by " << aTrack.GetDefinition()->GetParticleName()
              << " with energy = " << incidentE / GeV << " [GeV], DM energy = " << DME / GeV <<" [GeV]"<< std::endl;


    return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
}
