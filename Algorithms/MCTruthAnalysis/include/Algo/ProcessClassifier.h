//
// Created by Danning Liu on 2021/9/10
//

#ifndef DSIMU_PROCESSCLASSIFIER_H
#define DSIMU_PROCESSCLASSIFIER_H

#include <utility>
#include <tuple>
#include "TString.h"
#include "Object/McParticle.h"
#include "Object/DStep.h"
#include "Core/AnaProcessor.h"

typedef std::tuple<TString, TString, double, double> yulei;

class ProcessClassifier {

public:

    explicit ProcessClassifier(const shared_ptr<EventStoreAndWriter> &evtWrt) : EvtWrt(evtWrt) {}

    void initialization();

    void RegisterParameters();
    
    yulei defineProcessName(AnaEvent* evt, McParticle* mcp = nullptr);

private:

    TString ProcessName;
    TString PVName;
    double Process_Vertex_Z;
    double ProcessEnergy;

    const shared_ptr<EventStoreAndWriter> &EvtWrt;
};

#endif //DSIMU_PROCESSCLASSIFIER_H
