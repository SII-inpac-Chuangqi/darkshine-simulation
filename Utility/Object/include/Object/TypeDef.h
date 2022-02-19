//
// Created by Zhang Yulei on 9/19/20.
//

#ifndef DSIMU_TYPEDEF_H
#define DSIMU_TYPEDEF_H

#include "TBuffer.h"
#include <iostream>

#include <vector>
#include <map>
#include <unordered_map>
#include <iterator>
#include <memory>

class DStep_b1;

class McParticle_b1;

class ReconstructedParticle_b1;

class SimulatedHit;

class CalorimeterHit_b1;

//class OpticalHit;

class DigiForm_b1;

class AnaProcessor;

typedef std::vector<DStep_b1 *> DStepVec;
typedef std::map<std::string, DStepVec* > DStepMap;

typedef std::vector<McParticle_b1 *> MCParticleVec;
typedef std::map<std::string, MCParticleVec* > MCParticleMap;

typedef std::vector<ReconstructedParticle_b1 *> RecParticleVec;
typedef std::map<std::string, RecParticleVec* > RecParticleMap;

typedef std::vector<SimulatedHit *> SimulatedHitVec;
typedef std::map<std::string, SimulatedHitVec* > SimulatedHitMap;

typedef std::vector<CalorimeterHit_b1 *> CalorimeterHitVec;
typedef std::map<std::string, CalorimeterHitVec* > CalorimeterHitMap;

typedef std::vector<DigiForm_b1 *> DigiFormVec;
typedef std::map<std::string, DigiFormVec* > DigiFormMap;

typedef std::shared_ptr<AnaProcessor> AnaProcessorUniPtr;
typedef std::vector<AnaProcessorUniPtr> AnaProcessorVec;
typedef std::shared_ptr<AnaProcessorVec> AnaProcessorVecUniPtr;
typedef std::vector<AnaProcessorUniPtr>::iterator AnaProcessorVecItr;
typedef std::map<std::string, std::shared_ptr<AnaProcessor> > AnaProcessorMap;
typedef std::map<std::string, AnaProcessor* > AnaProcessorMapO;
typedef std::shared_ptr<AnaProcessorMap> AnaProcessorMapUniPtr;
typedef std::map<std::string, std::shared_ptr<AnaProcessor> >::iterator AnaProcessorMapItr;

#endif //DSIMU_TYPEDEF_H
