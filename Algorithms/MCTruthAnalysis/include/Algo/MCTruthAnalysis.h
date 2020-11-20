//
// Created by Zhang Yulei on 9/19/20.
//

#ifndef DSIMU_MCTruthAnalysis_H
#define DSIMU_MCTruthAnalysis_H

#include <utility>

#include "Core/AnaProcessor.h"
#include "Algo/SecondaryMaxEFinder.h"

using namespace std;

class MCTruthAnalysis : public AnaProcessor {
public:
    // No need to change anything here
    // Must initialized with Name
    explicit MCTruthAnalysis(string name, shared_ptr<EventStoreAndWriter> evtwrt);

    ~MCTruthAnalysis() override = default;

    void Begin() override;

    void ProcessEvt(AnaEvent *evt) override;

    void CheckEvt(AnaEvent *evt) override;

    void End() override;

    // Define some functions here if necessary

private:

    int verbose{0};

    std::shared_ptr<SecondaryMaxEFinder> SecFinder;

};


#endif //DSIMU_MCTruthAnalysis_H
