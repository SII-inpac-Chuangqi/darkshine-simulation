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

class SimulatedHit : public DHit {
public:
    // Constructor and Destructor
    SimulatedHit();

    SimulatedHit(const SimulatedHit &);

    ~SimulatedHit() override {
        //PContribution_TrackID.clear();
        //PContribution_TrackID.shrink_to_fit();
    };

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

    // Add Methods
    void addEdep(double EEm, double EHad) {
        EdepEm += EEm;
        EdepHad += EHad;

        E += (EEm + EHad);
    };

    //void addPContribution_TrackID(int ID) {
    //    PContribution_TrackID.emplace_back(ID);
    //}

private:
    double ELeak_Wrapper{0.};
    double EdepEm{0.};
    double EdepHad{0.};

    // the corresponding MC particle contributing to this hit
    //std::vector<int> PContribution_TrackID;
    MCParticleVec PContribution;
    CalorimeterHitVec CaloHits;

ClassDef(SimulatedHit, 11)

};


#endif //DSIMU_SIMULATEDHIT_H
