//
// Created by Zhang Yulei on 9/19/20.
//

#include "Core/AlgoManager.h"

void AlgoManager::RegisterAnaProcessor(AnaProcessor *AnaP) {
    AnaProcessors.insert(std::pair<std::string, AnaProcessor*>(AnaP->getName(), AnaP));
}

void AlgoManager::BeginAnaProcessors() {
    for ( auto itr : AnaProcessors)
        itr.second->Begin();
}

void AlgoManager::ProcessEvtAnaProcessors(DEvent* evt) {
    for ( auto itr : AnaProcessors)
        itr.second->ProcessEvt(evt);
}

void AlgoManager::CheckEvtAnaProcessors(DEvent* evt) {
    for ( auto itr : AnaProcessors)
        itr.second->CheckEvt(evt);
}

void AlgoManager::EndAnaProcessors() {
    for ( auto itr : AnaProcessors)
        itr.second->End();
}
