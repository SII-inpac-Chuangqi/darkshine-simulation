//
// Created by Joseph Zhang on 10/31/23.
//

#ifndef DSIMU_TRUTHPARTICLEPROCESSOR_H
#define DSIMU_TRUTHPARTICLEPROCESSOR_H

#ifndef RETURN
#define RETURN std::nan("RETURN")
#endif

//................................................................................//
//C++
#include <utility>
#include <ostream>

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

class TruthParticleProcessor : public AnaProcessor
{
public:
    explicit TruthParticleProcessor(string name, shared_ptr<EventStoreAndWriter> evtwrt);

    virtual ~TruthParticleProcessor() {};

    void Begin() override;
//................................................................................//
//Initialize all vars to be stored in output files
    void InitEvt() override;
//................................................................................//
//Fill truth variables
    void FillTruth(DTruth *truth_info,
                   std::vector<DStep *> *initial_steps,
                   std::vector<McParticle *> *raw_mc_ptl);
    void ProcessEvt(AnaEvent* evt) override;
    void CheckEvt(AnaEvent* evt) override;
    void End() override;



private:
//................................................................................//
//New root file
//................................................................................//
    TFile* outputFile;
    TTree* outputTree;
//................................................................................//
//Truth
//................................................................................//
//particle identifiers
    int event_id{-1};
    std::vector<unsigned long> particle_id{};
    std::vector<int> particle_type{};   //PDG id
    vector<unsigned int> process{};
//initial vertexes space-time coordinates
    std::vector<float> vx{};
    std::vector<float> vy{};
    std::vector<float> vz{};
    std::vector<float> vt{};
//3-momentum and mass
    std::vector<float> px{};
    std::vector<float> py{};
    std::vector<float> pz{};
    std::vector<float> m{};
//tracking parameters
    std::vector<float> q{};
    std::vector<float> p{};
    std::vector<float> pt{};
//    std::vector<float> eta{};
//    std::vector<float> phy{};

};


#endif //DSIMU_TRUTHPARTICLEPROCESSOR_H
