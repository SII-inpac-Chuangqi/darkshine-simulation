//
// Created by Zhang Yulei on 9/18/20.
//

#ifndef DSIMU_SIMULATEDHIT_H
#define DSIMU_SIMULATEDHIT_H

#include "Object/TypeDef.h"
#include "Object/McParticle.h"
#include "Object/CalorimeterHit.h"
#include "Utility/DHit.h"

class SimulatedHit : public DHit {
public:
    // Constructor and Destructor
    SimulatedHit();

    SimulatedHit(const SimulatedHit &);

    ~SimulatedHit() override;

    bool operator==(const SimulatedHit &rhs) const;

    bool operator!=(const SimulatedHit &rhs) const;

    SimulatedHit &operator=(const SimulatedHit &rhs);

    // Get Methods
    double getEdepEm() const;

    double getEdepHad() const;

    const MCParticleVec &getPContribution() const;

    const CalorimeterHitVec &getCaloHits() const;

    double getELeakWrapper() const;

    // Set Methods
    void setEdepEm(double edepEm);

    void setEdepHad(double edepHad);

    void setPContribution(const MCParticleVec &pContribution);

    void setCaloHits(const CalorimeterHitVec &caloHits);

    void setELeakWrapper(double eLeakWrapper);

private:
    double ELeak_Wrapper{0.};
    double EdepEm{0.};
    double EdepHad{0.};

    // the corresponding MC particle contributing to this hit
    MCParticleVec PContribution;
    CalorimeterHitVec CaloHits;
};


#endif //DSIMU_SIMULATEDHIT_H
