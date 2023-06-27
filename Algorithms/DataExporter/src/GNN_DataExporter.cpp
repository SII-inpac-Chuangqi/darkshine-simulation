//
// Created by Zhang Yulei on 9/19/20.
//

#include <utility>
#include <filesystem>
#include <numeric>
#include <chrono>
#include <sstream>
#include <algorithm>

#include "Core/AnaData.h"
#include "Algo/GNN_DataExporter.h"
#include "Algo/GNN_Digitization.h"


std::vector<std::string> split(std::string_view strv, char delimiter) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = strv.find(delimiter);
    while (end != std::string_view::npos) {
        result.emplace_back(strv.substr(start, end - start));
        start = end + 1;
        end = strv.find(delimiter, start);
    }
    result.emplace_back(strv.substr(start));
    return result;
}

void GNN_DataExporter::Begin() {

    // initialize the magnetic field
    magnets = dAnaData->getMagFieldVec();
    if (magnets.size() != 3 || !magnets.at(0) || !magnets.at(1) || !magnets.at(2)) {
        dAnaData->setConstMagnetField({0, BField, 0});
        std::cout << "[GNN_DataExporter]: No magnetic field found, set to default value: " << BField << std::endl;
    } else {
        std::cout << "[GNN_DataExporter]: Magnetic field found in Geometry" << std::endl;
    }


    f = new TFile("Tracker_GNN.root", "RECREATE");
    t = new TTree("dp", "dp");

    collections = split(arg_collections, ',');
//    collections = {"DigitizedTagTrk", "DigitizedRecTrk","TagTrk"};

    std::vector<std::string> gnn_node = {"x", "y", "z", "Bx", "By", "Bz"};
    std::vector<std::string> gnn_edge = {"start", "end", "truth", "p"};

    for (const auto &col: collections) {
        node.insert({col, std::map<std::string, std::vector<double>>()});
        edge.insert({col, std::map<std::string, std::vector<size_t>>()});
        weight.insert({col, 0.0});
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

    t->Branch("run_num", &run_num, "run_num/L");
    t->Branch("evt_num", &evt_num, "evt_num/L");
}

std::vector<size_t>
GNN_DataExporter::sort_by_key(std::map<std::string, std::vector<double>> &hits, const std::string &key) {
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

void GNN_DataExporter::export_track(const std::string &CollectionName, const SimulatedHitMap &TrackerCollection,
                                    long long evtNum) {
    if (TrackerCollection.count(CollectionName) != 0) {
        auto col = TrackerCollection.at(CollectionName);

        if (col->empty()) {
            if (verbose > 1) {
                cerr << "[ " << evtNum << " ]Collection: " << CollectionName << " is empty" << endl;

                weight.at(CollectionName) = 0.0;
            }
            return;
        }

        // Step 1: Record hits information (x,y,z)
        // Create index of hits
        std::vector<size_t> indices(col->size());
        std::iota(indices.begin(), indices.end(), 0);
        for (auto itr: *col) {
            node[CollectionName]["x"].push_back(itr->getX());
            node[CollectionName]["y"].push_back(itr->getY());
            node[CollectionName]["z"].push_back(itr->getZ());

            auto BFieldAtPoint = dAnaData->getMagnetFieldAt({itr->getX(), itr->getY(), itr->getZ()});

            node[CollectionName]["Bx"].push_back(BFieldAtPoint[0]);
            node[CollectionName]["By"].push_back(BFieldAtPoint[1]);
            node[CollectionName]["Bz"].push_back(BFieldAtPoint[2]);
        }
        auto sorted_indices = sort_by_key(node[CollectionName], "z");


        // Step 2: Link nodes (only link the neighbour layers)

        // --> First to split hits into different layers
        double prev_z = -9999999;
        size_t idx = -1;
        std::vector<std::vector<size_t>> split_indices;
        for (size_t i: indices) {
            auto cur_z = node[CollectionName]["z"][i];
            if (fabs(cur_z - prev_z) < eps)
                split_indices[idx].push_back(i);
            else if (cur_z > prev_z) {
                split_indices.push_back({i});
                idx++;
                prev_z = cur_z;
            } else {
                cerr << i << " error state." << endl;
            }
        }

        // --> Second fill indices in bi-direction
        size_t split_indices_size = split_indices.size();
        for (size_t i = 1; i < split_indices_size; ++i) {
            for (size_t j: split_indices[i - 1]) {
                for (size_t k: split_indices[i]) {
                    // bi-direction but only record forward here
                    // forward
                    edge[CollectionName]["start"].push_back(j);
                    edge[CollectionName]["end"].push_back(k);
                }
            }
        }

        // Step 3: Record Truth Edge Relation
        std::map<int, std::vector<std::tuple<size_t, float>>> truth_edge;

        size_t sorted_indices_size = sorted_indices.size();
        for (size_t i = 0; i < sorted_indices_size; ++i) {
            for (const auto &mc: col->at(sorted_indices[i])->getPContribution()) {
                if (truth_edge.find(mc.getId()) != truth_edge.end()) {
                    truth_edge[mc.getId()].emplace_back(std::make_tuple(i, mc.getEnergy()));
                } else {
                    truth_edge.insert({mc.getId(), {std::make_tuple(i, mc.getEnergy())}});
                }
            }
        }

        // --> For each edge, find if it's in the truth_edge
        // map<mc_id, vector<node_id>>
        std::map<int, std::vector<size_t> > edge_remove_mc;
        size_t edge_start_size = edge[CollectionName]["start"].size();
        unsigned long truth_count = 0;
        for (size_t i = 0; i < edge_start_size; ++i) {
            auto vs = edge[CollectionName]["start"][i];
            auto ve = edge[CollectionName]["end"][i];
            edge[CollectionName]["truth"].push_back(0);
            edge[CollectionName]["p"].push_back(0);
            for (const auto &trk: truth_edge) {
                size_t trk_second_size = trk.second.size();
                for (unsigned j = 1; j < trk_second_size; ++j) {
                    if (
                            (vs == std::get<0>(trk.second[j - 1]) && ve == std::get<0>(trk.second[j])) ||
                            (ve == std::get<0>(trk.second[j - 1]) && vs == std::get<0>(trk.second[j]))
                            ) {
                        edge[CollectionName]["truth"][i] = 1;
                        edge[CollectionName]["p"][i] =
                                static_cast<size_t>(std::get<1>(trk.second[j - 1]));
                        truth_count++;

                        edge_remove_mc[trk.first].push_back(i);
                        break;
                    }
                }
            }
        }
        // Remove MC particles if they only contributes to less than three hits (< 2 edges)
        for (const auto &trk: edge_remove_mc) {
            if (trk.second.size() == 1) {
                if (verbose > 1)
                    cout << "MC " << trk.first << " only contributes to one edge, remove it." << endl;
                edge[CollectionName]["truth"][trk.second.front()] = 0;
                truth_count--;
            }
        }

        weight.at(CollectionName) = static_cast<double >(truth_count) /
                                    static_cast<double >(edge.at(CollectionName).at("start").size());

        if (verbose > 3) {
            cout << "[ " << evtNum << " ]Collection: " << CollectionName << endl;
            cout << "  -- Truth Edge Count: " << truth_count << endl;
            cout << "  -- Total Edge Count: " << edge.at(CollectionName).at("start").size() << endl;
            cout << "  -- Weight: " << weight.at(CollectionName) << endl;

            if (std::isnan(weight.at(CollectionName))) {
                cerr << "Weight is nan" << endl;
                exit(1);
            }
        }

    } else {
        cerr << CollectionName << " not found" << endl;
    }
}

void GNN_DataExporter::ProcessEvt(AnaEvent *evt) {

    run_num = evt->getRunId();
    evt_num = evt->getEventId();

    const auto &TrackerCollection = evt->getSimulatedHitCollection();

    auto contains = [this](const std::string &str) -> bool {
        return std::find(collections.begin(), collections.end(), str) != collections.end();
    };

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
        if (contains("AllTrk")) {
            SimulatedHitVec *TotalTrk = evt->RegisterSimulatedHitCollection("AllTrk");
            TotalTrk->reserve(tag1_col->size() + rec1_col->size() + tag2_col->size() + rec2_col->size());

            std::copy(std::begin(*tag1_col), std::end(*tag1_col), std::back_inserter(*TotalTrk));
            std::copy(std::begin(*rec1_col), std::end(*rec1_col), std::back_inserter(*TotalTrk));
            std::copy(std::begin(*tag2_col), std::end(*tag2_col), std::back_inserter(*TotalTrk));
            std::copy(std::begin(*rec2_col), std::end(*rec2_col), std::back_inserter(*TotalTrk));
        }
        if (contains("TagTrk")) {
            SimulatedHitVec *TotalTagTrk = evt->RegisterSimulatedHitCollection("TagTrk");
            TotalTagTrk->reserve(tag1_col->size() + tag1_col->size()); // Optionally reserve space to avoid reallocation
            std::copy(std::begin(*tag1_col), std::end(*tag1_col), std::back_inserter(*TotalTagTrk));
            std::copy(std::begin(*tag2_col), std::end(*tag2_col), std::back_inserter(*TotalTagTrk));
        }
        if (contains("RecTrk")) {
            SimulatedHitVec *TotalRecTrk = evt->RegisterSimulatedHitCollection("RecTrk");
            TotalRecTrk->reserve(rec1_col->size() + tag1_col->size()); // Optionally reserve space to avoid reallocation
            std::copy(std::begin(*rec1_col), std::end(*rec1_col), std::back_inserter(*TotalRecTrk));
            std::copy(std::begin(*rec2_col), std::end(*rec2_col), std::back_inserter(*TotalRecTrk));
        }

        if (contains("DigitizedTagTrk") || contains("AllDigitizedTrk")) {
            // Add digitized tracker hits collections
            SimulatedHitVec *DigiTagTrk = evt->RegisterSimulatedHitCollection("DigitizedTagTrk");
            SimulatedHitVec tmp_digi_tag = GNN_Digitization::Run(tag1_col, tag2_col, true, MinEnergy);
            std::move(tmp_digi_tag.begin(), tmp_digi_tag.end(), std::back_inserter(*DigiTagTrk));
            tmp_digi_tag.clear();
        }

        if (contains("DigitizedRecTrk") || contains("AllDigitizedTrk")) {
            SimulatedHitVec *DigiRecTrk = evt->RegisterSimulatedHitCollection("DigitizedRecTrk");
            SimulatedHitVec tmp_digi_rec = GNN_Digitization::Run(rec1_col, rec2_col, false, MinEnergy);
            std::move(tmp_digi_rec.begin(), tmp_digi_rec.end(), std::back_inserter(*DigiRecTrk));
            tmp_digi_rec.clear();
        }

        if (contains("AllDigitizedTrk")) {
            SimulatedHitVec *AllDigiTrk = evt->RegisterSimulatedHitCollection("AllDigitizedTrk");
            auto tag_digi = TrackerCollection.at("DigitizedTagTrk");
            auto rec_digi = TrackerCollection.at("DigitizedRecTrk");

            AllDigiTrk->reserve(tag_digi->size() + rec_digi->size());
            std::copy(std::begin(*tag_digi), std::end(*tag_digi), std::back_inserter(*AllDigiTrk));
            std::copy(std::begin(*rec_digi), std::end(*rec_digi), std::back_inserter(*AllDigiTrk));
        }

    }

    for (const auto &c: collections) {
        export_track(c, TrackerCollection, evt->getEventId());
    }

    if (t)
        t->Fill();

    if (contains("AllTrk")) {
        evt->DeleteCollection("AllTrk");
    }
    if (contains("TagTrk")) {
        evt->DeleteCollection("TagTrk");
    }
    if (contains("RecTrk")) {
        evt->DeleteCollection("RecTrk");
    }
    if (contains("DigitizedTagTrk")) {
        evt->DeleteCollection("DigitizedTagTrk");
    }
    if (contains("DigitizedRecTrk")) {
        evt->DeleteCollection("DigitizedRecTrk");
    }
    if (contains("AllDigitizedTrk")) {
        evt->DeleteCollection("AllDigitizedTrk");
    }

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

void GNN_DataExporter::CheckEvt(AnaEvent * /*evt*/) {
    //cout<<"Check!"<<endl;

}

void GNN_DataExporter::End() {
    if (f && t) {
        f->cd();
        t->Write();
        f->Close();
    }
}

GNN_DataExporter::GNN_DataExporter(string name, shared_ptr<EventStoreAndWriter> evtwrt) : AnaProcessor(
        std::move(name), std::move(evtwrt)
) {
    // Add description for this AnaProcessor
    Description = "Export wanted data.";

    eps = 0.1;

    RegisterIntParameter("Verbose", "Verbosity Variable", &verbose, 0);
    // Register the magnetic filed value (+y direction)
    RegisterDoubleParameter("BField", "Magnetic field value (+y)", &BField, -1.5);
    // Register the minimum energy for truth mc particles to be considered
    RegisterDoubleParameter("MinEnergy", "Minimum energy [MeV] for truth mc particles to be considered",
                            &MinEnergy, 1.0);
    RegisterStringParameter(
            "Collections",
            "Select from [DigitizedTagTrk, DigitizedRecTrk, TagTrk, RecTrk, AllTrk, AllDigitizedTrk], split with comma and no space",
            &arg_collections,
            "DigitizedTagTrk,DigitizedRecTrk");

}

void GNN_DataExporter::InitEvt() {}


