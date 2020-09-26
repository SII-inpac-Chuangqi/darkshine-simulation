//
// Created by Zhang Yulei on 9/26/20.
//

#ifndef DSIMU_ANAEVNT_H
#define DSIMU_ANAEVNT_H

#include "Object/DEvent.h"
#include "TTreeReaderValue.h"

class AnaEvnt : public DEvent{
public:
    AnaEvnt() = default;

    ~AnaEvnt() override = default;

    void ConvertTreeValuePtr(const std::shared_ptr<TTreeReaderValue<DEvent> >& evt);

    void Initialization(CleanType ct) override;

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


#endif //DSIMU_ANAEVNT_H
