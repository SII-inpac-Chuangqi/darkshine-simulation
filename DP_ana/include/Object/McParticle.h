//
// Created by Zhang Yulei on 9/18/20.
//

#ifndef DSIMU_MCPARTICLE_H
#define DSIMU_MCPARTICLE_H

#include "Object/TypeDef.h"
#include "Object/ReconstructedParticle.h"
#include "Object/SimulatedHit.h"
#include "Utility/DParticle.h"

class McParticle : public DParticle
{
public:
    // Constructor and Destructor
    McParticle();
    McParticle(const McParticle&);
    ~McParticle() override;

    // Operators
    bool operator==(const McParticle &rhs) const;

    bool operator!=(const McParticle &rhs) const;

    McParticle &operator=(const McParticle &rhs );

    // Get Methods
    double getERemain() const;

    const RecParticleVec &getRecParticles() const;

    const SimulatedHitVec &getSimHits() const;

    // Set Methods
    void setERemain(double eRemain);

    void setRecParticles(const RecParticleVec &recParticles);

    void setSimHits(const SimulatedHitVec &simHits);

private:
    // the remaining energy while leaving the world
    double ERemain{};

    RecParticleVec RecParticles;
    SimulatedHitVec SimHits;

};


#endif //DSIMU_MCPARTICLE_H
