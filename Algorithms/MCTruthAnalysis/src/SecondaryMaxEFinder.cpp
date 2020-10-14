//
// Created by Zhang Yulei on 9/24/20.
//

#include "Algo/SecondaryMaxEFinder.h"

void SecondaryMaxEFinder::RegisterParameters() {
    // Register Output Variables
    EvtWrt->RegisterIntVariable("Secondary_PDG", &Secondary_PDG, "Secondary_PDG/I");
    EvtWrt->RegisterDoubleVariable("Secondary_MaxE", &Secondary_MaxE, "Secondary_MaxE/D");
    EvtWrt->RegisterStrVariable("Secondary_MaxE_PVName", &Secondary_MaxE_PVName);
    EvtWrt->RegisterStrVariable("Secondary_MaxE_Process", &Secondary_MaxE_Process);
}

McParticle *SecondaryMaxEFinder::FindSecondary(int PDG, double Emin, McParticle *mcp) {

    bool PDG_all = (PDG == 0);
    bool Emin_all = (Emin == 0.);
    McParticle *MCP_Emax = nullptr;
    double EMax = 0.;
    auto mcps = Evt->getMcParticleCollection().at("RawMCParticle");

    // Select Initial Particle or the input particle
    auto itrp = (mcp == nullptr) ? mcps->at(0) : mcp;
    // Loop Children
    for (auto p : *(itrp->getChildren())) {
        if ((PDG_all || p->getPdg() == PDG) && (Emin_all || p->getP() >= Emin)) {
            MCP_Emax = (p->getP() > EMax) ? p : MCP_Emax;
            EMax = (p->getP() > EMax) ? p->getP() : EMax;
        }
    }

    if (MCP_Emax) {
        Secondary_PDG = MCP_Emax->getPdg();
        Secondary_MaxE = EMax;
        Secondary_MaxE_Process = MCP_Emax->getCreateProcess();
    }

    return MCP_Emax;
}

