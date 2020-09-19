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
    // Must initialized with Name
    explicit ExampleProcessor(string name) : AnaProcessor(std::move(name)) {};
    ~ExampleProcessor() {};

    void Begin() override;

    void ProcessEvt(DEvent* evt) override;

    void CheckEvt(DEvent* evt) override;

    void End() override;

private:

};


#endif //DSIMU_EXAMPLEPROCESSOR_H
