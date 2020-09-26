//
// Created by Zhang Yulei on 9/18/20.
//

#ifndef DSIMU_CALORIMETERHIT_H
#define DSIMU_CALORIMETERHIT_H

#include "Object/TypeDef.h"
#include "Object/SimulatedHit.h"
#include "Object/ReconstructedParticle.h"
#include "DHit.h"

class CalorimeterHit : public DHit {
public:
    // Constructor and Destructor
    CalorimeterHit();

    CalorimeterHit(const CalorimeterHit &);

    ~CalorimeterHit() override;

    // Operators
    bool operator==(const CalorimeterHit &rhs) const;

    bool operator!=(const CalorimeterHit &rhs) const;

    CalorimeterHit &operator=(const CalorimeterHit &rhs);

    // Get Methods
    const RecParticleVec &getPContribution() const;

    const SimulatedHitVec &getSimHits() const;

    // Set Methods
    void setPContribution(const RecParticleVec &pContribution);

    void setSimHits(const SimulatedHitVec &simHits);

private:
    RecParticleVec PContribution;
    SimulatedHitVec SimHits;

ClassDef(CalorimeterHit,12);

};


#endif //DSIMU_CALORIMETERHIT_H
