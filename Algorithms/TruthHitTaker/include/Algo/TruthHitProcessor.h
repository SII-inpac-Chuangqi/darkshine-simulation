#ifndef TRUTHHITPROCESSOR_H
#define TRUTHHITPROCESSOR_H

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
#include "Algo/Digitization.h"


using namespace std;

class TruthHitProcessor : public AnaProcessor{
public:
    // Must initialized with Name
    explicit TruthHitProcessor(string name, shared_ptr<EventStoreAndWriter> evtwrt);
    //~TruthHitProcessor() {};
    ~TruthHitProcessor() override = default;

    void Begin() override;
//................................................................................//
//Initialize all vars to be stored in output files
    void InitEvt() override;
//................................................................................//
//Fill truth variables
    void FillTruth(DTruth *truth_info, std::vector<TrkHit> rawTagTrk2Hits, std::vector<TrkHit> rawRecTrk2Hits);
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
//Geometry
//................................................................................//
    Digitization digitizer;
//................................................................................//
//Truth hit from DSimu
    std::vector<double> TagTrk2_simu_hit_x{};
    std::vector<double> TagTrk2_simu_hit_y{};
    std::vector<double> TagTrk2_simu_hit_z{};
    std::vector<double> RecTrk2_simu_hit_x{};
    std::vector<double> RecTrk2_simu_hit_y{};
    std::vector<double> RecTrk2_simu_hit_z{};
    //................................................................................//
//Truth hit after smeared
    std::vector<double> TagTrk2_smeared_hit_x{};
    std::vector<double> TagTrk2_smeared_hit_y{};
    std::vector<double> TagTrk2_smeared_hit_z{};
    std::vector<double> RecTrk2_smeared_hit_x{};
    std::vector<double> RecTrk2_smeared_hit_y{};
    std::vector<double> RecTrk2_smeared_hit_z{};

};
#endif
