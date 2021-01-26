//
// Created by Zhang Yulei on 9/19/20.
//

#include "Algo/Digitizer.h"

#include <utility>


Digitizer::Digitizer(string name, shared_ptr<EventStoreAndWriter> evtwrt) : AnaProcessor(std::move(name),
                                                                                         std::move(evtwrt)) {
    // Add description for this AnaProcessor
    Description = "Digitizer for Calorimeter with optical process";

    // Register Double parameter
    RegisterDoubleParameter("Scale_Factor", "Scale_Factor", &scale_factor, 1.);

    if (EvtWrt) {
        EvtWrt->RegisterDoubleVariable("Digitized_Signal", &digitized_total_energy, "Digitized_Signal/D");
    }
}

void Digitizer::Begin() {
    /*
     *
     *  DEFINE Processor explicitly with NAME in ControlManager.cpp L.50
     *  DEFINE Processor explicitly with NAME in ControlManager.cpp L.50
     *  DEFINE Processor explicitly with NAME in ControlManager.cpp L.50
     *
     */


}

void Digitizer::ProcessEvt(AnaEvent *evt) {

    // Get MCCollections for the current event
    const auto &OpticalCollection = evt->getOpticalCollection();

    std::string CollectionName = "ECAL";

    // IMPORTANT: check if the collection exists
    if (OpticalCollection.count(CollectionName) != 0) {
        const auto &optical = OpticalCollection.at(CollectionName);
        // if exists, then do something

        auto DigitizedCollection = evt->RegisterCalorimeterHitCollection(CollectionName);

        digitized_total_energy = 0.;
        // Loop the collection to print out the information of each particle
        for (auto itr : *optical) {
            auto hit = new CalorimeterHit();
            double energy = itr->GetIntegral(true) * scale_factor;
            hit->setE(energy);
            hit->setCellId(itr->GetDetID());

            DigitizedCollection->push_back(hit);

            digitized_total_energy += energy;
        }
    } else {
        // if not exists, print out error
        cerr << "MCCollection not found" << endl;
    }
}

void Digitizer::CheckEvt(AnaEvent *evt) {
    //cout<<"Check!"<<endl;

}

void Digitizer::End() {
    //cout<<"End!"<<endl;

}


