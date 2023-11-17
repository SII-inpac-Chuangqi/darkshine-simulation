//
// Created by xuliang on 2023/11/17.
//

#ifndef DSIMU_RECHCAL_H
#define DSIMU_RECHCAL_H

#include <utility>
#include <iostream>
#include <sstream>

#include "Core/AnaProcessor.h"
#include "Core/AnaData.h"

using namespace std;

class RecHCAL : public AnaProcessor {
public:
    // No need to change anything here
    // Must initialized with Name
    explicit RecHCAL(string name, shared_ptr<EventStoreAndWriter> evtwrt);

    ~RecHCAL() override = default;

    void Begin() override;

    void ProcessEvt(AnaEvent *evt) override;

    void CheckEvt(AnaEvent *evt) override;

    void End() override;

    // Should be called each time at the beginning of ProcessEvt()
    void InitEvt() override {
        HCAL_total.clear();
        HCAL_E_Max_Cell.clear();
        SideHCAL_total.clear();
        SideHCAL_E_Max_Cell.clear();
        HCAL_E_Cali.clear();
    };

    [[nodiscard]] const string &getDescription() const override {
        return Description;
    }
    void ReadCollections();

    double TruthCalibration(int cali_pdg, double E_before_cali);

private:
    // Verbosity
    int verbose{0};
    // Input Parameter
    string hcal_col_use;
    string sidehcal_col_use;
    // Output Variables
    vector<string> hcal_cols;
    vector<string> sidehcal_cols;
    int hcal_col_size;
    int sidehcal_col_size;
    vector<double> HCAL_total;
    vector<double> HCAL_E_Max_Cell;
    vector<double> SideHCAL_total;
    vector<double> SideHCAL_E_Max_Cell;
    vector<double> HCAL_E_Cali;

};

#endif //DSIMU_RECHCAL_H
