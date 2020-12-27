//
// Created by Zhang Yulei on 12/27/20.
//

#include <utility>
#include <numeric>
#include <functional>

#include "Algo/Cluster_Analysis.h"

Cluster_Analysis::Cluster_Analysis(SimulatedHitVecUniPtr clusterVec) : ClusterVec(std::move(clusterVec)) {
    std::sort(ClusterVec->begin(), ClusterVec->end(), sortbyE<SimulatedHit>);
    E_Tot = FindETotal();
}

SimulatedHit *Cluster_Analysis::FindMaxEHit() {
    // return the hit with the maximal energy
    return ClusterVec->at(0);
}

double Cluster_Analysis::FindETotal() {
    // return the total deposit Energy
    double E = 0.;

    // Lambda Expression
    //for_each(ClusterVec->begin(), ClusterVec->end(), [&E] (SimulatedHit* h) {E += h->getE();} );

    for (auto hit : *ClusterVec) E += hit->getE();

    return E;
}

double Cluster_Analysis::FindEFraction(unsigned n) {
    if (n < 0) return -1;
    if (n >= ClusterVec->size()) n = ClusterVec->size();
    double E = 0.;
    for (unsigned i = 0; i < n; ++i) E += ClusterVec->at(i)->getE();
    E = E / E_Tot;

    return E;
}

double Cluster_Analysis::FindMoment(unsigned n, int type, bool center) {
    // Sanity Check
    if (n <= 0) return -1;

    // Initialize Layer vector to store hits energy in the same layer
    //            pos vector to store the hits position
    auto layer_vec = std::vector<double>();
    auto pos_vec = std::vector<double>();
    if (type != 0) { // along certain axis
        int max_layer = 0;
        for (auto hit: *ClusterVec) {
            if (type == 1) max_layer = (hit->getCellIdX() > max_layer) ? hit->getCellIdX() : max_layer;
            else if (type == 2) max_layer = (hit->getCellIdY() > max_layer) ? hit->getCellIdY() : max_layer;
            else if (type == 3) max_layer = (hit->getCellIdZ() > max_layer) ? hit->getCellIdZ() : max_layer;
        }
        for (int i = 0; i < max_layer; ++i) {
            layer_vec.push_back(0.);
            pos_vec.push_back(0.);
        }

        // if no layer, just return -1
        if (max_layer <= 0) return -1;

        // Fill hits into different layers;
        // Energy is sum
        for (auto hit : *ClusterVec) {
            // Note that cell id starts from 1
            if (type == 1) {
                layer_vec.at(hit->getCellIdX() - 1) += hit->getE();
                pos_vec.at(hit->getCellIdX() - 1) = hit->getX();
            } else if (type == 2) {
                layer_vec.at(hit->getCellIdY() - 1) += hit->getE();
                pos_vec.at(hit->getCellIdY() - 1) = hit->getY();
            } else if (type == 3) {
                layer_vec.at(hit->getCellIdZ() - 1) += hit->getE();
                pos_vec.at(hit->getCellIdZ() - 1) = hit->getZ();
            }
        }
    } else { // for all hits
        for (auto hit : *ClusterVec) {
            layer_vec.push_back(hit->getE());
            pos_vec.push_back(sqrt(pow(hit->getX(), 2) + pow(hit->getY(), 2) + pow(hit->getZ(), 2)));
        }
    }

    // convert from mm to cm
    //for (auto &i : pos_vec) i /= 10.;

    double sum_layer = std::accumulate(pos_vec.begin(), pos_vec.end(), 0.);
    // If to calculate centralized moments
    double mean = (center && n != 1) ? sum_layer / layer_vec.size() : 0.;
    double out_moment = 0.;

    // define temp variable
    std::vector<double> diff(layer_vec.size());
    // calculate distance to mean and store in the temp variable vector
    std::transform(pos_vec.begin(), pos_vec.end(), diff.begin(), [mean](double x) { return x - mean; });
    // Calculate the n-th power sum, weighted by energy
    out_moment = std::inner_product(diff.begin(), diff.end(), layer_vec.begin(), out_moment,
                                    std::plus<>(),
                                    [n](double x, double e) { return e * std::pow(x, n); });
    // Divided by total Energy to normalize weight
    out_moment /= (E_Tot * layer_vec.size());

    return out_moment;
}









