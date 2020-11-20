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

        // Find Secondary
        SecFinder->setEvt(evt);
        auto mcSec = SecFinder->FindSecondary();


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



