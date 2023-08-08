//
// Created by Zhang Yulei on 9/20/20.
//

#include "Core/ConfigManager.h"

#include <utility>

ConfigManager::ConfigManager(string _configfile, AlgoManager *_algomgr) : configfile(std::move(_configfile)),
                                                                               algomgr(_algomgr) {
    config = new Config(configfile);
}

void ConfigManager::ReadConst() {
    /*
     * Read Const Running Configuration
     */
    inputfile = config->Read("InputFile", std::string("dp_out.root"));
    inputGeofile = config->Read("InputGeoFile", inputfile);
    outputfile = config->Read("OutputFile", std::string("dp_ana.root"));
    RunNumber = config->Read("RunNumber", 0);
    EventNumber = config->Read("EventNumber", -1);
    SkipNumber = config->Read("SkipNumber", 0);

    // Read Verbosity
    AlgoManager_Verbose = config->Read("AlgoManager.Verbose", 0);
    EventReader_Verbose = config->Read("EventReader.Verbose", 0);
    AnaProcessor_Verbose = config->Read("Processor.Verbose", 0);
    DEvent_Verbose = config->Read("Event.Verbose", 0);
    EventStoreAndWriter_Verbose = config->Read("EventStoreAndWriter.Verbose", 0);
    MemoryCheck_Verbose = config->Read("MemoryCheck.Verbose", 0);
}


void ConfigManager::ReadAlgoList() {
    /*
     * Read Algorithm Processors List
     */

    AlgoList = config->Read("Algorithm.List", std::string(""));
    algomgr->SetAnaProcessorsList(AlgoList);

}

void ConfigManager::ReadAnaParameters() {
    /*
     * Read Parameters from Configuration
     * for registered AnaProcessors
     */
    auto AnaPro = algomgr->getAnaProcessors();
    for (const auto &itr : AnaPro) {
        //auto algo_name = itr.first;
        auto algo_proccessor = itr.second;
        auto algo_name = itr.first;

        SetupAnaParameters(config,algo_name,algo_proccessor.get());
    }
}

ConfigManager::~ConfigManager() {
    delete config;
}

