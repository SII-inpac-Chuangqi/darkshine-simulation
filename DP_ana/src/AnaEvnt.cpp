//
// Created by Zhang Yulei on 9/26/20.
//

#include "Event/AnaEvnt.h"

void AnaEvnt::ConvertTreeValuePtr(const std::shared_ptr<TTreeReaderValue<DEvent>> &evt) {
    /* Convert some normal variables */
    RunID = (*evt)->getRunId();
    EventID = (*evt)->getEventId();
    for (int i = 0; i < 4; ++i) Rndm[i] = (*evt)->getRndm()[i];
    PNEnergy_Target = (*evt)->getPnEnergyTarget();
    PNEnergy_ECAL = (*evt)->getPnEnergyEcal();
    Eleak_ECAL = (*evt)->getEleakEcal();

    /* Convert Collections */
    for (const auto &i : (*evt)->getStepCollection()) {
        DStepVecUniPtr tmpVec = std::make_shared<DStepVec>();
        for (auto j : *(i.second)) tmpVec->emplace_back(j);

        StepCollectionSP.emplace(std::make_pair(i.first, tmpVec));
    }

    for (const auto &i : (*evt)->getSimulatedHitCollection()) {
        SimulatedHitVecUniPtr tmpVec = std::make_shared<SimulatedHitVec>();
        for (auto j : *(i.second)) tmpVec->emplace_back(j);

        SimulatedHitCollectionSP.emplace(std::make_pair(i.first, tmpVec));
    }

    for (const auto &i : (*evt)->getMcParticleCollection()) {
        MCParticleVecUniPtr tmpVec = std::make_shared<MCParticleVec>();
        for (auto j : *(i.second)) tmpVec->emplace_back(j);

        MCParticleCollectionSP.emplace(std::make_pair(i.first, tmpVec));
    }

    for (const auto &i : (*evt)->getCalorimeterHitCollection()) {
        CalorimeterHitVecUniPtr tmpVec = std::make_shared<CalorimeterHitVec>();
        for (auto j : *(i.second)) tmpVec->emplace_back(j);

        CalorimeterHitCollectionSP.emplace(std::make_pair(i.first, tmpVec));
    }

    for (const auto &i : (*evt)->getRecParticleCollection()) {
        RecParticleVecUniPtr tmpVec = std::make_shared<RecParticleVec>();
        for (auto j : *(i.second)) tmpVec->emplace_back(j);

        RecParticleCollectionSP.emplace(std::make_pair(i.first, tmpVec));
    }
}
