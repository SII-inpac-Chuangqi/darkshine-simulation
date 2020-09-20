//
// Created by Zhang Yulei on 9/19/20.
//

#include "Algo/ExampleProcessor.h"

void ExampleProcessor::Begin() {
    cout<<"Begin of Processor: "<<this->Name<<endl;
    // Register Int parameter
    RegisterIntParameter("intVar","Int Variable",&intVar,0);

    // Register Double parameter
    RegisterDoubleParameter("DoubleVar","Double Var",&doubleVar,0.);

    // Register String Parameter
    RegisterStringParameter("StrVar","String Variable", &strVar, "test");

}

void ExampleProcessor::ProcessEvt(DEvent* evt) {
    cout<<"Process # "<<evt->getEventId()<<" event ..."<<endl;

    cout<<"p1: "<<intVar<<endl;
    cout<<"p2: "<<doubleVar<<endl;
    cout<<"p3: "<<strVar<<endl;

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
    cout<<"Check!"<<endl;

}

void ExampleProcessor::End() {
    cout<<"End!"<<endl;

}


