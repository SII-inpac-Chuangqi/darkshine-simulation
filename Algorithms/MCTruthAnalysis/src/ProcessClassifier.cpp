//
// Created by Zhang Yulei on 9/10/21
//

#include "Algo/ProcessClassifier.h"

using namespace std;

void ProcessClassifier::RegisterParameters() {
    EvtWrt->RegisterStrVariable("Process_Type", &ProcessName);
    EvtWrt->RegisterStrVariable("Process_PVName", &PVName);
    EvtWrt->RegisterDoubleVariable("Process_Vertex_Z", &Process_Vertex_Z, "Process_Vertex_Z/D");
    EvtWrt->RegisterDoubleVariable("Process_E", &ProcessEnergy, "Process_E/D");
}

yulei ProcessClassifier::defineProcessName(AnaEvent *Evt, McParticle *mcp) {
    auto mcps = Evt->getMcParticleCollection().at("RawMCParticle");
    auto itrp = (mcp == nullptr) ? mcps->at(0) : mcp;
    ProcessName = "inclusive";

    { // For gamma process
        for (auto p: *(itrp->getChildren())) {
            auto Children_PDG = p->getPdg();
            auto Children_E = p->getEnergy();

            if (Children_E > 4000. && Children_PDG == 22) {
                ProcessName = "hardbrem";

                ProcessEnergy = Children_E;
                Process_Vertex_Z = p->getEndPointZ();

                int n_mu = 0;
                double Max_pnE = 0;
                for (auto pc: *(p->getChildren())) {

                    //=======GammaToMuPair=========
                    if (std::abs(pc->getPdg()) == 13) {
                        n_mu += 1;
                    }

                    //======== photonNuclear ========
                    const string &Name = pc->getCreateProcess();
                    if (Name == "photonNuclear") {
                        if (auto energy = pc->getEnergy(); energy > Max_pnE) {
                            Max_pnE = energy;
                            ProcessName = "photonNuclear";
                            Process_Vertex_Z = pc->getVertexZ();

                            if (pc->getVertexZ() != p->getEndPointZ())
                                cerr << "Error -- " << Evt->getEventId()<<": " << pc->getVertexZ() << ", " << p->getEndPointZ() << endl;
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

        double maxE = 0.;
        for (auto step: *stepIni) {
            if (step->getProcessName() == "electronNuclear") {
                if (prev_s != nullptr) {
                    if (auto deltaE = prev_s->getE() - step->getE(); deltaE >= maxE) {
                        ProcessEnergy = deltaE;
                        ProcessName = "electronNuclear";
                        PVName = step->getPVName();
                        Process_Vertex_Z = step->getZ();

                        maxE = deltaE;
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
