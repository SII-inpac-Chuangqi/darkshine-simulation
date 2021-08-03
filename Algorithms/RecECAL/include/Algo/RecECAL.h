//
// Created by Zhang Yulei on 9/19/20.
//

#ifndef DSIMU_RECECAL_H
#define DSIMU_RECECAL_H

#include <utility>

#include "Core/AnaProcessor.h"
#include "Algo/Cluster_Analysis.h"

using namespace std;

class RecECAL : public AnaProcessor {
public:
    // No need to change anything here
    // Must initialized with Name
    explicit RecECAL(string name, shared_ptr<EventStoreAndWriter> evtwrt);

    ~RecECAL() override = default;

    void Begin() override;

    void ProcessEvt(AnaEvent *evt) override;

    void CheckEvt(AnaEvent *evt) override;

    void End() override;

    // Define some functions here if necessary
    void initialization() {
        E_total.clear();
        E_max.clear();
        E_frac.clear();
        Moments_Lat.clear();
        HCAL_total.clear();
        HCAL_E_Max_Cell.clear();
        E_frac_vec.clear();
        Moments_R.clear();
        Moments_X.clear();
        Moments_Y.clear();
        Moments_Z.clear();
    }

    [[nodiscard]] const string &getDescription() const override {
        return Description;
    }

    void ReadCollections();

private:

    vector<double> HCAL_total;
    vector<double> HCAL_E_Max_Cell;

    // output variables
    vector<double> E_total{};
    vector<double> E_max{};
    vector<double> E_frac{};
    vector<double> Moments_Lat{};
    // Move to vector for multi-output
//    double E_frac_vec[8] = {0.}; // 5 10 20 50 100 125 150 200
//    double Moments_R[4] = {0.};
//    double Moments_X[4] = {0.};
//    double Moments_Y[4] = {0.};
//    double Moments_Z[4] = {0.};
    vector<vector<double>> E_frac_vec;
    vector<vector<double>> Moments_R;
    vector<vector<double>> Moments_X;
    vector<vector<double>> Moments_Y;
    vector<vector<double>> Moments_Z;

    // Verbosity
    int verbose{0};
    // Input Parameter
    int n_fraction{1};
    string ecal_col_use;
    string hcal_col_use;

    vector<string> ecal_cols;
    int ecal_col_size;

    vector<string> hcal_cols;
    int hcal_col_size;

    // Internal Algorithm Processors
};


#endif //DSIMU_RECECAL_H
