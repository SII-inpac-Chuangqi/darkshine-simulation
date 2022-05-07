//
// Created by Zhang Yulei on 9/18/20.
//

#ifndef DSIMU_MCPARTICLE_B1_5_H
#define DSIMU_MCPARTICLE_B1_5_H

#include "Object/TypeDef.h"
#include "Object/ReconstructedParticle.h"
#include "Object/SimulatedHit.h"
#include "Object/DParticle.h"

/// class description:
/// \brief Secondary particles. TRUTH information which is known only in simulation.
class McParticle_b1_5 : public DParticle {
public:
    // Constructor and Destructor
    McParticle_b1_5();

    McParticle_b1_5(const McParticle_b1_5 &);

    ~McParticle_b1_5() override;

    // Operators
    bool operator==(const McParticle_b1_5 &rhs) const;

    bool operator!=(const McParticle_b1_5 &rhs) const;

    McParticle_b1_5 &operator=(const McParticle_b1_5 &rhs);

    // Get Methods
    float getERemain() const;

    McParticle_b1_5 *getParents() const {
        return Parents;
    }

    MCParticleVec *getChildren() const {
        return Children;
    }

    ReconstructedParticle_b1_5 *getRecParticles() const {
        return RecParticles;
    }

    const SimulatedHitVec &getSimHits() const {
        return SimHits;
    }

    // Dump
    friend std::ostream &operator<<(std::ostream &os, const McParticle_b1_5 &particle) {
        TString str(Form("| %-5d | %-6d | %8.3f, %8.3f, %8.3f | %8.3f, %8.3f, %8.3f |    %8.3f, %8.3f, %8.3f | %8.3f, %8.3f, %8.3f | %-16s %-8d |",
                    particle.id,
                    particle.PDG,
                    particle.Px, particle.Py, particle.Pz,
                    particle.Energy, particle.Mass, particle.ERemain,
                    particle.VertexX, particle.VertexY, particle.VertexZ,
                    particle.EndPointX, particle.EndPointY, particle.EndPointZ,
                    particle.CreateProcess.data(), (particle.getParents()) ? particle.getParents()->getId() : 0));

        os << str;

        return os;
    }


    // Set Methods
    void setERemain(float eRemain);

    void setParents(McParticle_b1_5 *parents) {
        Parents = parents;
    }

    void setChildren(MCParticleVec *children) {
        Children = children;
    }

    void setRecParticles(ReconstructedParticle_b1_5 *recParticles) {
        RecParticles = recParticles;
    }

    void setSimHits(const SimulatedHitVec &simHits) {
        SimHits = simHits;
    }

    // Search Methods
    static McParticle_b1_5* SearchID(MCParticleVec* mv, int ID) {
        for (auto itr : *mv) {
            if (itr->getId() == ID) return itr;
        }
        return nullptr;
    }

    // Add Methods
    void addChildren(McParticle_b1_5* mcp) {
        Children->emplace_back(mcp);
    }

private:
    // 00 00 00 00   00 00 00 00   00 00 00 00  -- -- -- -- // Last line of DParticle Memory
    // the remaining energy while leaving the world
    float ERemain{0.};

    McParticle_b1_5* Parents{};
    MCParticleVec* Children{};

    ReconstructedParticle_b1_5* RecParticles{};
    SimulatedHitVec SimHits;

    // 00 00 00 00   00 00 00 00   00 00 00 00  00 00 00 00 // Last line of MCParticle Memory

ClassDefOverride(McParticle_b1_5, 10)

};


#endif //DSIMU_MCPARTICLE_B1_5_H
