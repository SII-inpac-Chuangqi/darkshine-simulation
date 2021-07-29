//
// Created by Zhang Yulei on 9/19/20.
//

#ifndef DSIMU_CONTROLMANAGER_H
#define DSIMU_CONTROLMANAGER_H

#include "Core/AnaData.h"
#include "Core/AlgoManager.h"
#include "Event/EventReader.h"
#include "Core/ConfigManager.h"
#include "Event/EventStoreAndWriter.h"

class ControlManager {
    /*
     * Control the whole analysis workflow
     */
public:
    ControlManager() = default;

    ~ControlManager() {
        delete EvtReader;
        delete algo;
        delete ConfMgr;
    };

    void run();

    void PrintConfig();



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

    ConfigManager *getConfMgr() const {
        return ConfMgr;
    }

    void setConfMgr(ConfigManager *confMgr) {
        ConfMgr = confMgr;
    }

    const string &getOutName() const {
        return OutName;
    }

    void setOutName(const string &outName) {
        OutName = outName;
    }

    void setOnlyPrintUsage(bool onlyPrintUsage) {
        Only_PrintUsage = onlyPrintUsage;
    }

private:
    int RunNumber{0};
    int EventNumber{-1};
    int SkipNumber{0};

    std::string FileName;
    std::string OutName;

    AlgoManager *algo{};
    EventReader *EvtReader{};
    ConfigManager *ConfMgr{};

    bool Only_PrintUsage = false;

};


#endif //DSIMU_CONTROLMANAGER_H
