//
// Created by Zhang Yulei on 9/19/20.
//

#ifndef DSIMU_RECECAL_H
#define DSIMU_RECECAL_H

#include <utility>
#include <unordered_map>

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
    void InitEvt() override {
        E_total.clear();
        E_max.clear();
        E_frac.clear();
        Moments_Lat.clear();
        HCAL_total.clear();
        HCAL_E_Max_Cell.clear();
        SideHCAL_total.clear();
        SideHCAL_E_Max_Cell.clear();
        E_frac_vec.clear();
        Moments_R.clear();
        Moments_X.clear();
        Moments_Y.clear();
        Moments_Z.clear();

        ECAL_ClusterSub_N=0;
        ECAL_ClusterSub_E_total=0;
        ECAL_ClusterSub_NCell_total=0;
        ECAL_Cluster_E.clear();
        ECAL_Cluster_X.clear();
        ECAL_Cluster_Y.clear();
        ECAL_Cluster_Z.clear();
        ECAL_Cluster_Width_X.clear();
        ECAL_Cluster_Width_Y.clear();
        ECAL_Cluster_Width_Z.clear();
        ECAL_Cluster_NCell.clear();
        ECAL_Cluster_NSub.clear();
        ECAL_Cluster_P0.clear();
        ECAL_Cluster_cosTheta.clear();
        ECAL_Cluster_phi.clear();
        ECAL_Cluster_X_cast.clear();
        ECAL_Cluster_Y_cast.clear();

        ECAL_Cluster_N=0;
        ECAL_Cluster_E_total=0;
        ECAL_Cluster_NCell_total=0;
        ECAL_ClusterSub_E.clear();
        ECAL_ClusterSub_X.clear();
        ECAL_ClusterSub_Y.clear();
        ECAL_ClusterSub_Z.clear();
        ECAL_ClusterSub_Width_X.clear();
        ECAL_ClusterSub_Width_Y.clear();
        ECAL_ClusterSub_Width_Z.clear();
        ECAL_ClusterSub_NCell.clear();
        ECAL_ClusterSub_P0.clear();
        ECAL_ClusterSub_P1.clear();
        ECAL_ClusterSub_cosTheta.clear();
        ECAL_ClusterSub_phi.clear();
        ECAL_ClusterSub_X_cast.clear();
        ECAL_ClusterSub_Y_cast.clear();

        ECAL_trkSeed_X.clear();
        ECAL_trkSeed_Y.clear();
        ECAL_trkSeed_POQ.clear();
        ECAL_trkSeed_cosTheta.clear();
        ECAL_trkSeed_phi.clear();

        ECAL_truthSeed_X.clear();
        ECAL_truthSeed_Y.clear();
        ECAL_truthSeed_E.clear();
        ECAL_truthSeed_P.clear();
        ECAL_truthSeed_PDG.clear();
        ECAL_truthSeed_ID.clear();
        ECAL_truthSeed_parentPDG.clear();
        ECAL_truthSeed_parentID.clear();
        ECAL_truthSeed_genProcess.clear();
        ECAL_truthSeed_cosTheta.clear();
        ECAL_truthSeed_phi.clear();
    }

    [[nodiscard]] const string &getDescription() const override {
        return Description;
    }

    void ReadCollections();

private:

    vector<double> HCAL_total;
    vector<double> HCAL_E_Max_Cell;
    vector<double> SideHCAL_total;
    vector<double> SideHCAL_E_Max_Cell;

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

    vector<double> ECAL_trkSeed_X{};
    vector<double> ECAL_trkSeed_Y{};
    vector<double> ECAL_trkSeed_POQ{};
    vector<double> ECAL_trkSeed_cosTheta{};
    vector<double> ECAL_trkSeed_phi{};

    vector<double> ECAL_truthSeed_X{};
    vector<double> ECAL_truthSeed_Y{};
    vector<double> ECAL_truthSeed_E{};
    vector<double> ECAL_truthSeed_P{};
    vector<int> ECAL_truthSeed_PDG{};
    vector<int> ECAL_truthSeed_ID{};
    vector<int> ECAL_truthSeed_parentPDG{};
    vector<int> ECAL_truthSeed_parentID{};
    vector<int> ECAL_truthSeed_genProcess{};
    vector<double> ECAL_truthSeed_cosTheta{};
    vector<double> ECAL_truthSeed_phi{};

    vector<double> ECAL_ECell_XY{};
    vector<double> ECAL_ECell_XZ{};
    vector<double> ECAL_ECell_YZ{};
    vector<double> ECAL_ECell_Z{};
    vector<double> ECAL_ECell_XYZ{};
    vector<int> ECAL_NCell_max_XY{};
    vector<int> ECAL_NCell_XY{};

    vector<double> ECAL_Cluster_E{};
    vector<double> ECAL_Cluster_X{};
    vector<double> ECAL_Cluster_Y{};
    vector<double> ECAL_Cluster_Z{};
    vector<double> ECAL_Cluster_Width_X{};
    vector<double> ECAL_Cluster_Width_Y{};
    vector<double> ECAL_Cluster_Width_Z{};
    vector<int> ECAL_Cluster_NCell{};
    vector<int> ECAL_Cluster_NSub;
    vector<int> ECAL_Cluster_P0{};
    vector<double> ECAL_Cluster_cosTheta{};
    vector<double> ECAL_Cluster_phi{};
    vector<double> ECAL_Cluster_X_cast{};
    vector<double> ECAL_Cluster_Y_cast{};
    double ECAL_Cluster_E_total;
    int ECAL_Cluster_NCell_total;
    int ECAL_Cluster_N;

    vector<double> ECAL_ClusterSub_E{};
    vector<double> ECAL_ClusterSub_X{};
    vector<double> ECAL_ClusterSub_Y{};
    vector<double> ECAL_ClusterSub_Z{};
    vector<double> ECAL_ClusterSub_Width_X{};
    vector<double> ECAL_ClusterSub_Width_Y{};
    vector<double> ECAL_ClusterSub_Width_Z{};
    vector<int> ECAL_ClusterSub_NCell{};
    vector<int> ECAL_ClusterSub_P0{};
    vector<int> ECAL_ClusterSub_P1{};
    vector<double> ECAL_ClusterSub_cosTheta{};
    vector<double> ECAL_ClusterSub_phi{};
    vector<double> ECAL_ClusterSub_X_cast{};
    vector<double> ECAL_ClusterSub_Y_cast{};
    double ECAL_ClusterSub_E_total;
    int ECAL_ClusterSub_NCell_total;
    int ECAL_ClusterSub_N;



    // Verbosity
    int verbose{0};
    int SkipEmpty{0};
    // Input Parameter
    int n_fraction{1};
    int enAda{0};
    int SaveTrackInfo{0};
    int SaveTruthInfo{0};
    string ecal_col_use;
    string hcal_col_use;
    string sidehcal_col_use;

    vector<string> ecal_cols;
    int ecal_col_size;

    vector<string> hcal_cols;
    vector<string> sidehcal_cols;
    int hcal_col_size;
    int sidehcal_col_size;

    int _dNX,_dNY,_dNZ,_SurfaceZ; //geometry parameter extracted from dANA

    int dNX(){return _dNX;};
    int dNY(){return _dNY;};
    int dNZ(){return _dNZ;};
    double SurfaceZ(){return _SurfaceZ;}

    void setNX(int v){_dNX=v;};
    void setNY(int v){_dNY=v;};
    void setNZ(int v){_dNZ=v;};
    void setSurfaceZ(double v){_SurfaceZ=v;};
    // Internal Algorithm Processors

    struct Esorter_descendingCluster
    {
        inline bool operator() (const std::map<std::string, double>& o1, const std::map<std::string, double>& o2)
        {
            return (o1.at("E") > o2.at("E"));
        }
    };
};


#endif //DSIMU_RECECAL_H
