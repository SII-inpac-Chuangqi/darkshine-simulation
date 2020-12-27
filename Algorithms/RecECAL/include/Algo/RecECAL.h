//
// Created by Zhang Yulei on 9/19/20.
//

#ifndef DSIMU_RECECAL_H
#define DSIMU_RECECAL_H

#include <utility>

#include "Core/AnaProcessor.h"
//#include "Algo/Trk_LineFit.h"
//#include "Algo/ECAL_Writer.h"
//#include "Algo/ECAL_RNN.h"
#include "Algo/ECAL_Cluster.h"
#include "Algo/Cluster_Analysis.h"

using namespace std;

class RecECAL : public AnaProcessor {
public:
    // No need to change anything here
    // Must initialized with Name
    explicit RecECAL(string name, shared_ptr<EventStoreAndWriter> evtwrt);
    ~RecECAL() override = default;

    void Begin() override;

    void ProcessEvt(AnaEvent* evt) override;

    void CheckEvt(AnaEvent* evt) override;

    void End() override;

    // Define some functions here if necessary
    void initialization() {
        //ECAL_TF->Clean();
    }

private:
    // output variables
    double E_total{0.};
    double E_max{0.};
    double E_frac{0.};
    double Moments_R[4] = {0.};
    double Moments_X[4] = {0.};
    double Moments_Y[4] = {0.};
    double Moments_Z[4] = {0.};


    // Verbosity
    int verbose{0};
    // Input Parameter
    double W0{0.};
    int nb_z{1};
    double d_cut{1.};
    double r_cut{0.5};
    int n_fraction{1};


    // Internal Algorithm Processors
    shared_ptr<ECAL_Cluster> ECAL_cluster;
};


#endif //DSIMU_RECECAL_H
