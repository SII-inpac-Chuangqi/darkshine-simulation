//
// Created by Zhang Yulei on 12/27/20.
// Clustering Algo from Q.LIU from y2022.
//

#include <utility>
#include <numeric>
#include <functional>

#include "Algo/Cluster_Analysis.h"

Cluster_Analysis::Cluster_Analysis(CalorimeterHitVec *clusterVec, bool isStaggered) : ClusterVec(clusterVec) {
    std::sort(ClusterVec->begin(), ClusterVec->end(), sortbyE<CalorimeterHit>);
    E_Tot = FindETotal();
    setNX(dAnaData->getNECalCellX());
    setNY(dAnaData->getNECalCellY());
    setNZ(dAnaData->getNECalCellZ());
    
    m_isStaggered=isStaggered;       
}

CalorimeterHit *Cluster_Analysis::FindMaxEHit() {
    // return the hit with the maximal energy
    return ClusterVec->at(0);
}

double Cluster_Analysis::FindETotal() {
    // return the total deposit Energy
    double E = 0.;

    // Lambda Expression
    //for_each(ClusterVec->begin(), ClusterVec->end(), [&E] (CalorimeterHit* h) {E += h->getE();} );

    for (auto hit: *ClusterVec) E += hit->getE();

    return E;
}

double Cluster_Analysis::FindEFraction(unsigned n) {
    if (static_cast<int>(n) < 0) return -1;
    if (n >= ClusterVec->size()) n = ClusterVec->size();
    double E = 0.;

    double E_tmp_total = 0;
    for (auto hit: *ClusterVec) {
        if (hit->getE() < 0) cout << hit->getE() << endl;
        E_tmp_total += hit->getE();
    }

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
        for (auto hit: *ClusterVec) {
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
        for (auto hit: *ClusterVec) {
            layer_vec.push_back(hit->getE());
            pos_vec.push_back(sqrt(pow(hit->getX(), 2) + pow(hit->getY(), 2) + pow(hit->getZ(), 2)));
        }
    }

    // convert from mm to cm
    //for (auto &i : pos_vec) i /= 10.;

    [[maybe_unused]] double sum_layer = std::accumulate(pos_vec.begin(), pos_vec.end(), 0.);
    double out_moment = 0.;
    if (!center || n == 1) {
        // Calculate the n-th power sum, weighted by energy
        out_moment = std::inner_product(pos_vec.begin(), pos_vec.end(), layer_vec.begin(), out_moment,
                                        std::plus<>(),
                                        [n](double x, double e) { return e * std::pow(x, n); });
        // Divided by total Energy to normalize weight
        out_moment /= E_Tot;
        return out_moment;
    }
    // If to calculate centralized moments
    double mean = std::inner_product(pos_vec.begin(), pos_vec.end(), layer_vec.begin(), 0.);
    mean /= E_Tot;
    // define temp variable
    std::vector<double> diff(layer_vec.size());
    // calculate distance to mean and store in the temp variable vector
    std::transform(pos_vec.begin(), pos_vec.end(), diff.begin(), [mean](double x) { return x - mean; });
    out_moment = std::inner_product(diff.begin(), diff.end(), layer_vec.begin(), out_moment,
                                    std::plus<>(),
                                    [n](double x, double e) { return e * std::pow(x, n); });
    out_moment /= E_Tot;
    return out_moment;
}

double Cluster_Analysis::FindLatMoment() {
    double center_x = FindMoment(1, 1, false);
    double center_y = FindMoment(1, 2, false);
    //Temperarily let central axis be parallel to z axis

    //too few hits
    if (ClusterVec->size() <= 2) return 0;

    double lat = 0;
    for (auto h: *ClusterVec) {
        double diffX = h->getX() - center_x;
        double diffY = h->getY() - center_y;
        lat += h->getE() * std::pow((std::pow(diffX, 2) + std::pow(diffY, 2)), 0.5);
    }
    lat /= E_Tot;
    return lat;
}

bool Cluster_Analysis::FineECellXY(double ECell[]) { //sumZ
    if (ECell == NULL) return false;
    double temp[MAX_ECAL_CELLS]={0};
    for (auto hit: *ClusterVec) {
        int Xid = hit->getCellIdX() - 1;
        int Yid = hit->getCellIdY() - 1;
        temp[Xid + dNX() * Yid] += hit->getE(); // Y-X
    }
    for (int i = 0; i < dNX()*dNY(); ++i) ECell[i] = temp[i];
    return true;
}

bool Cluster_Analysis::FineECellXZ(double ECell[]) { //sumY
    if (ECell == NULL) return false;
    double temp[MAX_ECAL_CELLS]={0};
    for (auto hit: *ClusterVec) {
        int Xid = hit->getCellIdX() - 1;
        int Zid = hit->getCellIdZ() - 1;
        temp[Xid + dNX() * Zid] += hit->getE(); //Z-X
    }
    for (int i = 0; i < dNX()*dNZ(); ++i) ECell[i] = temp[i];
    return true;
}

bool Cluster_Analysis::FineECellYZ(double ECell[]) { //sumX
    if (ECell == NULL) return false;
    double temp[MAX_ECAL_CELLS]={0};
    for (auto hit: *ClusterVec) {
        int Yid = hit->getCellIdX() - 1;
        int Zid = hit->getCellIdZ() - 1;
        temp[Yid + dNY() * Zid] += hit->getE(); //Z-Y
    }
    for (int i = 0; i < dNY()*dNZ(); ++i) ECell[i] = temp[i];
    return true;
}

bool Cluster_Analysis::FineECellZ(double ECell[]) { //sumXY
    if (ECell == NULL) return false;
    double temp[MAX_ECAL_CELLS]={0};
    for (auto hit: *ClusterVec) {
        int Zid = hit->getCellIdZ() - 1;
        temp[Zid] += hit->getE(); //Z-Y
    }
    for (int i = 0; i <dNZ(); ++i) ECell[i] = temp[i];
    return true;
}

bool Cluster_Analysis::maxXY(int ECell[]) { //sumZ
    if (ECell == NULL) return false;
    int temp[MAX_ECAL_CELLS] = {0};
    for (auto hit: *ClusterVec) {
        int Xid = hit->getCellIdX() - 1;
        int Yid = hit->getCellIdY() - 1;
        int Zid = hit->getCellIdZ();
        temp[Xid + dNX() * Yid] =max(temp[Xid + dNX() * Yid], Zid); // Y-X
    }
    for (int i = 0; i < dNX()*dNY(); ++i) ECell[i] = temp[i];
    return true;
}

bool Cluster_Analysis::FineECellXYZ(double ECell[]) { //all hits
    if (ECell == NULL) return false;
    double temp[MAX_ECAL_CELLS]={0};
    for (auto hit: *ClusterVec) {
        int Xid = hit->getCellIdX() - 1;
        int Yid = hit->getCellIdY() - 1;
        int Zid = hit->getCellIdZ() - 1;
        temp[Xid + dNX()*Yid + dNX()*dNY()*Zid] = hit->getE(); //X-Y-Z
    }
    for (int i = 0; i <dNX()*dNY()*dNZ(); ++i) ECell[i] = temp[i];
    return true;
}

bool Cluster_Analysis::NXY(int ECell[]) { //sumZ
    if (ECell == NULL) return false;
    int temp[MAX_ECAL_CELLS]={0};
    for (auto hit: *ClusterVec) {
        int Xid = hit->getCellIdX() - 1;
        int Yid = hit->getCellIdY() - 1;
        temp[Xid + dNX() * Yid] += 1; // Y-X
    }
    for (int i = 0; i < dNX()*dNY(); ++i) ECell[i] = temp[i];
    return true;
}
