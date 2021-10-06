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

    EvtWrt->RegisterIntVariable("Process_HardBrem_Target", &Process_HardBrem_Target, "Process_HardBrem_Target/I");
    EvtWrt->RegisterIntVariable("Process_HardBrem_ECAL", &Process_HardBrem_ECAL, "Process_HardBrem_ECAL/I");
    EvtWrt->RegisterIntVariable("Process_EN_Pre_Target", &Process_EN_Pre_Target, "Process_EN_Pre_Target/I");
    EvtWrt->RegisterIntVariable("Process_EN_Pre_ECAL", &Process_EN_Pre_ECAL, "Process_EN_Pre_ECAL/I");
}

void ProcessClassifier::defineProcessName(AnaEvent *Evt, McParticle *mcp) {
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

                if (p->getVertexZ() <= 7.5 && p->getVertexZ() >= -7.5) Process_HardBrem_Target = 1;
                if (p->getVertexZ() <= 636.5 && p->getVertexZ() >= 181.3) Process_HardBrem_ECAL = 1;

                int n_mu = 0;
                double mu_Z = -611.;
                double Max_pnE = 0;
                for (auto pc: *(p->getChildren())) {

                    //=======GammaToMuPair=========
                    if (std::abs(pc->getPdg()) == 13) {
                        n_mu += 1;
                        mu_Z = pc->getVertexZ();
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
                    Process_Vertex_Z = mu_Z;
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

            if (step->getZ() < -7.5) Process_EN_Pre_Target = (step->getE() >= 4000.);
            if (step->getZ() < 181.3) Process_EN_Pre_ECAL = (step->getE() >= 4000.);

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
}

void ProcessClassifier::initialization() {
    ProcessName = "inclusive";
    PVName = "";
    ProcessEnergy = 0.;
    Process_Vertex_Z = -611.;

    Process_HardBrem_Target = 0;
    Process_HardBrem_ECAL = 0;

    Process_EN_Pre_Target = 0;
    Process_EN_Pre_ECAL = 0;
}
