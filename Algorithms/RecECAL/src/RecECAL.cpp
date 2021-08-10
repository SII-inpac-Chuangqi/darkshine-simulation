//
// Created by Zhang Yulei on 9/19/20.
//

#include "Algo/RecECAL.h"

#include <cmath>
#include <iostream>
#include <iomanip>
#include <utility>
#include <sstream>

RecECAL::RecECAL(string name, shared_ptr<EventStoreAndWriter> evtwrt) : AnaProcessor(std::move(name),
                                                                                     std::move(evtwrt)) {

    // Add description for this AnaProcessor
    Description = "ECAL Reconstruction Processor";

    // Register parameters
    RegisterIntParameter("Verbose", "Verbosity Variable", &verbose, 0);
    RegisterStringParameter("ECollectionToUse", "Calorimeter (ECAL) Collection to Use", &ecal_col_use, "ECAL_FS0");
    RegisterStringParameter("HCollectionToUse", "Calorimeter (HCAL) Collection to Use", &hcal_col_use, "FS0");
    RegisterIntParameter("E_n_fraction", "the n-th large E fraction", &n_fraction, 20);

}

void RecECAL::Begin() {

    ReadCollections();
    ecal_col_size = static_cast<int>(ecal_cols.size());
    hcal_col_size = static_cast<int>(hcal_cols.size());

    // Register Output Variable
    if (EvtWrt) {
        EvtWrt->RegisterIntVariable("ECAL_COL_SIZE", &ecal_col_size, "ECAL_COL_SIZE/I");
//        EvtWrt->RegisterDoubleVariable("ECAL_E_total", &E_total, "ECAL_E_total/D");
//        EvtWrt->RegisterDoubleVariable("ECAL_E_max", &E_max, "ECAL_E_max/D");
//        EvtWrt->RegisterDoubleVariable("ECAL_E_frac", &E_frac, "ECAL_E_frac/D");
//        EvtWrt->RegisterDoubleVariable("ECAL_Moment_Lat", &Moments_Lat, "ECAL_Moment_Lat/D");
//        EvtWrt->RegisterDoubleVariable("ECAL_E_frac_vec", E_frac_vec, "ECAL_E_frac_vec[8]/D");
//        EvtWrt->RegisterDoubleVariable("ECAL_Moment_R", Moments_R, "ECAL_Moment_R[4]/D");
//        EvtWrt->RegisterDoubleVariable("ECAL_Moment_X", Moments_X, "ECAL_Moment_X[4]/D");
//        EvtWrt->RegisterDoubleVariable("ECAL_Moment_Y", Moments_Y, "ECAL_Moment_Y[4]/D");
//        EvtWrt->RegisterDoubleVariable("ECAL_Moment_Z", Moments_Z, "ECAL_Moment_Z[4]/D");
//
//        EvtWrt->RegisterDoubleVariable("HCAL_E_total", &HCAL_total, "HCAL_E_total/D");
//        EvtWrt->RegisterDoubleVariable("HCAL_E_Max_Cell", &HCAL_E_Max_Cell, "HCAL_E_Max_Cell/D");

        EvtWrt->RegisterOutVariable("ECAL_E_total", &E_total);
        EvtWrt->RegisterOutVariable("ECAL_E_max", &E_max);
        EvtWrt->RegisterOutVariable("ECAL_E_frac", &E_frac);
        EvtWrt->RegisterOutVariable("ECAL_Moment_Lat", &Moments_Lat);
        EvtWrt->RegisterOutVariable("ECAL_E_frac_vec", &E_frac_vec);
        EvtWrt->RegisterOutVariable("ECAL_Moment_R", &Moments_R);
        EvtWrt->RegisterOutVariable("ECAL_Moment_X", &Moments_X);
        EvtWrt->RegisterOutVariable("ECAL_Moment_Y", &Moments_Y);
        EvtWrt->RegisterOutVariable("ECAL_Moment_Z", &Moments_Z);

        EvtWrt->RegisterIntVariable("HCAL_COL_SIZE", &hcal_col_size, "HCAL_COL_SIZE/I");
        EvtWrt->RegisterOutVariable("HCAL_E_total", &HCAL_total);
        EvtWrt->RegisterOutVariable("HCAL_E_Max_Cell", &HCAL_E_Max_Cell);
    }
}

void RecECAL::ProcessEvt(AnaEvent *evt) {
    // Initialization
    initialization();

    // Get Simulated Hits for the current event
    //const auto &HitCollection = evt->getSimulatedHitCollection();
    const auto &HitCollection = evt->getCalorimeterHitCollection();

    for (const auto &HitCollectionName: ecal_cols) {
        // define the collection name (RawMCParticle) to find.
        // IMPORTANT: check if the collection exists
        if (HitCollection.count(HitCollectionName) != 0) {
            const auto &hits = HitCollection.at(HitCollectionName);
            // Calculate some cluster parameters ( moments...)
            auto cluster_ana = std::shared_ptr<Cluster_Analysis>(new Cluster_Analysis(hits));
            E_total.push_back(cluster_ana->FindETotal());
            E_max.push_back(cluster_ana->FindMaxEHit()->getE());
            E_frac.push_back(cluster_ana->FindEFraction(n_fraction));
            Moments_Lat.push_back(cluster_ana->FindLatMoment());

            auto temp_v = vector<double>();
            int j = 0;
            for (auto i : {5, 10, 20, 50, 100, 125, 150, 200}) {
                temp_v.push_back(cluster_ana->FindEFraction(i));
                j++;
            }
            E_frac_vec.push_back(temp_v);

            temp_v.clear();
            for (unsigned i = 1; i <= 4; ++i) temp_v.push_back(cluster_ana->FindMoment(i, 0, true));
            Moments_R.push_back(temp_v);

            temp_v.clear();
            for (unsigned i = 1; i <= 4; ++i) temp_v.push_back(cluster_ana->FindMoment(i, 1, true));
            Moments_X.push_back(temp_v);

            temp_v.clear();
            for (unsigned i = 1; i <= 4; ++i) temp_v.push_back(cluster_ana->FindMoment(i, 2, true));
            Moments_Y.push_back(temp_v);

            temp_v.clear();
            for (unsigned i = 1; i <= 4; ++i) temp_v.push_back(cluster_ana->FindMoment(i, 3, true));
            Moments_Z.push_back(temp_v);

        } else {
            // if not exists, print out error
            cerr << HitCollectionName << " not found" << endl;
        }
    }

    for (const auto &HCAL_Collection_Name: hcal_cols) {
        // temporary HCAL Analyzer
        double HCAL_E = 0;
        double HCAL_E_Max_cell = 0;
        for (int i = 0; i < 16; i++) {
            auto Collection_String = Form("HCAL_%d_%s", i, HCAL_Collection_Name.c_str());
            if (HitCollection.count(Collection_String) != 0) {
                const auto &hits = HitCollection.at(Collection_String);

                for (auto hit : *hits) {
                    HCAL_E += hit->getE();
                    HCAL_E_Max_cell = (HCAL_E_Max_cell >= hit->getE()) ? HCAL_E_Max_cell : hit->getE();
                }
            }
        }
        HCAL_total.push_back(HCAL_E);
        HCAL_E_Max_Cell.push_back(HCAL_E_Max_cell);
    }
}

void RecECAL::CheckEvt(AnaEvent *evt) {
    //cout<<"Check!"<<endl;
}

void RecECAL::End() {
    //cout<<"End!"<<endl;
}

void RecECAL::ReadCollections() {

    auto format_str = [](const string &str, vector<string> &cols) {
        stringstream s_stream(str); //create string stream from the string
        while (s_stream.good()) {
            string substr;
            getline(s_stream, substr, ','); //get first string delimited by comma


            substr.erase(remove(substr.begin(), substr.end(), ' '), substr.end());
            cols.push_back(substr);
        }
    };

    format_str(ecal_col_use, ecal_cols);
    format_str(hcal_col_use, hcal_cols);
}


