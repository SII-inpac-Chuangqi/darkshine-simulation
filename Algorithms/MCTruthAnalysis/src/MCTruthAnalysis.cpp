//
// Created by Zhang Yulei on 9/19/20.
//

#include "Algo/MCTruthAnalysis.h"

#include <utility>

#include "TString.h"


MCTruthAnalysis::MCTruthAnalysis(string name, shared_ptr<EventStoreAndWriter> evtwrt) : AnaProcessor(std::move(name),
                                                                                                     std::move(
                                                                                                             evtwrt)) {
    SecFinder = std::shared_ptr<SecondaryMaxEFinder>(new SecondaryMaxEFinder(EvtWrt));
}

void MCTruthAnalysis::Begin() {

    // Add description for this AnaProcessor
    Description = "MC Truth Analysis";

    // Register Parameters
    RegisterIntParameter("Verbose", "Verbosity", &verbose, 0);

    // Register Outputs
    if (EvtWrt) {
        //EvtWrt->RegisterIntVariable("Initial_PDG", &Initial_PDG, "Initial_PDG/D");
        EvtWrt->RegisterDoubleVariable("Initial_Px", &Initial_Px, "Initial_Px/D");
        EvtWrt->RegisterDoubleVariable("Initial_Py", &Initial_Py, "Initial_Py/D");
        EvtWrt->RegisterDoubleVariable("Initial_Pz", &Initial_Pz, "Initial_Pz/D");
        EvtWrt->RegisterDoubleVariable("Initial_X", &Initial_X, "Initial_X/D");
        EvtWrt->RegisterDoubleVariable("Initial_Y", &Initial_Y, "Initial_Y/D");
        EvtWrt->RegisterDoubleVariable("Initial_Z", &Initial_Z, "Initial_Z/D");

        EvtWrt->RegisterDoubleVariable("Parent_E", &Parent_E, "Parent_E/D");
        EvtWrt->RegisterDoubleVariable("Parent_P", Parent_P, "Parent_P[3]/D");
        EvtWrt->RegisterStrVariable("Parent_Volume", &Parent_PVName);
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
        auto mcSec = SecFinder->FindSecondary(500012);

        DStep *prev_s = nullptr;
        for (auto s : *steps) {
            if (s->getProcessName() == "DMProcessDMBrem" && prev_s != nullptr) {
                if (mcSec->getEnergy() > prev_s->getE())
                    cout << "E_parent: " << prev_s->getE() << ", E_remain: " << s->getE() << ", DM_E: "
                         << mcSec->getEnergy() << endl;
                Parent_E = prev_s->getE();
                Parent_P[0] = prev_s->getPx();
                Parent_P[1] = prev_s->getPy();
                Parent_P[2] = prev_s->getPz();
                Parent_PVName = TString(prev_s->getPVName());
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

