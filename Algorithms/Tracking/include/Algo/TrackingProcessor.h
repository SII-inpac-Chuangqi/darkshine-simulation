#ifndef _OFF_TRACKING
#ifndef TRACKINGPROCESSOR_H
#define TRACKINGPROCESSOR_H

#ifndef RETURN
#define RETURN -INFINITY
#endif

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
    int Tag_fit_method = -1;
    int Rec_fit_method = -1;

//................................................................................//
//Magnet
//................................................................................//
    std::vector<DMagnet *> magnets;

//................................................................................//
//Truth
//................................................................................//
    int TagTrk2_No = -1;
    double TagTrk2_pp_truth_ini = RETURN;
    double TagTrk2_pp_truth_fin = RETURN;
    double TagTrk2_x[250] = {RETURN};
    double TagTrk2_y[250] = {RETURN};
    double TagTrk2_z[250] = {RETURN};
    double TagTrk2_e[250] = {RETURN};
    
    int RecTrk2_No = -1;
    double RecTrk2_pp_truth_ini = RETURN;
    double RecTrk2_pp_truth_fin = RETURN;
    double RecTrk2_x[500] = {RETURN};
    double RecTrk2_y[500] = {RETURN};
    double RecTrk2_z[500] = {RETURN};
    double RecTrk2_e[500] = {RETURN};

//................................................................................//
//Reconstructed
//................................................................................//
    int TagTrk2_track_No = -1;
    int TagTrk2_rechit_No = -1;
    int TagTrk2_rectrk_hit_No[250] = {-1};
    double TagTrk2_pp[250] = {RETURN};
    double TagTrk2_pl[250] = {RETURN};
    double TagTrk2_track_r[250] = {RETURN};
    double TagTrk2_track_xcenter[250] = {RETURN};
    double TagTrk2_track_zcenter[250] = {RETURN};

    double TagTrk2_track_quality[250] = {RETURN};
    double TagTrk2_track_chi2[250] = {RETURN};
    double TagTrk2_track_x_sigma[250] = {RETURN};
    double TagTrk2_track_y_sigma[250] = {RETURN};

    double TagTrk2_track_x[250] = {RETURN};
    double TagTrk2_track_y[250] = {RETURN};
    double TagTrk2_track_z[250] = {RETURN};

//................................................................................//
    int RecTrk2_track_No = -1;
    int RecTrk2_rechit_No = -1;
    int RecTrk2_rectrk_hit_No[250] = {-1};
    double RecTrk2_pp[250] = {RETURN};
    double RecTrk2_pl[250] = {RETURN};
    double RecTrk2_track_r[250] = {RETURN};
    double RecTrk2_track_xcenter[250] = {RETURN};
    double RecTrk2_track_zcenter[250] = {RETURN};

    double RecTrk2_track_quality[250] = {RETURN};
    double RecTrk2_track_chi2[250] = {RETURN};
    double RecTrk2_track_x_sigma[250] = {RETURN};
    double RecTrk2_track_y_sigma[250] = {RETURN};

    double RecTrk2_track_x[250] = {RETURN};
    double RecTrk2_track_y[250] = {RETURN};
    double RecTrk2_track_z[250] = {RETURN};
};


#endif
#endif
