#include "DP_simu/RootManager.hh"
#include "DP_simu/RootMessenger.hh"

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TGeoManager.h"

#include "TObjectTable.h"
#include "TROOT.h"

#include "G4TouchableHistory.hh"

#include <stdexcept>
#include <sstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Required by Singleton
RootManager *dRootMng = nullptr;

// Get Instance Class
RootManager *RootManager::CreateInstance() {
    if (dRootMng == nullptr)
        dRootMng = new RootManager();

    return dRootMng;
}

RootManager::RootManager()
        : rootFile(nullptr), tr(nullptr), if_clean(false) {

    fMessenger = new RootMessenger();
    outfile_name = dControl->outfile_Name;

    Evt = new DEvent();
    Evt->Initialization(nALL);
    initialize();

    fStart = dControl->Run_Number;
    fEvtNb = dControl->Total_Event_Number;

    OpticalHCALYield = dControl->Optical_HCAL_Yield;

#ifdef MEMCK
    if (dControl->Memory_Check) {
        Printf("============================================================");
        Printf("-- Remeber to include the following strings in ~/.rootrc");
        Printf("Root.MemStat:            1");
        Printf("Root.MemStat.size:       1");
        Printf("Root.MemStat.cnt:        1");
        Printf("Root.ObjectStat:         1");
        Printf("============================================================");
    }
#endif

}

/// \brief Clean Optical stuff.
void RootManager::initialize() { //event level init
    EventID = 0;
    for (double &i : Rndm) i = 0;

    Evt->Initialization(nVector);

    // Initialize fDigits in Digitizer
    if (dControl->if_optical) {
        for (auto dig: fDigitizers) dig.second->ClearDigits();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// RootManager::~RootManager() {
//     delete fMessenger;
//     delete rootFile;
// }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/// \brief Create rootFile.
void RootManager::book() { //run level init for all
    G4String fileName = outfile_name;
    rootFile = new TFile(fileName, "RECREATE");

    if (!rootFile) {
        G4cout << " RootManager::book :"
               << " problem creating the ROOT TFile "
               << G4endl;
        return;
    }

    rnd.SetSeed(fStart + fEvtNb);
    tr = new TTree(dControl->tree_Name, dControl->tree_Name);
    tr->SetAutoSave();
    if (if_clean) {
        G4cout << "Clean Mode..." << G4endl;
    } else {
        tr->Branch("RunNumber", &fStart, "RunNumber/I");
        tr->Branch("EventNumber", &fEvtN, "EventNumber/I");
        tr->Branch("Rndm", &Rndm, "Rndm[4]/D");
        tr->Branch("Weight", &weight, "Weight/D");
        tr->Branch("OpticalHCALYield", &OpticalHCALYield, "OpticalHCALYield/D");

        // truth
        if (dControl->save_MC)
            Evt->RegisterMCParticleCollection(dControl->RawMCCollection_Name);
        if (dControl->save_initial_particle_step)
            Evt->RegisterStepCollection(dControl->InitialParticleStepCollection_Name);
        if (dControl->save_mcp_helper)
            Evt->RegisterMCPHelperCollection(dControl->MCPHelperCollection_Name);
    }

    tr->Branch("DEvent", &Evt, 320000000, 0);

    G4cout << "===> ROOT file is opened in " << fileName << G4endl;
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......
/// \brief 
/// \param[in] cIn
void RootManager::bookCollection(const G4String &cIn) {  //run level initilize based on det name

    G4cout << "[Root Manager] ==> Booking tree for " << cIn << " ..." << G4endl;

    Evt->RegisterSimulatedHitCollection(cIn);

    if (dControl->Optical_UseLUT) {
        //nullptr trick
        //init ar first event, then not clear the nullptr.
        if (cIn == "ECAL") {
            [[maybe_unused]] auto v = Evt->RegisterOpticalCollection(cIn);
            auto Placeholder = dControl->ECAL_Center_Module_No;
            G4int No = Placeholder.x() * Placeholder.y() * Placeholder.z();

            fDigitizers.insert(std::pair<G4String, OpticalDigitizer *>(cIn, new OpticalDigitizer(cIn, No)));
            fDigitizers["ECAL"]->Print();
        } else if (cIn == "HCAL");
//        Evt->RegisterOpticalCollection(cIn);
        else
            G4cerr << "No need for Optical: " << cIn << G4endl;
    }
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......
/// \brief Save ROOT file of Simulation tree.
void RootManager::save() {
    if (rootFile) {
        rootFile->WriteTObject(tr, "", "Overwrite");
        rootFile->Close();
        G4cout << "[Root Manager] ==> Simulation Tree is saved \n" << G4endl;
    }
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......
/// \brief Fill Photon-Nuclear reaction Energy.
/// \param[in] E1 if E1 > PNEnergy_Target, replace it by E1
/// \param[in] E1 if E2 > PNEnergy_Ecal, replace it by E2
/// \param[in] Z - z position
void RootManager::FillPNE(G4double E1, G4double E2, G4double Z) {
    auto EnergyTarget = Evt->getPnEnergyTarget();
    auto EnergyECAL = Evt->getPnEnergyEcal();
    if (E1 > EnergyTarget) {
        Evt->setPnEnergyTarget(E1);
        Evt->setPnZTarget(Z);
    } else {

    }
    if (E2 > EnergyECAL) {
        Evt->setPnEnergyEcal(E2);
        Evt->setPnZEcal(Z);
    } else {

    }
//    Evt->setPnEnergyTarget(E1 > EnergyTarget ? E1 : EnergyTarget);
//    Evt->setPnEnergyEcal(E2 > EnergyECAL ? E2 : EnergyECAL);
}

/// \brief Fill Electron-Nuclear reaction Energy.
/// \param[in] E1
/// \param[in] E2
/// \param[in] Z
void RootManager::FillENE(G4double E1, G4double E2, G4double Z) {
    auto EnergyTarget = Evt->getEnEnergyTarget();
    auto EnergyECAL = Evt->getEnEnergyEcal();
    if (E1 > EnergyTarget) {
        Evt->setEnEnergyTarget(E1);
        Evt->setEnZTarget(Z);
    } else {

    }
    if (E2 > EnergyECAL) {
        Evt->setEnEnergyEcal(E2);
        Evt->setEnZEcal(Z);
    } else {

    }
    //    Evt->setPnEnergyTarget(E1 > EnergyTarget ? E1 : EnergyTarget);
    //    Evt->setPnEnergyEcal(E2 > EnergyECAL ? E2 : EnergyECAL);
}


//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......
/// \brief
/// \param[in] mc
/// \param[in] ParentID
void RootManager::FillMC(McParticle *fMC, int ParentID) {
    if (if_clean) return;

    auto mc = new McParticle(*fMC);

    auto mcps = Evt->getMcParticleCollection().at(dControl->RawMCCollection_Name);
    mc->setParents(McParticle::SearchID(mcps, ParentID));

    auto tmp1 = G4String(mc->getCreateProcess());
    const char *tmp2;
    if (tmp1.contains("biasWrapper"))
        tmp2 = tmp1(tmp1.index("(") + 1, tmp1.index(")") - tmp1.index("(") - 1).data();
    else
        tmp2 = tmp1.data();
    mc->setCreateProcess(std::string(tmp2));

    mcps->emplace_back(mc);
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......
/// \brief
/// \param[in] McPHelper
/// \param[in] McParticle ID
void RootManager::FillMCPHelper(McPHelper* fMCPH, int mcpId) {
    if (if_clean) return;

    auto mcph = new McPHelper(*fMCPH);

    auto mcphVec = Evt->getMcPHelperCollection().at(dControl->MCPHelperCollection_Name);
    auto mcps = Evt->getMcParticleCollection().at(dControl->RawMCCollection_Name);
    mcph->setMcParticle(McParticle::SearchID(mcps, mcpId));

    mcphVec->emplace_back(mcph);
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......
/// \brief
/// \param[in] eventID
/// \param[in] RnD
void RootManager::FillSim(Int_t eventID, const Double_t *Rnd) {

    EventID = eventID + fEvtNb * fStart;
    fEvtN = eventID;
    for (int i = 0; i < 4; i++) {
        Rndm[i] = *(Rnd + i);
    }

    Evt->setEventId(EventID);
    Evt->setRndm(Rnd);
    Evt->setWeight(weight);

    tr->Fill();

#ifdef MEMCK
    if (dControl->Memory_Check) DEvent::PrintObjectStatistics("Waiting for Filling the tree");
#endif

    initialize();
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

void RootManager::FillSimHit(const G4String &cIn, SimulatedHit *hit) {

    auto SimHits = Evt->getSimulatedHitCollection();
    auto Hits = new SimulatedHit(*hit);

    SimHits.at(cIn)->emplace_back(Hits);
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

void RootManager::FillEleak(const G4Step *in, const G4String &type) {
    auto deltaE = in->GetTotalEnergyDeposit();

    if (type == "ECAL") Evt->setEleakEcal(Evt->getEleakEcal() + deltaE);
//    else if (type.contains("_PVW")) {
//        auto cin = type.remove(type.index("_PVW"));
//
//        auto SimHits = Evt->getSimulatedHitCollection();
//        auto itr = SimHits.at(cin)->end() - 1;
//        (*itr)->setELeakWrapper( (*itr)->getELeakWrapper() + deltaE);
//    }
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......
// Optical Part Start ///////////////////////////////////////////////////////////

bool RootManager::SetOpticalTimeZero(G4double /*T0*/, const G4String& /*cIn*/) { //global T0 for on unit across many particles
    //dummy, not used here. 
    //Time zero is used for pileup simulation
    return true;
}

bool RootManager::FillOpticalLUTs(std::vector<OpticalHit *> *hits, G4int GenNo, const G4String &cIn,
                                  int copyNum) //or we can fill as a whole, like a vector, how about memory???
{
    //check collection
    auto DiGiMap = Evt->getOpticalCollection();
    if (DiGiMap.find(cIn) == DiGiMap.end()) {
        G4cerr << "Not found optical collection cIn=" << cIn << G4endl;
        return false;
    }
    //check digitizer
    if (fDigitizers.find(cIn) == fDigitizers.end()) {
        G4cerr << "Not found odigitizer cIn=" << cIn << G4endl;
        return false;
    }

    auto DiGi = fDigitizers[cIn]->GetDiGi(copyNum);
    if (!DiGi) {
        return false;
    }

    for (auto h:*hits)
        DiGi->AddTimeSeq(h->GetArrivalT());
    DiGi->SetVoltageToADC(dControl->Optical_voltageToADC());
    DiGi->SetYieldFactor(dControl->Optical_YieldFactor);
    DiGi->SetRangeMin(dControl->Optical_range_min);
    DiGi->SetRangeMax(dControl->Optical_range_max);
    DiGi->SetPedestal(dControl->Optical_pedestalLevel);
    DiGi->AddOpticalGen(GenNo);
    fDigitizers[cIn]->AddHits(hits, copyNum);
    return true;
}

bool RootManager::FinalizeOptical() {
    for (const auto &imap : Evt->getOpticalCollection()) {
        auto DiGis = imap.second; //vec
        auto cIn = imap.first;
        auto digitizer = fDigitizers[cIn];

        digitizer->DigitizeAll(); //will scan the DiGis inside, and return the DiGis
        for (auto d: digitizer->GetDiGis()) { //fill really DEvent object
            if (d && d->GetDetID() >= 0) {
                auto temp_in = new DigiForm((*d));
                DiGis->push_back(temp_in);
            }
        }

        digitizer->ResetPixelHistory();
        digitizer->ClearHits();
    }
    return true;
}

// Optical Part End    ///////////////////////////////////////////////////////////
//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......
void RootManager::FillParticleStep(const G4Step *aStep) {
    G4StepPoint *prev = aStep->GetPreStepPoint();
    G4StepPoint *post = aStep->GetPostStepPoint();

    auto Steps = Evt->getStepCollection().at(dControl->InitialParticleStepCollection_Name);
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

void RootManager::FillGeometry(const G4String &filename) {

    auto geoM = new TGeoManager();
    TGeoManager::Import(filename);

    rootFile->cd();
    geoM->Write("DetGeoManager");

    std::remove(filename);

}

