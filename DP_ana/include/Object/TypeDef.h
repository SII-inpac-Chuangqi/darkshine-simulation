//
// Created by Zhang Yulei on 9/19/20.
//

#ifndef DSIMU_TYPEDEF_H
#define DSIMU_TYPEDEF_H

#include <vector>

class McParticle;
class ReconstructedParticle;
class SimulatedHit;
class CalorimeterHit;

typedef std::vector<McParticle* > MCParticleVec;
typedef std::vector<McParticle* >::iterator MCParticleVecItr;
typedef std::vector<ReconstructedParticle* > RecParticleVec;
typedef std::vector<ReconstructedParticle* >::iterator RecParticleVecItr;
typedef std::vector<SimulatedHit* > SimulatedHitVec;
typedef std::vector<SimulatedHit* >::iterator SimulatedHitVecItr;
typedef std::vector<CalorimeterHit* > CalorimeterHitVec;
typedef std::vector<CalorimeterHit* >::iterator CalorimeterHitVecItr;

#endif //DSIMU_TYPEDEF_H
