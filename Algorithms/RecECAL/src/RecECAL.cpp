//
// Created by Zhang Yulei on 9/19/20.
//

#include "Algo/RecECAL.h"

#include <cmath>
#include <iostream>
#include <iomanip>

void RecECAL::Begin() {

    // Add description for this AnaProcessor
    Description = "ECAL Reconstruction Processor";

    // Register parameters
    RegisterIntParameter("Verbose", "Verbosity Variable", &verbose, 0);
    RegisterDoubleParameter("W0", "W0", &W0, 0.);

    // Register Output Variable
    EvtWrt->RegisterIntVariable("FindCenter", &FindCenter, "FindCenter/I");
    EvtWrt->RegisterDoubleVariable("center_x", &center_x, "center_x/D");
    EvtWrt->RegisterDoubleVariable("center_y", &center_y, "center_y/D");
    EvtWrt->RegisterDoubleVariable("err_x", &err_x, "err_x/D");
    EvtWrt->RegisterDoubleVariable("err_y", &err_y, "err_y/D");

}

void RecECAL::ProcessEvt(AnaEvent *evt) {
    // Initialization
    initialization();

    // Get Simulated Hits for the current event
    const auto &HitCollection = evt->getSimulatedHitCollection();
    const auto &StepCollection = evt->getStepCollection();

    // define the collection name (RawMCParticle) to find.
    std::string HitCollectionName = "ECAL_Center";
    std::string StepCollectionName = "Initial_Particle_Step";

    // IMPORTANT: check if the collection exists
    if (HitCollection.count(HitCollectionName) != 0
        && StepCollection.count(StepCollectionName) != 0) {
        const auto &hits = HitCollection.at(HitCollectionName);
        const auto &steps = StepCollection.at(StepCollectionName);

        //Find Center Hit
        SingleCenterFinding(hits, steps);
        if (verbose > 0) {
            std::cout << "-- # of hits in ECAL_Center: " << hits->size() << endl;
            std::cout << "-- Reconstructed Position: " << (FindCenter ? "Found" : "NOT Found") << endl;
            std::cout << fixed << setprecision(3) << right;
            std::cout << "-- MC X: " << setw(6) << mc_x << " [mm]" << std::endl;
            std::cout << "-- Reconstructed X: " << setw(6) << center_x << " +- " << setw(6) << err_x << " [mm]"
                      << std::endl;
            std::cout << "-- MC Y: " << setw(6) << mc_y << " [mm]" << std::endl;
            std::cout << "-- Reconstructed Y: " << setw(6) << center_y << " +- " << setw(6) << err_y << " [mm]"
                      << std::endl;
        }
    } else {
        // if not exists, print out error
        cerr << "MCCollection not found" << endl;
    }
}

void RecECAL::CheckEvt(AnaEvent *evt) {
    //cout<<"Check!"<<endl;
}

void RecECAL::End() {
    //cout<<"End!"<<endl;
}

double RecECAL::SingleCenterFinding(const SimulatedHitVecUniPtr &hits, const DStepVecUniPtr &steps) {

    // Calculate total Energy
    double E_sum = 0.;
    for (auto hit : *hits) E_sum += hit->getE();

    // Calculate weight sum
    double weight_sum = 0.;
    for (auto hit : *hits) weight_sum += max(0., W0 + log(hit->getEdepEm() / E_sum));

    // Calculate weighted center x,y
    for (auto hit : *hits) center_x += max(0., W0 + log(hit->getEdepEm() / E_sum)) * hit->getX() / weight_sum;
    for (auto hit : *hits) center_y += max(0., W0 + log(hit->getEdepEm() / E_sum)) * hit->getY() / weight_sum;

    // Return if no reconstructed center
    if (isnan(center_x) || isnan(center_y)) return -999.;

    // Calculate Error with truth x,y
    if (steps->size() >= 3) {
        for (auto step = steps->begin() + 1; step != steps->end() - 1; step++) {
            if ((*step)->getPVName() == std::string("ECAL")) {
                mc_x = (*step)->getX();
                mc_y = (*step)->getY();

                break;
            }
        }
        err_x = center_x - mc_x;
        err_y = center_y - mc_y;

        FindCenter = 1;
    } else {
        cerr << "[WARNING] ==> Not enough step points." << endl;
        return -999.;
    }
    return 0.;
}




