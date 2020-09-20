//
// Created by Zhang Yulei on 9/19/20.
//

#include "Object/DEvent.h"

void DEvent::Initialization() {

    for (auto itr : MCParticleCollection ) {
        itr.second->clear();
        itr.second->shrink_to_fit();
    }
    MCParticleCollection.clear();

    for (auto itr : RecParticleCollection ) {
        itr.second->clear();
        itr.second->shrink_to_fit();
    }
    RecParticleCollection.clear();

    for (auto itr : SimulatedHitCollection ) {
        itr.second->clear();
        itr.second->shrink_to_fit();
    }
    SimulatedHitCollection.clear();

    for (auto itr : CalorimeterHitCollection ) {
        itr.second->clear();
        itr.second->shrink_to_fit();
    }
    CalorimeterHitCollection.clear();

    for (auto itr : StepCollection ) {
        itr.second->clear();
        itr.second->shrink_to_fit();
    }
    StepCollection.clear();
}

DStepVec *DEvent::RegisterStepCollection(const std::string &str) {
    if (StepCollection.count(str) != 0) {
        std::cerr<<"[WARNING] ==> Key already exists. Return the existing Key value."<<std::endl;
        return StepCollection.at(str);
    }
    auto tmpVec = new DStepVec;
    StepCollection.insert(std::pair<std::string, DStepVec*>(str, tmpVec) );

    if (Verbose > 1) {
        std::cout << "[STEP REGISTER] ==> A new collection " + str +
                     " has been successfully added to MCParticle Collection." << std::endl;
    }

    return tmpVec;
}

MCParticleVec* DEvent::RegisterMCParticleCollection(const std::string & str) {
    if (MCParticleCollection.count(str) != 0) {
        std::cerr<<"[WARNING] ==> Key already exists. Return the existing Key value."<<std::endl;
        return MCParticleCollection.at(str);
    }
    auto tmpVec = new MCParticleVec;
    MCParticleCollection.insert(std::pair<std::string, MCParticleVec*>(str, tmpVec) );

    if (Verbose > 1) {
        std::cout << "[MC REGISTER] ==> A new collection " + str +
                     " has been successfully added to MCParticle Collection." << std::endl;
    }

    return tmpVec;
}


RecParticleVec *DEvent::RegisterRecParticleCollection(const std::string & str) {
    if (RecParticleCollection.count(str) != 0) {
        std::cerr<<"[WARNING] ==> Key already exists. Return the existing Key value."<<std::endl;
        return RecParticleCollection.at(str);
    }
    auto tmpVec = new RecParticleVec;
    RecParticleCollection.insert(std::pair<std::string, RecParticleVec*>(str, tmpVec) );

    if (Verbose > 1) {
        std::cout << "[REC REGISTER] ==> A new collection " + str +
                     " has been successfully added to RecParticle Collection." << std::endl;
    }

    return tmpVec;
}

SimulatedHitVec *DEvent::RegisterSimulatedHitCollection(const std::string & str) {
    if (SimulatedHitCollection.count(str) != 0) {
        std::cerr<<"[WARNING] ==> Key already exists. Return the existing Key value."<<std::endl;
        return SimulatedHitCollection.at(str);
    }
    auto tmpVec = new SimulatedHitVec;
    SimulatedHitCollection.insert(std::pair<std::string, SimulatedHitVec*>(str, tmpVec) );

    if (Verbose > 1) {
        std::cout << "[MC REGISTER] ==> A new collection " + str +
                     " has been successfully added to SimulatedHit Collection." << std::endl;
    }

    return tmpVec;
}

CalorimeterHitVec *DEvent::RegisterCalorimeterHitCollection(const std::string & str) {
    if (CalorimeterHitCollection.count(str) != 0) {
        std::cerr<<"[WARNING] ==> Key already exists. Return the existing Key value."<<std::endl;
        return CalorimeterHitCollection.at(str);
    }
    auto tmpVec = new CalorimeterHitVec;
    CalorimeterHitCollection.insert(std::pair<std::string, CalorimeterHitVec*>(str, tmpVec) );

    if (Verbose > 1) {
        std::cout << "[REC REGISTER] ==> A new collection " + str +
                     " has been successfully added to CalorimterHit Collection" << std::endl;
    }

    return tmpVec;
}

std::vector<std::string> *DEvent::ListAllCollections() {
    auto tmp = new std::vector<std::string>;

    auto s = ListCollections(StepCollection);
    tmp->insert(tmp->end(), s->begin(), s->end());

    s = ListCollections(MCParticleCollection);
    tmp->insert(tmp->end(), s->begin(), s->end());

    s = ListCollections(RecParticleCollection);
    tmp->insert(tmp->end(), s->begin(), s->end());

    s = ListCollections(SimulatedHitCollection);
    tmp->insert(tmp->end(), s->begin(), s->end());

    s = ListCollections(CalorimeterHitCollection);
    tmp->insert(tmp->end(), s->begin(), s->end());



    return tmp;
}

void DEvent::DeleteCollection(const std::string & str) {
    auto itr1 = MCParticleCollection.find(str);
    auto itr2 = RecParticleCollection.find(str);
    auto itr3 = SimulatedHitCollection.find(str);
    auto itr4 = CalorimeterHitCollection.find(str);
    auto itr5 = StepCollection.find(str);

    if (itr1 != MCParticleCollection.end() ) {
        MCParticleCollection.erase(itr1);
        std::cout<<"[MC DELETE] ==> Collection "+str+" has been successfully removed."<<std::endl;
    }
    else if (itr2 != RecParticleCollection.end() ) {
        RecParticleCollection.erase(itr2);
        std::cout<<"[MC DELETE] ==> Collection "+str+" has been successfully removed."<<std::endl;
    }
    else if (itr3 != SimulatedHitCollection.end() ) {
        SimulatedHitCollection.erase(itr3);
        std::cout<<"[MC DELETE] ==> Collection "+str+" has been successfully removed."<<std::endl;
    }
    else if (itr4 != CalorimeterHitCollection.end() ) {
        CalorimeterHitCollection.erase(itr4);
        std::cout<<"[MC DELETE] ==> Collection "+str+" has been successfully removed."<<std::endl;
    }
    else if (itr5 != StepCollection.end() ) {
        StepCollection.erase(itr5);
        std::cout<<"[STEP DELETE] ==> Collection "+str+" has been successfully removed."<<std::endl;
    }
    else
        std::cerr<<"[WARNING] ==> No Key named "+str+"."<<std::endl;
}






