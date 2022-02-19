//
// Created by Zhang Yulei on 9/18/20.
//

#ifndef DSIMU_CALORIMETERHIT_B1_H
#define DSIMU_CALORIMETERHIT_B1_H

#include "Object/TypeDef.h"
#include "Object/SimulatedHit.h"
#include "Object/ReconstructedParticle.h"
#include "Object/DHit.h"

/// class descriptoin:
/// \brief With optical photon detected, the
/// next step is to change the optical signal to CalorimeterHit,
/// which is calld digitization. CalorimeterHit collection is used in
/// real analysis instead of SimulatedHit.
class CalorimeterHit_b1 : public DHit {
public:
    // Constructor and Destructor
    CalorimeterHit_b1();

    CalorimeterHit_b1(const CalorimeterHit_b1 &);

    ~CalorimeterHit_b1() override;

    // Operators
    bool operator==(const CalorimeterHit_b1 &rhs) const;

    bool operator!=(const CalorimeterHit_b1 &rhs) const;

    CalorimeterHit_b1 &operator=(const CalorimeterHit_b1 &rhs);

    // Get Methods
    const RecParticleVec &getPContribution() const;

    const SimulatedHitVec &getSimHits() const;

    // Set Methods
    void setPContribution(const RecParticleVec &pContribution);

    void setSimHits(const SimulatedHitVec &simHits);

private:
    RecParticleVec PContribution;
    SimulatedHitVec SimHits;

ClassDefOverride(CalorimeterHit,12);

};


#endif //DSIMU_CALORIMETERHIT_B1_H
