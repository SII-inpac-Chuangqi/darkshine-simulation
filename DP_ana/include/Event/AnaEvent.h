//
// Created by Zhang Yulei on 9/26/20.
//

#ifndef DSIMU_ANAEVENT_H
#define DSIMU_ANAEVENT_H

#include "Object/DEvent.h"
#include "TTreeReaderValue.h"

class AnaEvent : public DEvent{
public:
    AnaEvent() = default;

    ~AnaEvent() override = default;

    void ConvertTreeValuePtr(const std::shared_ptr<TTreeReaderValue<DEvent> >& evt);

    void Initialization(CleanType ct) override;

    // Register Collections
    DStepVecUniPtr RegisterStepCollection(const std::string &);

    MCParticleVecUniPtr RegisterMCParticleCollection(const std::string &);

    RecParticleVecUniPtr RegisterRecParticleCollection(const std::string &);

    SimulatedHitVecUniPtr RegisterSimulatedHitCollection(const std::string &);

    CalorimeterHitVecUniPtr RegisterCalorimeterHitCollection(const std::string &);

    std::vector<std::string> *ListAllCollections() override;

    const DStepMap &getStepCollection() const  {
        return StepCollectionSP;
    }

    const MCParticleMap &getMcParticleCollection() const  {
        return MCParticleCollectionSP;
    }

    const RecParticleMap &getRecParticleCollection() const {
        return RecParticleCollectionSP;
    }

    const SimulatedHitMap &getSimulatedHitCollection() const {
        return SimulatedHitCollectionSP;
    }

    const CalorimeterHitMap &getCalorimeterHitCollection() const {
        return CalorimeterHitCollectionSP;
    }

    // Link Parent && Children
    void LinkChildren();

private:
    // Step Collection
    DStepMap StepCollectionSP;

    // Event Physics Quantities
    MCParticleMap MCParticleCollectionSP;
    RecParticleMap RecParticleCollectionSP;
    SimulatedHitMap SimulatedHitCollectionSP;
    CalorimeterHitMap CalorimeterHitCollectionSP;

};


#endif //DSIMU_ANAEVENT_H
