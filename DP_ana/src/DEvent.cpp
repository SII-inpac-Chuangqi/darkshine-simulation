//
// Created by Zhang Yulei on 9/19/20.
//

#include "Object/DEvent.h"

void DEvent::Initialization() {

    for (auto itr : MCParticleCollection ) {
        itr.second->clear();
        itr.second->shrink_to_fit();
    }
    for (auto itr : RecParticleCollection ) {
        itr.second->clear();
        itr.second->shrink_to_fit();
    }
    for (auto itr : SimulatedHitCollection ) {
        itr.second->clear();
        itr.second->shrink_to_fit();
    }
    for (auto itr : CalorimeterHitCollection ) {
        itr.second->clear();
        itr.second->shrink_to_fit();
    }
}

MCParticleVec* DEvent::RegisterMCParticleCollection(const std::string & str) {
    if (MCParticleCollection.count(str) != 0) {
        std::cerr<<"[WARNING] ==> Key already exists. Return the existing Key value."<<std::endl;
        return MCParticleCollection.at(str);
    }
    auto tmpVec = new MCParticleVec;
    MCParticleCollection.insert(std::pair<std::string, MCParticleVec*>(str, tmpVec) );
    std::cout<<"[MC REGISTER] ==> A new collection "+str+" has been successfully added to MCParticle Collection."<<std::endl;
    return tmpVec;
}


RecParticleVec *DEvent::RegisterRecParticleCollection(const std::string & str) {
    if (RecParticleCollection.count(str) != 0) {
        std::cerr<<"[WARNING] ==> Key already exists. Return the existing Key value."<<std::endl;
        return RecParticleCollection.at(str);
    }
    auto tmpVec = new RecParticleVec;
    RecParticleCollection.insert(std::pair<std::string, RecParticleVec*>(str, tmpVec) );
    std::cout<<"[REC REGISTER] ==> A new collection "+str+" has been successfully added to RecParticle Collection."<<std::endl;
    return tmpVec;
}

SimulatedHitVec *DEvent::RegisterSimulatedHitCollection(const std::string & str) {
    if (SimulatedHitCollection.count(str) != 0) {
        std::cerr<<"[WARNING] ==> Key already exists. Return the existing Key value."<<std::endl;
        return SimulatedHitCollection.at(str);
    }
    auto tmpVec = new SimulatedHitVec;
    SimulatedHitCollection.insert(std::pair<std::string, SimulatedHitVec*>(str, tmpVec) );
    std::cout<<"[MC REGISTER] ==> A new collection "+str+" has been successfully added to SimulatedHit Collection."<<std::endl;
    return tmpVec;
}

CalorimeterHitVec *DEvent::RegisterCalorimeterHitCollection(const std::string & str) {
    if (CalorimeterHitCollection.count(str) != 0) {
        std::cerr<<"[WARNING] ==> Key already exists. Return the existing Key value."<<std::endl;
        return CalorimeterHitCollection.at(str);
    }
    auto tmpVec = new CalorimeterHitVec;
    CalorimeterHitCollection.insert(std::pair<std::string, CalorimeterHitVec*>(str, tmpVec) );
    std::cout<<"[REC REGISTER] ==> A new collection "+str+" has been successfully added to CalorimterHit Collection"<<std::endl;
    return tmpVec;
}

std::vector<std::string> *DEvent::ListAllCollections() {
    auto tmp = new std::vector<std::string>;

    auto s = ListCollections(MCParticleCollection);
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
    else
        std::cerr<<"[WARNING] ==> No Key named "+str+"."<<std::endl;
}




