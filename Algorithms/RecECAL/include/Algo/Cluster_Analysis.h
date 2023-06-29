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

    bool FineECellXY(double ECell[]); //ECell must be the address of the first element in array double[400]
    bool FineECellXZ(double ECell[]);
    bool FineECellYZ(double ECell[]);
    bool FineECellZ(double ECell[]);
    bool FineECellXYZ(double ECell[]);
    bool maxXY(int ECell[]);
    bool NXY(int ECell[]);
    
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
