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

    int* TagTrk2_track_No{};
    int* RecTrk2_track_No{};

    double* TagTrk2_pp{};
    double* RecTrk2_pp{};
    double* ECAL_E_total{};
    double* HCAL_E_total{};
    double* HCAL_E_Max_Cell{};

    double momentum_diff{};
    double ECAL_E_Min{};
    double HCAL_E_Max{};
    double HCAL_E_Cell_Max{};

    std::string weight;

    TH1F *h_cut{};
    TH1F *h_tmp{};
    vector<std::string> cut_chain;

    int save_new{};
};


#endif //DSIMU_CUTFLOWANALYSIS_H
