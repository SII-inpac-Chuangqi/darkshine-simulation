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
#include "Algo/Digitization.h"

using namespace std;

class TrackingProcessor : public AnaProcessor
{
public:
    // Must initialized with Name
    explicit TrackingProcessor(string name, shared_ptr<EventStoreAndWriter> evtwrt);
    ~TrackingProcessor() {};

    void Begin() override;

    void CleanEvt();

    void FillTruth(std::vector<DStep*> *stepIni,
                   std::vector<TrkHit> rawTagTrk2Hits, std::vector<TrkHit> rawRecTrk2Hits);

    void ProcessEvt(AnaEvent* evt) override;

    void CheckEvt(AnaEvent* evt) override;

    void End() override;

private:

//................................................................................//
//Parameters from config file
//................................................................................//
    int clean = 0;
    int Tag_fit_method{-1};
    int Rec_fit_method{-1};

//................................................................................//
//Geometry
//................................................................................//
    Digitization digitizer;

//................................................................................//
//Magnet
//................................................................................//
    std::vector<DMagnet *> magnets;

//................................................................................//
//Truth
//................................................................................//
    int TagTrk2_No{-1};
    double TagTrk2_pp_truth_ini{RETURN};
    double TagTrk2_pp_truth_fin{RETURN};
    std::vector<double> TagTrk2_x{};
    std::vector<double> TagTrk2_y{};
    std::vector<double> TagTrk2_z{};
    std::vector<double> TagTrk2_e{};
    
    int RecTrk2_No{-1};
    double RecTrk2_pp_truth_ini{RETURN};
    double RecTrk2_pp_truth_fin{RETURN};
    std::vector<double> RecTrk2_x{};
    std::vector<double> RecTrk2_y{};
    std::vector<double> RecTrk2_z{};
    std::vector<double> RecTrk2_e{};

//................................................................................//
//Reconstructed
//................................................................................//
    int TagTrk2_track_No{-1};
    int TagTrk2_rechit_No{-1};
    std::vector<int> TagTrk2_rectrk_hit_No{};
    std::vector<double> TagTrk2_pp{};
    std::vector<double> TagTrk2_pl{};
    std::vector<double> TagTrk2_track_r{};
    std::vector<double> TagTrk2_track_xcenter{};
    std::vector<double> TagTrk2_track_zcenter{};

    std::vector<double> TagTrk2_track_quality{};
    std::vector<double> TagTrk2_track_chi2{};
    std::vector<double> TagTrk2_track_x_sigma{};
    std::vector<double> TagTrk2_track_y_sigma{};

    std::vector<double> TagTrk2_track_x{};
    std::vector<double> TagTrk2_track_y{};
    std::vector<double> TagTrk2_track_z{};

//................................................................................//
    int RecTrk2_track_No{-1};
    int RecTrk2_rechit_No{-1};
    std::vector<int> RecTrk2_rectrk_hit_No{};
    std::vector<double> RecTrk2_pp{};
    std::vector<double> RecTrk2_pl{};
    std::vector<double> RecTrk2_track_r{};
    std::vector<double> RecTrk2_track_xcenter{};
    std::vector<double> RecTrk2_track_zcenter{};

    std::vector<double> RecTrk2_track_quality{};
    std::vector<double> RecTrk2_track_chi2{};
    std::vector<double> RecTrk2_track_x_sigma{};
    std::vector<double> RecTrk2_track_y_sigma{};

    std::vector<double> RecTrk2_track_x{};
    std::vector<double> RecTrk2_track_y{};
    std::vector<double> RecTrk2_track_z{};
};


#endif
#endif
