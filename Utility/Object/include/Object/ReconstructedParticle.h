//
// Created by Zhang Yulei on 9/18/20.
//

#ifndef DSIMU_RECONSTRUCTEDPARTICLE_H
#define DSIMU_RECONSTRUCTEDPARTICLE_H

#include "Object/TypeDef.h"
#include "Object/CalorimeterHit.h"
#include "Object/McParticle.h"
#include "DParticle.h"

class ReconstructedParticle : public DParticle {
public:
    // Constructor and Destructor
    ReconstructedParticle();

    ReconstructedParticle(const ReconstructedParticle &);

    virtual ~ReconstructedParticle();

    bool operator==(const ReconstructedParticle &rhs) const;

    bool operator!=(const ReconstructedParticle &rhs) const;

    ReconstructedParticle &operator=(const ReconstructedParticle &rhs);

    // Get Methods
    const MCParticleVec &getMcParticles() const;

    const CalorimeterHitVec &getCaloHits() const;

    const RecParticleVec &getParents() const;

    const RecParticleVec &getChildren() const;

    // Set Methods
    void setMcParticles(const MCParticleVec &mcParticles);

    void setCaloHits(const CalorimeterHitVec &caloHits);

    void setParents(const RecParticleVec &parents);

    void setChildren(const RecParticleVec &children);

private:
    RecParticleVec Parents;
    RecParticleVec Children;

    MCParticleVec MCParticles;
    CalorimeterHitVec CaloHits;

ClassDef(ReconstructedParticle,13);

};


#endif //DSIMU_RECONSTRUCTEDPARTICLE_H
