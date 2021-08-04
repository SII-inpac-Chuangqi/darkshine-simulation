//
// Created by Zhang Yulei on 9/19/20.
//

#ifndef DSIMU_ALGOMANAGER_H
#define DSIMU_ALGOMANAGER_H

#include "Object/TypeDef.h"
#include "Core/AnaProcessor.h"

class AlgoManager {
public:

    // Constructor and Destructor
    AlgoManager() = default;

    ~AlgoManager() = default;

    // Get Methods
    [[nodiscard]] const AnaProcessorMap &getAnaProcessors() const {
        return AnaProcessors;
    }

    [[nodiscard]] int getVerbose() const {
        return Verbose;
    }

    [[nodiscard]] long long int getProcessedEvt() const {
        return Processed_Evt;
    }

    [[nodiscard]] const map<std::string, double> &getProcessingAvgTime() const {
        return processing_avg_time;
    }

    [[nodiscard]] const vector<std::string> &getAnaProcessorList() const {
        return AnaProcessorList;
    }

    [[nodiscard]] const vector<std::string> &getAnaProcessorListDefault() const {
        return AnaProcessorList_default;
    }

    AnaProcessorVecUniPtr getAllAnaProcessors();

    // Set Methods
    void setAnaProcessors(const AnaProcessorMap &anaProcessors) {
        AnaProcessors = anaProcessors;
    }

    void setVerbose(int verbose) {
        Verbose = verbose;
    }

    void setProcessedEvt(long long int processedEvt) {
        Processed_Evt = processedEvt;
    }

    // Summary Log
    void PrintRunLog();

    // Register Processors
    void SetAnaProcessorsList(const std::string &ProcessorList);

    void RegisterAnaProcessor(const std::shared_ptr<AnaProcessor>& AnaP);

    // Run AnaProcessor
    void BeginAnaProcessors();

    void ProcessEvtAnaProcessors(AnaEvent *, int cur_evt_num = 0);

    void CheckEvtAnaProcessors(AnaEvent *);

    void EndAnaProcessors();

private:
    // Time and Event Log
    long long Processed_Evt{0};
    clock_t start_processing;
    clock_t end_processing;
    clock_t global_start;
    clock_t global_end;
    std::map<std::string, double> processing_avg_time;

    // Verbosity
    int Verbose{0};

    AnaProcessorMap AnaProcessors;
    vector<std::string> AnaProcessorList;

    vector<std::string> AnaProcessorList_default;
};


#endif //DSIMU_ALGOMANAGER_H
