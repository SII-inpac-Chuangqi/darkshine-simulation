//
// Created by Joseph Zhang on 10/26/23.
//

#ifndef DSIMU_TRUTHHITPROCESSOR_H
#define DSIMU_TRUTHHITPROCESSOR_H

#ifndef RETURN
#define RETURN std::nan("RETURN")
#endif

//................................................................................//
//C++
#include <utility>

//................................................................................//
//FRAMEWORK
#include "Core/AnaProcessor.h"

//................................................................................//
//TRACKING
#include "Algo/TrkHit.h"
#include "Algo/DTrack.h"
#include "Algo/Vertex/DVertex.h"
#include "Algo/Digitization.h"

using namespace std;

class TruthHitProcessor : public AnaProcessor
{
public:
    explicit TruthHitProcessor(string name, shared_ptr<EventStoreAndWriter> evtwrt);

    virtual ~TruthHitProcessor() {};

    void Begin() override;
//................................................................................//
//Initialize all vars to be stored in output files
    void InitEvt() override;
//................................................................................//
//Fill truth variables
    void FillTruth(DTruth *truth_info, std::vector<DStep*> *stepIni,
                   std::vector<TrkHit> rawTagTrk2Hits, std::vector<TrkHit> rawRecTrk2Hits);

    void ProcessEvt(AnaEvent* evt) override;

    void CheckEvt(AnaEvent* evt) override;

    void End() override;

private:

//................................................................................//
//Parameters from config file
//................................................................................//
//Strip structure
//-- 0: False
//-- 1: True, automatically load strip structures from geometry
    int if_strip{1};
//................................................................................//
//Smear
//-- 0: False
//-- 1: True, add smear in hit reconstruction
    int if_smear{1};
//................................................................................//
//Magnet
//Const magnet field value to be used in const magnet condition or handle exceptions
    double con_field{-1.5};

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
//Momentum entering Tracker from Initial Particle Step
//Momentum exiting  Tracker from Initial Particle Step
    double TagTrk2_pp_truth_ini{RETURN};
    double TagTrk2_pp_truth_fin{RETURN};
    double RecTrk2_pp_truth_ini{RETURN};
    double RecTrk2_pp_truth_fin{RETURN};

//x, y, z & energy deposition of truth hits
    std::vector<double> TagTrk2_truth_hit_x{};
    std::vector<double> TagTrk2_truth_hit_y{};
    std::vector<double> TagTrk2_truth_hit_z{};
    std::vector<double> TagTrk2_truth_hit_e{};

    std::vector<double> RecTrk2_truth_hit_x{};
    std::vector<double> RecTrk2_truth_hit_y{};
    std::vector<double> RecTrk2_truth_hit_z{};
    std::vector<double> RecTrk2_truth_hit_e{};

};


#endif //DSIMU_TRUTHHITPROCESSOR_H
