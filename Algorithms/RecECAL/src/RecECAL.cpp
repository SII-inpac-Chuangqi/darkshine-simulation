//
// Created by Zhang Yulei on 9/19/20.
//

#include "Algo/RecECAL.h"

#include <cmath>
#include <iostream>
#include <iomanip>
#include <utility>

#include "TChain.h"

RecECAL::RecECAL(string name, shared_ptr<EventStoreAndWriter> evtwrt) : AnaProcessor(std::move(name),
                                                                                     std::move(evtwrt)) {

    ECAL_TF = std::shared_ptr<Trk_LineFit>(new Trk_LineFit());
    ECAL_Wrt = std::shared_ptr<ECAL_Writer>(new ECAL_Writer());
    ECAL_rnn = std::shared_ptr<ECAL_RNN>(new ECAL_RNN());

    // Register parameters
    RegisterIntParameter("Verbose", "Verbosity Variable", &verbose, 0);
    RegisterDoubleParameter("W0", "W0", &W0, 0.);
    RegisterIntParameter("Channels", "Nb of Channels", &nb_ch, 1);
    RegisterStringParameter("RNN_Status", "Train or Apply", &RNN_Status, "none");
    RegisterStringParameter("RNN_Path", "Weight xml path", &RNN_Path, "none");
    RegisterStringParameter("RNN_Sig_Path", "Signal File path", &RNN_Sig_Path, "none");
    RegisterStringParameter("RNN_Bkg_Path", "Background File path", &RNN_Bkg_Path, "none");

}

void RecECAL::Begin() {

    // Add description for this AnaProcessor
    Description = "ECAL Reconstruction Processor";

    // Register Output Variable
    EvtWrt->RegisterIntVariable("FindCenter", &FindCenter, "FindCenter/I");
    EvtWrt->RegisterDoubleVariable("center_x", &center_x, "center_x/D");
    EvtWrt->RegisterDoubleVariable("center_y", &center_y, "center_y/D");
    EvtWrt->RegisterDoubleVariable("err_x", &err_x, "err_x/D");
    EvtWrt->RegisterDoubleVariable("err_y", &err_y, "err_y/D");

    // For Zhenting He
    //EvtWrt->RegisterDoubleVariable("ECAL_Hits", Hits_E, "ECAL_Hits[400]/D");

    // For DNN Training and Application
    if (RNN_Status == "train") {
        cout << "==> Apply training on RNN..." << endl;
    } else if (RNN_Status == "apply") {
        EvtWrt->RegisterDoubleVariable("RNN_Score", &RNN_Score, "RNN_Score/D");
        ECAL_rnn->LoadModel(nb_ch, RNN_Path, "dp_DNN");
    }
    ECAL_Wrt->BookTree("ECAL_Hits.root", "dp", nb_ch);
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

        // Find Center Hit
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

        // RNN
        if (RNN_Status == "apply")
            RNN_Score = ECAL_rnn->ApplyDNN(hits);

        ECAL_Wrt->FillHits(hits);

        // Find Trackers in ECAL
        for (auto hit: *hits) {
            double x = hit->getX();
            double y = hit->getY();
            double z = hit->getZ();

            int cell_id = hit->getCellId();

            //Hits_E[cell_id-1] = ( hit->getE() > 1e-6 && !isnan(hit->getE()) ) ? hit->getE() : 0. ;

            ECAL_TF->AddPoint(x, y, z);
        }

        std::pair<V3, V3> result = ECAL_TF->best_line_from_points();
        //std::cout << "origin:\n" << result.first << "\naxis:\n" << result.second;
    } else {
        // if not exists, print out error
        cerr << "MCCollection not found" << endl;
    }
}

void RecECAL::CheckEvt(AnaEvent *evt) {
    //cout<<"Check!"<<endl;
}

void RecECAL::End() {
    // Training for RNN
    if (RNN_Status == "train") {
        //Read Sig and Bkg TChain
        auto sig = new TChain("dp");
        sig->Add(RNN_Sig_Path.data());
        auto bkg = new TChain("dp");
        bkg->Add(RNN_Sig_Path.data());

        ECAL_rnn->TrainDNN(sig, bkg, nb_ch, "dp_DNN");
    }

    ECAL_Wrt->SaveTree();
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
            if ((*step)->getPVName().find(std::string("ECAL_Center_PVW")) != std::string::npos) {
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




