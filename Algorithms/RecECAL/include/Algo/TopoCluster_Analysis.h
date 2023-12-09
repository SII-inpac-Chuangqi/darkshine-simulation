//
// Created by Zhang Yulei on 12/27/20.
//

#ifndef DANA_TOPOCLUSTER_ANALYSIS_H
#define DANA_TOPOCLUSTER_ANALYSIS_H

#include "Core/AnaProcessor.h"
#include "Core/AnaData.h"

//never include any templete header like Eigen, XYZVector, so on. see https://stackoverflow.com/questions/67529835/eigen3-take-a-long-time-to-compile-and-very-slow-when-debug

#include "Algo/Cluster_Analysis.h"

#ifndef _DNAN
    #define _DNAN (nan(""))
#endif

#include "Algo/CHit.h"

/// \brief Base Analysis Class for Cluster
/// \note The input data vector will be automatically sorted in desending order by energy
class TopoCluster_Analysis : public Cluster_Analysis {
public:
    TopoCluster_Analysis() = default;
    //explicit: prevent accidental conversion from clusterVec to TopoCluster_Analysis
    explicit TopoCluster_Analysis(CalorimeterHitVec* clusterVec, bool isStaggered=true);

    virtual ~TopoCluster_Analysis() = default;
    // this could be override by PFTopo
    virtual bool Do(std::vector<std::map<std::string, double>> *ret1,std::vector<std::map<std::string, double>> *ret2, 
                CHitVec *dump=nullptr);
    
    // parameters
    void set_Enoise(double v){Enoise=v;};
    void set_EThres_S(double v){EThres_S=v;};
    void set_EThres_N(double v){EThres_N=v;};
    void set_EThres_P(double v){EThres_P=v;};
    void set_Critical_E(double v){Critical_E=v;};
    void set_Critical_N(double v){Critical_N=v;};
    void set_EM_SCALE_LENGTH_mm(double v){EM_SCALE_LENGTH_mm=v;};
    void set_EM_ENERGY_SCALE_MeV(double v){EM_ENERGY_SCALE_MeV=v;};
    void set_ENERGY_SHIFT_MeV(double v){ENERGY_SHIFT_MeV=v;};
    void set_weight_type(int v){weight_type=static_cast<WEIGHT_TYPE>(v);};

    double get_Enoise(){return Enoise;};
    double get_EThres_S(){return EThres_S;};
    double get_EThres_N(){return EThres_N;};
    double get_EThres_P(){return EThres_P;};
    double get_Critical_E(){return Critical_E;};
    double get_Critical_N(){return Critical_N;};
    double get_EM_SCALE_LENGTH_mm(){return EM_SCALE_LENGTH_mm;};
    double get_EM_ENERGY_SCALE_MeV(){return EM_ENERGY_SCALE_MeV;};
    double get_ENERGY_SHIFT_MeV(){return ENERGY_SHIFT_MeV;};
    int get_weight_type(){return static_cast<int>(weight_type);};

    std::string printCell(CHit* cell);

protected:
    bool makeSortedCenterIdNeighborsCHitMap();

    [[nodiscard]] double calDistance(CHit* h, const TVector3& loc); //input unit: cm; ouput unit: EM scale (5cm)
    [[nodiscard]] double calWeight(double E1,  double E2, double d1, double d2); //calculate the weight for splitting

    void calXYZ(const CHitVec cluster,double ret[]){return calXYZ(_DUMMY, _DUMMY, cluster, ret);}; //P0==-1 means fail CHit. P0==_DUMMY mean dummay value used for main cluster
    [[nodiscard]] bool calXYEW(const CHitVec cluster,int Z, double ret[]){return calXYEW(_DUMMY, _DUMMY, cluster, Z, ret);};
    [[nodiscard]] bool calEtaPhiXY(const CHitVec cluster,double ret[]){return calEtaPhiXY(_DUMMY, _DUMMY, cluster, ret);};
    [[nodiscard]] double calE(const CHitVec cluster){return calE(_DUMMY,_DUMMY,cluster);};
    [[nodiscard]] int calNCell(const CHitVec cluster){return calNCell(_DUMMY,_DUMMY,cluster);};

    void calXYZ(int P0, int P1, const CHitVec cluster, double ret[]);
    [[nodiscard]] bool calXYEW(int P0, int P1, const CHitVec cluster,int Z, double ret[]);
    [[nodiscard]] bool calEtaPhiXY(int P0, int P1, const CHitVec cluster, double ret[]);
    [[nodiscard]] double calE(int P0, int P1, const CHitVec cluster);
    [[nodiscard]] int calNCell(int P0, int P1, const CHitVec cluster);

    void calXYZCellWidth(const CHitVec cluster, double ret[]){calXYZCellWidth(_DUMMY,_DUMMY,cluster,ret);}; 
    void calXYZCellWidth(int P0, int P1, const CHitVec cluster, double ret[]); 

    std::vector<TVector3> _POS{};
    double _SurfaceZ{-999};

    const std::vector<TVector3>& POS(){return _POS;}; //const??
    void setPOS(const std::vector<TVector3>& v){_POS=v;};
    double SurfaceZ(){return _SurfaceZ;};
    void setSurfaceZ(double v){_SurfaceZ=v;};

    const TVector3& toPos(CHit* h); //make it const????

    // configs
    double Enoise{1}; // MeV or Digit
    double EThres_S{4}; // *Enoise
    double EThres_N{2}; // *Enoise
    double EThres_P{0}; // *Enoise
    double Critical_E{100}; // MeV or Digit
    double Critical_N{4};
    // energy weighting
    double EM_SCALE_LENGTH_mm{50};
    double EM_ENERGY_SCALE_MeV{1};
    double ENERGY_SHIFT_MeV{0};
    enum WEIGHT_TYPE 
    {  linear=0,
       log=1
    } weight_type{linear};

    // Hard-coded weight function 
    // "w=log((E+delta)/scale+1)"
    double log_weight(double E){return std::log( 1 + (E+ENERGY_SHIFT_MeV)/EM_ENERGY_SCALE_MeV);};
     // "w=E/scale"
    double linear_weight(double E){return (E+ENERGY_SHIFT_MeV)/EM_ENERGY_SCALE_MeV;};

    double WEIGHT(double E){
        switch(weight_type){
            case linear:
                return linear_weight(E);
                break;
            case log:
                return log_weight(E);
                break;
            default:
                return E;
        }
    }

    //storage
    CHitVec allhits; // all CHits used to handle clustering info, converted from calorimeter
    std::vector<CHit*> POSMAP;
    std::vector<CHitVec> clusters; // clusteded information
    std::vector<int> N_subcluster{}; // sub cluster information
    std::vector<CHitVec> centerIdNeighborsCHit;

    bool Clustering();
    bool ConvHits();
    bool MakeHMAP();
    bool MakePOSMAP();
};


#endif //DANA_TOPOCLUSTER_ANALYSIS_H
