//
// Created by Zhang Yulei on 9/19/20.
//

#ifndef DSIMU_EXAMPLEPROCESSOR_H
#define DSIMU_EXAMPLEPROCESSOR_H

#include <utility>

#include "Core/AnaProcessor.h"

using namespace std;

class ExampleProcessor : public AnaProcessor {
public:
    // No need to change anything here
    // Must initialized with Name
    explicit ExampleProcessor(string name, shared_ptr<EventStoreAndWriter> evtwrt) : AnaProcessor(std::move(name), std::move(evtwrt)) {};
    ~ExampleProcessor() {};

    void Begin() override;

    void ProcessEvt(AnaEvent* evt) override;

    void CheckEvt(AnaEvent* evt) override;

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


#endif //DSIMU_EXAMPLEPROCESSOR_H
