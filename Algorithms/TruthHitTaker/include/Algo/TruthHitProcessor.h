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
//ROOT
#include "TString.h"
#include <TFile.h>
#include <TTree.h>
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
    void FillTruth(DTruth *truth_info,
                   std::vector<TrkHit> rawRecTrk1Hits,
                   std::vector<TrkHit> rawRecTrk2Hits);

    void ProcessEvt(AnaEvent* evt) override;

    void CheckEvt(AnaEvent* evt) override;

    void End() override;

    TFile* outputFile;
    TTree* outputTree;

private:

//................................................................................//
//Parameters from config file
//................................................................................//
//Strip structure
//-- 0: False
//-- 1: True, automatically load strip structures from geometry
    int if_strip{1};
//................................................................................//
//Geometry
//................................................................................//
    Digitization digitizer;
//................................................................................//
//................................................................................//
//Truth
//................................................................................//
//    double TagTrk2_pp_truth_x{RETURN};
//    double TagTrk2_pp_truth_y{RETURN};
//    double TagTrk2_pp_truth_z{RETURN};
//    double TagTrk2_pp_truth_e{RETURN};

//x, y, z & energy deposition of truth hits
//    double TagTrk2_truth_hit_x{RETURN};
//    double TagTrk2_truth_hit_y{RETURN};
//    double TagTrk2_truth_hit_z{RETURN};
//    double TagTrk2_truth_hit_t{RETURN};

    double RecTrk2_truth_hit_x{RETURN};
    double RecTrk2_truth_hit_y{RETURN};
    double RecTrk2_truth_hit_z{RETURN};
    double RecTrk2_truth_hit_t{RETURN};

    double RecTrk2_pp_truth_x{RETURN};
    double RecTrk2_pp_truth_y{RETURN};
    double RecTrk2_pp_truth_z{RETURN};
    double RecTrk2_pp_truth_e{RETURN};

    double deltapx{RETURN};
    double deltapy{RETURN};
    double deltapz{RETURN};
    double deltae {RETURN};

    int geometry_id{-1};
    int particle_id{-1};
    int event_id{-1};


};


#endif //DSIMU_TRUTHHITPROCESSOR_H
