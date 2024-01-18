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

    TTree* MakeTree();
//................................................................................//
//Initialize all vars to be stored in output files
    void InitEvt() override;
//................................................................................//
//Fill truth variables
    void FillTruth(DTruth *truth_info,
                   //std::vector<DStep*> *initial_steps,
                   //std::vector<McParticle *> *raw_mc_ptl,
                   //TFile *outputFile,
                   TTree *outputTree,
                   std::vector<TrkHit> rawTrk1Hits,
                   std::vector<TrkHit> rawTrk2Hits);
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
//Truth recording in some tracking region
//--0: False
//--1: True
    int if_tagging{0};
    int if_recoil{1};
//Geometry
//................................................................................//
    Digitization digitizer;
//................................................................................//
//Truth
//................................................................................//

    float truth_hit_x = 0;
    float truth_hit_y = 0;
    float truth_hit_z = 0;
    float truth_hit_t = 0;

    float pp_truth_x = 0;
    float pp_truth_y = 0;
    float pp_truth_z = 0;
    float pp_truth_e = 0;

    float deltapx = 0;
    float deltapy = 0;
    float deltapz = 0;
    float deltae  = 0;

    uint64_t geometry_id = 0;
    uint64_t particle_id = 0;
    uint32_t event_id = 0;
    int32_t index = 0;

    // Decoded hit surface identifier components.
    uint32_t volumeId = 0;
    uint32_t boundaryId = 0;
    uint32_t layerId = 0;
    uint32_t approachId = 0;
    uint32_t sensitiveId = 0;
//................................................................................//
//New root file
//................................................................................//
//    TFile* outputFile;
    TFile *fileT;
    TFile *fileR;
    TTree* outputTreeT;
    TTree* outputTreeR;

};

#endif //DSIMU_TRUTHHITPROCESSOR_H
