//
// Created by Zhang Yulei on 12/27/20.
//

#ifndef DSIMU_TOPOCLUSTER_ANALYSIS_H
#define DSIMU_TOPOCLUSTER_ANALYSIS_H

#include "Core/AnaProcessor.h"
#include "Core/AnaData.h"

#include <cassert> 
#include <numeric> 
#include <vector>
#include <array>
#include <set>
#include <utility>
#include <algorithm>
#include "TVector3.h"
//never include any templete header like Eigen, XYZVector, so on. see https://stackoverflow.com/questions/67529835/eigen3-take-a-long-time-to-compile-and-very-slow-when-debug

#include "Algo/Cluster_Analysis.h"

#ifndef _DNAN
    #define _DNAN (nan(""))
#endif

#ifndef MAX_ECAL_CELLS // defined in AnaData as shared information
    #define MAX_ECAL_CELLS (25*25*15)
#endif

#ifdef CLUSTER_DEBUG
    #pragma message "The maximum support Ncell now is (increase if you need and pay attention of the stack)" 
    //#pragma message MAX_ECAL_CELLS
#endif

//HMAP is larger than cell, to allow for the overflwo/underflow bin
#if MAX_ECAL_CELLS < 1000
    #define HMAP_LENGTH 10000
#else
    #define HMAP_LENGTH (MAX_ECAL_CELLS*2)
#endif
//used for dummy value for cluster id
#define _DUMMY -2

template<class CHit>
bool Esorter_ascendingC( std::shared_ptr<CHit> a,  std::shared_ptr<CHit> b) { return (a->E() < b->E()); } // ascending
template<class CHit>
bool Esorter_descendingC( std::shared_ptr<CHit> a,  std::shared_ptr<CHit> b) { return (a->E() > b->E()); } // descending

class CHit { 
    public: 
        CHit(CalorimeterHit *h){
            _E=h->getE();
            _X=h->getCellIdX(); //this should start from 1
            _Y=h->getCellIdY();
            _Z=h->getCellIdZ();
            hit=h; // keep the link to reco hit
        };
        #ifdef CLUSTER_DEBUG
            CHit(SimulatedHit *h){ // only used for debugging...
                _E=h->getE();
                _X=h->getCellIdX(); //this should start from 1
                _Y=h->getCellIdY();
                _Z=h->getCellIdZ();
                isTruth=true;
                hit=h; // keep the link to truth hit
            };
        #endif
        double E(){return _E;};
        double E(int P0, int P1){
            assert(P0 >= 0);
            if(isClustered(P0,P1)){
                return (P1==_P1)?E_sub1():E_sub2();
            }else
                return 0;
        };

        int X(){return _X;};
        int Y(){return _Y;};
        int Z(){return _Z;};
        int P0(){return _P0;};
        int P1(){return _P1;};
        int P2(){return _P2;};
        bool isClustered(int P0, int P1){
            assert(P0 >= 0);
            return (P0==_P0 && (P1==_P1 || P1==_P2));
        };
        bool isSeeded(){return _isSeeded;};
        void setSeeded(){_isSeeded=true;};
        bool isLocalMax(){return _isLocalMax;};
        bool isShared(){return (!_isLocalMax && _P1>=0 && _P2>=0);};
        bool isLocked(){return locked;};
        bool isError(){return error;};
        bool isValid(){
            if(error) return false;
            if(!locked) return false; // when check it should be locked(energy splitted)
            if(_E != _E_sub1 + _E_sub2) return false;
            if(_isLocalMax && _P1==_P2 && _P1>=0) //localMax
                return true;
            if(!_isLocalMax && _P1==_P2 && _P1==-1) // no localMax
                return true;
            if(!_isLocalMax && _P1>=0) //assiciated to one/two localMaxima
                return true;
            return false;
        };

        double E_sub1(){return locked?_E_sub1:_DNAN;};
        double E_sub2(){return locked?_E_sub2:_DNAN;};

        void calE_sub(double w1)
        {
            if (!locked) //prevent ouble claculte the splitting!!
            {
                _E_sub1 = _E * w1;
                _E_sub2 = _E * (1 - w1);
                locked = true;
            }
            else
                error = true;
        };
        void calE_sub()
        {
            if (!locked)
            {
                if (_isLocalMax || _P2 < 0) //prevent splitting cell enter this func
                {
                    _E_sub1 = _E;
                    _E_sub2 = 0;
                    locked = true;
                }
                else
                    error = true;
            }
            else
                error = true;
        };

        void addNeighbor(std::shared_ptr<CHit> h){
            //assert h->isLocalMax()
            //assert h->P1()==h->P2()
            if(_P1<0){
                _P1=h->P1();
                _E_sub1=h->E();
            }
            else if(_P2<0){
                if(h->E()>_E_sub1){
                    if(h->P1()!=_P1){
                        _P2=_P1;
                        _P1=h->P1();
                        _E_sub2=_E_sub1;
                    }
                    _E_sub1=h->E();
                }else if(h->P1()!=_P1){
                    _P1=h->P1();
                    _E_sub1=h->E();
                }
            }
            else{
                //method1: use desencding localMax and keep only first P1 P2
                // ;
                //method2: use leading two neighbors
                if(h->E()>_E_sub1){
                    if(h->P1()!=_P1){
                        _P2=_P1;
                        _P1=h->P1();
                        _E_sub2=_E_sub1;
                     }
                     _E_sub1=h->E();
                }else if(h->E()>_E_sub2 && h->P1()!=_P1){
                        _P2=h->P1();
                        _E_sub2=h->E();
                }else{
                   ; 
                }
                //method3: determined by cluster energy -- need recalculation
                //;
            }
        };

        void setP0(int v){_P0=v;};
        void setP1(int v){_P1=v;};
        void setP2(int v){_P2=v;};
        void setLocalMax(){_isLocalMax=true;};
        
        // bool isNeighbor(std::shared_ptr<CHit> b){
        //     return (std::abs(X-b->X)+std::abs(Y-b->Y)+std::abs(Z-b->Z)==1);
        // };
        // bool isNeighbor(CHit b){
        //     return (std::abs(X-b.X)+std::abs(Y-b.Y)+std::abs(Z-b.Z)==1);
        // };
        bool isTruth{false}; // TODO: better to be private
        #ifdef CLUSTER_DEBUG
            DHit *hit; // needed for DDis --> the abs location of the cell.
        #else
            CalorimeterHit *hit;
        #endif

    private:
        double _E{-1.};
        int _X{-1};
        int _Y{-1};
        int _Z{-1};
        int _P0{-1}; // id of parent cluster
        int _P1{-1}; // id of first sassociated sub cluster inside the parent cluster
        int _P2{-1}; // id of potential second associated sub cluster inside the parent cluster
        double _E_sub1{-1.};//neighbor cell energy, used in splitting; or splitted energy used in final summation
        double _E_sub2{-1.};
        bool locked{false}; //when locaked, the neighbor could not be added and the E_sub means the splitted energy not the neoghbor energy
        bool _isLocalMax{false}; //mark whether this is the maxima cell (obviously, is the localMaxima of P0_P1)
        bool error{false}; // mark fatal error
        bool _isSeeded{false};
}; 

/// \brief Base Analysis Class for Cluster
/// \note The input data vector will be automatically sorted in desending order by energy
class TopoCluster_Analysis : public Cluster_Analysis {
public:
    TopoCluster_Analysis() = default;

    explicit TopoCluster_Analysis(CalorimeterHitVec* clusterVec, bool isStaggered=true);

    virtual ~TopoCluster_Analysis() = default;

    bool Do(std::vector<std::map<std::string, double>> *ret1,std::vector<std::map<std::string, double>> *ret2, 
                std::vector<std::shared_ptr<CHit>> *dump=nullptr);
    
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

private:
    std::vector<std::shared_ptr<CHit>> findNeighbors(std::shared_ptr<CHit> center, const std::array<std::shared_ptr<CHit>,HMAP_LENGTH>& HMAP);
    std::vector<std::shared_ptr<CHit>> findNeighbors_legacy(std::shared_ptr<CHit> center, const std::array<std::shared_ptr<CHit>,HMAP_LENGTH>& HMAP);
    std::vector<std::shared_ptr<CHit>> findNeighbors_staggered(std::shared_ptr<CHit> center, const std::array<std::shared_ptr<CHit>,HMAP_LENGTH>& HMAP);

    [[nodiscard]] double calDistance(std::shared_ptr<CHit> h, const TVector3& loc); //input unit: cm; ouput unit: EM scale (5cm)
    [[nodiscard]] double calWeight(double E1,  double E2, double d1, double d2); //calculate the weight for splitting

    void calXYZ(const std::vector<std::shared_ptr<CHit>> cluster,double ret[]){return calXYZ(_DUMMY, _DUMMY, cluster, ret);}; //P0==-1 means fail CHit. P0==_DUMMY mean dummay value used for main cluster
    [[nodiscard]] bool calXYEW(const std::vector<std::shared_ptr<CHit>> cluster,int Z, double ret[]){return calXYEW(_DUMMY, _DUMMY, cluster, Z, ret);};
    [[nodiscard]] bool calEtaPhiXY(const std::vector<std::shared_ptr<CHit>> cluster,double ret[]){return calEtaPhiXY(_DUMMY, _DUMMY, cluster, ret);};
    [[nodiscard]] double calE(const std::vector<std::shared_ptr<CHit>> cluster){return calE(_DUMMY,_DUMMY,cluster);};
    [[nodiscard]] int calNCell(const std::vector<std::shared_ptr<CHit>> cluster){return calNCell(_DUMMY,_DUMMY,cluster);};

    void calXYZ(int P0, int P1, const std::vector<std::shared_ptr<CHit>> cluster, double ret[]);
    [[nodiscard]] bool calXYEW(int P0, int P1, const std::vector<std::shared_ptr<CHit>> cluster,int Z, double ret[]);
    [[nodiscard]] bool calEtaPhiXY(int P0, int P1, const std::vector<std::shared_ptr<CHit>> cluster, double ret[]);
    [[nodiscard]] double calE(int P0, int P1, const std::vector<std::shared_ptr<CHit>> cluster);
    [[nodiscard]] int calNCell(int P0, int P1, const std::vector<std::shared_ptr<CHit>> cluster);

    void calXYZCellWidth(const std::vector<std::shared_ptr<CHit>> cluster, double ret[]){calXYZCellWidth(_DUMMY,_DUMMY,cluster,ret);}; 
    void calXYZCellWidth(int P0, int P1, const std::vector<std::shared_ptr<CHit>> cluster, double ret[]); 

    std::array<TVector3,MAX_ECAL_CELLS> _POS{};
    double _SurfaceZ{-999};

    const std::array<TVector3,MAX_ECAL_CELLS>& POS(){return _POS;}; //const??
    void setPOS(const std::array<TVector3,MAX_ECAL_CELLS>& v){_POS=v;};
    double SurfaceZ(){return _SurfaceZ;};
    void setSurfaceZ(double v){_SurfaceZ=v;};

    const TVector3& toPos(std::shared_ptr<CHit> h); //make it const????

    // configs
    double Enoise{1}; // MeV or Digit
    double EThres_S{4}; // *Enoise
    double EThres_N{2}; // *Enoise
    double EThres_P{0}; // *Enoise
    double Critical_E{500}; // MeV or Digit
    double Critical_N{4};
    // energy weighting
    double EM_SCALE_LENGTH_mm{50};
    double EM_ENERGY_SCALE_MeV{1};
    double ENERGY_SHIFT_MeV{0};
    enum WEIGHT_TYPE 
    {  linear=0,
       log=1
    } weight_type{linear};
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
};


#endif //DSIMU_TOPOCLUSTER_ANALYSIS_H
