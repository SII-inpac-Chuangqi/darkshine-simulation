//
// Created by Zhang Yulei on 9/19/20.
//

#include "Algo/MCTruthAnalysis.h"

#include <utility>

#include "TString.h"
#include "TLorentzVector.h"


MCTruthAnalysis::MCTruthAnalysis(string name, shared_ptr<EventStoreAndWriter> evtwrt) : AnaProcessor(std::move(name),
                                                                                                     std::move(
                                                                                                             evtwrt)) {
    SecFinder = std::shared_ptr<SecondaryMaxEFinder>(new SecondaryMaxEFinder(EvtWrt));

    // Add description for this AnaProcessor
    Description = "MC Truth Analysis";

    // Register Parameters
    RegisterIntParameter("Verbose", "Verbosity", &verbose, 0);
    RegisterIntParameter("Sec_PDG", "PDG of secondary", &Sec_PDG, 0);
}

void MCTruthAnalysis::Begin() {

    // Register Outputs
    if (EvtWrt) {
        EvtWrt->RegisterDoubleVariable("Initial_Px", &Initial_Px, "Initial_Px/D");
        EvtWrt->RegisterDoubleVariable("Initial_Py", &Initial_Py, "Initial_Py/D");
        EvtWrt->RegisterDoubleVariable("Initial_Pz", &Initial_Pz, "Initial_Pz/D");
        EvtWrt->RegisterDoubleVariable("Initial_X", &Initial_X, "Initial_X/D");
        EvtWrt->RegisterDoubleVariable("Initial_Y", &Initial_Y, "Initial_Y/D");
        EvtWrt->RegisterDoubleVariable("Initial_Z", &Initial_Z, "Initial_Z/D");

        EvtWrt->RegisterDoubleVariable("Recoil_E", &Recoil_E, "Recoil_E/D");
        EvtWrt->RegisterDoubleVariable("Recoil_P", Recoil_P, "Recoil_P[3]/D");
        EvtWrt->RegisterDoubleVariable("Recoil_pT", &Recoil_pT, "Recoil_pT/D");
        EvtWrt->RegisterDoubleVariable("Recoil_theta", &Recoil_theta, "Recoil_theta/D");

        EvtWrt->RegisterDoubleVariable("Parent_E", &Parent_E, "Parent_E/D");
        EvtWrt->RegisterDoubleVariable("Parent_P", Parent_P, "Parent_P[3]/D");
        EvtWrt->RegisterStrVariable("Parent_Volume", &Parent_PVName);

        EvtWrt->RegisterDoubleVariable("Truth_Pi", &Pi, "Truth_Pi/D");
        EvtWrt->RegisterDoubleVariable("Truth_Pf", &Pf, "Truth_Pf/D");

        EvtWrt->RegisterStrVariable("Process_Type", &ProcessName);
        EvtWrt->RegisterStrVariable("Process_PVName", &PVName);
        EvtWrt->RegisterDoubleVariable("Process_Vertex_Z", &ProcessVertexZ, "Process_Vertex_Z/D");
        EvtWrt->RegisterDoubleVariable("Process_E", &Process_E, "Process_E/D");
    }

    SecFinder->RegisterParameters();
}

void MCTruthAnalysis::ProcessEvt(AnaEvent *evt) {

    /*
     * Get MC Particle collection
     */

    // Get MCCollections & StepCollection
    const auto &MCCollection = evt->getMcParticleCollection();
    const auto &StepCollection = evt->getStepCollection();

    // define the collection name (RawMCParticle) to find.
    std::string CollectionName = "RawMCParticle";
    std::string StepCollectionName = "Initial_Particle_Step";

    // IMPORTANT: check if the collection exists
    if (MCCollection.count(CollectionName) != 0
        && StepCollection.count(StepCollectionName) != 0) {

        const auto &mc = MCCollection.at(CollectionName);
        const auto &steps = StepCollection.at(StepCollectionName);

        // Process MCTruthEvent -- ProcessClassifier
        PCFinder.initialization();
        bool isFound   = PCFinder.FoundInitial(evt);
        process  = PCFinder.defineProcessName(isFound, evt);
        ProcessName = std::get<0>(process);
        PVName      = std::get<1>(process);
        ProcessVertexZ  = std::get<2>(process);
        Process_E   = std::get<3>(process);

        // Record Initial Particle Status
        auto step = steps->begin();
        Initial_Px = (*step)->getPx();
        Initial_Py = (*step)->getPy();
        Initial_Pz = (*step)->getPz();
        Initial_X = (*step)->getX();
        Initial_Y = (*step)->getY();
        Initial_Z = (*step)->getZ();

        // Find Secondary
        SecFinder->setEvt(evt);
        auto mcSec = SecFinder->FindSecondary(Sec_PDG);
        if (mcSec) {
            DStep *prev_s = nullptr;
            for (auto s : *steps) {
                if (s->getProcessName() == mcSec->getCreateProcess() && prev_s != nullptr) {
                    if (s->getX() == mcSec->getVertexX()
                        && s->getY() == mcSec->getVertexY()
                        && s->getZ() == mcSec->getVertexZ()) {
                        Parent_E = prev_s->getE();
                        Parent_P[0] = prev_s->getPx();
                        Parent_P[1] = prev_s->getPy();
                        Parent_P[2] = prev_s->getPz();
                        Parent_PVName = TString(prev_s->getPVName());

                        Recoil_E = s->getE();
                        Recoil_P[0] = s->getPx();
                        Recoil_P[1] = s->getPy();
                        Recoil_P[2] = s->getPz();

                        TLorentzVector l(Recoil_P, Recoil_E);
                        Recoil_pT = l.Perp();
                        Recoil_theta = l.Theta();

                        break;
                    }
                }
                prev_s = s;
            }
        }
        // Search for truth Pi && Pf for initial electron
        Pi = -999.;
        Pf = -999.;
        DStep *prev_s = nullptr;
        TLorentzVector electron_prev, electron_post;
        for (auto s: *steps) {
            if (s->getProcessName() == "DMProcessDMBrem" && prev_s != nullptr) {
                electron_prev.SetPxPyPzE(prev_s->getPx(), prev_s->getPy(), prev_s->getPz(), prev_s->getE());
                electron_post.SetPxPyPzE(s->getPx(), s->getPy(), s->getPz(), s->getE());
                Pi = electron_prev.P();
                Pf = electron_post.P();
            }
            prev_s = s;
        }
    } else {
        // if not exists, print out error
        if (verbose > 0)
            cerr << "MCCollection not found" << endl;

    }
}

void MCTruthAnalysis::CheckEvt(AnaEvent *evt) {
    //cout<<"Check!"<<endl;

}

void MCTruthAnalysis::End() {
    //cout<<"End!"<<endl;

}

