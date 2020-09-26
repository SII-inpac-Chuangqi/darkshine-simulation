//
// Created by Zhang Yulei on 9/18/20.
//

#ifndef DSIMU_MCPARTICLE_H
#define DSIMU_MCPARTICLE_H

#include "Object/TypeDef.h"
#include "Object/ReconstructedParticle.h"
#include "Object/SimulatedHit.h"
#include "DParticle.h"

class McParticle : public DParticle {
public:
    // Constructor and Destructor
    McParticle();

    McParticle(const McParticle &);

    ~McParticle() override;

    // Operators
    bool operator==(const McParticle &rhs) const;

    bool operator!=(const McParticle &rhs) const;

    McParticle &operator=(const McParticle &rhs);

    // Get Methods
    double getERemain() const;

    McParticle *getParents() const {
        return Parents;
    }

    MCParticleVec *getChildren() const {
        return Children;
    }

    ReconstructedParticle *getRecParticles() const {
        return RecParticles;
    }

    const SimulatedHitVec &getSimHits() const {
        return SimHits;
    }

    // Set Methods
    void setERemain(double eRemain);

    void setParents(McParticle *parents) {
        Parents = parents;
    }

    void setChildren(MCParticleVec *children) {
        Children = children;
    }

    void setRecParticles(ReconstructedParticle *recParticles) {
        RecParticles = recParticles;
    }

    void setSimHits(const SimulatedHitVec &simHits) {
        SimHits = simHits;
    }

    // Search Methods
    McParticle* SearchID(MCParticleVec* mv, int ID) {
        for (auto itr : *mv) {
            if (itr->getId() == ID) return itr;
        }
        return nullptr;
    }

    // Add Methods
    void addChildren(McParticle* mcp) {
        Children->emplace_back(mcp);
    }
private:

    // the remaining energy while leaving the world
    double ERemain{0.};

    McParticle* Parents;
    MCParticleVec* Children;

    ReconstructedParticle* RecParticles;
    SimulatedHitVec SimHits;

ClassDef(McParticle,10)

};


#endif //DSIMU_MCPARTICLE_H
