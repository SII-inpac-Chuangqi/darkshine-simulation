//
// Created by Zhang Yulei on 9/19/20.
//

#ifndef DSIMU_TYPEDEF_H
#define DSIMU_TYPEDEF_H

#include <vector>
#include <map>
#include <unordered_map>
#include <iterator>
#include <memory>

class DStep;

class McParticle;

class ReconstructedParticle;

class SimulatedHit;

class CalorimeterHit;

class AnaProcessor;

typedef std::vector<DStep *> DStepVec;
typedef std::shared_ptr<DStepVec> DStepVecUniPtr;
typedef std::vector<DStep *>::iterator DStepVecItr;
typedef std::map<std::string, DStepVecUniPtr> DStepMap;
typedef std::map<std::string, DStepVec* > DStepMapO;
typedef std::shared_ptr<DStepMap> DStepMapUniPtr;
typedef std::map<std::string, DStepVecUniPtr>::iterator DStepMapItr;

typedef std::vector<McParticle *> MCParticleVec;
typedef std::shared_ptr<MCParticleVec> MCParticleVecUniPtr;
typedef std::vector<McParticle *>::iterator MCParticleVecItr;
typedef std::map<std::string, MCParticleVecUniPtr> MCParticleMap;
typedef std::map<std::string, MCParticleVec* > MCParticleMapO;
typedef std::shared_ptr<MCParticleMap> MCParticleMapUniPtr;
typedef std::map<std::string, MCParticleVecUniPtr>::iterator MCParticleMapItr;

typedef std::vector<ReconstructedParticle *> RecParticleVec;
typedef std::shared_ptr<RecParticleVec> RecParticleVecUniPtr;
typedef std::vector<ReconstructedParticle *>::iterator RecParticleVecItr;
typedef std::map<std::string, RecParticleVecUniPtr> RecParticleMap;
typedef std::map<std::string, RecParticleVec* > RecParticleMapO;
typedef std::shared_ptr<RecParticleMap> RecParticleMapUniPtr;
typedef std::map<std::string, RecParticleVecUniPtr>::iterator RecParticleMapItr;

typedef std::vector<SimulatedHit *> SimulatedHitVec;
typedef std::shared_ptr<SimulatedHitVec> SimulatedHitVecUniPtr;
typedef std::vector<SimulatedHit *>::iterator SimulatedHitVecItr;
typedef std::map<std::string, SimulatedHitVecUniPtr> SimulatedHitMap;
typedef std::map<std::string, SimulatedHitVec* > SimulatedHitMapO;
typedef std::shared_ptr<SimulatedHitMap> SimulatedHitMapUniPtr;
typedef std::map<std::string, SimulatedHitVecUniPtr>::iterator SimulatedHitMapItr;

typedef std::vector<CalorimeterHit *> CalorimeterHitVec;
typedef std::shared_ptr<CalorimeterHitVec> CalorimeterHitVecUniPtr;
typedef std::vector<CalorimeterHit *>::iterator CalorimeterHitVecItr;
typedef std::map<std::string, CalorimeterHitVecUniPtr> CalorimeterHitMap;
typedef std::map<std::string, CalorimeterHitVec* > CalorimeterHitMapO;
typedef std::shared_ptr<CalorimeterHitMap> CalorimeterHitMapUniPtr;
typedef std::map<std::string, CalorimeterHitVecUniPtr>::iterator CalorimeterHitMapItr;

typedef std::shared_ptr<AnaProcessor> AnaProcessorUniPtr;
typedef std::vector<AnaProcessorUniPtr> AnaProcessorVec;
typedef std::shared_ptr<AnaProcessorVec> AnaProcessorVecUniPtr;
typedef std::vector<AnaProcessorUniPtr>::iterator AnaProcessorVecItr;
typedef std::map<std::string, std::shared_ptr<AnaProcessor> > AnaProcessorMap;
typedef std::map<std::string, AnaProcessor* > AnaProcessorMapO;
typedef std::shared_ptr<AnaProcessorMap> AnaProcessorMapUniPtr;
typedef std::map<std::string, std::shared_ptr<AnaProcessor> >::iterator AnaProcessorMapItr;

#endif //DSIMU_TYPEDEF_H
