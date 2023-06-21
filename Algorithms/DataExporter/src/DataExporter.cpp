//
// Created by Zhang Yulei on 9/19/20.
//

#include <utility>
#include <filesystem>
#include <numeric>

#include "Algo/DataExporter.h"


void DataExporter::Begin() {

    f = new TFile("Tracker_GNN.root", "RECREATE");
    t = new TTree("dp", "dp");

//    collections = {"TagTrk1", "TagTrk2", "RecTrk1", "RecTrk2"};
    collections = {"TagTrk1", "RecTrk1", "TagTrk1+RecTrk1", "AllTrk"};

    std::vector<std::string> gnn_node = {"x", "y", "z"};
    std::vector<std::string> gnn_edge = {"start", "end", "truth", "p"};

    for (const auto &col: collections) {
        node.insert({col, std::map<std::string, std::vector<double>>()});
        edge.insert({col, std::map<std::string, std::vector<size_t>>()});
        weight.insert({col, 1.0});
        for (const auto &p: gnn_node) {
            node.at(col).insert({p, std::vector<double>()});
            t->Branch((col + "_" + p).data(), &node.at(col).at(p));
        }
        for (const auto &p: gnn_edge) {
            edge.at(col).insert({p, std::vector<size_t>()});
            t->Branch((col + "_" + p).data(), &edge.at(col).at(p));
        }
        t->Branch((col + "_weight").data(), &weight.at(col));

    }
}

std::vector<size_t>
DataExporter::sort_by_key(std::map<std::string, std::vector<double>> &hits, const std::string &key) {
    // Get a reference to the vector we want to sort by
    auto &sort_vec = hits[key];

    // Get the original indices of the vector
    std::vector<size_t> indices(sort_vec.size());
    std::iota(indices.begin(), indices.end(), 0);

    // Sort the indices based on the sort_vec values
    std::sort(
            indices.begin(), indices.end(),
            [&sort_vec](size_t i, size_t j) { return sort_vec[i] < sort_vec[j]; }
    );

    // Helper lambda to apply the sorted indices to a vector
    auto apply_sorted_indices = [&indices](std::vector<double> &vec) {
        std::vector<double> sorted_vec(vec.size());
        for (size_t i = 0; i < vec.size(); ++i) {
            sorted_vec[i] = vec[indices[i]];
        }
        vec = std::move(sorted_vec);
    };

    // Apply the sorted indices to all vectors in the map
    for (auto &it: hits) {
        apply_sorted_indices(it.second);
    }

    return indices;
}

void DataExporter::ProcessEvt(AnaEvent *evt) {

    const auto &TrackerCollection = evt->getSimulatedHitCollection();

    // Append TagTrk and RecTrk
    if (TrackerCollection.count("TagTrk1") != 0 && TrackerCollection.count("RecTrk1") != 0) {
        auto tag_col = TrackerCollection.at("TagTrk1");
        auto rec_col = TrackerCollection.at("RecTrk1");
        SimulatedHitVec *TotalTrk = evt->RegisterSimulatedHitCollection("TagTrk1+RecTrk1");
        TotalTrk->reserve(tag_col->size() + rec_col->size()); // Optionally reserve space to avoid reallocation

        std::copy(std::begin(*tag_col), std::end(*tag_col), std::back_inserter(*TotalTrk));
        std::copy(std::begin(*rec_col), std::end(*rec_col), std::back_inserter(*TotalTrk));
    }
    if (
            TrackerCollection.count("TagTrk1") != 0
            && TrackerCollection.count("RecTrk1") != 0
            && TrackerCollection.count("TagTrk2") != 0
            && TrackerCollection.count("RecTrk2") != 0
            ) {
        auto tag1_col = TrackerCollection.at("TagTrk1");
        auto rec1_col = TrackerCollection.at("RecTrk1");
        auto tag2_col = TrackerCollection.at("TagTrk2");
        auto rec2_col = TrackerCollection.at("RecTrk2");
        SimulatedHitVec *TotalTrk = evt->RegisterSimulatedHitCollection("AllTrk");
        TotalTrk->reserve(tag1_col->size() + rec1_col->size() + tag2_col->size() + rec2_col->size());

        std::copy(std::begin(*tag1_col), std::end(*tag1_col), std::back_inserter(*TotalTrk));
        std::copy(std::begin(*rec1_col), std::end(*rec1_col), std::back_inserter(*TotalTrk));
        std::copy(std::begin(*tag2_col), std::end(*tag2_col), std::back_inserter(*TotalTrk));
        std::copy(std::begin(*rec2_col), std::end(*rec2_col), std::back_inserter(*TotalTrk));
    }


    auto ExportTrack = [this, TrackerCollection](const std::string &CollectionName) {
        if (TrackerCollection.count(CollectionName) != 0) {
            auto col = TrackerCollection.at(CollectionName);

            // Step 1: Record hits information (x,y,z)
            // Create index of hits
            std::vector<size_t> indices(col->size());
            std::iota(indices.begin(), indices.end(), 0);
            for (auto itr: *col) {
                node.at(CollectionName).at("x").push_back(itr->getX());
                node.at(CollectionName).at("y").push_back(itr->getY());
                node.at(CollectionName).at("z").push_back(itr->getZ());
            }
            auto sorted_indices = sort_by_key(node.at(CollectionName), "z");


            // Step 2: Link nodes (only link the neighbour layers)

            // --> First to split hits into different layers
            double prev_z = -9999999;
            size_t idx = -1;
            std::vector<std::vector<size_t>> split_indices;
            for (size_t i: indices) {
                auto cur_z = node.at(CollectionName).at("z").at(i);
                if (fabs(cur_z - prev_z) < eps)
                    split_indices.at(idx).push_back(i);
                else if (cur_z > prev_z) {
                    split_indices.push_back({i});
                    idx++;
                    prev_z = cur_z;
                } else {
                    cerr << i << " error state." << endl;
                }
            }
            // --> Second fill indices in bi-direction
            for (size_t i = 1; i < split_indices.size(); ++i) {
                for (size_t j: split_indices.at(i - 1)) {
                    for (size_t k: split_indices.at(i)) {
                        // bi-direction but only record forward here
                        // forward
                        edge.at(CollectionName).at("start").push_back(j);
                        edge.at(CollectionName).at("end").push_back(k);
                    }
                }
            }

            // Step 3: Record Truth Edge Relation
            std::map<int, std::vector<std::tuple<size_t, float>>> truth_edge;

            for (size_t i = 0; i < sorted_indices.size(); ++i) {
                for (const auto &mc: col->at(sorted_indices.at(i))->getPContribution()) {
                    if (truth_edge.find(mc.getId()) != truth_edge.end()) {
                        truth_edge.at(mc.getId()).emplace_back(std::make_tuple(i, mc.getEnergy()));
                    } else {
                        truth_edge.insert({mc.getId(), {std::make_tuple(i, mc.getEnergy())}});
                    }
                }
            }

            // --> For each edge, find if it's in the truth_edge
            unsigned long truth_count = 0;
            for (size_t i = 0; i < edge.at(CollectionName).at("start").size(); ++i) {
                auto vs = edge.at(CollectionName).at("start").at(i);
                auto ve = edge.at(CollectionName).at("end").at(i);
                edge.at(CollectionName).at("truth").push_back(0);
                edge.at(CollectionName).at("p").push_back(0);
                for (const auto &trk: truth_edge) {
                    for (unsigned j = 1; j < trk.second.size(); ++j) {
                        if (
                                (vs == std::get<0>(trk.second.at(j - 1)) && ve == std::get<0>(trk.second.at(j))) ||
                                (ve == std::get<0>(trk.second.at(j - 1)) && vs == std::get<0>(trk.second.at(j)))
                                ) {
                            edge.at(CollectionName).at("truth").at(i) = 1;
                            edge.at(CollectionName).at("p").at(i) =
                                    static_cast<size_t>(std::get<1>(trk.second.at(j - 1)));
                            truth_count++;
                            break;
                        }
                    }
                }
            }
            weight.at(CollectionName) = static_cast<double >(truth_count) /
                                        static_cast<double >(edge.at(CollectionName).at("start").size());
        } else {
            cerr << CollectionName << " not found" << endl;
        }
    };
    for (const auto &c: collections) {
        ExportTrack(c);
    }

    if (t)
        t->Fill();

    evt->DeleteCollection("TagTrk1+RecTrk1");
    evt->DeleteCollection("AllTrk");

    for (auto &i: node) {
        for (auto &j: i.second) {
            j.second.clear();
        }
    }

    for (auto &i: edge) {
        for (auto &j: i.second) {
            j.second.clear();
        }
    }
}

void DataExporter::CheckEvt(AnaEvent * /*evt*/) {
    //cout<<"Check!"<<endl;

}

void DataExporter::End() {
    if (f && t) {
        f->cd();
        t->Write();
        f->Close();
    }
}

DataExporter::DataExporter(string name, shared_ptr<EventStoreAndWriter> evtwrt) : AnaProcessor(std::move(name),
                                                                                               std::move(evtwrt)) {
    // Add description for this AnaProcessor
    Description = "Export wanted data.";

    eps = 1e-6;

    // Register Int parameter
    RegisterIntParameter("Verbose", "Verbosity Variable", &verbose, 0);
}

void DataExporter::InitEvt() {

}


