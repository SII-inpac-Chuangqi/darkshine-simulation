//
// Created by Zhang Yulei on 9/19/20.
//

#include "Object/DEvent.h"

void DEvent::Initialization() {

    for (auto itr : MCParticleCollection) {
        for (auto itr2 : *itr.second) {
            delete itr2;
        }
        itr.second->clear();
        itr.second->shrink_to_fit();
    }
    MCParticleCollection.clear();

    for (auto itr : RecParticleCollection) {
        for (auto itr2 : *itr.second) {
            delete itr2;
        }
        itr.second->clear();
        itr.second->shrink_to_fit();
    }
    RecParticleCollection.clear();

    for (auto itr : SimulatedHitCollection) {
        for (auto itr2 : *itr.second) {
            delete itr2;
        }
        itr.second->clear();
        itr.second->shrink_to_fit();
    }
    SimulatedHitCollection.clear();

    for (auto itr : CalorimeterHitCollection) {
        for (auto itr2 : *itr.second) {
            delete itr2;
        }
        itr.second->clear();
        itr.second->shrink_to_fit();
    }
    CalorimeterHitCollection.clear();

    for (auto itr : StepCollection) {
        for (auto itr2 : *itr.second) {
            delete itr2;
        }
        itr.second->clear();
        itr.second->shrink_to_fit();
    }
    StepCollection.clear();
}

DStepVecUniPtr DEvent::RegisterStepCollection(const std::string &str) {
    if (StepCollection.count(str) != 0) {
        std::cerr << "[WARNING] ==> Key already exists. Return the existing Key value." << std::endl;
        return StepCollection.at(str);
    }
    DStepVecUniPtr tmpVec(new DStepVec());
    StepCollection.emplace(std::pair<std::string, DStepVecUniPtr>(str, tmpVec));

    if (Verbose > 1) {
        std::cout << "[STEP REGISTER] : (Verbosity 2) ==> A new collection " + str +
                     " has been successfully added to MCParticle Collection." << std::endl;
    }

    return tmpVec;
}

MCParticleVecUniPtr DEvent::RegisterMCParticleCollection(const std::string &str) {
    if (MCParticleCollection.count(str) != 0) {
        std::cerr << "[WARNING] ==> Key already exists. Return the existing Key value." << std::endl;
        return MCParticleCollection.at(str);
    }
    MCParticleVecUniPtr tmpVec(new MCParticleVec());
    MCParticleCollection.emplace(std::pair<std::string, MCParticleVecUniPtr>(str, tmpVec));

    if (Verbose > 1) {
        std::cout << "[MC REGISTER] : (Verbosity 2) ==> A new collection " + str +
                     " has been successfully added to MCParticle Collection." << std::endl;
    }

    return tmpVec;
}


RecParticleVecUniPtr DEvent::RegisterRecParticleCollection(const std::string &str) {
    if (RecParticleCollection.count(str) != 0) {
        std::cerr << "[WARNING] ==> Key already exists. Return the existing Key value." << std::endl;
        return RecParticleCollection.at(str);
    }
    RecParticleVecUniPtr tmpVec( new RecParticleVec());
    RecParticleCollection.emplace(std::pair<std::string, RecParticleVecUniPtr>(str, tmpVec));

    if (Verbose > 1) {
        std::cout << "[REC REGISTER] : (Verbosity 2) ==> A new collection " + str +
                     " has been successfully added to RecParticle Collection." << std::endl;
    }

    return tmpVec;
}

SimulatedHitVecUniPtr DEvent::RegisterSimulatedHitCollection(const std::string &str) {
    if (SimulatedHitCollection.count(str) != 0) {
        std::cerr << "[WARNING] ==> Key already exists. Return the existing Key value." << std::endl;
        return SimulatedHitCollection.at(str);
    }
    SimulatedHitVecUniPtr tmpVec( new SimulatedHitVec());
    SimulatedHitCollection.emplace(std::pair<std::string, SimulatedHitVecUniPtr >(str, tmpVec));

    if (Verbose > 1) {
        std::cout << "[MC REGISTER] : (Verbosity 2) ==> A new collection " + str +
                     " has been successfully added to SimulatedHit Collection." << std::endl;
    }

    return tmpVec;
}

CalorimeterHitVecUniPtr DEvent::RegisterCalorimeterHitCollection(const std::string &str) {
    if (CalorimeterHitCollection.count(str) != 0) {
        std::cerr << "[WARNING] ==> Key already exists. Return the existing Key value." << std::endl;
        return CalorimeterHitCollection.at(str);
    }
    CalorimeterHitVecUniPtr tmpVec( new CalorimeterHitVec());
    CalorimeterHitCollection.emplace(std::pair<std::string, CalorimeterHitVecUniPtr >(str, tmpVec));

    if (Verbose > 1) {
        std::cout << "[REC REGISTER] : (Verbosity 2) ==> A new collection " + str +
                     " has been successfully added to CalorimterHit Collection" << std::endl;
    }

    return tmpVec;
}

std::vector<std::string> *DEvent::ListAllCollections() {
    auto tmp = new std::vector<std::string>;

    auto s = ListCollections(StepCollection);
    tmp->insert(tmp->end(), s->begin(), s->end());
    delete s;

    s = ListCollections(MCParticleCollection);
    tmp->insert(tmp->end(), s->begin(), s->end());
    delete s;

    s = ListCollections(RecParticleCollection);
    tmp->insert(tmp->end(), s->begin(), s->end());
    delete s;

    s = ListCollections(SimulatedHitCollection);
    tmp->insert(tmp->end(), s->begin(), s->end());
    delete s;

    s = ListCollections(CalorimeterHitCollection);
    tmp->insert(tmp->end(), s->begin(), s->end());
    delete s;

    return tmp;
}

void DEvent::DeleteCollection(const std::string &str) {
    auto itr1 = MCParticleCollection.find(str);
    auto itr2 = RecParticleCollection.find(str);
    auto itr3 = SimulatedHitCollection.find(str);
    auto itr4 = CalorimeterHitCollection.find(str);
    auto itr5 = StepCollection.find(str);

    if (itr1 != MCParticleCollection.end()) {
        MCParticleCollection.erase(itr1);
        std::cout << "[MC DELETE] ==> Collection " + str + " has been successfully removed." << std::endl;
    } else if (itr2 != RecParticleCollection.end()) {
        RecParticleCollection.erase(itr2);
        std::cout << "[MC DELETE] ==> Collection " + str + " has been successfully removed." << std::endl;
    } else if (itr3 != SimulatedHitCollection.end()) {
        SimulatedHitCollection.erase(itr3);
        std::cout << "[MC DELETE] ==> Collection " + str + " has been successfully removed." << std::endl;
    } else if (itr4 != CalorimeterHitCollection.end()) {
        CalorimeterHitCollection.erase(itr4);
        std::cout << "[MC DELETE] ==> Collection " + str + " has been successfully removed." << std::endl;
    } else if (itr5 != StepCollection.end()) {
        StepCollection.erase(itr5);
        std::cout << "[STEP DELETE] ==> Collection " + str + " has been successfully removed." << std::endl;
    } else
        std::cerr << "[WARNING] ==> No Key named " + str + "." << std::endl;
}






