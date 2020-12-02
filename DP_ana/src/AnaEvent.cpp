//
// Created by Zhang Yulei on 9/26/20.
//

#include "Event/AnaEvent.h"

void AnaEvent::Initialization(CleanType ct) {
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

void AnaEvent::ConvertTreeValuePtr(const std::shared_ptr<TTreeReaderValue<DEvent>> &evt) {
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

std::vector<std::string> *AnaEvent::ListAllCollections() {
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

void AnaEvent::LinkChildren() {
    if (MCParticleCollectionSP.empty()) return;
    for (const auto &collection : MCParticleCollectionSP) {
        for (auto itr : *(collection.second)) {
            // If parent exists
            if (itr->getParents()) itr->getParents()->addChildren(itr);
        }
    }
}

DStepVecUniPtr AnaEvent::RegisterStepCollection(const std::string &str) {
    if (StepCollectionSP.count(str) != 0) {
        std::cerr << "[WARNING] ==> Key already exists. Return the existing Key value." << std::endl;
        return nullptr;
    }
    auto tmpVec = std::shared_ptr<DStepVec>(new DStepVec());
    StepCollectionSP.emplace(std::pair<std::string, DStepVecUniPtr>(str, tmpVec));

    if (Verbose > 1) {
        std::cout << "[STEP REGISTER] : (Verbosity 2) ==> A new collection " + str +
                     " has been successfully added to MCParticle Collection." << std::endl;
    }

    return tmpVec;
}

MCParticleVecUniPtr AnaEvent::RegisterMCParticleCollection(const std::string &str) {
    if (MCParticleCollectionSP.count(str) != 0) {
        std::cerr << "[WARNING] ==> Key already exists. Return the existing Key value." << std::endl;
        return nullptr;
    }
    auto tmpVec = std::shared_ptr<MCParticleVec>(new MCParticleVec());
    MCParticleCollectionSP.emplace(std::pair<std::string, MCParticleVecUniPtr>(str, tmpVec));

    if (Verbose > 1) {
        std::cout << "[MC REGISTER] : (Verbosity 2) ==> A new collection " + str +
                     " has been successfully added to MCParticle Collection." << std::endl;
    }

    return tmpVec;
}

RecParticleVecUniPtr AnaEvent::RegisterRecParticleCollection(const std::string &str) {
    if (RecParticleCollectionSP.count(str) != 0) {
        std::cerr << "[WARNING] ==> Key already exists. Return the existing Key value." << std::endl;
        return nullptr;
    }
    auto tmpVec = std::shared_ptr<RecParticleVec>(new RecParticleVec());
    RecParticleCollectionSP.emplace(std::pair<std::string, RecParticleVecUniPtr >(str, tmpVec));

    if (Verbose > 1) {
        std::cout << "[REC REGISTER] : (Verbosity 2) ==> A new collection " + str +
                     " has been successfully added to RecParticle Collection." << std::endl;
    }

    return tmpVec;
}

SimulatedHitVecUniPtr AnaEvent::RegisterSimulatedHitCollection(const std::string &str) {
    if (SimulatedHitCollectionSP.count(str) != 0) {
        std::cerr << "[WARNING] ==> Key already exists. Return the existing Key value." << std::endl;
        return nullptr;
    }
    auto tmpVec = std::shared_ptr<SimulatedHitVec>(new SimulatedHitVec());
    SimulatedHitCollectionSP.emplace(std::pair<std::string, SimulatedHitVecUniPtr >(str, tmpVec));

    if (Verbose > 1) {
        std::cout << "[MC REGISTER] : (Verbosity 2) ==> A new collection " + str +
                     " has been successfully added to SimulatedHit Collection." << std::endl;
    }

    return tmpVec;
}

CalorimeterHitVecUniPtr AnaEvent::RegisterCalorimeterHitCollection(const std::string &str) {
    if (CalorimeterHitCollectionSP.count(str) != 0) {
        std::cerr << "[WARNING] ==> Key already exists. Return the existing Key value." << std::endl;
        return nullptr;
    }
    auto tmpVec = std::shared_ptr<CalorimeterHitVec>(new CalorimeterHitVec());
    CalorimeterHitCollectionSP.emplace(std::pair<std::string, CalorimeterHitVecUniPtr >(str, tmpVec));

    if (Verbose > 1) {
        std::cout << "[REC REGISTER] : (Verbosity 2) ==> A new collection " + str +
                     " has been successfully added to CalorimterHit Collection" << std::endl;
    }

    return tmpVec;
}