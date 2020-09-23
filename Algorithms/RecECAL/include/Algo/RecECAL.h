//
// Created by Zhang Yulei on 9/19/20.
//

#ifndef DSIMU_RECECAL_H
#define DSIMU_RECECAL_H

#include <utility>

#include "Core/AnaProcessor.h"

using namespace std;

class RecECAL : public AnaProcessor {
public:
    // No need to change anything here
    // Must initialized with Name
    explicit RecECAL(string name) : AnaProcessor(std::move(name)) {};
    ~RecECAL() {};

    void Begin() override;

    void ProcessEvt(DEvent* evt) override;

    void CheckEvt(DEvent* evt) override;

    void End() override;

    // Define some functions here if necessary

private:

    // Define some variables if necessary
    // IMPORTANT: variables need to be registered by their types:
    // Currently supported: int, double, string
    int intVar;
    int verbose;
    double doubleVar;
    std::string strVar;
};


#endif //DSIMU_RECECAL_H
