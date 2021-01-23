//
// Created by Zhang Yulei on 9/19/20.
//

#include "Algo/RecECAL.h"

#include <cmath>
#include <iostream>
#include <iomanip>
#include <utility>

RecECAL::RecECAL(string name, shared_ptr<EventStoreAndWriter> evtwrt) : AnaProcessor(std::move(name),
                                                                                     std::move(evtwrt)) {

    // Add description for this AnaProcessor
    Description = "ECAL Reconstruction Processor";

    // Register parameters
    RegisterIntParameter("Verbose", "Verbosity Variable", &verbose, 0);
    RegisterDoubleParameter("W0", "W0", &W0, 0.);
    RegisterDoubleParameter("d_cut", "Cluster: d_cut", &d_cut, 0.2);
    RegisterDoubleParameter("r_cut", "Cluster: r_cut", &r_cut, 0.5);
    //RegisterIntParameter("Z_Layers", "Nb of Z layers", &nb_z, 1);
    RegisterIntParameter("E_n_fraction", "the n-th large E fraction", &n_fraction, 20);

}

void RecECAL::Begin() {

    // Register Output Variable
    if (EvtWrt) {
        EvtWrt->RegisterDoubleVariable("ECAL_E_total", &E_total, "ECAL_E_total/D");
        EvtWrt->RegisterDoubleVariable("ECAL_E_max", &E_max, "ECAL_E_max/D");
        EvtWrt->RegisterDoubleVariable("ECAL_E_frac", &E_frac, "ECAL_E_frac/D");
        EvtWrt->RegisterDoubleVariable("ECAL_E_frac_vec", E_frac_vec, "ECAL_E_frac_vec[8]/D");
        EvtWrt->RegisterDoubleVariable("ECAL_Moment_R", Moments_R, "ECAL_Moment_R[4]/D");
        EvtWrt->RegisterDoubleVariable("ECAL_Moment_X", Moments_X, "ECAL_Moment_X[4]/D");
        EvtWrt->RegisterDoubleVariable("ECAL_Moment_Y", Moments_Y, "ECAL_Moment_Y[4]/D");
        EvtWrt->RegisterDoubleVariable("ECAL_Moment_Z", Moments_Z, "ECAL_Moment_Z[4]/D");
    }
}

void RecECAL::ProcessEvt(AnaEvent *evt) {
    // Initialization
    initialization();

    // Get Simulated Hits for the current event
    const auto &HitCollection = evt->getSimulatedHitCollection();

    // define the collection name (RawMCParticle) to find.
    std::string HitCollectionName = "ECAL_Center";

    // IMPORTANT: check if the collection exists
    if (HitCollection.count(HitCollectionName) != 0) {
        const auto& hits = HitCollection.at(HitCollectionName);
        // Calculate some cluster parameters ( moments...)
        auto cluster_ana = std::shared_ptr<Cluster_Analysis>(new Cluster_Analysis(hits));
        E_total = cluster_ana->FindETotal();
        E_max = cluster_ana->FindMaxEHit()->getE();
        E_frac = cluster_ana->FindEFraction(n_fraction);
        int j = 0;
        for (auto i : {5, 10, 20, 50, 100, 125, 150, 200}) {
            E_frac_vec[j] = cluster_ana->FindEFraction(i);
            j++;
        }
        for (unsigned i = 1; i <= 4; ++i) {
            Moments_R[i-1] = cluster_ana->FindMoment(i, 0, true);
            Moments_X[i-1] = cluster_ana->FindMoment(i, 1, true);
            Moments_Y[i-1] = cluster_ana->FindMoment(i, 2, true);
            Moments_Z[i-1] = cluster_ana->FindMoment(i, 3, true);
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



