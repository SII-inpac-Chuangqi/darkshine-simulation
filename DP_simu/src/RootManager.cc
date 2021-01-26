#include "DP_simu/RootManager.hh"
#include "DP_simu/RootMessenger.hh"

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TGeoManager.h"

#include "G4TouchableHistory.hh"

#include <stdexcept>
#include <sstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RootManager::RootManager()
        : rootFile(nullptr), tr(nullptr), if_clean(false) {

    fMessenger = new RootMessenger(this);
    outfile_name = dControl->outfile_Name;

    Evt = new DEvent();
    Evt->Initialization(nALL);
    initialize();
    // if_Optical = dControl->if_optical;

    fStart = dControl->Run_Number;
    fEvtNb = dControl->Total_Event_Number;
    //move to bookCollection
//    if(dControl->Optical_UseLUT){
//        fDigitizers.insert(std::pair<G4String, OpticalDigitizer>("ECAL",OpticalDigitizer()));
//        fDigitizers["ECAL"].Print();
//    }
}

/// \brief Clean Optical stuff.
void RootManager::initialize() { //event level init
    EventID = 0;
    for (double &i : Rndm) i = 0;

    Evt->Initialization(nVector);

    // if (if_Optical) {
    //     for (auto o : Optical_No) o.second = 0;
    //     for (auto o : Optical_Time) o.second->clear();
    //     for (auto o : Optical_E) o.second->clear();
    //     for (auto o : Optical_DetID) o.second->clear();
    // }

    // initilize optical
//    if (dControl->Optical_UseLUT)
//    {
//        G4ThreeVector Placeholder(0, 0, 0);
//        if (cIn == "ECAL")
//            Placeholder = dControl->ECAL_Center_Module_No;
//        else if (cIn == "HCAL")
//            Placeholder = dControl->ECAL_Center_Module_No;
//        else
//            G4cerr << "No need for Optical: " << cIn << G4endl;
//        G4int No = Placeholder.x() * Placeholder.y() * Placeholder.z();
//        if (No != 0)
//        {
//            Evt->RegisterOpticalCollection(cIn);
//            auto DiGis = *(Evt->getOpticalCollection_Old().at(cIn)); //why not map of vec..
//            //event level clear is trigged by FillSim-->Initialization(nVector)
//            G4cout << "DiGis Initilized. " << cIn << G4endl;
//            for(int i=0;i<No;i++)
////                DiGis.emplace_back(new DigiForm());
//                DiGis.emplace_back(nullptr);
////            DiGis.insert(DiGis.begin(), No, nullptr); //nullptr trick
//            G4cout << "Initized Unit Number: " << DiGis.size() << G4endl;
//            ;
//        }
//    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RootManager::~RootManager() {
    delete fMessenger;
    delete rootFile;
}

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

        // truth
        if (dControl->save_MC)
            Evt->RegisterMCParticleCollection(dControl->RawMCCollection_Name);
        if (dControl->save_initial_particle_step)
            Evt->RegisterStepCollection(dControl->InitialParticleStepCollection_Name);
    }

    tr->Branch("DEvent", &Evt, 320000000, 0);

    G4cout << "===> ROOT file is opened in " << fileName << G4endl;
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......
/// \brief 
/// \param[in] cIn
void RootManager::bookCollection(const G4String &cIn) {  //run level initilize based on det name

    G4cout << "[Root Manager] ==> Booking tree for " << cIn << " ..." << G4endl;

    // if (if_Optical) {
    //     Optical_No.insert(std::pair<G4String, int>(cIn, 0));
    //     Optical_Time.insert(std::pair<G4String, std::vector<double> *>(cIn, new std::vector<double>));
    //     Optical_E.insert(std::pair<G4String, std::vector<double> *>(cIn, new std::vector<double>));
    //     Optical_DetID.insert(std::pair<G4String, std::vector<int> *>(cIn, new std::vector<int>));
    //     //Optical_DetID_x.insert(std::pair<G4String, int *>(cIn, new int[MaxOptPhoton]));
    //     //Optical_DetID_y.insert(std::pair<G4String, int *>(cIn, new int[MaxOptPhoton]));
    //     //Optical_DetID_z.insert(std::pair<G4String, int *>(cIn, new int[MaxOptPhoton]));
    // }

    // if (if_Optical) {
    //     tr->Branch((cIn + "_Optical_No").data(), &Optical_No[cIn], (cIn + "_Optical_No/I").data());
    //     tr->Branch((cIn + "_Optical_Time").data(), Optical_Time[cIn]);
    //     tr->Branch((cIn + "_Optical_E").data(), Optical_E[cIn]);
    //     tr->Branch((cIn + "_Optical_DetID").data(), Optical_DetID[cIn]);
    //     //tr->Branch((cIn + "_Optical_DetID_x").data(), Optical_DetID_x[cIn],
    //     //           (cIn + "_Optical_DetID_x[" + cIn + "_Optical_No]/I").data());
    //     //tr->Branch((cIn + "_Optical_DetID_y").data(), Optical_DetID_y[cIn],
    //     //           (cIn + "_Optical_DetID_y[" + cIn + "_Optical_No]/I").data());
    //     //tr->Branch((cIn + "_Optical_DetID_z").data(), Optical_DetID_z[cIn],
    //     //           (cIn + "_Optical_DetID_z[" + cIn + "_Optical_No]/I").data());
    // }

    Evt->RegisterSimulatedHitCollection(cIn);
    //    if(dControl->Optical_UseLUT){
//        fDigitizers.insert(std::pair<G4String, OpticalDigitizer>("ECAL",OpticalDigitizer()));
//        fDigitizers["ECAL"].Print();
//    }
    if (dControl->Optical_UseLUT) {
        //nullptr trick
        //init ar first event, then not clear the nullptr.
        if (cIn == "ECAL"){
            auto v = Evt->RegisterOpticalCollection(cIn);
            auto Placeholder = dControl->ECAL_Center_Module_No;
            G4int No = Placeholder.x() * Placeholder.y() * Placeholder.z();
//            v->insert(v->begin(), No, nullptr);
//            G4cout << "Initized Unit Number: " << v->size() << G4endl;
            fDigitizers.insert(std::pair<G4String, OpticalDigitizer>(cIn,OpticalDigitizer(cIn, No)));
            fDigitizers["ECAL"].Print();
        }
        else if (cIn == "HCAL");
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
void RootManager::FillPNE(G4double E1, G4double E2) {
    auto EnergyTarget = Evt->getPnEnergyTarget();
    Evt->setPnEnergyTarget(E1 > EnergyTarget ? E1 : EnergyTarget);
    auto EnergyECAL = Evt->getPnEnergyEcal();
    Evt->setPnEnergyEcal(E2 > EnergyECAL ? E2 : EnergyECAL);
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......
/// \brief
/// \param[in] mc
/// \param[in] ParentID
void RootManager::FillMC(McParticle *fMC, int ParentID) {
    if (if_clean) return;

    auto mc = new McParticle(*fMC);

    auto mcps = Evt->getMcParticleCollection_Old().at(dControl->RawMCCollection_Name);
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

    //now deal with optical: first squzee nullptr, and record number
    //then fill
    //then recover nullptr
//    std::map<G4String,G4int> squzeedCell({});
//    for (auto &imap : Evt->getOpticalCollection_Old())
//    {
//        auto DiGis = *(imap.second); //vec
//        auto cIn = imap.first;
//        squzeedCell.insert(std::pair<G4String,G4int>(cIn,0));
//        auto *counter=&(squzeedCell.at(cIn));
//        auto it = DiGis.begin();
//        for (; it != DiGis.end();) {
//            if (!(*it)) {
//                it = DiGis.erase(it);
//                *counter++;
//            } else if ((*it)->GetDetID() < 0) {
//                delete *it;
//                it = DiGis.erase(it);
//                *counter++;
//            }
//        }
//        DiGis.shrink_to_fit(); //not clear here, wo do at FillSim
//    }

    tr->Fill();
//    Evt->Initialization(nVector);//do this in initialize
    initialize();
    //revocer optical nullptr trick
//    for (auto &imap : Evt->getOpticalCollection_Old()) {
//        auto DiGis = *(imap.second); //vec
//        auto cIn = imap.first;
//        const auto count=squzeedCell.at(cIn);
//        DiGis.insert(DiGis.begin(), count, nullptr);
//    }
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

void RootManager::FillSimHit(const G4String &cIn, SimulatedHit *hit) {

    auto SimHits = Evt->getSimulatedHitCollection_Old();
    auto Hits = new SimulatedHit(*hit);

    SimHits.at(cIn)->emplace_back(Hits);
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

void RootManager::FillEleak(const G4Step *in, const G4String& type) {
    auto deltaE = in->GetTotalEnergyDeposit();

    if (type == "ECAL") Evt->setEleakEcal(Evt->getEleakEcal() + deltaE);
//    else if (type.contains("_PVW")) {
//        auto cin = type.remove(type.index("_PVW"));
//
//        auto SimHits = Evt->getSimulatedHitCollection_Old();
//        auto itr = SimHits.at(cin)->end() - 1;
//        (*itr)->setELeakWrapper( (*itr)->getELeakWrapper() + deltaE);
//    }
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......
// Optical Part Start ///////////////////////////////////////////////////////////
/*
bool RootManager::FillOpticalLUTs(const G4Step *in, G4String type) {
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
*/
bool RootManager::SetOpticalTimeZero(G4double T0, G4String cIn){ //global T0 for on unit across many particles
    //dummy, not used here. 
    //Time zero is used for pileup simulation
    return true;
}
bool RootManager::FillOpticalLUTs(std::vector<OpticalHit *>& hits, G4int GenNo, G4String cIn, int copyNum) //or we can fill as a whole, like a vector, how about memory???
{   
    //check collection
    auto DiGiMap = Evt->getOpticalCollection_Old();
    if (DiGiMap.find(cIn) == DiGiMap.end()){
        G4cerr<<"Not found optical collection cIn="<<cIn<<G4endl;
        return false;
    }
    //check digitizer
    if (fDigitizers.find(cIn) == fDigitizers.end()){
        G4cerr<<"Not found odigitizer cIn="<<cIn<<G4endl;
        return false;
    }
//    auto DiGis = fDigitizers[cIn].GetDiGis();
//    if (DiGis.size() == 0){
//        G4cerr<<"Invalid optical collection cIn="<<cIn<<" Size="<<DiGis.size()<<G4endl;
//        return false;
//    }

//    !DiGis[copyNum] && DiGis[copyNum]->GetDetID()>=0
//    if(fDigitizers[cIn].){
//        DiGis[copyNum] = new DigiForm();
//        DiGis[copyNum]->Initilize(copyNum,dControl->Optical_GetDetType(cIn),dControl->Optical_GetDigiScheme(cIn),dControl->Optical_sampleInterval);
//    }
//    //fill digiForm
//    auto DiGi=DiGis[copyNum];
    auto DiGi = fDigitizers[cIn].GetDiGi(copyNum);
    if(!DiGi){
        return false;
    }
    // DiGi->SetTimeSeqZero(0);
    for(auto* h:hits)
        DiGi->AddTimeSeq(h->GetArrivalT());
    DiGi->SetVoltageToADC(dControl->Optical_voltageToADC());
    DiGi->SetRangeMin(dControl->Optical_range_min);
    DiGi->SetRangeMax(dControl->Optical_range_max);
    DiGi->SetPedestal(dControl->Optical_pedestalLevel);
    DiGi->AddOpticalGen(GenNo);
    fDigitizers[cIn].AddHits(hits,copyNum); //hits are swap to digitizer, then cleared there.
    return true;
}
bool RootManager::FinalizeOptical()
{
    for (auto &imap : Evt->getOpticalCollection_Old())
    {
        auto DiGis = *(imap.second); //vec
        auto cIn = imap.first;
        auto digitizer = fDigitizers[cIn];
//        G4int cId=-1;
//        for (auto *DiGi : DiGis)
//            if (DiGi && (cId=DiGi->GetDetID()) >= 0) {
//                digitizer.Digitize(DiGi, cId);
//            }

        digitizer.DigitizeAll(); //will scan the DiGis inside, and return the DiGis
        for (auto *d: digitizer.GetDiGis()) { //fill really DEvent object
            if (d)
                DiGis.push_back(d);
        }
        digitizer.ResetPixelHistory();
        digitizer.ClearHits();
        //clean nullptr vector
        // DiGis.remove(nullptr); //Erase–remove idiom...
//        DiGis.shrink_to_fit(); //not clear here, wo do at FillSim
    }
    return true;
}

// Optical Part End    ///////////////////////////////////////////////////////////
//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......
void RootManager::FillParticleStep(const G4Step *aStep) {
    G4StepPoint *prev = aStep->GetPreStepPoint();
    G4StepPoint *post = aStep->GetPostStepPoint();

    auto Steps = Evt->getStepCollection_Old().at(dControl->InitialParticleStepCollection_Name);
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
