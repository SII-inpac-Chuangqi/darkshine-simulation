//
// Created by Zhang Yulei on 9/18/20.
//

#ifndef DSIMU_SIMULATEDHIT_H
#define DSIMU_SIMULATEDHIT_H

#include "Object/TypeDef.h"
#include "Object/McParticle.h"
#include "Object/CalorimeterHit.h"
#include "DHit.h"

#include <vector>

class McParticle;

/// class description:
/// \brief Deposit energy. TRUTH information which is known only in simulation.
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

    double getELeakWrapper() const;

    const std::vector<McParticle > &getPContribution() const {
        return MCPContribution;
    }

    const CalorimeterHitVec &getCaloHits() const {
        return CaloHits;
    }

    // Set Methods
    void setEdepEm(double edepEm);

    void setEdepHad(double edepHad);

    void setELeakWrapper(double eLeakWrapper);

    void setPContribution(const std::vector<McParticle> &pContribution) {
        MCPContribution = pContribution;
    }

    void setCaloHits(const CalorimeterHitVec &caloHits) {
        CaloHits = caloHits;
    }

    // Add Methods
    void addEdep(double EEm, double EHad) {
        EdepEm += EEm;
        EdepHad += EHad;

        E += (EEm + EHad);
    };

    void addParticleContribution(const McParticle& mcp, double Edep);

private:
    double ELeak_Wrapper{0.};
    double EdepEm{0.};
    double EdepHad{0.};

    // the corresponding MC particle contributing to this hit
    std::vector<McParticle > MCPContribution;
    // the corresponding Edep for this MC particle in this hit
    std::vector<double> SimHits_Edep;

    CalorimeterHitVec CaloHits;

ClassDefOverride(SimulatedHit, 11)

};

#endif //DSIMU_SIMULATEDHIT_H
