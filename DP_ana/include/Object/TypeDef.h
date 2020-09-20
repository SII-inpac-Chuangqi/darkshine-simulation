//
// Created by Zhang Yulei on 9/19/20.
//

#ifndef DSIMU_TYPEDEF_H
#define DSIMU_TYPEDEF_H

#include <vector>
#include <map>
#include <unordered_map>
#include <iterator>

class DStep;
class McParticle;
class ReconstructedParticle;
class SimulatedHit;
class CalorimeterHit;
class AnaProcessor;

typedef std::vector<DStep* > DStepVec;
typedef std::vector<DStep* >::iterator DStepVecItr;
typedef std::map<std::string, DStepVec* > DStepMap;
typedef std::map<std::string, DStepVec* >::iterator DStepMapItr;

typedef std::vector<McParticle* > MCParticleVec;
typedef std::vector<McParticle* >::iterator MCParticleVecItr;
typedef std::map<std::string, MCParticleVec*> MCParticleMap;
typedef std::map<std::string, MCParticleVec*>::iterator MCParticleMapItr;

typedef std::vector<ReconstructedParticle* > RecParticleVec;
typedef std::vector<ReconstructedParticle* >::iterator RecParticleVecItr;
typedef std::map<std::string, RecParticleVec*> RecParticleMap;
typedef std::map<std::string, RecParticleVec*>::iterator RecParticleMapItr;

typedef std::vector<SimulatedHit* > SimulatedHitVec;
typedef std::vector<SimulatedHit* >::iterator SimulatedHitVecItr;
typedef std::map<std::string, SimulatedHitVec*> SimulatedHitMap;
typedef std::map<std::string, SimulatedHitVec*>::iterator SimulatedHitMapItr;

typedef std::vector<CalorimeterHit* > CalorimeterHitVec;
typedef std::vector<CalorimeterHit* >::iterator CalorimeterHitVecItr;
typedef std::map<std::string, CalorimeterHitVec*> CalorimeterHitMap;
typedef std::map<std::string, CalorimeterHitVec*>::iterator CalorimeterHitMapItr;

typedef std::vector<AnaProcessor*> AnaProcessorVec;
typedef std::vector<AnaProcessor*>::iterator AnaProcessorVecItr;
typedef std::map<std::string, AnaProcessor*> AnaProcessorMap;
typedef std::map<std::string, AnaProcessor*>::iterator AnaProcessorMapItr;

#endif //DSIMU_TYPEDEF_H
