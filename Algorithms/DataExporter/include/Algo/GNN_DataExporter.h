//
// Created by Zhang Yulei on 9/19/20.
//

#ifndef DSIMU_DataExporter_H
#define DSIMU_DataExporter_H

#include <utility>
#include <TFile.h>
#include <TTree.h>

#include "Core/AnaProcessor.h"

using namespace std;

class GNN_DataExporter : public AnaProcessor {
public:
    // No need to change anything here
    // Must initialize with Name
    explicit GNN_DataExporter(string name, shared_ptr<EventStoreAndWriter> evtwrt);

    ~GNN_DataExporter() override = default;

    void Begin() override;

    void InitEvt() override;

    void ProcessEvt(AnaEvent *evt) override;

    void CheckEvt(AnaEvent *evt) override;

    void End() override;

    // Define some functions here if necessary
    static std::vector<size_t> sort_by_key(std::map<std::string, std::vector<double>> &hits, const std::string &key);

    void export_track(const std::string &CollectionName, const SimulatedHitMap &TrackerCollection,
                      long long evtNum);

private:

    // Define some variables if necessary
    // IMPORTANT: variables need to be registered by their types:
    // Currently supported: int, double, string
    int verbose{};
    double eps{};
    std::string arg_collections;
    TFile *f{};
    TTree *t{};

    std::vector<std::string> collections;
    std::map<std::string, std::map<std::string, std::vector<double> > > node;
    std::map<std::string, std::map<std::string, std::vector<size_t> > > edge;
    std::map<std::string, double> weight;
};


#endif //DSIMU_DataExporter_H
