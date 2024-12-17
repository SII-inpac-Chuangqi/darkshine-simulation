//
// Created by Zhang Yulei on 12/27/20.
//

#ifndef DANA_CLUSTER_ANALYSIS_H
#define DANA_CLUSTER_ANALYSIS_H

#include "Core/AnaProcessor.h"
#include "Core/AnaData.h"

#include <cassert>  
#include <vector>
#include <array>
#include <set>
#include <utility>
#include <algorithm>
#include "TVector3.h"
#include "TROOT.h"
#include "TMath.h"
#include <Math/Minimizer.h>
#include <Math/Factory.h>
#include <Math/Functor.h>

// Utility functions
template<class ClusterHit>
bool sortbyE(ClusterHit *a, ClusterHit *b) { return (a->getE() > b->getE()); } // descending

template<class ClusterHit>
bool sortbyX(ClusterHit *a, ClusterHit *b) { return (a->getCellIdX() < b->getCellIdX()); } // ascending

template<class ClusterHit>
bool sortbyY(ClusterHit *a, ClusterHit *b) { return (a->getCellIdY() < b->getCellIdY()); } // ascending

template<class ClusterHit>
bool sortbyZ(ClusterHit *a, ClusterHit *b) { return (a->getCellIdZ() < b->getCellIdXZ()); } // ascending

/// \brief Base Analysis Class for Cluster
/// \note The input data vector will be automatically sorted in desending order by energy
class Cluster_Analysis {
public:
    Cluster_Analysis() = default;

    explicit Cluster_Analysis(CalorimeterHitVec* clusterVec, bool isStaggered=true);

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

    bool FineECellXY(std::vector<double>* ECell);
    bool FineECellXZ(std::vector<double>* ECell);
    bool FineECellYZ(std::vector<double>* ECell);
    bool FineECellZ(std::vector<double>* ECell);
    bool FineECellXYZ(std::vector<double>* ECell);
    bool FinePosCellXYZ(std::vector<TVector3>* PosCell);
    bool maxXY(std::vector<int>* ECell);
    bool NXY(std::vector<int>* ECell);
    
    bool Calculate_COG(std::vector<TVector3>* PosCell, std::vector<double>* ECell,
                       double& COG_X, double& COG_Y, double& COG_Z);
    TVector3 PosToIndex(TVector3 PosCell);
    double GetECellNeighbor(TVector3 IndexCell, std::vector<double>* ECell, int n);
    int GetShowerStartLayer(std::vector<int>* hits_on_layer);
    int GetShowerEndLayer(std::vector<int>* hits_on_layer);
    bool GetLayerCOGWidth(std::vector<TVector3>* COG_Layer, std::vector<TVector3>* shower_layer_width, std::vector<int>* hits_on_layer, std::vector<double>* ECell, std::vector<TVector3> *PosCell);
    double DistanceToLine(const double* par, const TVector3& point);
    // double SumDist2(const double* par);
    bool GetRMSRadius(std::vector<TVector3>* COG_Layer, std::vector<double>* ECell, std::vector<TVector3> *PosCell, double* radius, double* radius_w);
    double GetShowerDensity(std::vector<TVector3>* IndexCell, std::vector<double>* ECell, double ECAL_Hit_Threshold, int n);
    int NewScale(std::vector<TVector3>* PosCell, std::vector<TVector3>* IndexCell, int ratio_x, int ratio_y, int ratio_z);
    bool GetFD2D(std::vector<TVector3>* PosCell, std::vector<TVector3>* IndexCell, double* mean, double* rms);
    bool GetFD3D(std::vector<TVector3>* PosCell, std::vector<TVector3>* IndexCell, double* mean, double* rms);

protected:
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

    int _dNX,_dNY,_dNZ; //geometry parameter (NCell) extracted from dANA

    int dNX(){return _dNX;};
    int dNY(){return _dNY;};
    int dNZ(){return _dNZ;};
    
    void setNX(int v){_dNX=v;};
    void setNY(int v){_dNY=v;};
    void setNZ(int v){_dNZ=v;};
    

    bool m_isStaggered;

};


#endif //DANA_CLUSTER_ANALYSIS_H
