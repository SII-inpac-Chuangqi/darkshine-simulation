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

    // Set Methods
    void setAnaProcessors(const AnaProcessorMap &anaProcessors) {
        AnaProcessors = anaProcessors;
    }

    // Register Processors
    void RegisterAnaProcessor(AnaProcessor* AnaP);

    // Run AnaProcessor
    void BeginAnaProcessors();
    void ProcessEvtAnaProcessors(DEvent* );
    void CheckEvtAnaProcessors(DEvent* );
    void EndAnaProcessors();

private:
    AnaProcessorMap AnaProcessors;
};


#endif //DSIMU_ALGOMANAGER_H
