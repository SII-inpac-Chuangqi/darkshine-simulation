#ifndef TRACKINGPROCESSOR_H
#define TRACKINGPROCESSOR_H

//................................................................................//
//CPP STL
#include <utility>

//................................................................................//
//FRAMEWORK
#include "Core/AnaProcessor.h"

//................................................................................//
//TRACKING
#include "Algo/TrkHit.h"

using namespace std;

class TrackingProcessor : public AnaProcessor
{
public:
    // Must initialized with Name
    explicit TrackingProcessor(string name, shared_ptr<EventStoreAndWriter> evtwrt);
    ~TrackingProcessor() {};

    void Begin() override;

    void ProcessEvt(AnaEvent* evt) override;

    void CheckEvt(AnaEvent* evt) override;

    void End() override;

private:

//................................................................................//
//Parameters from config file
//................................................................................//
    int clean = 0;
    double RecTrk_B = 1.5;

//................................................................................//
//Truth
//................................................................................//
    int TagTrk2_No = -1;
    double TagTrk2_pp_truth_ini = -999.;
    double TagTrk2_pp_truth_fin = -999.;
    double TagTrk2_x[250] = {-999.};
    double TagTrk2_y[250] = {-999.};
    double TagTrk2_z[250] = {-999.};
    double TagTrk2_e[250] = {-999.};
    
    int RecTrk2_No = -1;
    double RecTrk2_pp_truth_ini = -999.;
    double RecTrk2_pp_truth_fin = -999.;
    double RecTrk2_x[500] = {-999.};
    double RecTrk2_y[500] = {-999.};
    double RecTrk2_z[500] = {-999.};
    double RecTrk2_e[500] = {-999.};

//................................................................................//
//Reconstructed
//................................................................................//
    int TagTrk2_track_No = -1;
    int TagTrk2_rechit_No = -1;
    int TagTrk2_rectrk_hit_No[250] = {-1};
    double TagTrk2_pp[250] = {-999.};
    double TagTrk2_pl[250] = {-999.};
    double TagTrk2_track_r[250] = {-999.};
    double TagTrk2_track_xcenter[250] = {-999999.};
    double TagTrk2_track_zcenter[250] = {-999999.};

    double TagTrk2_track_chi2[250] = {-999.};
    double TagTrk2_track_x_sigma[250] = {-999.};
    double TagTrk2_track_y_sigma[250] = {-999.};

    double TagTrk2_track_x[250] = {-999.};
    double TagTrk2_track_y[250] = {-999.};
    double TagTrk2_track_z[250] = {-999.};

    double TagTrk2_pp_pre[250] = {-999.};
    double TagTrk2_goodness_pre[250] = {-999.};

//................................................................................//
    int RecTrk2_track_No = -1;
    int RecTrk2_rechit_No = -1;
    int RecTrk2_rectrk_hit_No[250] = {-1};
    double RecTrk2_pp[250] = {-999.};
    double RecTrk2_pl[250] = {-999.};
    double RecTrk2_track_r[250] = {-999.};
    double RecTrk2_track_xcenter[250] = {-999999.};
    double RecTrk2_track_zcenter[250] = {-999999.};

    double RecTrk2_track_chi2[250] = {-999.};
    double RecTrk2_track_x_sigma[250] = {-999.};
    double RecTrk2_track_y_sigma[250] = {-999.};

    double RecTrk2_track_x[250] = {-999.};
    double RecTrk2_track_y[250] = {-999.};
    double RecTrk2_track_z[250] = {-999.};

    double RecTrk2_pp_pre[250] = {-999.};
    double RecTrk2_goodness_pre[250] = {-999.};
};


#endif
