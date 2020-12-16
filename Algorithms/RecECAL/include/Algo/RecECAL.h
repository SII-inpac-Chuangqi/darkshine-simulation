//
// Created by Zhang Yulei on 9/19/20.
//

#ifndef DSIMU_RECECAL_H
#define DSIMU_RECECAL_H

#include <utility>

#include "Core/AnaProcessor.h"
#include "Algo/Trk_LineFit.h"
#include "Algo/ECAL_Writer.h"
#include "Algo/ECAL_RNN.h"
#include "Algo/ECAL_Cluster.h"

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
        center_x = 0.;
        center_y = 0.;
        mc_x = 0.;
        mc_y = 0.;
        err_x = 0.;
        err_y = 0.;

        ECAL_TF->Clean();
    }

    double SingleCenterFinding(const SimulatedHitVecUniPtr&, const DStepVecUniPtr &);

private:

    int FindCenter{0};

    double center_x{0.};
    double center_y{0.};
    double mc_x{0.};
    double mc_y{0.};
    double err_x{0.};
    double err_y{0.};

    double RNN_Score{0.};

    // double Hits_E[400];
    // Verbosity
    int verbose{0};
    // Input Parameter
    double W0{0.};
    int nb_ch{1};
    int nb_z{1};
    string RNN_Status{};
    string RNN_Path{};
    string RNN_Sig_Path{};
    string RNN_Bkg_Path{};

    double d_cut{1.};
    double r_cut{0.5};

    // Internal Algorithm Processors
    //shared_ptr<Trk_LineFit> ECAL_TF;
    shared_ptr<ECAL_Writer> ECAL_Wrt;
    shared_ptr<ECAL_RNN>    ECAL_rnn;
    shared_ptr<ECAL_Cluster> ECAL_cluster;
};


#endif //DSIMU_RECECAL_H
