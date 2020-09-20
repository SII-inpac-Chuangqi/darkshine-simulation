//
// Created by Zhang Yulei on 9/20/20.
//

#ifndef DSIMU_CONFIGMANAGER_H
#define DSIMU_CONFIGMANAGER_H

#include <utility>

#include "Utility/Config.h"
#include "Core/AlgoManager.h"

class ConfigManager
{
private:
    std::string inputfile; // input root file
    std::string outputfile; // output root file
    std::string AlgoList;
    int RunNumber{0};
    int EventNumber{-1};
    int SkipNumber{0};

public:
    ConfigManager() = default;

    ConfigManager(const string &configfile, AlgoManager *algomgr);

    ~ConfigManager() = default;

    // Read
    void ReadConst();
    void ReadAlgoList();
    void ReadAnaParameters();

    // Get Methods
    const string &getInputfile() const {
        return inputfile;
    }

    const string &getOutputfile() const {
        return outputfile;
    }

    int getRunNumber() const {
        return RunNumber;
    }

    int getEventNumber() const {
        return EventNumber;
    }

    int getSkipNumber() const {
        return SkipNumber;
    }

    const string &getAlgoList() const {
        return AlgoList;
    }

    // Set Methods
    void setInputfile(const string &in) {
        ConfigManager::inputfile = in;
    }

    void setOutputfile(const string &in) {
        ConfigManager::outputfile = in;
    }

    void setRunNumber(int runNumber) {
        RunNumber = runNumber;
    }

    void setEventNumber(int eventNumber) {
        EventNumber = eventNumber;
    }

    void setSkipNumber(int skipNumber) {
        SkipNumber = skipNumber;
    }

    void setAlgoList(const string &algoList) {
        AlgoList = algoList;
    }

private:
    std::string configfile;

    AlgoManager* algomgr{};
    Config* config{};
};


#endif //DSIMU_CONFIGMANAGER_H
