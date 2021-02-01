//
// Created by Zhang Yulei on 9/19/20.
//

#ifndef DANA_DIGITIZER_H
#define DANA_DIGITIZER_H

#include <utility>

#include "Core/AnaProcessor.h"

using namespace std;

class Digitizer : public AnaProcessor {
public:
    // No need to change anything here
    // Must initialized with Name
    explicit Digitizer(string name, shared_ptr<EventStoreAndWriter> evtwrt);
    ~Digitizer() override = default;

    void Begin() override;

    void ProcessEvt(AnaEvent* evt) override;

    void CheckEvt(AnaEvent* evt) override;

    void End() override;

    // Define some functions here if necessary

private:

    // Define some variables if necessary
    // IMPORTANT: variables need to be registered by their types:
    // Currently supported: int, double, string
    double scale_factor{0.};
    double voltageToADC{0.};
    int rangeMin{0};
    int rangeMax{0};
    int pedestal{0};

    // Some output variables which would be recorded in the output root file
    double digitized_total_energy{0.};
    double digitized_total_energy_digitized{0.};
    int digitized_total_No{0};
    int digitized_total_NoGen{0};
};


#endif //DSIMU_EXAMPLEPROCESSOR_H
