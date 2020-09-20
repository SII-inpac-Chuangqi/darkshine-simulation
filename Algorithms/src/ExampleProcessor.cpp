//
// Created by Zhang Yulei on 9/19/20.
//

#include "ExampleProcessor.h"

void ExampleProcessor::Begin() {
    cout<<"Begin of Processor: "<<this->Name<<endl;
}

void ExampleProcessor::ProcessEvt(DEvent* evt) {
    cout<<"Process # "<<evt->getEventId()<<" event ..."<<endl;


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
}

void ExampleProcessor::CheckEvt(DEvent* evt) {
    cout<<"Check!"<<endl;

}

void ExampleProcessor::End() {
    cout<<"End!"<<endl;

}


