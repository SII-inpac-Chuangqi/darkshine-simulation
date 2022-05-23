//
// Created by Zhang Yulei on 9/19/20.
//

#include "Algo/Digitizer.h"

#include <utility>

Digitizer::Digitizer(string name, shared_ptr<EventStoreAndWriter> evtwrt) : AnaProcessor(std::move(name),
                                                                                         std::move(evtwrt)) {
    // Add description for this AnaProcessor
    Description = "Digitizer for Calorimeter(ECAL) with optical process";

    // Register Fast Smearing
    fs = shared_ptr<FastSmear>(new FastSmear(evtwrt));
    // Register Parameters for FastSmear
    RegisterIntParameter("RandomSeed", "Random seed for smearing", &fs_rnd_seed, 0);
    RegisterIntParameter("ApplytoECAL", "Smearing on ECAL", &fs_apply_to_ecal, true);
    RegisterIntParameter("ApplytoHCAL", "Smearing on HCAL", &fs_apply_to_hcal, true);
    RegisterStringParameter("CalibrationFile", "YAML file for reference", &fs_config_file, "");

    // Register Double parameter
    RegisterDoubleParameter("Calibration_Factor", "Calibration Factor", &scale_factor,
                            1.); //this is for post-calibration
    RegisterDoubleParameter("Nominal_Yield", "Nominal Yield of material, like 20000/MeV for LYSO", &nominal_yield,
                            20000.);
    RegisterDoubleParameter("voltageToADC", "voltageToADC: fullRangeMV/ADCbits", &voltageToADC,
                            5000. / 4096); //this is for digitization study
    RegisterIntParameter("rangeMin", "rangeMin", &rangeMin, -2047);
    RegisterIntParameter("rangeMax", "rangeMax", &rangeMax, 2048);
    RegisterIntParameter("pedestal", "pedestal", &pedestal, 0);

    if (EvtWrt) {
        // EvtWrt->RegisterDoubleVariable("Digitized_Signal", &digitized_total_energy,
        //                                "Digitized_Signal/D"); //sum up for all ECAL
        // EvtWrt->RegisterDoubleVariable("Digitized_Signal_digitized", &digitized_total_energy_digitized,
        //                                "Digitized_Signal_digitized/D");
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

    fs->RegisterParameters(fs_config_file, fs_apply_to_ecal, fs_apply_to_hcal, fs_rnd_seed);
}

void Digitizer::ProcessEvt(AnaEvent *evt) {

    // Get MCCollections for the current event
    DigiFormMap OpticalCollection;
    if (!evt->getOpticalCollection().empty())
        OpticalCollection = evt->getOpticalCollection();

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
            double SF = scale_factor / (nominal_yield * YieldFactor);
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
            // hit->setE(energy); 
            //disable digitization for debug
            hit->setE(No);
            hit->setCellId(itr->GetDetID());
            DigitizedCollection->push_back(hit);
        }
    } else {
        // if not exists, print out error
        //cerr << "OpticalCollection not found" << endl;
    }

    // For FastSmear
    if (fs_apply_to_ecal) fs->Process(evt, "ECAL", "ECAL");
    if (fs_apply_to_hcal) {
        fs->Process(evt,"HCAL", "HCAL");
//        for (int i = 1; i <= 1; ++i) {
//            fs->Process(evt, "HCAL_" + to_string(i), "HCAL");
//        }
    }
}

void Digitizer::CheckEvt(AnaEvent* /*evt*/) {
    //cout<<"Check!"<<endl;

}

void Digitizer::End() {
    //cout<<"End!"<<endl;

}


