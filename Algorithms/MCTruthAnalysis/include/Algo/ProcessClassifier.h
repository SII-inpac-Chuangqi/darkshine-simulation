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

    void initialization();

    bool FoundInitial(AnaEvent* evt);
    
    yulei defineProcessName(bool isFound, AnaEvent* evt, McParticle* mcp = nullptr);

private:

//    AnaEvent* Evt{};

    bool isFoundInit;

    double Children_E;
    double Children_pointZ;
    int    Children_ID;
    int    Children_PDG;

    int n_mu;

    TString ProcessName;
    TString PVName;
    double EndPointZ;
    double ProcessEnergy;

};

#endif //DSIMU_PROCESSCLASSIFIER_H
