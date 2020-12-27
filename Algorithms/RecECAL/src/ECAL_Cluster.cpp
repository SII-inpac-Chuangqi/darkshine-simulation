//
// Created by Zhang Yulei on 12/8/20.
//

#include "Algo/ECAL_Cluster.h"

#include <algorithm>
#include <cmath>

// Root Library
#include "TVector3.h"
#include "TAxis.h"
#include "TH2D.h"

// Driver function to sort the vector elements
// by hit energy
bool sortbyenergy(SimulatedHit *a,
                  SimulatedHit *b) {
    return (a->getE() < b->getE());
}

void ECAL_Cluster::Clean() {
    nb_hits_clustered = 0;
    nb_hits_not_clustered = 0;

    // Clean the whole raw clusters
    for (const auto &itr : Raw_Clusters) {
        itr->clear();
        itr->shrink_to_fit();
    }
    Raw_Clusters.clear();
    Raw_Clusters.shrink_to_fit();

    for (const auto &itr : Layer_Clusters_Centers) {
        itr->clear();
        itr->shrink_to_fit();
    }
    Layer_Clusters_Centers.clear();

    for (const auto &itr : Layer_Hits) {
        itr->clear();
    }
    Layer_Hits.clear();

    for (const auto &itr : *OutCollection) {
        delete itr;
    }
    OutCollection->clear();

    //Cluster_Centers->clear();
    //Cluster_Centers->shrink_to_fit();
}

void ECAL_Cluster::ClusterHits(const SimulatedHitVecUniPtr &hits, int nb_Z, double distance_cut, double R_cut) {
    // Initialization
    Clean();

    d_cut = distance_cut;
    r_cut = R_cut;
    nb_z = nb_Z;

    // Step 1: Split hits into different layers
    for (int i = 0; i < nb_z; ++i) {
        Layer_Hits.push_back(shared_ptr<SimulatedHitVec>(new SimulatedHitVec()));
    }

    for (auto hit: *hits) {
        int z_layer = hit->getCellIdZ();
        Layer_Hits.at(z_layer - 1)->push_back(hit);
    }

    // Step 2: Cluster layer by layer
    for (int i = 0; i < nb_z; ++i) {
        if (!Layer_Hits.at(i)->empty())
            ClusterHits_Layer(Layer_Hits.at(i), i);
    }

    for (const auto& Layer_Centers : Layer_Clusters_Centers ) {
        for (auto center: *Layer_Centers) {
            auto rec_p = new CalorimeterHit();
            rec_p->setX(center->getX());
            rec_p->setY(center->getY());
            rec_p->setZ(center->getZ());
            rec_p->setE(center->getE());
            rec_p->setCellId(center->getCellId());
            rec_p->setCellIdX(center->getCellIdX());
            rec_p->setCellIdY(center->getCellIdY());
            rec_p->setCellIdZ(center->getCellIdZ());

            OutCollection->push_back(rec_p);
        }
    }
}

void ECAL_Cluster::ClusterHits_Layer(const SimulatedHitVecUniPtr &hits, int nb_layer) {

    // Calculate the total max energy deposition
    double E_max = 0.;
    for (auto hit: *hits) E_max = (E_max > hit->getE()) ? E_max : hit->getE();

    // The minimal energy required for a new cluster center
    double E_cut = r_cut * E_max;

    // Step 1: Loop all the hits and find all local cluster centers of energy > E_cut
    //         Also they are not within 3*3*3 grid for each other
    SimulatedHitVecUniPtr center_candidates = shared_ptr<SimulatedHitVec>(new SimulatedHitVec());
    SimulatedHitVecUniPtr Cluster_Centers = shared_ptr<SimulatedHitVec>(new SimulatedHitVec());
    for (auto hit: *hits) {
        if (hit->getE() >= E_cut)
            center_candidates->push_back(hit);
    }
    // Sort the vector with ascending order
    std::sort(center_candidates->begin(), center_candidates->end(), sortbyenergy);

    for (auto center: *center_candidates) {
        bool separated = true;
        for (auto others: *center_candidates) {
            // cell id to calculate if within 3*3*3 grid
            int diff_x = abs(center->getCellIdX() - others->getCellIdX());
            int diff_y = abs(center->getCellIdY() - others->getCellIdY());
            if (others->getE() <= center->getE()) continue;
            if (diff_x == 0 && diff_y == 0) continue;
            if (diff_x <= 1 && diff_y <= 1) {
                separated = false;
                break;
            }
        }
        if (separated) Cluster_Centers->push_back(center);
    }

    Layer_Clusters_Centers.push_back(Cluster_Centers);

//    // prepare the final cluster vector
//    for (auto center : *Cluster_Centers)
//        Raw_Clusters.push_back(shared_ptr<SimulatedHitVec>(new SimulatedHitVec()));
//
//    // Step 2: Cluster all the hits in different centers
//    //         by distance_cut
//    nb_hits_clustered = 0;
//    nb_hits_not_clustered = 0;
//    for (auto hit: *hits) {
//        auto if_fill = FillClusterByMinDistance(hit, Cluster_Centers, d_cut);
//        if (if_fill) nb_hits_clustered++;
//        else nb_hits_not_clustered++;
//    }

    //OutCollection = shared_ptr<CalorimeterHitVec>(new CalorimeterHitVec());

}

bool
ECAL_Cluster::FillClusterByMinDistance(SimulatedHit *hit, const SimulatedHitVecUniPtr &centers, double dis_cut) {
    bool fill = false;

    // distance to calculate cluster radius
    TVector3 v_hit(hit->getX(), hit->getY(), hit->getZ());

    // Logic 1: every cluster has a size of radius d_cut * energy
    // Cuz the center vector is descending order, so hits are more likely
    // to fill in the clusters with larger energy
//    for (unsigned i = 0; i < centers->size(); ++i) {
//        auto center = centers->at(i);
//        auto raw_cluster_itr = Raw_Clusters.at(i);
//        TVector3 v_center(center->getX(), center->getY(), center->getZ());
//        double distance = fabs(v_hit.Mag() - v_center.Mag());
//        double center_radius = d_cut * center->getE();
//        if (distance < center_radius) {
//            raw_cluster_itr->push_back(hit);
//            fill = true;
//            break;
//        }
//    }

    // Logic 2 : fill the hit with its nearest cluster center
    unsigned idx_MinDistance = 0;
    double min_Distance = 1000000.;
    for (unsigned i = 0; i < centers->size(); ++i) {
        auto center = centers->at(i);
        auto raw_cluster_itr = Raw_Clusters.at(i);
        TVector3 v_center(center->getX(), center->getY(), center->getZ());
        double distance = fabs(v_hit.Mag() - v_center.Mag());
        if (distance <= min_Distance) {
            idx_MinDistance = i;
            min_Distance = distance;
            fill = true;
        }
    }
    Raw_Clusters.at(idx_MinDistance)->push_back(hit);

    return fill;
}