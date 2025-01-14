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
        HCAL_Particle_Pdg.clear();
        HCAL_Particle_X.clear();
        HCAL_Particle_Y.clear();
        HCAL_Particle_Z.clear();
        HCAL_Particle_Px.clear();
        HCAL_Particle_Py.clear();
        HCAL_Particle_Pz.clear();
        HCAL_Particle_E.clear();
        HCAL_Particle_Process.clear();
        HCAL_Hit_X.clear();
        HCAL_Hit_Y.clear();
        HCAL_Hit_Z.clear();
        HCAL_Hit_IDX.clear();
        HCAL_Hit_IDY.clear();
        HCAL_Hit_IDZ.clear();
        HCAL_Cell_E.clear();
        SideHCAL_Hit_X.clear();
        SideHCAL_Hit_Y.clear();
        SideHCAL_Hit_Z.clear();
        SideHCAL_Hit_IDX.clear();
        SideHCAL_Hit_IDY.clear();
        SideHCAL_Hit_IDZ.clear();
        SideHCAL_Cell_E.clear();
        HCAL_E_front = 0.;
        HCAL_E_back = 0.;
        HCAL_layer1_E.clear();
        HCAL_X_average.clear();
        HCAL_Y_average.clear();
        HCAL_E_per_layer.clear();
        HCAL_E_Min_Cell.clear();
        HCAL_E_concentration.clear();
        HCAL_E_Max_layer.clear();
        HCAL_E_Max_layer_num.clear();
        HCAL_lighted_cells_per_lighted_layer.clear();
        HCAL_total_length.clear();
        HCAL_total_lighted_layer.clear();
        HCAL_E_main_side_ratio.clear();
    };

    [[nodiscard]] const string &getDescription() const override {
        return Description;
    }
    void ReadCollections();

    double TruthCalibration(int cali_pdg, double E_before_cali);

private:
    // Verbosity
    int verbose{0};
    // extra info
    int particle_store;
    int hit_store;
    int bdt_var_store;
    // Input Parameter
    string hcal_col_use;
    string sidehcal_col_use;
    // Output Variables
    vector<string> hcal_cols;
    vector<string> sidehcal_cols;
    int hcal_col_size;
    int sidehcal_col_size;
    double HCAL_E_front;
    double HCAL_E_back;
    vector<double> HCAL_total;
    vector<double> HCAL_E_Max_Cell;
    vector<double> SideHCAL_total;
    vector<double> SideHCAL_E_Max_Cell;
    vector<double> HCAL_E_Cali;
    vector<int>    HCAL_Particle_Pdg;
    vector<double> HCAL_Particle_X;
    vector<double> HCAL_Particle_Y;
    vector<double> HCAL_Particle_Z;
    vector<double> HCAL_Particle_Px;
    vector<double> HCAL_Particle_Py;
    vector<double> HCAL_Particle_Pz;
    vector<double> HCAL_Particle_E;
    vector<std::string> HCAL_Particle_Process;
    vector<double> HCAL_Hit_X;
    vector<double> HCAL_Hit_Y;
    vector<double> HCAL_Hit_Z;
    vector<int> HCAL_Hit_IDX;
    vector<int> HCAL_Hit_IDY;
    vector<int> HCAL_Hit_IDZ;
    vector<double> HCAL_Cell_E;
    vector<double> SideHCAL_Hit_X;
    vector<double> SideHCAL_Hit_Y;
    vector<double> SideHCAL_Hit_Z;
    vector<int> SideHCAL_Hit_IDX;
    vector<int> SideHCAL_Hit_IDY;
    vector<int> SideHCAL_Hit_IDZ;
    vector<double> SideHCAL_Cell_E;
    vector<double> HCAL_layer1_E;
    vector<double> HCAL_X_average;
    vector<double> HCAL_Y_average;
    vector<double> HCAL_E_per_layer;
    vector<double> HCAL_E_Min_Cell;
    vector<double> HCAL_E_concentration;
    vector<double> HCAL_E_Max_layer;
    vector<int> HCAL_E_Max_layer_num;
    vector<double> HCAL_lighted_cells_per_lighted_layer;
    vector<int> HCAL_total_length;
    vector<int> HCAL_total_lighted_layer;
    vector<double> HCAL_E_main_side_ratio;


};

#endif //DSIMU_RECHCAL_H
