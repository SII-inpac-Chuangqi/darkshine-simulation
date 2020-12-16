//
// Created by Zhang Yulei on 12/8/20.
//

#include "Algorithms/RecECAL/include/Algo/ECAL_Cluster.h"

#include <algorithm>
#include <cmath>

// Root Library
#include "TVector3.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TFrame.h"
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

    double b = 0.;
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

void ECAL_Cluster::DrawClusterResults(const SimulatedHitVecUniPtr &hits) {

    // Define the canvas
    auto c1 = new TCanvas("c1", "c1", 1500, 1200);

    // Define Color
    int col[27] = {632, 400, 591, 95, 876, 8, 4, 7, 5, 6, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
                   25};

    auto gr = DrawGraph(hits, "Raw Hits", kBlack);
    if (gr) {
        gr->Draw("P0 FBBB");
        gr->SetMarkerStyle(4);
    }

    // Draw Clusters
    std::vector<shared_ptr<TGraph2D> > gr_v;
    for (auto cluster: Layer_Clusters_Centers)
        gr_v.push_back(shared_ptr<TGraph2D>(new TGraph2D()));
    for (unsigned i = 0; i < Layer_Clusters_Centers.size(); ++i) {
        gr_v.at(i) = DrawGraph(Layer_Clusters_Centers.at(i), to_string(i), col[i], 0.85);
        if (gr_v.at(i)) {
            gr_v.at(i)->Draw("P FBBB Same");
            gr_v.at(i)->SetMarkerStyle(29);
        }
    }

    // Draw Cluster Centers
//    auto gr_c = DrawGraph(Cluster_Centers, to_string(20), 54, 0.75, 5);
//    if (gr_c) {
//        gr_c->Draw("P FBBB Same");
//        gr_c->SetMarkerStyle(29);
//    }

//    std::vector<shared_ptr<TGraph2D> > gr_c;
//    for (auto cluster: *Cluster_Centers)
//        gr_c.push_back(shared_ptr<TGraph2D>(new TGraph2D()));
//    for (unsigned i = 0; i < Cluster_Centers->size(); ++i) {
//        SimulatedHitVecUniPtr tmp = shared_ptr<SimulatedHitVec>(new SimulatedHitVec());
//        tmp->push_back(Cluster_Centers->at(i));
//        gr_c.at(i) = DrawGraph(tmp, to_string(i+100), col[i], 1.0, 5);
//        gr_c.at(i)->Draw("P FBBB Same");
//        gr_c.at(i)->SetMarkerStyle(29);
//    }

    // Rotate Canvas
    c1->SetPhi(0-0.01);
    c1->SetTheta(0+0.01);
    c1->SaveAs(("./plots/ECAL_Hits" + to_string(draw_n) + "_r" + to_string(r_cut) + ".png").data());

    draw_n++;
    delete c1;
}

void ECAL_Cluster::FormatGraph(const shared_ptr<TGraph2D> &gr) {
    gr->SetTitle("ECAL Hits");
    gr->GetXaxis()->SetTitle("z [mm]");
    gr->GetYaxis()->SetTitle("x [mm]");
    gr->GetZaxis()->SetTitle("y [mm]");

//    gr->GetXaxis()->SetLabelOffset(999);
//    gr->GetYaxis()->SetLabelOffset(999);
//    gr->GetZaxis()->SetLabelOffset(999);

    gr->GetXaxis()->SetTitleOffset(1.4);
    gr->GetYaxis()->SetTitleOffset(2.2);
    gr->GetZaxis()->SetTitleOffset(1.4);

//
//    gr->GetXaxis()->SetLabelSize(0);
//    gr->GetYaxis()->SetLabelSize(0);
//    gr->GetZaxis()->SetLabelSize(0);

    gr->GetXaxis()->CenterTitle();
    gr->GetYaxis()->CenterTitle();
    gr->GetZaxis()->CenterTitle();

}

shared_ptr<TGraph2D>
ECAL_Cluster::DrawGraph(const SimulatedHitVecUniPtr &hits, const TString &name, int marker_color, double alpha,
                        int marker_size) {
    // Convert vector to array
    std::vector<double> vx;
    std::vector<double> vy;
    std::vector<double> vz;

    for (auto hit : *hits) {
        vx.push_back(hit->getX());
        vy.push_back(hit->getY());
        vz.push_back(hit->getZ());
    }
    auto hits_x = vx.data();
    auto hits_y = vy.data();
    auto hits_z = vz.data();
    int hits_n = hits->size();
    if (hits_n < 1) return nullptr;
    auto gr = shared_ptr<TGraph2D>(new TGraph2D(hits_n, hits_z, hits_x, hits_y));
    gr->SetName(name);
    FormatGraph(gr);

    gr->SetMarkerColorAlpha(marker_color, alpha);
    gr->SetMarkerSize(marker_size);

    return gr;
}