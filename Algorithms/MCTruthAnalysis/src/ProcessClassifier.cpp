//
// Created by Zhang Yulei on 9/10/21
//

#include "Algo/ProcessClassifier.h"

using namespace std;

bool ProcessClassifier::FoundInitial(AnaEvent *Evt) {
    auto mcps = Evt->getMcParticleCollection().at("RawMCParticle");
    int Initial_ID = (*mcps->begin())->getId();
    int Initial_PDG = (*mcps->begin())->getPdg();
    if (Initial_ID == 1 && Initial_PDG == 11) {
        isFoundInit = true;
    }

    return isFoundInit;
}

yulei ProcessClassifier::defineProcessName(bool isFound, AnaEvent *Evt, McParticle *mcp) {
    auto mcps = Evt->getMcParticleCollection().at("RawMCParticle");
    auto itrp = (mcp == nullptr) ? mcps->at(0) : mcp;
    ProcessName = "inclusive";
    if (isFound) {    // Found Initial Electron Particle
        for (auto p: *(itrp->getChildren())) {
            Children_PDG = p->getPdg();
            Children_E = p->getEnergy();

            if (Children_E > 4000. && Children_PDG == 22) {
                ProcessName = "hardbrem";

                ProcessEnergy = Children_E;
                Process_Vertex_Z = p->getEndPointZ();

                n_mu = 0;
                double Max_pnE = 0;
                for (auto pc: *(p->getChildren())) {
                    Children_PDG = pc->getPdg();
                    Children_ID = pc->getId();

                    //=======GammaToMuPair=========
                    if (std::abs(Children_PDG) == 13) {
                        n_mu += 1;
                    }

                    //======== photonNuclear ========
                    const string &Name = pc->getCreateProcess();
                    if (Name == "photonNuclear") {
                        double energy = pc->getEnergy();
                        if (energy > Max_pnE) {
                            Max_pnE = energy;
                            ProcessName = "photonNuclear";
                            Process_Vertex_Z = pc->getVertexZ();

                            if (pc->getVertexZ() != p->getEndPointZ())
                                cerr << "Error: " << pc->getVertexZ() << ", " << p->getEndPointZ() << endl;
                        }
                    }
                }
                if (n_mu == 2) {
                    ProcessName = "GammaToMuPair";
                    Process_Vertex_Z = p->getEndPointZ();
                }
            }
        }
    }

    if (ProcessName == "inclusive") {
        //======== electronNuclear =======
        auto stepCollection = Evt->getStepCollection();
        auto stepIni = stepCollection.at("Initial_Particle_Step");
        DStep *prev_s = nullptr;

        for (auto step: *stepIni) {
            if (step->getProcessName() == "electronNuclear") {
                if (prev_s != nullptr) {
                    if (auto deltaE = prev_s->getE() - step->getE(); deltaE > 4000.) {
                        ProcessEnergy = deltaE;
                        ProcessName = "electronNuclear";
                        PVName = step->getPVName();
                        Process_Vertex_Z = step->getZ();

                        break;
                    }
                }
            }
            prev_s = step;
        }
    }

    return std::make_tuple(ProcessName, PVName, Process_Vertex_Z, ProcessEnergy);
}

void ProcessClassifier::initialization() {
    ProcessName = "inclusive";
    PVName = "";
    ProcessEnergy = 0.;
    Process_Vertex_Z = -611.;
}
