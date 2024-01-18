//
// Created by Zhang Yulei on 9/24/20.
//

#include "TGeoManager.h"
#include "TLorentzVector.h"

#include "Algo/SecondaryMaxEFinder.h"

void SecondaryMaxEFinder::RegisterParameters() {
    // Register Output Variables
    EvtWrt->RegisterIntVariable("Secondary_Found", &Secondary_Found, "Secondary_Found/I");
    EvtWrt->RegisterIntVariable("Secondary_PDG", &Secondary_PDG, "Secondary_PDG/I");
    EvtWrt->RegisterDoubleVariable("Secondary_MaxE", &Secondary_MaxE, "Secondary_MaxE/D");
    EvtWrt->RegisterDoubleVariable("Secondary_MaxE_P", Secondary_MaxE_P, "Secondary_MaxE_P[3]/D");
    EvtWrt->RegisterStrVariable("Secondary_MaxE_PVName", &Secondary_MaxE_PVName);
    EvtWrt->RegisterStrVariable("Secondary_MaxE_Process", &Secondary_MaxE_Process);
}

// add for visible decay
void SecondaryMaxEFinder::RegisterParameters_visible(){
    // Register some visible decay products
    EvtWrt->RegisterIntVariable("DP_decay", &DP_decay, "DP_decay/I");
    EvtWrt->RegisterDoubleVariable("E_decay_1", &E_decay_1, "E_decay_1/D");
    EvtWrt->RegisterDoubleVariable("E_decay_2", &E_decay_2, "E_decay_2/D");
    EvtWrt->RegisterDoubleVariable("P_decay_1", P_decay_1, "P_decay_1[3]/D");
    EvtWrt->RegisterDoubleVariable("P_decay_2", P_decay_2, "P_decay_2[3]/D");
    EvtWrt->RegisterIntVariable("PDG_decay_1", &PDG_decay_1, "PDG_decay_1/I");
    EvtWrt->RegisterIntVariable("PDG_decay_2", &PDG_decay_2, "PDG_decay_2/I");

}


McParticle *SecondaryMaxEFinder::FindSecondary(int PDG, double Emin, McParticle *mcp) {
    Secondary_Found = 0;

    bool PDG_all = (PDG == 0);
    bool Emin_all = (Emin == 0.);
    McParticle *MCP_Emax = nullptr;
    double EMax = 0.;
    auto mcps = Evt->getMcParticleCollection().at("RawMCParticle");

    // Select Initial Particle or the input particle
    auto itrp = (mcp == nullptr) ? mcps->at(0) : mcp;
    // Loop Children
    for (auto p : *(itrp->getChildren())) {

        TLorentzVector pV;
        pV.SetXYZM(p->getPx(), p->getPy(), p->getPz(), p->getMass());

        if ((PDG_all || p->getPdg() == PDG) && (Emin_all || pV.E() >= Emin)) {
            MCP_Emax = (pV.E() > EMax) ? p : MCP_Emax;
            EMax = (pV.E() > EMax) ? pV.E() : EMax;
        }
    }

    if (MCP_Emax) {
        Secondary_Found = 1;
        Secondary_PDG = MCP_Emax->getPdg();
        Secondary_MaxE = EMax;
        Secondary_MaxE_P[0] = MCP_Emax->getPx();
        Secondary_MaxE_P[1] = MCP_Emax->getPy();
        Secondary_MaxE_P[2] = MCP_Emax->getPz();
        Secondary_MaxE_Process = MCP_Emax->getCreateProcess();
        if (gGeoManager)
            Secondary_MaxE_PVName = gGeoManager->FindNode(MCP_Emax->getVertexX() / 10.,
                                                          MCP_Emax->getVertexY() / 10.,
                                                          MCP_Emax->getVertexZ() / 10.)->GetVolume()->GetName();
    }

    return MCP_Emax;
}

McParticle *SecondaryMaxEFinder::FindDPDecay(McParticle *mcp){
    auto mcps = Evt->getMcParticleCollection().at("RawMCParticle");
    auto itrp = (mcp == nullptr) ? mcps->at(0) : mcp;
    int n_particles = 0;
    for ([[maybe_unused]] auto p : *(itrp->getChildren())) {
        n_particles+=1;
     }
    if (n_particles == 2){
        DP_decay=1;// get 2 decay products
    }
    for (auto p : *(itrp->getChildren())) {
        TLorentzVector pV;
        pV.SetXYZM(p->getPx(), p->getPy(), p->getPz(), p->getMass());
        //1 is particle like 11, 2 is for postive anti-particles
        if (p->getPdg() >0 ){
            PDG_decay_1 = p->getPdg();
            E_decay_1 = p->getEnergy();
            P_decay_1[0] = p->getPx() ;
            P_decay_1[1] = p->getPy() ;
            P_decay_1[2] = p->getPz() ;
        }
        if (p->getPdg() <0 ){
            PDG_decay_2 = p->getPdg();
            E_decay_2 = p->getEnergy();
            P_decay_2[0] = p->getPx() ;
            P_decay_2[1] = p->getPy() ;
            P_decay_2[2] = p->getPz() ;
        }
     }

    //std::cout<< "DP childrens " << n_particles <<std::endl;
    return mcp;
}
