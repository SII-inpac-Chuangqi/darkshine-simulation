//
// Created by Zhang Yulei on 9/19/20.
//

#include "Algo/ExampleProcessor.h"

void ExampleProcessor::Begin() {
    /*
     *
     *  DEFINE Processor explicitly with NAME in ControlManager.cpp L.37
     *  DEFINE Processor explicitly with NAME in ControlManager.cpp L.37
     *  DEFINE Processor explicitly with NAME in ControlManager.cpp L.37
     *
     */

    // Add description for this AnaProcessor
    Description = "Just an Example Processor.";

    /*
     * How to register parameters?
     *
     * RegisterIntParamter -> register int type variables
     * RegisterIntParameter( "intVar", // variable name
     *                       "Int Variable", // variable description
     *                       &intVar,  // variable address
     *                       0);       // varaible default value
     *
     * You can always register multiple variables with same type.
     * Parameters can be access from the config file by variable name.
     *
     *
     */

    // Register Int parameter
    RegisterIntParameter("intVar", "Int Variable", &intVar, 0);
    RegisterIntParameter("Verbose", "Verbosity Variable", &verbose, 0);

    // Register Double parameter
    RegisterDoubleParameter("DoubleVar", "Double Var", &doubleVar, 0.);

    // Register String Parameter
    RegisterStringParameter("StrVar", "String Variable", &strVar, "test");

}

void ExampleProcessor::ProcessEvt(AnaEvent *evt) {

    //cout<<"p1: "<<intVar<<endl;
    //cout<<"p2: "<<doubleVar<<endl;
    //cout<<"p3: "<<strVar<<endl;

    cout << "Event ID: " << evt->getEventId() << endl;

    //for ( const auto& t : *evt->ListAllCollections() ) std::cout<<t<<std::endl;

    /*
     * Example to get MC Particle collection
     */

    // Get MCCollections for the current event
    const auto &MCCollection = evt->getMcParticleCollection();

    // define the collection name (RawMCParticle) to find.
    std::string CollectionName = "RawMCParticle";

    // IMPORTANT: check if the collection exists
    if (MCCollection.count(CollectionName) != 0) {
        const auto& mc = MCCollection.at(CollectionName);
        // if exists, then do something

        // Loop the collection to print out the information of each particle
        for (auto itr : *mc) {
            if (verbose > 0) {
                cout << "Particle " << itr->getId() << ": ";
                cout << ", PDG: " << itr->getPdg();
                cout << ", Energy: " << itr->getEnergy();
                cout << ", Create Process:" << itr->getCreateProcess() << std::endl;
            }
            //itr->setId(1000);
            //cout << "Particle " << itr->getId() << std::endl;
        }
    } else {
        // if not exists, print out error
        if (verbose > 0)
            cerr << "MCCollection not found" << endl;

    }
}

void ExampleProcessor::CheckEvt(AnaEvent *evt) {
    //cout<<"Check!"<<endl;

}

void ExampleProcessor::End() {
    //cout<<"End!"<<endl;

}


