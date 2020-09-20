//
// Created by Zhang Yulei on 9/19/20.
//

#include "Core/AlgoManager.h"
#include <sstream>

void AlgoManager::RegisterAnaProcessor(AnaProcessor *AnaP) {
    if ( AnaProcessors.count(AnaP->getName()) != 0 )
        std::cerr<<"[WARNING] ==> Algo Processor Name already exists."<<std::endl;
    else
        AnaProcessors.insert(std::pair<std::string, AnaProcessor*>(AnaP->getName(), AnaP));
}

void AlgoManager::BeginAnaProcessors() {
    for ( const auto& itr : AnaProcessorList)
        AnaProcessors.at(itr)->Begin();
}

void AlgoManager::ProcessEvtAnaProcessors(DEvent* evt) {
    for ( const auto& itr : AnaProcessorList)
        AnaProcessors.at(itr)->ProcessEvt(evt);
}

void AlgoManager::CheckEvtAnaProcessors(DEvent* evt) {
    for ( const auto& itr : AnaProcessorList)
        AnaProcessors.at(itr)->CheckEvt(evt);
}

void AlgoManager::EndAnaProcessors() {
    for ( const auto& itr : AnaProcessorList)
        AnaProcessors.at(itr)->End();
}

AnaProcessorVec* AlgoManager::getAllAnaProcessors() {
    auto tmp = new AnaProcessorVec();
    for ( const auto& itr : AnaProcessors)
        tmp->emplace_back(itr.second);

    return tmp;
}

void AlgoManager::SetAnaProcessorsList(const std::string& ProcessorList) {

    istringstream sin(ProcessorList);
    do {
        std::string ProcessorName;
        sin >> ProcessorName;

        if ( !ProcessorName.empty()) {
            if (std::find(AnaProcessorList.begin(), AnaProcessorList.end(), ProcessorName) == AnaProcessorList.end()) {
                cout << "\"" << ProcessorName << "\"" << endl;
                AnaProcessorList.emplace_back(ProcessorName);
            }
            else
                std::cerr<<"[WARNING] ==> Duplicate Algo Processor Name."<<std::endl;
        }
    } while (sin);
}
