//
// Created by Zhang Yulei on 9/18/20.
//

#ifndef DSIMU_DEVENT_B1_5_H
#define DSIMU_DEVENT_B1_5_H

#include <cstddef>
#include <cstdlib>
#include <map>
#include <iostream>
#include <memory>

#include "Object/CalorimeterHit.h"
#include "Object/McParticle.h"
#include "Object/ReconstructedParticle.h"
#include "Object/SimulatedHit.h"
#include "Object/DigiForm.hh"
#include "Object/DStep.h"
#include "Object/McPHelper.h"

#include "TObject.h"
#include "TString.h"
#include "TBuffer.h"

enum CleanType {
    nALL, nVector
};

/// class description:
/// \brief Contains all the necessary information for analysis.
class DEvent_b1_5 : public TObject {
public:

    // Constructor
    DEvent_b1_5() {
        //std::cout<<"Devent Constructor"<<std::endl;
        //Initialization(nALL);
    };

    ~DEvent_b1_5() override {
        //std::cout<<"Devent Destructor"<<std::endl;
        Initialization(nALL);
    }

    // Operators

    // Get Methods
    [[nodiscard]] int getRunId() const {
        return RunID;
    }

    [[nodiscard]] int getEventId() const {
        return EventID;
    }

    [[nodiscard]] int getVerbose() const {
        return Verbose;
    }

    [[nodiscard]] const double *getRndm() const {
        return Rndm;
    }

    [[nodiscard]] const DStepMap &getStepCollection() const {
        return StepCollection;
    }

    [[nodiscard]] const DigiFormMap &getOpticalCollection() const {
        return OpticalCollection;
    }

    [[nodiscard]] const MCParticleMap &getMcParticleCollection() const {
        return MCParticleCollection;
    }

    [[nodiscard]] const MCPHelperMap &getMcPHelperCollection() const {
        return MCPHelperCollection;
    }

    [[nodiscard]] const RecParticleMap &getRecParticleCollection() const {
        return RecParticleCollection;
    }

    [[nodiscard]] const SimulatedHitMap &getSimulatedHitCollection() const {
        return SimulatedHitCollection;
    }

    [[nodiscard]] const CalorimeterHitMap &getCalorimeterHitCollection() const {
        return CalorimeterHitCollection;
    }

    void PrintDetails();

    // Set Methods
    void setRunId(int runId) {
        RunID = runId;
    }

    void setEventId(int eventId) {
        EventID = eventId;
    }

    void setVerbose(int verbose) {
        Verbose = verbose;
    }

    void setRndm(const double *rndm) {
        for (int i = 0; i < 4; ++i)
            Rndm[i] = rndm[i];
    }

    virtual // Initialization
    void Initialization(CleanType);

    // Register Collections
    DStepVec *RegisterStepCollection(const std::string &);

    DigiFormVec *RegisterOpticalCollection(const std::string &str);

    MCParticleVec *RegisterMCParticleCollection(const std::string &);

    MCPHelperVec *RegisterMCPHelperCollection(const std::string &);

    RecParticleVec *RegisterRecParticleCollection(const std::string &);

    SimulatedHitVec *RegisterSimulatedHitCollection(const std::string &);

    CalorimeterHitVec *RegisterCalorimeterHitCollection(const std::string &);

    // Delete Collections
    void DeleteCollection(const std::string &);

    // List all Registered Collections for template T
    template<class T>
    std::vector<std::string> *ListCollections(const T &);

    virtual std::vector<std::string> *ListAllCollections();

    /*
     * Miscellaneous (truth)
     */
    float getPnEnergyTarget() const {
        return PNEnergy_Target;
    }

    void setPnEnergyTarget(float pnEnergyTarget) {
        PNEnergy_Target = pnEnergyTarget;
    }

    float getPnEnergyEcal() const {
        return PNEnergy_ECAL;
    }

    void setPnEnergyEcal(float pnEnergyEcal) {
        PNEnergy_ECAL = pnEnergyEcal;
    }

    void setPnZTarget(float pnzTarget) {
        PNZ_Target = pnzTarget;
    }

    void setPnZEcal(float pnzEcal) {
        PNZ_ECAL = pnzEcal;
    }

    float getEnEnergyTarget() const {
        return ENEnergy_Target;
    }

    void setEnEnergyTarget(float enEnergyTarget) {
        ENEnergy_Target = enEnergyTarget;
    }

    float getEnEnergyEcal() const {
        return ENEnergy_ECAL;
    }

    void setEnEnergyEcal(float enEnergyEcal) {
        ENEnergy_ECAL = enEnergyEcal;
    }

    void setEnZTarget(float enzTarget) {
        ENZ_Target = enzTarget;
    }

    void setEnZEcal(float enzEcal) {
        ENZ_ECAL = enzEcal;
    }

    float getEleakEcal() const {
        return Eleak_ECAL;
    }

    void setEleakEcal(float eleakEcal) {
        Eleak_ECAL = eleakEcal;
    }

    float getPnzTarget() const {
        return PNZ_Target;
    }

    float getPnzEcal() const {
        return PNZ_ECAL;
    }

    float getEnzTarget() const {
        return ENZ_Target;
    }

    float getEnzEcal() const {
        return ENZ_ECAL;
    }

    float getWeight() const {
        return weight;
    }

    void setWeight(double w) {
        weight = w;
    }

    McParticle *SearchID(MCParticleVec *mv, int ID) {
        for (auto itr : *mv) {
            if (itr->getId() == ID) return itr;
        }
        return nullptr;
    }

    McPHelper *SearchID(MCPHelperVec *mv, int ID) {
        for (auto itr : *mv) {
            if (itr->getId() == ID) return itr;
        }
        return nullptr;
    }

    void LinkChildren();

#ifdef MEMCK
    static void PrintObjectStatistics(const TString& str="");
#endif

protected:
    // run number
    int RunID{0};

    // event number
    int EventID{0};

    /*
     * Miscellaneous (truth)
     */
    float weight{1.};
    double Rndm[4]{}; // Random Number Seeds
    float PNEnergy_Target{0.}; // Photon-Nuclear reaction Energy in Target region
    float PNEnergy_ECAL{0.}; // Photon-Nuclear reaction Energy in ECAL region
    float PNZ_Target{0.};
    float PNZ_ECAL{0.};
    float Eleak_ECAL{0.}; // Energy leakage in ECAL holder
    float ENEnergy_Target{0.};
    float ENEnergy_ECAL{0.};
    float ENZ_Target{0.};
    float ENZ_ECAL{0.};

    /*
     * Miscellaneous (Reconstructed)
     */
    float TotalRecEnergy{0.};
    float ECALRecEnergy{0.};
    float HCALRecEnergy{0.};

    // Verbosity
    int Verbose{0};

    // Step Collection
    DStepMap StepCollection;

    // Optical Collection
    DigiFormMap OpticalCollection;

    // Event Physics Quantities
    MCParticleMap MCParticleCollection;
    MCPHelperMap MCPHelperCollection;
    RecParticleMap RecParticleCollection;
    SimulatedHitMap SimulatedHitCollection;
    CalorimeterHitMap CalorimeterHitCollection;

ClassDefOverride(DEvent_b1_5, 1);

};

// Some inline
template<class T>
std::vector<std::string> *DEvent_b1_5::ListCollections(const T &in) {
    auto tmp = new std::vector<std::string>;
    for (auto itr : in) tmp->emplace_back(itr.first);

    return tmp;
}

#endif //DSIMU_DEVENT_B1_5_H

