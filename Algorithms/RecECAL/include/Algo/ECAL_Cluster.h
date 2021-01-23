//
// Created by Zhang Yulei on 12/8/20.
//

#ifndef DSIMU_ECAL_CLUSTER_H
#define DSIMU_ECAL_CLUSTER_H

#include <TGraph2D.h>
#include "vector"
#include "Core/AnaProcessor.h"

class ECAL_Cluster {
public:
    ECAL_Cluster() = default;

    virtual ~ECAL_Cluster() = default;

    void ClusterHits(const SimulatedHitVecUniPtr &hits, int nb_Z, double distance_cut = 500., double R_cut = 0.5);
    void ClusterHits_Layer(const SimulatedHitVecUniPtr &hits, int nb_layer);

    bool FillClusterByMinDistance(SimulatedHit *hit, const SimulatedHitVecUniPtr &centers, double dis_cut);

    void Clean();

    int getNbHitsClustered() const {
        return nb_hits_clustered;
    }

    int getNbHitsNotClustered() const {
        return nb_hits_not_clustered;
    }

    const vector<SimulatedHitVecUniPtr> &getRawClusters() const {
        return Raw_Clusters;
    }

    const vector<SimulatedHitVecUniPtr> &getLayerClustersCenters() const {
        return Layer_Clusters_Centers;
    }

    void setOutCollection(const CalorimeterHitVecUniPtr &outCollection) {
        OutCollection = outCollection;
    }

private:
    int nb_z{1};
    double r_cut{0.1};
    double d_cut{100.};
    int nb_hits_clustered{0};
    int nb_hits_not_clustered{0};

    // SimulatedHitVecUniPtr Cluster_Centers;
    std::vector<SimulatedHitVecUniPtr> Raw_Clusters;

    // Cluster by layer
    std::vector<SimulatedHitVecUniPtr> Layer_Clusters_Centers;
    std::vector<SimulatedHitVecUniPtr> Layer_Hits;

    // Medium Cluster

    // Output Collection
    CalorimeterHitVecUniPtr OutCollection;

    int draw_n{0};
};


#endif //DSIMU_ECAL_CLUSTER_H
