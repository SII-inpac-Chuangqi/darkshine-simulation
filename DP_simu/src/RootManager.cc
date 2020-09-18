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
:rootFile(nullptr), tr(nullptr), fStart(0),fEvtNb(100000), if_clean(false)
{
    //gInterpreter->GenerateDictionary("vector<double*>","vector");
    //gInterpreter->GenerateDictionary("vector<TVector3>","vector");

    fMessenger = new RootMessenger(this);
    outfilename = "dp_out.root";
    initialize();
    if_Optical = false;
    if_record_ip = true;

}

void RootManager::initialize()
{
    // Initialization
    EventID	            =0;
    for(double & i : Rndm) i    = 0 ;

    // truth 
    for(int i=0; i<3; i++) {
        t_e1_Momentum[i] = 0.;
        t_e1_Pos[i] = 0.;
        t_e2_Momentum[i] = 0.;
        t_e2_Pos[i] = 0.;
    }

    t_mc_Nb = 0;
    t_mc_PNEnergy_Tar = 0;
    t_mc_PNEnergy_ECal = 0;
    t_mc_Eleak_ECAL = 0;
    for(int i=0; i<MaxMCPs; i++) {
        t_mc_id[i] = 0 ;
        t_mc_PDG[i] = 0 ;
        t_mc_ParentID[i] = 0 ;
        t_mc_Px[i] = 0. ;
        t_mc_Py[i] = 0. ;
        t_mc_Pz[i] = 0. ;
        t_mc_E[i] = 0. ;
        t_mc_Eremain[i] = 0. ;
        t_mc_VPosx[i] = 0. ;
        t_mc_VPosy[i] = 0. ;
        t_mc_VPosz[i] = 0. ;
        t_mc_EPosx[i] = 0. ;
        t_mc_EPosy[i] = 0. ;
        t_mc_EPosz[i] = 0. ;

        t_mc_ProcessType[i] = 0 ;
        t_mc_ProcessSubType[i] = 0 ;
    }

    if (if_record_ip)
    {
        ip_Pos.clear();
        ip_Mom.clear();
        ip_Energy.clear();
        ip_PVName.clear();
        ip_ProcessName.clear();
    }


    for(itr_i = Hit_No.begin(); itr_i != Hit_No.end(); itr_i++ ) itr_i->second = 0;
    for(itr_d = Hit_Eleak_Wrapper.begin(); itr_d != Hit_Eleak_Wrapper.end(); itr_d++ ) itr_d->second = 0.;
    for(itr_int = Hit_Type.begin(); itr_int != Hit_Type.end(); itr_int++ ) {
        std::fill(itr_int->second,itr_int->second+MaxHitsE,0);
        //for(int i=0; i<MaxHitsE; i++) (itr_int->second)[i] = 0;
    }
    for(itr_int = Hit_ID.begin(); itr_int != Hit_ID.end(); itr_int++ ) {
        std::fill(itr_int->second,itr_int->second+MaxHitsE,0);
        //for(int i=0; i<MaxHitsE; i++) (itr_int->second)[i] = 0;
    }
    for(itr_int = Hit_PDG.begin(); itr_int != Hit_PDG.end(); itr_int++ ) {
        std::fill(itr_int->second,itr_int->second+MaxHitsE,0);
        //for(int i=0; i<MaxHitsE; i++) (itr_int->second)[i] = 0;
    }
    for(itr_int = Hit_DetectorID.begin(); itr_int != Hit_DetectorID.end(); itr_int++ ) {
        std::fill(itr_int->second,itr_int->second+MaxHitsE,0);
        //for(int i=0; i<MaxHitsE; i++) (itr_int->second)[i] = 0;
    }
    for(itr_int = Hit_DetectorID_x.begin(); itr_int != Hit_DetectorID_x.end(); itr_int++ ) {
        std::fill(itr_int->second,itr_int->second+MaxHitsE,0);
        //for(int i=0; i<MaxHitsE; i++) (itr_int->second)[i] = 0;
    }
    for(itr_int = Hit_DetectorID_y.begin(); itr_int != Hit_DetectorID_y.end(); itr_int++ ) {
        std::fill(itr_int->second,itr_int->second+MaxHitsE,0);
        //for(int i=0; i<MaxHitsE; i++) (itr_int->second)[i] = 0;
    }
    for(itr_int = Hit_DetectorID_z.begin(); itr_int != Hit_DetectorID_z.end(); itr_int++ ) {
        std::fill(itr_int->second,itr_int->second+MaxHitsE,0);
        //for(int i=0; i<MaxHitsE; i++) (itr_int->second)[i] = 0;
    }
    for(itr_double = Hit_Time.begin(); itr_double != Hit_Time.end(); itr_double++ ) {
        std::fill(itr_double->second,itr_double->second+MaxHitsE,0.);
        //for(int i=0; i<MaxHitsE; i++) (itr_double->second)[i] = 0;
    }
    for(itr_double = Hit_Edep.begin(); itr_double != Hit_Edep.end(); itr_double++ ) {
        std::fill(itr_double->second,itr_double->second+MaxHitsE,0.);
        //for(int i=0; i<MaxHitsE; i++) (itr_double->second)[i] = 0;
    }
    for(itr_double = Hit_EdepEM.begin(); itr_double != Hit_EdepEM.end(); itr_double++ ) {
        std::fill(itr_double->second,itr_double->second+MaxHitsE,0.);
        //for(int i=0; i<MaxHitsE; i++) (itr_double->second)[i] = 0;
    }
    for(itr_double = Hit_EdepHad.begin(); itr_double != Hit_EdepHad.end(); itr_double++ ) {
        std::fill(itr_double->second,itr_double->second+MaxHitsE,0.);
        //for(int i=0; i<MaxHitsE; i++) (itr_double->second)[i] = 0;
    }
    for(itr_double = Hit_X.begin(); itr_double != Hit_X.end(); itr_double++ ) {
        std::fill(itr_double->second,itr_double->second+MaxHitsE,0.);
        //for(int i=0; i<MaxHitsE; i++) (itr_double->second)[i] = 0;
    }
    for(itr_double = Hit_Y.begin(); itr_double != Hit_Y.end(); itr_double++ ) {
        std::fill(itr_double->second,itr_double->second+MaxHitsE,0.);
        //for(int i=0; i<MaxHitsE; i++) (itr_double->second)[i] = 0;
    }
    for(itr_double = Hit_Z.begin(); itr_double != Hit_Z.end(); itr_double++ ) {
        std::fill(itr_double->second,itr_double->second+MaxHitsE,0.);
        //for(int i=0; i<MaxHitsE; i++) (itr_double->second)[i] = 0;
    }
    if (if_Optical) {
        for (itr_i = Optical_No.begin(); itr_i != Optical_No.end(); itr_i++) itr_i->second = 0;
        for (itrvec_double = Optical_Time.begin(); itrvec_double != Optical_Time.end(); itrvec_double++)
            itrvec_double->second->clear();

        for (itrvec_double = Optical_E.begin(); itrvec_double != Optical_E.end(); itrvec_double++)
            itrvec_double->second->clear();

        for (itrvec_int = Optical_DetID.begin(); itrvec_int != Optical_DetID.end(); itrvec_int++)
            itrvec_int->second->clear();
        //for (itr_int = Optical_DetID_x.begin(); itr_int != Optical_DetID_x.end(); itr_int++) {
        //    std::fill(itr_int->second,itr_int->second+MaxOptPhoton,0);
            //for (int i = 0; i < MaxOptPhoton; i++) (itr_int->second)[i] = 0;
        //}
        //for (itr_int = Optical_DetID_y.begin(); itr_int != Optical_DetID_y.end(); itr_int++) {
        //    std::fill(itr_int->second,itr_int->second+MaxOptPhoton,0);
            //for (int i = 0; i < MaxOptPhoton; i++) (itr_int->second)[i] = 0;
        //}
        //for (itr_int = Optical_DetID_z.begin(); itr_int != Optical_DetID_z.end(); itr_int++) {
        //    std::fill(itr_int->second,itr_int->second+MaxOptPhoton,0);
            //for (int i = 0; i < MaxOptPhoton; i++) (itr_int->second)[i] = 0;
        //}
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RootManager::~RootManager()
{
    delete fMessenger;
    delete rootFile;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RootManager::book() 
{
    G4String fileName = outfilename;
    rootFile = new TFile(fileName,"RECREATE");

    if(!rootFile) { 
        G4cout << " RootManager::book :"
               << " problem creating the ROOT TFile "
               << G4endl;
        return;
    }

    rnd.SetSeed(fStart+fEvtNb);
    tr = new TTree("Dark_Photon","Dark_Photon");
    if (if_clean) {
        G4cout<<"Clean Mode..."<<G4endl;
    }
    else {
        tr->Branch("RunNumber",    &fStart     ,   "RunNumber/I");
        tr->Branch("EventNumber",  &fEvtN      ,   "EventNumber/I");
        tr->Branch("Rndm",          &Rndm       ,   "Rndm[4]/D");
        // truth
        tr->Branch("TRUTH_e1_Momentum",    &t_e1_Momentum ,"t_e1_Momentum[3]/D");
        tr->Branch("TRUTH_e1_VPos",        &t_e1_Pos      ,"t_e1_VPos[3]/D");
        tr->Branch("TRUTH_e2_Momentum",    &t_e2_Momentum ,"t_e2_Momentum[3]/D");
        tr->Branch("TRUTH_e2_VPos",        &t_e2_Pos      ,"t_e2_VPos[3]/D");

        tr->Branch("TRUTH_MC_Nb"               ,&t_mc_Nb           , "TRUTH_MC_Nb/I"          );
        tr->Branch("TRUTH_MC_id"               ,&t_mc_id           , "TRUTH_MC_id[TRUTH_MC_Nb]/I"            );
        tr->Branch("TRUTH_MC_PDG"              ,&t_mc_PDG          , "TRUTH_MC_PDG[TRUTH_MC_Nb]/I"           );
        tr->Branch("TRUTH_MC_ParentID"         ,&t_mc_ParentID     , "TRUTH_MC_ParentID[TRUTH_MC_Nb]/I"      );
        tr->Branch("TRUTH_MC_Px"               ,&t_mc_Px           , "TRUTH_MC_Px[TRUTH_MC_Nb]/D"            );
        tr->Branch("TRUTH_MC_Py"               ,&t_mc_Py           , "TRUTH_MC_Py[TRUTH_MC_Nb]/D"            );
        tr->Branch("TRUTH_MC_Pz"               ,&t_mc_Pz           , "TRUTH_MC_Pz[TRUTH_MC_Nb]/D"            );
        tr->Branch("TRUTH_MC_E"                ,&t_mc_E            , "TRUTH_MC_E[TRUTH_MC_Nb]/D"            );
        tr->Branch("TRUTH_MC_Eremain"          ,&t_mc_Eremain      , "TRUTH_MC_Eremain[TRUTH_MC_Nb]/D"            );
        tr->Branch("TRUTH_MC_VPosx"            ,&t_mc_VPosx        , "TRUTH_MC_VPosx[TRUTH_MC_Nb]/D"            );
        tr->Branch("TRUTH_MC_VPosy"            ,&t_mc_VPosy        , "TRUTH_MC_VPosy[TRUTH_MC_Nb]/D"            );
        tr->Branch("TRUTH_MC_VPosz"            ,&t_mc_VPosz        , "TRUTH_MC_VPosz[TRUTH_MC_Nb]/D"            );
        tr->Branch("TRUTH_MC_EPosx"            ,&t_mc_EPosx        , "TRUTH_MC_EPosx[TRUTH_MC_Nb]/D"            );
        tr->Branch("TRUTH_MC_EPosy"            ,&t_mc_EPosy        , "TRUTH_MC_EPosy[TRUTH_MC_Nb]/D"            );
        tr->Branch("TRUTH_MC_EPosz"            ,&t_mc_EPosz        , "TRUTH_MC_EPosz[TRUTH_MC_Nb]/D"            );
        //tr->Branch("TRUTH_MC_CreProc"         ,t_mc_CreateProcess , "TRUTH_MC_CreProc[200]/C");  
        tr->Branch("TRUTH_MC_ProcType"          ,&t_mc_ProcessType      , "Process_Type[TRUTH_MC_Nb]/I");  
        tr->Branch("TRUTH_MC_ProcSubType"       ,&t_mc_ProcessSubType   , "Process_SubType[TRUTH_MC_Nb]/I");  
        tr->Branch("TRUTH_MC_PNEnergy_Tar"      ,&t_mc_PNEnergy_Tar     , "TRUTH_MC_PNEnergy_Tar/D");
        tr->Branch("TRUTH_MC_PNEnergy_ECal"     ,&t_mc_PNEnergy_ECal    , "TRUTH_MC_PNEnergy_ECal/D");
        tr->Branch("TRUTH_MC_Eleak_ECAL"        ,&t_mc_Eleak_ECAL       , "TRUTH_MC_Eleak_ECAL/D");

        if (if_record_ip)
        {
            tr->Branch("ip_Pos", &ip_Pos );
            tr->Branch("ip_Momentum", &ip_Mom );
            tr->Branch("ip_Energy", &ip_Energy );
            tr->Branch("ip_PVName", &ip_PVName );
            tr->Branch("ip_ProcessName", &ip_ProcessName );
        }
    }

    G4cout << "===> ROOT file is opened in " << fileName << G4endl;
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

void RootManager::bookCollection(G4String cIn) {

    G4cout<<"[Root Manager] ==> Booking tree for "<<cIn<<" ..." <<G4endl;     
    
    Hit_Eleak_Wrapper.insert( std::pair<G4String, double>(cIn, 0) );

    Hit_No.insert( std::pair<G4String, int>(cIn, 0) );
    Hit_Type.insert( std::pair<G4String, int*>(cIn, new int [MaxHitsE]) );
    Hit_ID.insert( std::pair<G4String, int*>(cIn, new int [MaxHitsE]) );
    Hit_PDG.insert( std::pair<G4String, int*>(cIn, new int [MaxHitsE]) );
    Hit_DetectorID.insert( std::pair<G4String, int*>(cIn, new int [MaxHitsE]) );
    Hit_DetectorID_x.insert( std::pair<G4String, int*>(cIn, new int [MaxHitsE]) );
    Hit_DetectorID_y.insert( std::pair<G4String, int*>(cIn, new int [MaxHitsE]) );
    Hit_DetectorID_z.insert( std::pair<G4String, int*>(cIn, new int [MaxHitsE]) );
    Hit_Time.insert( std::pair<G4String, double*>(cIn, new double [MaxHitsE]) );
    Hit_Edep.insert( std::pair<G4String, double*>(cIn, new double [MaxHitsE]) );
    Hit_EdepEM.insert( std::pair<G4String, double*>(cIn, new double [MaxHitsE]) );
    Hit_EdepHad.insert( std::pair<G4String, double*>(cIn, new double [MaxHitsE]) );
    Hit_X.insert( std::pair<G4String, double*>(cIn, new double [MaxHitsE]) );
    Hit_Y.insert( std::pair<G4String, double*>(cIn, new double [MaxHitsE]) );
    Hit_Z.insert( std::pair<G4String, double*>(cIn, new double [MaxHitsE]) );

    if (if_Optical) {
        Optical_No.insert(std::pair<G4String, int>(cIn, 0));
        Optical_Time.insert(std::pair<G4String, std::vector<double >* >(cIn, new std::vector<double >));
        Optical_E.insert(std::pair<G4String, std::vector<double >* >(cIn, new std::vector<double >));
        Optical_DetID.insert(std::pair<G4String, std::vector<int >* >(cIn, new std::vector<int >));
        //Optical_DetID_x.insert(std::pair<G4String, int *>(cIn, new int[MaxOptPhoton]));
        //Optical_DetID_y.insert(std::pair<G4String, int *>(cIn, new int[MaxOptPhoton]));
        //Optical_DetID_z.insert(std::pair<G4String, int *>(cIn, new int[MaxOptPhoton]));
    }

    tr->Branch( (cIn + "_No").data() , &Hit_No[cIn], (cIn + "_No/I").data() );
    tr->Branch( (cIn + "_Eleak_Wrapper").data() , &Hit_Eleak_Wrapper[cIn], (cIn + "_Eleak_Wrapper/D").data() );
    tr->Branch( (cIn + "_Type").data() , Hit_Type[cIn], (cIn + "_Type["+cIn+"_No]/I").data() );
    //tr->Branch( (cIn + "_ID").data() , Hit_ID[cIn], (cIn + "_ID["+cIn+"_No]/I").data() );
    //tr->Branch( (cIn + "_PDG").data() , Hit_PDG[cIn], (cIn + "_PDG["+cIn+"_No]/I").data() );
    tr->Branch( (cIn + "_DetectorID").data() , Hit_DetectorID[cIn], (cIn + "_DetectorID["+cIn+"_No]/I").data() );
    tr->Branch( (cIn + "_DetectorID_x").data() , Hit_DetectorID_x[cIn], (cIn + "_DetectorID_x["+cIn+"_No]/I").data() );
    tr->Branch( (cIn + "_DetectorID_y").data() , Hit_DetectorID_y[cIn], (cIn + "_DetectorID_y["+cIn+"_No]/I").data() );
    tr->Branch( (cIn + "_DetectorID_z").data() , Hit_DetectorID_z[cIn], (cIn + "_DetectorID_z["+cIn+"_No]/I").data() );
    tr->Branch( (cIn + "_Time").data() , Hit_Time[cIn], (cIn + "_Time["+cIn+"_No]/D").data() );
    tr->Branch( (cIn + "_Edep").data() , Hit_Edep[cIn], (cIn + "_Edep["+cIn+"_No]/D").data() );
    tr->Branch( (cIn + "_EdepEM").data() , Hit_EdepEM[cIn], (cIn + "_EdepEM["+cIn+"_No]/D").data() );
    tr->Branch( (cIn + "_EdepHad").data() , Hit_EdepHad[cIn], (cIn + "_EdepHad["+cIn+"_No]/D").data() );
    tr->Branch( (cIn + "_X").data() , Hit_X[cIn], (cIn + "_X["+cIn+"_No]/D").data() );
    tr->Branch( (cIn + "_Y").data() , Hit_Y[cIn], (cIn + "_Y["+cIn+"_No]/D").data() );
    tr->Branch( (cIn + "_Z").data() , Hit_Z[cIn], (cIn + "_Z["+cIn+"_No]/D").data() );

    if (if_Optical) {
        tr->Branch((cIn + "_Optical_No").data(), &Optical_No[cIn], (cIn + "_Optical_No/I").data());
        tr->Branch((cIn + "_Optical_Time").data(), Optical_Time[cIn] );
        tr->Branch((cIn + "_Optical_E").data(), Optical_E[cIn] );
        tr->Branch((cIn + "_Optical_DetID").data(), Optical_DetID[cIn] );
        //tr->Branch((cIn + "_Optical_DetID_x").data(), Optical_DetID_x[cIn],
        //           (cIn + "_Optical_DetID_x[" + cIn + "_Optical_No]/I").data());
        //tr->Branch((cIn + "_Optical_DetID_y").data(), Optical_DetID_y[cIn],
        //           (cIn + "_Optical_DetID_y[" + cIn + "_Optical_No]/I").data());
        //tr->Branch((cIn + "_Optical_DetID_z").data(), Optical_DetID_z[cIn],
        //           (cIn + "_Optical_DetID_z[" + cIn + "_Optical_No]/I").data());
    }
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

void RootManager::save()
{
    if (rootFile) {
        rootFile->WriteTObject(tr,"","Overwrite");
        rootFile->Close();
        G4cout << "[Root Manager] ==> Simulation Tree is saved \n" << G4endl;
    }
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

void RootManager::FillE1( MCParticle* mc ) {
    if(if_clean) return;

    for(int i=0; i<3; i++) {
        t_e1_Momentum[i]   =  mc->GetMomentum()[i] ;
        t_e1_Pos[i]        =  mc->GetVPos()[i] ;
    }
}

void RootManager::FillE2( MCParticle* mc ) {
    if(if_clean) return;

    for(int i=0; i<3; i++) {
        t_e2_Momentum[i]   =  mc->GetMomentum()[i] ;
        t_e2_Pos[i]        =  mc->GetVPos()[i] ;
    }
}

void RootManager::FillPNE( G4double E1, G4double E2 ) {

        t_mc_PNEnergy_Tar  = E1 > t_mc_PNEnergy_Tar  ? E1 : t_mc_PNEnergy_Tar; 
        t_mc_PNEnergy_ECal = E2 > t_mc_PNEnergy_ECal ? E2 : t_mc_PNEnergy_ECal; 
}


//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

void RootManager::FillMC( MCParticle* mc, G4double Eremain) {
    if(if_clean) return;

    t_mc_id[t_mc_Nb] = mc->Getid() ;          
    t_mc_PDG[t_mc_Nb] = mc->GetPDG() ;        
    t_mc_ParentID[t_mc_Nb] = mc->GetParentID();    
    t_mc_E[t_mc_Nb]      = mc->GetEnergy();
    t_mc_Eremain[t_mc_Nb]= Eremain;
    t_mc_Px[t_mc_Nb]     = mc->GetMomentum()[0]; 
    t_mc_Py[t_mc_Nb]     = mc->GetMomentum()[1]; 
    t_mc_Pz[t_mc_Nb]     = mc->GetMomentum()[2]; 
    t_mc_VPosx[t_mc_Nb]  =  mc->GetVPos()[0];
    t_mc_VPosy[t_mc_Nb]  =  mc->GetVPos()[1];
    t_mc_VPosz[t_mc_Nb]  =  mc->GetVPos()[2];
    t_mc_EPosx[t_mc_Nb]  =  mc->GetEPos()[0];
    t_mc_EPosy[t_mc_Nb]  =  mc->GetEPos()[1];
    t_mc_EPosz[t_mc_Nb]  =  mc->GetEPos()[2];

    t_mc_ProcessType[t_mc_Nb]       = mc->GetProcessType();
    t_mc_ProcessSubType[t_mc_Nb]    = mc->GetProcessSubType();
    //strncpy(t_mc_CreateProcess[t_mc_Nb],mc->GetProcess().data(),20);

    t_mc_Nb++;
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

void RootManager::FillSim(Int_t    eventID, const Double_t* Rnd)
{

    EventID     = eventID + fEvtNb*fStart ;
    fEvtN       = eventID      ;
    for(int i=0;i<4;i++){
        Rndm[i] = *(Rnd+i) ;
    }

    tr->Fill();

    initialize();
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

void RootManager::FillSimHit( const G4String& cIn, SimHit* hit )
{
    
    Hit_Type[cIn][ Hit_No[cIn] ] = hit->GetType();
    Hit_ID[cIn][ Hit_No[cIn] ] = hit->GetParticleID();
    Hit_PDG[cIn][ Hit_No[cIn] ] = hit->GetPDG();
    Hit_DetectorID[cIn][ Hit_No[cIn] ] = hit->GetDetectorRepNo();
    Hit_DetectorID_x[cIn][ Hit_No[cIn] ] = hit->GetDetectorID().x();
    Hit_DetectorID_y[cIn][ Hit_No[cIn] ] = hit->GetDetectorID().y();
    Hit_DetectorID_z[cIn][ Hit_No[cIn] ] = hit->GetDetectorID().z();
    Hit_Time[cIn][ Hit_No[cIn] ] = hit->GetTime();
    Hit_Edep[cIn][ Hit_No[cIn] ] = hit->GetEdep();
    Hit_EdepEM[cIn][ Hit_No[cIn] ] = hit->GetEdepEM();
    Hit_EdepHad[cIn][ Hit_No[cIn] ] = hit->GetEdepHad();
    Hit_X[cIn][ Hit_No[cIn] ] = hit->GetX();
    Hit_Y[cIn][ Hit_No[cIn] ] = hit->GetY();
    Hit_Z[cIn][ Hit_No[cIn] ] = hit->GetZ();

    Hit_No[cIn]++;
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

void RootManager::FillEleak(const G4Step* in, G4String type) 
{
    auto deltaE = in->GetTotalEnergyDeposit();

    if (type == "ECAL") t_mc_Eleak_ECAL += deltaE;
    else if ( type.contains("_PVW") ) {
        auto cin = type.remove( type.index("_PVW") );

        Hit_Eleak_Wrapper[cin] += deltaE;
    }
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

bool RootManager::FillOptical( const G4Step* in, G4String type) 
{
    bool flag = false;
    if ( type.contains("_APDWorld_PV") ) {
        //G4cout << "==>Optical Photon Detected in APD." << G4endl;
        auto cin = type.remove( type.index("_APDWorld_PV") );
        auto* touchable = (G4TouchableHistory*)(in->GetPreStepPoint()->GetTouchable());
        G4int reNumber = touchable->GetReplicaNumber();
        Optical_DetID[cin]->emplace_back(reNumber);

        Optical_Time[cin]->emplace_back(in->GetPostStepPoint()->GetGlobalTime()); // ns
        Optical_E[cin]->emplace_back(in->GetPostStepPoint()->GetTotalEnergy()/eV); // optical photon in eV unit

        Optical_No[cin] ++;

        flag = true;
    }

    return flag;
}
//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......
void RootManager::FillParticleStep(const G4Step * aStep)
{
    G4StepPoint* prev = aStep->GetPreStepPoint();
    G4StepPoint* post = aStep->GetPostStepPoint();
    if ( ip_Pos.size() == 0 )
    {

        ip_Energy.emplace_back(prev->GetTotalEnergy());

        double posD[] = {prev->GetPosition()[0],prev->GetPosition()[1],prev->GetPosition()[2]};
        ip_Pos.emplace_back(TArrayD(3,posD));

        double MomD[] = {prev->GetMomentum()[0],prev->GetMomentum()[1],prev->GetMomentum()[2]};
        ip_Mom.emplace_back(TArrayD(3,MomD));

        auto tmp1 = prev->GetPhysicalVolume()->GetName().data();
        ip_PVName.emplace_back(TString(tmp1));
        ip_ProcessName.emplace_back(TString("Initial Step"));

    }
    ip_Energy.emplace_back(post->GetTotalEnergy());

    double posD[] = {post->GetPosition()[0],post->GetPosition()[1],post->GetPosition()[2]};
    ip_Pos.emplace_back(TArrayD(3,posD));

    double MomD[] = {post->GetMomentum()[0],post->GetMomentum()[1],post->GetMomentum()[2]};
    ip_Mom.emplace_back(TArrayD(3,MomD));

    auto tmp1 = post->GetPhysicalVolume()->GetName().data();
    ip_PVName.emplace_back(TString(tmp1));
    auto tmp2 = post->GetProcessDefinedStep()->GetProcessName();
    const char* tmp3;
    if ( tmp2.contains("biasWrapper") )
        tmp3 = tmp2( tmp2.index("(") + 1, tmp2.index(")") - tmp2.index("(") - 1 ).data();
    else
        tmp3 = tmp2.data();
    ip_ProcessName.emplace_back(TString(tmp3));


}