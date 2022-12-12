//
// Created by Zhang Yulei on 9/19/20.
//

#ifndef DSIMU_MCTruthAnalysis_H
#define DSIMU_MCTruthAnalysis_H

#include <utility>

#include "Core/AnaProcessor.h"
#include "Algo/SecondaryMaxEFinder.h"
#include "Algo/ProcessReader.h"

using namespace std;

class MCTruthAnalysis : public AnaProcessor {
public:
    // No need to change anything here
    // Must initialized with Name
    explicit MCTruthAnalysis(string name, shared_ptr<EventStoreAndWriter> evtwrt);

    ~MCTruthAnalysis() override = default;

    void Begin() override;

    void InitEvt() override
    {
        Initial_Px = RETURN;
        Initial_Py = RETURN;
        Initial_Pz = RETURN;
        Initial_X = RETURN;
        Initial_Y = RETURN;
        Initial_Z = RETURN;

        Recoil_E = RETURN;
        Recoil_P[0] = RETURN; Recoil_P[1] = RETURN; Recoil_P[2] = RETURN;
        Recoil_pT = RETURN;
        Recoil_theta = RETURN;

        Parent_E = RETURN;
        Parent_P[0] = RETURN; Parent_P[1] = RETURN; Parent_P[2] = RETURN;
        Parent_PVName = "";

        Pi = RETURN;
        Pf = RETURN;

        Truth_P[0] = RETURN; Truth_P[1] = RETURN; Truth_P[2] = RETURN;
        Truth_Recoil_E = RETURN;
        Truth_Recoil_theta = RETURN;
        Truth_Recoil_pT = RETURN;
        trans_sep = RETURN;
        
        Decay_X = RETURN;
        Decay_Y = RETURN;
        Decay_Z = RETURN;
    }

    void ProcessEvt(AnaEvent *evt) override;

    void CheckEvt(AnaEvent *evt) override;

    void End() override;


private:

    int verbose{0};
    int Sec_PDG{0};

    std::shared_ptr<SecondaryMaxEFinder> SecFinder;
    std::shared_ptr<ProcessReader> ProcReader;

    int Initial_PDG{};
    double Initial_Px{};
    double Initial_Py{};
    double Initial_Pz{};
    double Initial_X{};
    double Initial_Y{};
    double Initial_Z{};

    // For Recoiled Electron
    double Recoil_E{};
    double Recoil_P[3] = {0};
    double Recoil_pT{};
    double Recoil_theta{};

    // For Dark Matter
    double Parent_E{};
    double Parent_P[3] = {0};
    TString Parent_PVName{};

    // For initial particle analysis
    double Pi{0.};
    double Pf{0.};

    double trans_sep{0};
    double Truth_P[3]={0};
    double Truth_Recoil_E;
    double Truth_Recoil_theta;
    double Truth_Recoil_pT;
    //double trans_sep;
    double Signal_Detectable=1;
    // For visible decay
    double Decay_X{0};
    double Decay_Y{0};
    double Decay_Z{0};

};


#endif //DSIMU_MCTruthAnalysis_H
