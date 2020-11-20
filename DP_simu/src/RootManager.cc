#include "DP_simu/RootManager.hh"
#include "DP_simu/RootMessenger.hh"

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TInterpreter.h"
#include "TGeoManager.h"

#include "G4TouchableHistory.hh"

#include <stdexcept>
#include <sstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RootManager::RootManager()
        : rootFile(nullptr), tr(nullptr), fStart(0), fEvtNb(100000), if_clean(false) {

    fMessenger = new RootMessenger(this);
    outfilename = "dp_out.root";

    Evt = new DEvent();
    Evt->Initialization(nALL);
    initialize();
    if_Optical = false;
    if_record_ip = true;
}

void RootManager::initialize() {
    // Initialization
    EventID = 0;
    for (double &i : Rndm) i = 0;

    if (if_Optical) {
        for (itr_i = Optical_No.begin(); itr_i != Optical_No.end(); itr_i++) itr_i->second = 0;
        for (itrvec_double = Optical_Time.begin(); itrvec_double != Optical_Time.end(); itrvec_double++)
            itrvec_double->second->clear();

        for (itrvec_double = Optical_E.begin(); itrvec_double != Optical_E.end(); itrvec_double++)
            itrvec_double->second->clear();

        for (itrvec_int = Optical_DetID.begin(); itrvec_int != Optical_DetID.end(); itrvec_int++)
            itrvec_int->second->clear();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RootManager::~RootManager() {
    delete fMessenger;
    delete rootFile;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RootManager::book() {
    G4String fileName = outfilename;
    rootFile = new TFile(fileName, "RECREATE");

    if (!rootFile) {
        G4cout << " RootManager::book :"
               << " problem creating the ROOT TFile "
               << G4endl;
        return;
    }

    rnd.SetSeed(fStart + fEvtNb);
    tr = new TTree("Dark_Photon", "Dark_Photon");
    if (if_clean) {
        G4cout << "Clean Mode..." << G4endl;
    } else {
        tr->Branch("RunNumber", &fStart, "RunNumber/I");
        tr->Branch("EventNumber", &fEvtN, "EventNumber/I");
        tr->Branch("Rndm", &Rndm, "Rndm[4]/D");

        // truth
        Evt->RegisterMCParticleCollection("RawMCParticle");
        if (if_record_ip)
            Evt->RegisterStepCollection("Initial_Particle_Step");
    }

    tr->Branch("DEvent", &Evt, 32000000, 0);

    G4cout << "===> ROOT file is opened in " << fileName << G4endl;
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

void RootManager::bookCollection(const G4String &cIn) {

    G4cout << "[Root Manager] ==> Booking tree for " << cIn << " ..." << G4endl;

    if (if_Optical) {
        Optical_No.insert(std::pair<G4String, int>(cIn, 0));
        Optical_Time.insert(std::pair<G4String, std::vector<double> *>(cIn, new std::vector<double>));
        Optical_E.insert(std::pair<G4String, std::vector<double> *>(cIn, new std::vector<double>));
        Optical_DetID.insert(std::pair<G4String, std::vector<int> *>(cIn, new std::vector<int>));
        //Optical_DetID_x.insert(std::pair<G4String, int *>(cIn, new int[MaxOptPhoton]));
        //Optical_DetID_y.insert(std::pair<G4String, int *>(cIn, new int[MaxOptPhoton]));
        //Optical_DetID_z.insert(std::pair<G4String, int *>(cIn, new int[MaxOptPhoton]));
    }

    if (if_Optical) {
        tr->Branch((cIn + "_Optical_No").data(), &Optical_No[cIn], (cIn + "_Optical_No/I").data());
        tr->Branch((cIn + "_Optical_Time").data(), Optical_Time[cIn]);
        tr->Branch((cIn + "_Optical_E").data(), Optical_E[cIn]);
        tr->Branch((cIn + "_Optical_DetID").data(), Optical_DetID[cIn]);
        //tr->Branch((cIn + "_Optical_DetID_x").data(), Optical_DetID_x[cIn],
        //           (cIn + "_Optical_DetID_x[" + cIn + "_Optical_No]/I").data());
        //tr->Branch((cIn + "_Optical_DetID_y").data(), Optical_DetID_y[cIn],
        //           (cIn + "_Optical_DetID_y[" + cIn + "_Optical_No]/I").data());
        //tr->Branch((cIn + "_Optical_DetID_z").data(), Optical_DetID_z[cIn],
        //           (cIn + "_Optical_DetID_z[" + cIn + "_Optical_No]/I").data());
    }

    Evt->RegisterSimulatedHitCollection(cIn);
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

void RootManager::save() {
    if (rootFile) {
        rootFile->WriteTObject(tr, "", "Overwrite");
        rootFile->Close();
        G4cout << "[Root Manager] ==> Simulation Tree is saved \n" << G4endl;
    }
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

void RootManager::FillPNE(G4double E1, G4double E2) {
    auto EnergyTarget = Evt->getPnEnergyTarget();
    Evt->setPnEnergyTarget(E1 > EnergyTarget ? E1 : EnergyTarget);
    auto EnergyECAL = Evt->getPnEnergyEcal();
    Evt->setPnEnergyEcal(E2 > EnergyECAL ? E2 : EnergyECAL);
}


//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

void RootManager::FillMC(McParticle *mc, int ParentID) {
    if (if_clean) return;

    auto mcps = Evt->getMcParticleCollection_Old().at("RawMCParticle");
    mc->setParents(mc->SearchID(mcps, ParentID));

    auto tmp1 = G4String(mc->getCreateProcess());
    const char *tmp2;
    if (tmp1.contains("biasWrapper"))
        tmp2 = tmp1(tmp1.index("(") + 1, tmp1.index(")") - tmp1.index("(") - 1).data();
    else
        tmp2 = tmp1.data();
    mc->setCreateProcess(std::string(tmp2));

    mcps->push_back(mc);
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

void RootManager::FillSim(Int_t eventID, const Double_t *Rnd) {

    EventID = eventID + fEvtNb * fStart;
    fEvtN = eventID;
    for (int i = 0; i < 4; i++) {
        Rndm[i] = *(Rnd + i);
    }

    Evt->setEventId(EventID);
    Evt->setRndm(Rnd);

    tr->Fill();

    Evt->Initialization(nVector);

    initialize();
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

void RootManager::FillSimHit(const G4String &cIn, SimulatedHit *hit) {

    auto SimHits = Evt->getSimulatedHitCollection_Old();
    auto Hits = new SimulatedHit(*hit);

    SimHits.at(cIn)->emplace_back(Hits);
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

void RootManager::FillEleak(const G4Step *in, G4String type) {
    auto deltaE = in->GetTotalEnergyDeposit();

    if (type == "ECAL") Evt->setEleakEcal(Evt->getEleakEcal() + deltaE);
    else if (type.contains("_PVW")) {
        auto cin = type.remove(type.index("_PVW"));

        //auto SimHits = Evt->getSimulatedHitCollection_Old();
        //auto itr = SimHits.at(cin)->end() - 1;
        //(*itr)->setELeakWrapper( (*itr)->getELeakWrapper() + deltaE);
    }
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

bool RootManager::FillOptical(const G4Step *in, G4String type) {
    bool flag = false;
    if (type.contains("_APDWorld_PV")) {
        auto cin = type.remove(type.index("_APDWorld_PV"));
        auto *touchable = (G4TouchableHistory *) (in->GetPreStepPoint()->GetTouchable());
        G4int reNumber = touchable->GetReplicaNumber();
        Optical_DetID[cin]->emplace_back(reNumber);

        Optical_Time[cin]->emplace_back(in->GetPostStepPoint()->GetGlobalTime()); // ns
        Optical_E[cin]->emplace_back(in->GetPostStepPoint()->GetTotalEnergy() / eV); // optical photon in eV unit

        Optical_No[cin]++;

        flag = true;
    }

    return flag;
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......
void RootManager::FillParticleStep(const G4Step *aStep) {
    G4StepPoint *prev = aStep->GetPreStepPoint();
    G4StepPoint *post = aStep->GetPostStepPoint();

    auto Steps = Evt->getStepCollection_Old().at("Initial_Particle_Step");
    auto step = new DStep();
    step->setId(static_cast<int>(Steps->size()));
    if (Steps->empty()) {
        step->setX(prev->GetPosition()[0]);
        step->setY(prev->GetPosition()[1]);
        step->setZ(prev->GetPosition()[2]);
        step->setPx(prev->GetMomentum()[0]);
        step->setPy(prev->GetMomentum()[1]);
        step->setPz(prev->GetMomentum()[2]);
        step->setE(prev->GetTotalEnergy());
        step->setPVName(prev->GetPhysicalVolume()->GetName().data());
        step->setProcessName("Initial Step");
    }
    step->setX(post->GetPosition()[0]);
    step->setY(post->GetPosition()[1]);
    step->setZ(post->GetPosition()[2]);
    step->setPx(post->GetMomentum()[0]);
    step->setPy(post->GetMomentum()[1]);
    step->setPz(post->GetMomentum()[2]);
    step->setE(post->GetTotalEnergy());


    if (post->GetPhysicalVolume() == nullptr) {
        step->setPVName("OutofWorld");
        step->setProcessName("Transportation");
    } else {
        step->setPVName(post->GetPhysicalVolume()->GetName().data());
        auto tmp2 = post->GetProcessDefinedStep()->GetProcessName();
        const char *tmp3;
        if (tmp2.contains("biasWrapper"))
            tmp3 = tmp2(tmp2.index("(") + 1, tmp2.index(")") - tmp2.index("(") - 1).data();
        else
            tmp3 = tmp2.data();
        step->setProcessName(std::string(tmp3));
    }

    Steps->emplace_back(step);
}

void RootManager::FillGeometry(const G4String& filename) {

    auto geoM = new TGeoManager();
    TGeoManager::Import(filename);

    rootFile->cd();
    geoM->Write("DetGeoManager");

    std::remove(filename);

}
