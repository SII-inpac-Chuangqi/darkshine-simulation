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
    explicit RecECAL(string name, shared_ptr<EventStoreAndWriter> evtwrt) : AnaProcessor(std::move(name), std::move(evtwrt)) {};
    ~RecECAL() override {};

    void Begin() override;

    void ProcessEvt(AnaEvent* evt) override;

    void CheckEvt(AnaEvent* evt) override;

    void End() override;

    // Define some functions here if necessary
    void initialization() {
        center_x = 0.;
        center_y = 0.;
        mc_x = 0.;
        mc_y = 0.;
        err_x = 0.;
        err_y = 0.;
    }

    double SingleCenterFinding(const SimulatedHitVecUniPtr&, const DStepVecUniPtr &);

private:

    int FindCenter{0};

    double center_x{0.};
    double center_y{0.};
    double mc_x{0.};
    double mc_y{0.};
    double err_x{0.};
    double err_y{0.};

    // Verbosity
    int verbose;
    // Input Parameter
    double W0{0.};

};


#endif //DSIMU_RECECAL_H
