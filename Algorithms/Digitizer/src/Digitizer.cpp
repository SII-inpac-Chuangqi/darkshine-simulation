//
// Created by Zhang Yulei on 9/19/20.
//

#include "Algo/Digitizer.h"

#include <utility>


Digitizer::Digitizer(string name, shared_ptr<EventStoreAndWriter> evtwrt) : AnaProcessor(std::move(name),
                                                                                         std::move(evtwrt)) {
    // Add description for this AnaProcessor
    Description = "Digitizer for Calorimeter(ECAL) with optical process";

    // Register Double parameter
    RegisterDoubleParameter("Calibration_Factor", "Calibration_Factor", &scale_factor,
                            1.); //this is for post-calibration
    RegisterDoubleParameter("voltageToADC", "voltageToADC: fullRangeMV/ADCbits", &voltageToADC,
                            5000. / 4096); //this is for digitization study
    RegisterIntParameter("rangeMin", "rangeMin", &rangeMin, -2047);
    RegisterIntParameter("rangeMax", "rangeMax", &rangeMax, 2048);
    RegisterIntParameter("pedestal", "pedestal", &pedestal, 0);

    if (EvtWrt) {
        EvtWrt->RegisterDoubleVariable("Digitized_Signal", &digitized_total_energy,
                                       "Digitized_Signal/D"); //sum up for all ECAL
        EvtWrt->RegisterDoubleVariable("Digitized_Signal_digitized", &digitized_total_energy_digitized,
                                       "Digitized_Signal_digitized/D");
        EvtWrt->RegisterIntVariable("Digitized_Signal_No", &digitized_total_No, "Digitized_Signal_No/I");
        EvtWrt->RegisterIntVariable("Digitized_Signal_NoGen", &digitized_total_NoGen, "Digitized_Signal_NoGen/I");
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

        auto DigitizedCollection = evt->RegisterCalorimeterHitCollection(CollectionName);

        digitized_total_energy = 0.;
        digitized_total_energy_digitized = 0.;
        digitized_total_No = 0;
        digitized_total_NoGen = 0;
        // Loop the collection to print out the information of each particle
        for (auto itr : *optical) {
            //get yieldFactor to recover full yield
            const double YieldFactor = itr->GetYieldFactor();
            double SF = scale_factor * YieldFactor;
            //integral as energy
            double energy = itr->GetIntegral(false) * SF; //half-dgitized
            digitized_total_energy += energy;
            //re-digitization with setting parameter
            itr->SetVoltageToADC(voltageToADC);
            itr->SetRangeMin(rangeMin);
            itr->SetRangeMax(rangeMax);
            itr->SetPedestal(pedestal);
            double energy_digit = itr->GetIntegral(true) * SF;
            digitized_total_energy_digitized += energy_digit;
            //Hit number as truth energy
            int No = static_cast<int>(itr->GetOpticalNo(false) * SF);
            int NoGen = static_cast<int>(itr->GetOpticalNo(true) * SF);
            digitized_total_No += No;
            digitized_total_NoGen += NoGen;

            //covert to calorimeter hit
            auto hit = new CalorimeterHit();
            hit->setE(energy);
            hit->setCellId(itr->GetDetID());
            DigitizedCollection->push_back(hit);
        }
    } else {
        // if not exists, print out error
        cerr << "OpticalCollection not found" << endl;
    }
}

void Digitizer::CheckEvt(AnaEvent *evt) {
    //cout<<"Check!"<<endl;

}

void Digitizer::End() {
    //cout<<"End!"<<endl;

}


