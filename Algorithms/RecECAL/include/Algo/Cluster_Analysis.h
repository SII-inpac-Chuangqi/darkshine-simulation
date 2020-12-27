//
// Created by Zhang Yulei on 12/27/20.
//

#ifndef DSIMU_CLUSTER_ANALYSIS_H
#define DSIMU_CLUSTER_ANALYSIS_H

#include "Core/AnaProcessor.h"
#include "Algo/Trk_LineFit.h"

#include <vector>
#include <utility>
#include <algorithm>

// Eigen
// Ignore Eigen for now
//#include <Eigen/Core>
//#include <Eigen/Dense>

// Utility functions
template<class ClusterHit>
bool sortbyE(ClusterHit *a, ClusterHit *b) {return (a->getE() > b->getE());} // descending

template<class ClusterHit>
bool sortbyX(ClusterHit *a, ClusterHit *b) {return (a->getCellIdX() < b->getCellIdX());} // ascending

template<class ClusterHit>
bool sortbyY(ClusterHit *a, ClusterHit *b) {return (a->getCellIdY() < b->getCellIdY());} // ascending

template<class ClusterHit>
bool sortbyZ(ClusterHit *a, ClusterHit *b) {return (a->getCellIdZ() < b->getCellIdXZ());} // ascending

/// \brief Base Analysis Class for Cluster
/// \note The input data vector will be automatically sorted in desending order by energy
class Cluster_Analysis {
public:
    Cluster_Analysis() = default;

    explicit Cluster_Analysis(SimulatedHitVecUniPtr clusterVec);

    virtual ~Cluster_Analysis() = default;

    void setClusterVec(const shared_ptr<std::vector<SimulatedHit *>> &clusterVec) {
        ClusterVec = clusterVec;
        std::sort(ClusterVec->begin(), ClusterVec->end(), sortbyE<SimulatedHit>);
        E_Tot = FindETotal();
    }

    /// \brief Find the hit with maximumal energy
    SimulatedHit* FindMaxEHit();

    /// \brief Find the total deposited energy
    double FindETotal();

    /// \brief Find the Cluster energy fractions
    double FindEFraction(unsigned n);

    /// \brief Find the n-th moment of the cluster along certain axis
    /// \param n: the n-th moments
    /// \param type: 0-all, 1-x, 2-y, 3-z
    double FindMoment(unsigned n, int type, bool center= false);


private:
    // data
    //std::shared_ptr<std::vector<ClusterHit *> > ClusterVec;
    SimulatedHitVecUniPtr ClusterVec;

    //-------------------------------------------

    // total deposit energy
    double E_Tot{0};

    // Maximum Energy Hit
    SimulatedHit* MaxHit{nullptr};

    // Cluster Energy Fraction ( to the n-th highest energy )
    double E_frac{0.};

    //-------------------------------------------

};


#endif //DSIMU_CLUSTER_ANALYSIS_H
