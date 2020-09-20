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

    // Register Int parameter
    RegisterIntParameter("intVar","Int Variable",&intVar,0);

    // Register Double parameter
    RegisterDoubleParameter("DoubleVar","Double Var",&doubleVar,0.);

    // Register String Parameter
    RegisterStringParameter("StrVar","String Variable", &strVar, "test");

}

void ExampleProcessor::ProcessEvt(DEvent* evt) {

    //cout<<"p1: "<<intVar<<endl;
    //cout<<"p2: "<<doubleVar<<endl;
    //cout<<"p3: "<<strVar<<endl;

    //cout<<evt->getEventId()<<endl;
/*
    for ( const auto& t : *evt->ListAllCollections() )
        std::cout<<t<<std::endl;
    const auto& MCCollection = evt->getMCParticleCollection();
    auto mc = MCCollection.at("RawMCParticle");
    for (auto itr : *mc)
    {
        cout<<"Particle " <<itr->getId() <<": ";
        cout<<", PDG: "<<itr->getPdg();
        cout<<", Energy: "<<itr->getEnergy();
        cout<<", Create Process:"<<itr->getCreateProcess()<<std::endl;
    }
*/
}

void ExampleProcessor::CheckEvt(DEvent* evt) {
    //cout<<"Check!"<<endl;

}

void ExampleProcessor::End() {
    //cout<<"End!"<<endl;

}


