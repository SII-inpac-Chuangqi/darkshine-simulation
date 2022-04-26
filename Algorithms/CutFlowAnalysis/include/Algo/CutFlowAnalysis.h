//
// Created by Zhang Yulei on 9/19/20.
//

#ifndef DSIMU_CUTFLOWANALYSIS_H
#define DSIMU_CUTFLOWANALYSIS_H

#include <utility>

#include "Core/AnaProcessor.h"

#include "TH1F.h"

using namespace std;

class CutFlowAnalysis : public AnaProcessor {
public:
    // No need to change anything here
    // Must initialized with Name
    explicit CutFlowAnalysis(string name, shared_ptr<EventStoreAndWriter> evtwrt);

    ~CutFlowAnalysis() override = default;

    void Begin() override;

    void ProcessEvt(AnaEvent *evt) override;

    void CheckEvt(AnaEvent *evt) override;

    void End() override;

    // Define some functions here if necessary


private:

    double *weight;

    std::vector<double> *ECal_seed_x;
};


#endif //DSIMU_CUTFLOWANALYSIS_H
