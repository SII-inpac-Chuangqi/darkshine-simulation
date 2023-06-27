//
// Created by Zhang Yulei on 9/20/20.
//

#ifndef DSIMU_CONFIGMANAGER_H
#define DSIMU_CONFIGMANAGER_H

#include <utility>

#include "Utility/Config.h"
#include "Core/AlgoManager.h"

class ConfigManager {
private:
    std::string inputfile; // input root file
    std::string inputGeofile; // input Geometry root file
    std::string outputfile; // output root file
    std::string AlgoList;
    int RunNumber{0};
    int EventNumber{-1};
    int SkipNumber{0};

    // Verbose
    int AlgoManager_Verbose{0};
    int EventReader_Verbose{0};
    int AnaProcessor_Verbose{0};
    int DEvent_Verbose{0};
    int EventStoreAndWriter_Verbose{0};
    int MemoryCheck_Verbose{0};

public:
    ConfigManager() = default;

    ConfigManager(string configfile, AlgoManager *algomgr);

    ~ConfigManager();

    // Read
    void ReadConst();

    void ReadAlgoList();

    void ReadAnaParameters();

    // Get Methods
    const string &getInputfile() const {
        return inputfile;
    }

    const string &getInputGeofile() const {
        return inputGeofile;
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

    int getAlgoManagerVerbose() const {
        return AlgoManager_Verbose;
    }

    int getEventReaderVerbose() const {
        return EventReader_Verbose;
    }

    int getAnaProcessorVerbose() const {
        return AnaProcessor_Verbose;
    }

    int getDEventVerbose() const {
        return DEvent_Verbose;
    }

    int getEventStoreAndWriterVerbose() const {
        return EventStoreAndWriter_Verbose;
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

    void setAlgoManagerVerbose(int algoManagerVerbose) {
        AlgoManager_Verbose = algoManagerVerbose;
    }

    void setEventReaderVerbose(int eventReaderVerbose) {
        EventReader_Verbose = eventReaderVerbose;
    }

    void setAnaProcessorVerbose(int anaProcessorVerbose) {
        AnaProcessor_Verbose = anaProcessorVerbose;
    }

    void setDEventVerbose(int dEventVerbose) {
        DEvent_Verbose = dEventVerbose;
    }

    void setEventStoreAndWriterVerbose(int eventStoreAndWriterVerbose) {
        EventStoreAndWriter_Verbose = eventStoreAndWriterVerbose;
    }

    int getMemoryCheckVerbose() const {
        return MemoryCheck_Verbose;
    }

    void setMemoryCheckVerbose(int memoryCheckVerbose) {
        MemoryCheck_Verbose = memoryCheckVerbose;
    }

    static bool SetupAnaParameters(Config *config, std::string algo_name, AnaProcessor *algo_proccessor) {
        // Read Int Parameter
        auto IntPara = algo_proccessor->getIntParameters();
        for (const auto &itr_int : IntPara) {
            auto parameter_name = itr_int.first;
            auto parameter_value = *(itr_int.second.second);
//            auto read_str = algo_name.append(".").append(parameter_name);
            auto read_str = algo_name + "." + parameter_name;
            auto readin_value = config->Read(read_str, parameter_value);
            algo_proccessor->setIntValue(parameter_name, readin_value);
        }
        // Read Double Parameter
        auto DoublePara = algo_proccessor->getDoubleParameters();
        for (const auto &itr_double : DoublePara) {
            auto parameter_name = itr_double.first;
            auto parameter_value = *(itr_double.second.second);
//            auto read_str = algo_name.append(".").append(parameter_name);
            auto read_str = algo_name + "." + parameter_name;
            auto readin_value = config->Read(read_str, parameter_value);
            algo_proccessor->setDoubleValue(parameter_name, readin_value);
        }
        // Read String Parameter
        auto StrPara = algo_proccessor->getStringParameters();
        for (const auto &itr_str : StrPara) {
            auto parameter_name = itr_str.first;
            auto parameter_value = *(itr_str.second.second);
//            auto read_str = algo_name.append(".").append(parameter_name);
            auto read_str = algo_name + "." + parameter_name;
            auto readin_value = config->Read(read_str, parameter_value);
            algo_proccessor->setStringValue(parameter_name, readin_value);
        }

        return true;
    }

private:
    std::string configfile;

    AlgoManager *algomgr{};
    Config *config{};
};


#endif //DSIMU_CONFIGMANAGER_H
