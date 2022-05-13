//
// Created by Zhang Yulei on 9/18/20.
//

#ifndef DSIMU_SIMULATEDHIT_B1_5_H
#define DSIMU_SIMULATEDHIT_B1_5_H

#include "Object/TypeDef.h"
#include "Object/McParticle.h"
#include "Object/CalorimeterHit.h"
#include "Object/DHit.h"

#include <vector>
#include "TString.h"

/// class description:
/// \brief Deposit energy. TRUTH information which is known only in simulation.
class SimulatedHit_b1_5 : public DHit {
public:
    // Constructor and Destructor
    SimulatedHit_b1_5();

    SimulatedHit_b1_5(const SimulatedHit_b1_5 &);

    ~SimulatedHit_b1_5() override;

    bool operator==(const SimulatedHit_b1_5 &rhs) const;

    bool operator!=(const SimulatedHit_b1_5 &rhs) const;

    SimulatedHit_b1_5 &operator=(const SimulatedHit_b1_5 &rhs);

    // Get Methods
    float getEdepEm() const;

    float getEdepHad() const;

    //float getELeakWrapper() const;

    int getPhotonNumber() const {
        return PhotonNumber;
    }

    const std::vector<McParticle_b1_5 > &getPContribution() const {
        return MCPContribution;
    }

    const CalorimeterHitVec &getCaloHits() const {
        return CaloHits;
    }

    //Dump
    friend std::ostream &operator<<(std::ostream &os, const SimulatedHit_b1_5 &hit) {
//        TString detector;
//        if     (Detector == nNone)    detector = "None";
//        else if(Detector == nTracker) detector = "Tracker";
//        else if(Detector == nECAL)    detector = "ECal";
//        else if(Detector == nHCAL)    detector = "HCal";

        TString str(Form("| %-5d | %8.3f, %8.3f, %8.3f, %8.3f, %8.3f | %-5d   |",
                          hit.id,
                          hit.getX(), hit.getY(), hit.getZ(), hit.T, hit.E,
                          hit.CellID));
        os << str;

        return os;
    }

    // Set Methods
    void setEdepEm(float edepEm);

    void setEdepHad(float edepHad);

    //void setELeakWrapper(float eLeakWrapper);

    void setPContribution(const std::vector<McParticle_b1_5> &pContribution) {
        MCPContribution = pContribution;
    }

    void setCaloHits(const CalorimeterHitVec &caloHits) {
        CaloHits = caloHits;
    }

    // Add Methods
    void addEdep(float EEm, float EHad) {
        EdepEm += EEm;
        EdepHad += EHad;

        E += (EEm + EHad);
    };

    void addPhoton(int num = 1) { PhotonNumber += num; };

    void addParticleContribution(const McParticle_b1_5& mcp, float Edep, bool record_all = false);

private:
    // 00 00 00 00   -- -- -- --   -- -- -- --   -- -- -- -- // Last line of DHit Memory
    int PhotonNumber{0};
    float EdepEm{0.};
    float EdepHad{0.};
    //float ELeak_Wrapper{0.};

    // the corresponding MC particle contributing to this hit
    std::vector<McParticle_b1_5 > MCPContribution;
    // the corresponding Edep for this MC particle in this hit
    std::vector<float> SimHits_Edep;

    CalorimeterHitVec CaloHits;

ClassDefOverride(SimulatedHit_b1_5, 11)

};

#endif //DSIMU_SIMULATEDHIT_B1_5_H
