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
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

//................................................................................//
//FRAMEWORK
#include "Core/AnaProcessor.h"
#include "Utility/PhysicsProcessDef.h"

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
//Map process string name to number. Utility/UTIL/include/Utility/PhysicsProcessDef.h
    unsigned int mapStringToUint(const std::string& str) const;


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
    uint32_t event_id;
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
//tracking parameters
    std::vector<float> m{};

    std::vector<float> q{};
    std::vector<float> p{};
    std::vector<float> pt{};
    std::vector<float> eta{};
    std::vector<float> phi{};
//others
    vector<unsigned int> vertex_primary{};
    vector<unsigned int> vertex_secondary{};
    vector<unsigned int> particle{};
    vector<unsigned int> generation{};
    vector<unsigned int> sub_particle{};

//process object
    PhysicsDef physicsDef;
    std::unordered_map<std::string, unsigned int> stringToUintMap;

};


#endif //DSIMU_TRUTHPARTICLEPROCESSOR_H
