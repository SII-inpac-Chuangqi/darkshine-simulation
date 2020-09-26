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
