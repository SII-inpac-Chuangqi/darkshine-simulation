//
// Created by Zhang Yulei on 9/26/20.
//

#include "Event/AnaEvnt.h"

void AnaEvnt::Initialization(CleanType ct) {
    DEvent::Initialization(ct);
    for (const auto &itr : MCParticleCollectionSP) {
        for (auto itr2 : *itr.second) {
            delete itr2;
        }
        (itr.second)->clear();
        (itr.second)->shrink_to_fit();
    }
    if (ct == nALL) MCParticleCollectionSP.clear();
    for (const auto &itr : RecParticleCollectionSP) {
        for (auto itr2 : *itr.second) {
            delete itr2;
        }
        (itr.second)->clear();
        (itr.second)->shrink_to_fit();
    }
    if (ct == nALL) RecParticleCollectionSP.clear();

    for (const auto &itr : SimulatedHitCollectionSP) {
        for (auto itr2 : *itr.second) {
            delete itr2;
        }
        (itr.second)->clear();
        (itr.second)->shrink_to_fit();
    }
    if (ct == nALL) SimulatedHitCollectionSP.clear();

    for (const auto &itr : CalorimeterHitCollectionSP) {
        for (auto itr2 : *itr.second) {
            delete itr2;
        }
        (itr.second)->clear();
        (itr.second)->shrink_to_fit();
    }
    if (ct == nALL) CalorimeterHitCollectionSP.clear();

    for (const auto &itr : StepCollectionSP) {
        for (auto itr2 : *itr.second) {
            delete itr2;
        }
        (itr.second)->clear();
        (itr.second)->shrink_to_fit();
    }
    if (ct == nALL) StepCollectionSP.clear();
}

void AnaEvnt::ConvertTreeValuePtr(const std::shared_ptr<TTreeReaderValue<DEvent>> &evt) {
    /* Convert some normal variables */
    RunID = (*evt)->getRunId();
    EventID = (*evt)->getEventId();
    for (int i = 0; i < 4; ++i) Rndm[i] = (*evt)->getRndm()[i];
    PNEnergy_Target = (*evt)->getPnEnergyTarget();
    PNEnergy_ECAL = (*evt)->getPnEnergyEcal();
    Eleak_ECAL = (*evt)->getEleakEcal();

    /* Convert Collections */
    for (const auto &i : (*evt)->getStepCollection_Old()) {
        DStepVecUniPtr tmpVec = std::make_shared<DStepVec>();
        for (auto j : *(i.second)) tmpVec->emplace_back(j);

        if (!tmpVec->empty())
            StepCollectionSP.emplace(std::make_pair(i.first, tmpVec));
    }

    for (const auto &i : (*evt)->getSimulatedHitCollection_Old()) {
        SimulatedHitVecUniPtr tmpVec = std::make_shared<SimulatedHitVec>();
        for (auto j : *(i.second)) tmpVec->emplace_back(j);

        if (!tmpVec->empty())
            SimulatedHitCollectionSP.emplace(std::make_pair(i.first, tmpVec));
    }

    for (const auto &i : (*evt)->getMcParticleCollection_Old()) {
        MCParticleVecUniPtr tmpVec = std::make_shared<MCParticleVec>();
        for (auto j : *(i.second)) tmpVec->emplace_back(j);

        if (!tmpVec->empty())
            MCParticleCollectionSP.emplace(std::make_pair(i.first, tmpVec));
    }

    for (const auto &i : (*evt)->getCalorimeterHitCollection_Old()) {
        CalorimeterHitVecUniPtr tmpVec = std::make_shared<CalorimeterHitVec>();
        for (auto j : *(i.second)) tmpVec->emplace_back(j);

        if (!tmpVec->empty())
            CalorimeterHitCollectionSP.emplace(std::make_pair(i.first, tmpVec));
    }

    for (const auto &i : (*evt)->getRecParticleCollection_Old()) {
        RecParticleVecUniPtr tmpVec = std::make_shared<RecParticleVec>();
        for (auto j : *(i.second)) tmpVec->emplace_back(j);

        if (!tmpVec->empty())
            RecParticleCollectionSP.emplace(std::make_pair(i.first, tmpVec));
    }
}

std::vector<std::string> *AnaEvnt::ListAllCollections() {
    auto tmp = new std::vector<std::string>;

    auto s = ListCollections(StepCollectionSP);
    tmp->insert(tmp->end(), s->begin(), s->end());
    delete s;

    s = ListCollections(MCParticleCollectionSP);
    tmp->insert(tmp->end(), s->begin(), s->end());
    delete s;

    s = ListCollections(RecParticleCollectionSP);
    tmp->insert(tmp->end(), s->begin(), s->end());
    delete s;

    s = ListCollections(SimulatedHitCollectionSP);
    tmp->insert(tmp->end(), s->begin(), s->end());
    delete s;

    s = ListCollections(CalorimeterHitCollectionSP);
    tmp->insert(tmp->end(), s->begin(), s->end());
    delete s;

    return tmp;
}

void AnaEvnt::LinkChildren() {
    if (MCParticleCollectionSP.empty()) return;
    for (const auto& collection : MCParticleCollectionSP) {
        for (auto itr : *(collection.second)) {
            // If parent exists
            if ( itr->getParents() ) itr->getParents()->addChildren(itr);
        }
    }
}


