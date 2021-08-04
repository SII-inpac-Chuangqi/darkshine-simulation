#include "DarkPhysics//DMBremProcess.hh"
#include "DarkPhysics/DarkMatter.hh"
#include "G4ParticleChange.hh"
#include "DarkPhysics/DMParticleAPrime.hh"
#include "G4ProcessType.hh"
#include "G4EmProcessSubType.hh"
#include "G4SystemOfUnits.hh"
#include "Control/Control.h"
#include <string>


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
//        std::cout << "DMMeanFreePath = " << myDarkMatter->GetMeanFreePathFactor() << std::endl;
//        std::cout << "SigmaTotal = " << myDarkMatter->GetSigmaTot(ekin) << " [pb]" << std::endl;
//        std::cout << "XMeanFreePath = " << XMeanFreePath << std::endl;
        return XMeanFreePath;
    }
    //if not emit, nothing
    return DBL_MAX;
}

////////////////////////////////////////////////////////
void DMBremProcess::LUTRandom() {
    TString file_name = dControl->signal_lookup_table;
    TString graph_name = std::to_string((int) dControl->signal_mass) + "MEV";
    std::cout << graph_name << std::endl;
    if (!f || !Lut_hist) {
        f = new TFile(file_name, "READ");
        Lut_hist = (TH2F *) f->Get(graph_name);
        //std::cout<<"read done"<<std::endl;
    }
    Lut_hist->GetRandom2(energy, recoil_E);
    //output energu and recoil_E
    var_LUT[0] = energy;
    var_LUT[1] = recoil_E;
    //delete f;
    //f= NULL;
}
///////////////////////////////////////////////////////

G4VParticleChange *DMBremProcess::PostStepDoIt(const G4Track &aTrack, const G4Step &aStep) {
//    std::cout << "Dark Process!!" << std::endl;

    const G4double incidentE = aTrack.GetKineticEnergy();
    const G4double DMMass = theDMParticlePtr->GetPDGMass();
    const G4ThreeVector &incidentDir = aTrack.GetMomentumDirection();

    G4double XAcc, angles[2];
    G4double e_rec_angles[2];

    DM_Mom dark_photon;
    DM_Mom electron;

    //if (XAcc > 0.00) myDarkMatter->EmissionSimulated();
    // Add look-up table --Xiang
    if (dControl->signal_use_LUT) {
        LUTRandom();
        //double temp_var[2] ={0,0};
        electron.rec_theta = var_LUT[1];
        XAcc = 1 - var_LUT[0] / 8;
        std::cout << "This time XAcc : " << XAcc << std::endl;

        electron.rec_phi = G4UniformRand() * 2. * M_PI;
        dark_photon.rec_phi = -electron.rec_phi;
        dark_photon.rec_theta = 0;
    } else {
        // Angle is Dark Photon
        XAcc = myDarkMatter->SimulateEmissionWithAngle(incidentE / GeV, dark_photon);
        // electron.rec_theta;
        electron.rec_phi = -dark_photon.rec_phi;
    }
    electron.E = incidentE * (1. - XAcc);
    dark_photon.E = incidentE * XAcc;


    // Initialize DM direction vector:
    G4ThreeVector DMDirection(0., 0., .1);
    {
        DMDirection.setMag(1.);
        DMDirection.setTheta(dark_photon.rec_theta);
        DMDirection.setPhi(dark_photon.rec_phi);
        DMDirection.rotateUz(incidentDir);
    }
    // Initialize new projectile particle direction vector:
    G4ThreeVector projDirection(0., 0., 1.);
    {
        projDirection.setMag(1.);
        projDirection.setTheta(electron.rec_theta);
        projDirection.setPhi(electron.rec_phi);
        projDirection.rotateUz(incidentDir);
    }

    auto *movingDM = new G4DynamicParticle(theDMParticlePtr, DMDirection, dark_photon.E - DMMass);
    aParticleChange.Initialize(aTrack);

    // Set DM:
    aParticleChange.SetNumberOfSecondaries(1);
    aParticleChange.AddSecondary(movingDM);
    // Set projectile changes:
    aParticleChange.ProposeEnergy(electron.E);
    aParticleChange.ProposeMomentumDirection(projDirection);

//    std::cout << "DM PDG ID = " << theDMParticlePtr->GetPDGEncoding()
//              << " emitted by " << aTrack.GetDefinition()->GetParticleName()
//              << " with energy = " << incidentE / GeV << " [GeV], DM energy = " << DME / GeV <<" [GeV]"<< std::endl;


    return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
}
