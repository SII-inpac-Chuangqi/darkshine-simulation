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
    const AnaProcessorMap &getAnaProcessors() const {
        return AnaProcessors;
    }

    int getVerbose() const {
        return Verbose;
    }

    AnaProcessorVec* getAllAnaProcessors();

    // Set Methods
    void setAnaProcessors(const AnaProcessorMap &anaProcessors) {
        AnaProcessors = anaProcessors;
    }

    void setVerbose(int verbose) {
        Verbose = verbose;
    }

    // Register Processors
    void SetAnaProcessorsList(const std::string& ProcessorList);
    void RegisterAnaProcessor(AnaProcessor* AnaP);

    // Run AnaProcessor
    void BeginAnaProcessors();
    void ProcessEvtAnaProcessors(DEvent* );
    void CheckEvtAnaProcessors(DEvent* );
    void EndAnaProcessors();

private:
    // Verbosity
    int Verbose{1};

    AnaProcessorMap AnaProcessors;
    vector<std::string> AnaProcessorList;
};


#endif //DSIMU_ALGOMANAGER_H
