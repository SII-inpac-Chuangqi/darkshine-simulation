//
// Created by Zhang Yulei on 12/27/20.
//

#ifndef DSIMU_CLUSTER_ANALYSIS_H
#define DSIMU_CLUSTER_ANALYSIS_H

#include "Core/AnaProcessor.h"
#include "Core/AnaData.h"

#include <cassert>  
#include <vector>
#include <array>
#include <set>
#include <utility>
#include <algorithm>
#include "TVector3.h"
//never include any templete header like Eigen, XYZVector, so on. see https://stackoverflow.com/questions/67529835/eigen3-take-a-long-time-to-compile-and-very-slow-when-debug

#ifndef _DNAN
    #define _DNAN (nan(""))
#endif

#ifndef MAX_ECAL_CELLS // defined in AnaData as shared information
    #define MAX_ECAL_CELLS (25*25*15)
#endif
#pragma message "The maximum support Ncell now is (increase if you need and pay attention of the stack)" 
#pragma message MAX_ECAL_CELLS

//HMAP is larger than cell, to allow for the overflwo/underflow bin
#if MAX_ECAL_CELLS < 1000
    #define HMAP_LENGTH 10000
#else
    #define HMAP_LENGTH (MAX_ECAL_CELLS*2)
#endif
//used for dummy value for cluster id
#define _DUMMY -2

// Utility functions
template<class ClusterHit>
bool sortbyE(ClusterHit *a, ClusterHit *b) { return (a->getE() > b->getE()); } // descending

template<class ClusterHit>
bool sortbyX(ClusterHit *a, ClusterHit *b) { return (a->getCellIdX() < b->getCellIdX()); } // ascending

template<class ClusterHit>
bool sortbyY(ClusterHit *a, ClusterHit *b) { return (a->getCellIdY() < b->getCellIdY()); } // ascending

template<class ClusterHit>
bool sortbyZ(ClusterHit *a, ClusterHit *b) { return (a->getCellIdZ() < b->getCellIdXZ()); } // ascending

template<class ClusterHit>
bool Esorter_ascending( ClusterHit *a,  ClusterHit *b) { return (a->E < b->E); } // ascending
template<class ClusterHit>
bool Esorter_descending( ClusterHit *a,  ClusterHit *b) { return (a->E > b->E); } // descending

template<class CHit>
bool Esorter_ascendingC( CHit *a,  CHit *b) { return (a->E() < b->E()); } // ascending
template<class CHit>
bool Esorter_descendingC( CHit *a,  CHit *b) { return (a->E() > b->E()); } // descending

class CHit { 
    public: 
        CHit(CalorimeterHit *h){
            _E=h->getE();
            _X=h->getCellIdX(); //this should start from 1
            _Y=h->getCellIdY();
            _Z=h->getCellIdZ();
        };
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

        void addNeighbor(CHit* h){
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
        
        // bool isNeighbor(CHit *b){
        //     return (std::abs(X-b->X)+std::abs(Y-b->Y)+std::abs(Z-b->Z)==1);
        // };
        // bool isNeighbor(CHit b){
        //     return (std::abs(X-b.X)+std::abs(Y-b.Y)+std::abs(Z-b.Z)==1);
        // };
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
class Cluster_Analysis {
public:
    Cluster_Analysis() = default;

    explicit Cluster_Analysis(CalorimeterHitVec* clusterVec);

    virtual ~Cluster_Analysis() = default;

    void setClusterVec(CalorimeterHitVec* clusterVec) {
        ClusterVec = clusterVec;
        std::sort(ClusterVec->begin(), ClusterVec->end(), sortbyE<CalorimeterHit>);
        E_Tot = FindETotal();
    }

    /// \brief Find the hit with maximumal energy
    CalorimeterHit *FindMaxEHit();

    /// \brief Find the total deposited energy
    double FindETotal();

    /// \brief Find the Cluster energy fractions
    double FindEFraction(unsigned n);

    /// \brief Find the n-th moment of the cluster along certain axis
    /// \param n: the n-th moments
    /// \param type: 0-all, 1-x, 2-y, 3-z
    double FindMoment(unsigned n, int type, bool center = false);

    /// Find the lateral moment of the cluster
    double FindLatMoment();

    bool FineECellXY(double ECell[]); //ECell must be the address of the first element in array double[400]
    bool FineECellXZ(double ECell[]);
    bool FineECellYZ(double ECell[]);
    bool FineECellZ(double ECell[]);
    bool FineECellXYZ(double ECell[]);
    bool maxXY(int ECell[]);
    bool NXY(int ECell[]);
    bool Do(std::vector<std::map<std::string, double>> *ret1,std::vector<std::map<std::string, double>> *ret2);
    bool Do(std::vector<std::map<std::string, double>> *ret1){return Do(ret1,nullptr);};
    bool DoV2(std::vector<std::map<std::string, double>> *ret2){return Do(nullptr,ret2);};

private:
    // data
    //std::shared_ptr<std::vector<ClusterHit *> > ClusterVec;
    CalorimeterHitVec* ClusterVec;
    //-------------------------------------------

    // total deposit energy
    double E_Tot{0};

    // Maximum Energy Hit
    CalorimeterHit *MaxHit{nullptr};

    // Cluster Energy Fraction ( to the n-th highest energy )
    double E_frac{0.};

    std::vector<CHit*> findNeighbors(CHit* center, const std::array<CHit*,HMAP_LENGTH>& HMAP);

    int _dNX,_dNY,_dNZ; //geometry parameter (NCell) extracted from dANA
    double _SurfaceZ{-999};

    int dNX(){return _dNX;};
    int dNY(){return _dNY;};
    int dNZ(){return _dNZ;};
    double SurfaceZ(){return _SurfaceZ;};

    void setNX(int v){_dNX=v;};
    void setNY(int v){_dNY=v;};
    void setNZ(int v){_dNZ=v;};
    void setSurfaceZ(double v){_SurfaceZ=v;};

    [[nodiscard]] double calDistance(CHit* h, const TVector3& loc); //input unit: cm; ouput unit: EM scale (5cm)
    [[nodiscard]] double calWeight(double E1,  double E2, double d1, double d2); //calculate the weight for splitting

    void calXYZ(const std::vector<CHit*> cluster,double ret[]){return calXYZ(_DUMMY, _DUMMY, cluster, ret);}; //P0==-1 means fail CHit. P0==_DUMMY mean dummay value used for main cluster
    [[nodiscard]] bool calXYEW(const std::vector<CHit*> cluster,int Z, double ret[]){return calXYEW(_DUMMY, _DUMMY, cluster, Z, ret);};
    [[nodiscard]] bool calEtaPhiXY(const std::vector<CHit*> cluster,double ret[]){return calEtaPhiXY(_DUMMY, _DUMMY, cluster, ret);};
    [[nodiscard]] double calE(const std::vector<CHit*> cluster){return calE(_DUMMY,_DUMMY,cluster);};
    [[nodiscard]] int calNCell(const std::vector<CHit*> cluster){return calNCell(_DUMMY,_DUMMY,cluster);};

    void calXYZ(int P0, int P1, const std::vector<CHit*> cluster, double ret[]);
    [[nodiscard]] bool calXYEW(int P0, int P1, const std::vector<CHit*> cluster,int Z, double ret[]);
    [[nodiscard]] bool calEtaPhiXY(int P0, int P1, const std::vector<CHit*> cluster, double ret[]);
    [[nodiscard]] double calE(int P0, int P1, const std::vector<CHit*> cluster);
    [[nodiscard]] int calNCell(int P0, int P1, const std::vector<CHit*> cluster);

    void calXYZCellWidth(const std::vector<CHit*> cluster, double ret[]){calXYZCellWidth(_DUMMY,_DUMMY,cluster,ret);}; 
    void calXYZCellWidth(int P0, int P1, const std::vector<CHit*> cluster, double ret[]); 


    std::array<TVector3,MAX_ECAL_CELLS> _POS{};

    const std::array<TVector3,MAX_ECAL_CELLS>& POS(){return _POS;}; //const??
    void setPOS(const std::array<TVector3,MAX_ECAL_CELLS>& v){_POS=v;};

    const TVector3& toPos(CHit* h); //make it const????
};


#endif //DSIMU_CLUSTER_ANALYSIS_H
