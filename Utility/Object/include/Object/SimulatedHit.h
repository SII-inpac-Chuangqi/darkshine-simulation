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
#include "TString.h"

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

    //Dump
    friend std::ostream &operator<<(std::ostream &os, const SimulatedHit &hit) {
//        TString detector;
//        if     (Detector == nNone)    detector = "None";
//        else if(Detector == nTracker) detector = "Tracker";
//        else if(Detector == nECAL)    detector = "ECal";
//        else if(Detector == nHCAL)    detector = "HCal";

        TString str(Form("| %-5d | % 8.3f, % 8.3f, % 8.3f, % 8.3f, % 8.3f | %-5d |",
                          hit.id,
                          hit.X, hit.Y, hit.Z, hit.T, hit.E,
                          hit.CellID));
        os << str;

        return os;
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

    void addParticleContribution(const McParticle& mcp, double Edep, bool record_all = false);

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
