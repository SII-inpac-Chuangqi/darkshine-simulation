#include "DP_simu/RootManager.hh"
#include "DP_simu/RootMessenger.hh"

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TRandom.h"
#include "TInterpreter.h"

#include "G4TouchableHistory.hh"

#include <iostream>
#include <stdexcept>
#include <sstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RootManager::RootManager()
        : rootFile(nullptr), tr(nullptr), fStart(0), fEvtNb(100000), if_clean(false) {

    fMessenger = new RootMessenger(this);
    outfilename = "dp_out.root";
    initialize();
    if_Optical = false;
    if_record_ip = true;

}

void RootManager::initialize() {
    // Initialization
    EventID = 0;
    for (double &i : Rndm) i = 0;

    // truth 
    for (int i = 0; i < 3; i++) {
        t_e1_Momentum[i] = 0.;
        t_e1_Pos[i] = 0.;
        t_e2_Momentum[i] = 0.;
        t_e2_Pos[i] = 0.;
    }

    t_mc_Nb = 0;
    t_mc_PNEnergy_Tar = 0;
    t_mc_PNEnergy_ECal = 0;
    t_mc_Eleak_ECAL = 0;

    t_mc_id.clear();
    t_mc_PDG.clear();
    t_mc_ParentID.clear();
    t_mc_Mom.clear();
    t_mc_E.clear();
    t_mc_Eremain.clear();
    t_mc_VPos.clear();
    t_mc_EPos.clear();
    t_mc_ProcessName.clear();

    if (if_record_ip) {
        ip_Pos.clear();
        ip_Mom.clear();
        ip_Energy.clear();
        ip_PVName.clear();
        ip_ProcessName.clear();
    }


    for (itr_i = Hit_No.begin(); itr_i != Hit_No.end(); itr_i++) itr_i->second = 0;
    for (itr_d = Hit_Eleak_Wrapper.begin(); itr_d != Hit_Eleak_Wrapper.end(); itr_d++) itr_d->second = 0.;
    for (itrvec_int = Hit_Type.begin(); itrvec_int != Hit_Type.end(); itrvec_int++) {
        itrvec_int->second->clear();
    }
    for (itrvec_int = Hit_ID.begin(); itrvec_int != Hit_ID.end(); itrvec_int++) {
        itrvec_int->second->clear();
    }
    for (itrvec_int = Hit_PDG.begin(); itrvec_int != Hit_PDG.end(); itrvec_int++) {
        itrvec_int->second->clear();
    }
    for (itrvec_int = Hit_DetectorID.begin(); itrvec_int != Hit_DetectorID.end(); itrvec_int++) {
        itrvec_int->second->clear();
    }
    for (itrvec_int = Hit_DetectorID_x.begin(); itrvec_int != Hit_DetectorID_x.end(); itrvec_int++) {
        itrvec_int->second->clear();
    }
    for (itrvec_int = Hit_DetectorID_y.begin(); itrvec_int != Hit_DetectorID_y.end(); itrvec_int++) {
        itrvec_int->second->clear();
    }
    for (itrvec_int = Hit_DetectorID_z.begin(); itrvec_int != Hit_DetectorID_z.end(); itrvec_int++) {
        itrvec_int->second->clear();
    }
    for (itrvec_double = Hit_Time.begin(); itrvec_double != Hit_Time.end(); itrvec_double++) {
        itrvec_double->second->clear();
    }
    for (itrvec_double = Hit_Edep.begin(); itrvec_double != Hit_Edep.end(); itrvec_double++) {
        itrvec_double->second->clear();
    }
    for (itrvec_double = Hit_EdepEM.begin(); itrvec_double != Hit_EdepEM.end(); itrvec_double++) {
        itrvec_double->second->clear();
    }
    for (itrvec_double = Hit_EdepHad.begin(); itrvec_double != Hit_EdepHad.end(); itrvec_double++) {
        itrvec_double->second->clear();
    }
    for (itrvec_double = Hit_X.begin(); itrvec_double != Hit_X.end(); itrvec_double++) {
        itrvec_double->second->clear();
    }
    for (itrvec_double = Hit_Y.begin(); itrvec_double != Hit_Y.end(); itrvec_double++) {
        itrvec_double->second->clear();
    }
    for (itrvec_double = Hit_Z.begin(); itrvec_double != Hit_Z.end(); itrvec_double++) {
        itrvec_double->second->clear();
    }
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
        tr->Branch("TRUTH_e1_Momentum", &t_e1_Momentum, "t_e1_Momentum[3]/D");
        tr->Branch("TRUTH_e1_VPos", &t_e1_Pos, "t_e1_VPos[3]/D");
        tr->Branch("TRUTH_e2_Momentum", &t_e2_Momentum, "t_e2_Momentum[3]/D");
        tr->Branch("TRUTH_e2_VPos", &t_e2_Pos, "t_e2_VPos[3]/D");

        tr->Branch("TRUTH_MC_Nb", &t_mc_Nb, "TRUTH_MC_Nb/I");
        tr->Branch("TRUTH_MC_id", &t_mc_id);
        tr->Branch("TRUTH_MC_PDG", &t_mc_PDG);
        tr->Branch("TRUTH_MC_ParentID", &t_mc_ParentID);
        tr->Branch("TRUTH_MC_Mom", &t_mc_Mom);
        tr->Branch("TRUTH_MC_E", &t_mc_E);
        tr->Branch("TRUTH_MC_Eremain", &t_mc_Eremain);
        tr->Branch("TRUTH_MC_VPos", &t_mc_VPos);
        tr->Branch("TRUTH_MC_EPos", &t_mc_EPos);
        tr->Branch("TRUTH_MC_ProcessName", &t_mc_ProcessName);

        tr->Branch("TRUTH_MC_PNEnergy_Tar", &t_mc_PNEnergy_Tar, "TRUTH_MC_PNEnergy_Tar/D");
        tr->Branch("TRUTH_MC_PNEnergy_ECal", &t_mc_PNEnergy_ECal, "TRUTH_MC_PNEnergy_ECal/D");
        tr->Branch("TRUTH_MC_Eleak_ECAL", &t_mc_Eleak_ECAL, "TRUTH_MC_Eleak_ECAL/D");

        if (if_record_ip) {
            tr->Branch("ip_Pos", &ip_Pos);
            tr->Branch("ip_Momentum", &ip_Mom);
            tr->Branch("ip_Energy", &ip_Energy);
            tr->Branch("ip_PVName", &ip_PVName);
            tr->Branch("ip_ProcessName", &ip_ProcessName);
        }
    }

    G4cout << "===> ROOT file is opened in " << fileName << G4endl;
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

void RootManager::bookCollection(G4String cIn) {

    G4cout << "[Root Manager] ==> Booking tree for " << cIn << " ..." << G4endl;

    Hit_Eleak_Wrapper.insert(std::pair<G4String, double>(cIn, 0));

    Hit_No.insert(std::pair<G4String, int>(cIn, 0));
    Hit_Type.insert(std::pair<G4String, std::vector<int> *>(cIn, new std::vector<int>));
    Hit_ID.insert(std::pair<G4String, std::vector<int> *>(cIn, new std::vector<int>));
    Hit_PDG.insert(std::pair<G4String, std::vector<int> *>(cIn, new std::vector<int>));
    Hit_DetectorID.insert(std::pair<G4String, std::vector<int> *>(cIn, new std::vector<int>));
    Hit_DetectorID_x.insert(std::pair<G4String, std::vector<int> *>(cIn, new std::vector<int>));
    Hit_DetectorID_y.insert(std::pair<G4String, std::vector<int> *>(cIn, new std::vector<int>));
    Hit_DetectorID_z.insert(std::pair<G4String, std::vector<int> *>(cIn, new std::vector<int>));
    Hit_Time.insert(std::pair<G4String, std::vector<double> *>(cIn, new std::vector<double>));
    Hit_Edep.insert(std::pair<G4String, std::vector<double> *>(cIn, new std::vector<double>));
    Hit_EdepEM.insert(std::pair<G4String, std::vector<double> *>(cIn, new std::vector<double>));
    Hit_EdepHad.insert(std::pair<G4String, std::vector<double> *>(cIn, new std::vector<double>));
    Hit_X.insert(std::pair<G4String, std::vector<double> *>(cIn, new std::vector<double>));
    Hit_Y.insert(std::pair<G4String, std::vector<double> *>(cIn, new std::vector<double>));
    Hit_Z.insert(std::pair<G4String, std::vector<double> *>(cIn, new std::vector<double>));

    if (if_Optical) {
        Optical_No.insert(std::pair<G4String, int>(cIn, 0));
        Optical_Time.insert(std::pair<G4String, std::vector<double> *>(cIn, new std::vector<double>));
        Optical_E.insert(std::pair<G4String, std::vector<double> *>(cIn, new std::vector<double>));
        Optical_DetID.insert(std::pair<G4String, std::vector<int> *>(cIn, new std::vector<int>));
        //Optical_DetID_x.insert(std::pair<G4String, int *>(cIn, new int[MaxOptPhoton]));
        //Optical_DetID_y.insert(std::pair<G4String, int *>(cIn, new int[MaxOptPhoton]));
        //Optical_DetID_z.insert(std::pair<G4String, int *>(cIn, new int[MaxOptPhoton]));
    }

    tr->Branch((cIn + "_No").data(), &Hit_No[cIn], (cIn + "_No/I").data());
    tr->Branch((cIn + "_Eleak_Wrapper").data(), &Hit_Eleak_Wrapper[cIn], (cIn + "_Eleak_Wrapper/D").data());
    tr->Branch((cIn + "_Type").data(), Hit_Type[cIn]);
    tr->Branch((cIn + "_DetectorID").data(), Hit_DetectorID[cIn]);
    tr->Branch((cIn + "_DetectorID_x").data(), Hit_DetectorID_x[cIn]);
    tr->Branch((cIn + "_DetectorID_y").data(), Hit_DetectorID_y[cIn]);
    tr->Branch((cIn + "_DetectorID_z").data(), Hit_DetectorID_z[cIn]);
    tr->Branch((cIn + "_Time").data(), Hit_Time[cIn]);
    tr->Branch((cIn + "_Edep").data(), Hit_Edep[cIn]);
    tr->Branch((cIn + "_EdepEM").data(), Hit_EdepEM[cIn]);
    tr->Branch((cIn + "_EdepHad").data(), Hit_EdepHad[cIn]);
    tr->Branch((cIn + "_X").data(), Hit_X[cIn]);
    tr->Branch((cIn + "_Y").data(), Hit_Y[cIn]);
    tr->Branch((cIn + "_Z").data(), Hit_Z[cIn]);

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

void RootManager::FillE1(MCParticle *mc) {
    if (if_clean) return;

    for (int i = 0; i < 3; i++) {
        t_e1_Momentum[i] = mc->GetMomentum()[i];
        t_e1_Pos[i] = mc->GetVPos()[i];
    }
}

void RootManager::FillE2(MCParticle *mc) {
    if (if_clean) return;

    for (int i = 0; i < 3; i++) {
        t_e2_Momentum[i] = mc->GetMomentum()[i];
        t_e2_Pos[i] = mc->GetVPos()[i];
    }
}

void RootManager::FillPNE(G4double E1, G4double E2) {

    t_mc_PNEnergy_Tar = E1 > t_mc_PNEnergy_Tar ? E1 : t_mc_PNEnergy_Tar;
    t_mc_PNEnergy_ECal = E2 > t_mc_PNEnergy_ECal ? E2 : t_mc_PNEnergy_ECal;
}


//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

void RootManager::FillMC(MCParticle *mc, G4double Eremain) {
    if (if_clean) return;

    t_mc_id.emplace_back(mc->Getid());
    t_mc_PDG.emplace_back(mc->GetPDG());
    t_mc_ParentID.emplace_back(mc->GetParentID());
    t_mc_E.emplace_back(mc->GetEnergy());
    t_mc_Eremain.emplace_back(Eremain);

    double tmp1[] = {mc->GetMomentum()[0], mc->GetMomentum()[1], mc->GetMomentum()[2]};
    t_mc_Mom.emplace_back(TArrayD(3, tmp1));
    double tmp2[] = {mc->GetVPos()[0], mc->GetVPos()[1], mc->GetVPos()[2]};
    t_mc_VPos.emplace_back(TArrayD(3, tmp2));
    double tmp3[] = {mc->GetEPos()[0], mc->GetEPos()[1], mc->GetEPos()[2]};
    t_mc_EPos.emplace_back(TArrayD(3, tmp3));

    t_mc_ProcessName.emplace_back(mc->GetProcess());

    t_mc_Nb++;
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

void RootManager::FillSim(Int_t eventID, const Double_t *Rnd) {

    EventID = eventID + fEvtNb * fStart;
    fEvtN = eventID;
    for (int i = 0; i < 4; i++) {
        Rndm[i] = *(Rnd + i);
    }

    tr->Fill();

    initialize();
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

void RootManager::FillSimHit(const G4String &cIn, SimHit *hit) {
    Hit_Type[cIn]->emplace_back(hit->GetType());
    Hit_ID[cIn]->emplace_back(hit->GetParticleID());
    Hit_PDG[cIn]->emplace_back(hit->GetPDG());
    Hit_DetectorID[cIn]->emplace_back(hit->GetDetectorRepNo());
    Hit_DetectorID_x[cIn]->emplace_back(hit->GetDetectorID().x());
    Hit_DetectorID_y[cIn]->emplace_back(hit->GetDetectorID().y());
    Hit_DetectorID_z[cIn]->emplace_back(hit->GetDetectorID().z());

    Hit_Time[cIn]->emplace_back(hit->GetTime());
    Hit_Edep[cIn]->emplace_back(hit->GetEdep());
    Hit_EdepEM[cIn]->emplace_back(hit->GetEdepEM());
    Hit_EdepHad[cIn]->emplace_back(hit->GetEdepHad());
    Hit_X[cIn]->emplace_back(hit->GetX());
    Hit_Y[cIn]->emplace_back(hit->GetY());
    Hit_Z[cIn]->emplace_back(hit->GetZ());

    Hit_No[cIn]++;
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

void RootManager::FillEleak(const G4Step *in, G4String type) {
    auto deltaE = in->GetTotalEnergyDeposit();

    if (type == "ECAL") t_mc_Eleak_ECAL += deltaE;
    else if (type.contains("_PVW")) {
        auto cin = type.remove(type.index("_PVW"));

        Hit_Eleak_Wrapper[cin] += deltaE;
    }
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

bool RootManager::FillOptical(const G4Step *in, G4String type) {
    bool flag = false;
    if (type.contains("_APDWorld_PV")) {
        //G4cout << "==>Optical Photon Detected in APD." << G4endl;
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
    if (ip_Pos.empty()) {

        ip_Energy.emplace_back(prev->GetTotalEnergy());

        double posD[] = {prev->GetPosition()[0], prev->GetPosition()[1], prev->GetPosition()[2]};
        ip_Pos.emplace_back(TArrayD(3, posD));

        double MomD[] = {prev->GetMomentum()[0], prev->GetMomentum()[1], prev->GetMomentum()[2]};
        ip_Mom.emplace_back(TArrayD(3, MomD));

        auto tmp1 = prev->GetPhysicalVolume()->GetName().data();
        ip_PVName.emplace_back(TString(tmp1));
        ip_ProcessName.emplace_back(TString("Initial Step"));

    }
    ip_Energy.emplace_back(post->GetTotalEnergy());

    double posD[] = {post->GetPosition()[0], post->GetPosition()[1], post->GetPosition()[2]};
    ip_Pos.emplace_back(TArrayD(3, posD));

    double MomD[] = {post->GetMomentum()[0], post->GetMomentum()[1], post->GetMomentum()[2]};
    ip_Mom.emplace_back(TArrayD(3, MomD));

    if (post->GetPhysicalVolume() == nullptr) {
        ip_PVName.emplace_back(TString("OutofWorld"));
        ip_ProcessName.emplace_back(TString("Transportation"));
    } else {
        auto tmp1 = post->GetPhysicalVolume()->GetName().data();
        ip_PVName.emplace_back(TString(tmp1));
        auto tmp2 = post->GetProcessDefinedStep()->GetProcessName();
        const char *tmp3;
        if (tmp2.contains("biasWrapper"))
            tmp3 = tmp2(tmp2.index("(") + 1, tmp2.index(")") - tmp2.index("(") - 1).data();
        else
            tmp3 = tmp2.data();
        ip_ProcessName.emplace_back(TString(tmp3));
    }
}