//
// Created by Zhang Yulei on 9/19/20.
//

#ifndef DSIMU_CONTROLMANAGER_H
#define DSIMU_CONTROLMANAGER_H

#include "Core/AlgoManager.h"
#include "Core/EventReader.h"

class ControlManager {
    /*
     * Control the whole analysis workflow
     */
public:
    ControlManager() = default;

    ~ControlManager() = default;

    void run();

    //

    AlgoManager *getAlgo() const {
        return algo;
    }

    void setAlgo(AlgoManager *in) {
        algo = in;
    }

    int getRunNumber() const {
        return RunNumber;
    }

    void setRunNumber(int runNumber) {
        RunNumber = runNumber;
    }

    int getEventNumber() const {
        return EventNumber;
    }

    void setEventNumber(int eventNumber) {
        EventNumber = eventNumber;
    }

    int getSkipNumber() const {
        return SkipNumber;
    }

    void setSkipNumber(int skipNumber) {
        SkipNumber = skipNumber;
    }

    EventReader *getEvtReader() const {
        return EvtReader;
    }

    void setEvtReader(EventReader *evtReader) {
        EvtReader = evtReader;
    }

    const string &getFileName() const {
        return FileName;
    }

    void setFileName(const string &fileName) {
        FileName = fileName;
    }

private:
    int RunNumber{0};
    int EventNumber{-1};
    int SkipNumber{0};

    std::string FileName;

    AlgoManager* algo;
    EventReader* EvtReader;
};


#endif //DSIMU_CONTROLMANAGER_H
