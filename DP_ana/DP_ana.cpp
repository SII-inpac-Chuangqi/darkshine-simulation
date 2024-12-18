//
// Created by Zhang Yulei on 9/18/20.
//

#include <iostream>
#include <string>

#include "Core/AlgoManager.h"
#include "Core/ControlManager.h"
#include "Utility/parser.h"

using namespace std;

namespace {
    bool if_introduction(true);

    void PrintIntroduction() {
        cerr << endl;
        cerr << "**************************************************************" << endl;
        cerr << " Copyright @DarkSHINE Project" << endl;
        cerr << "           By Master of C++," << endl;
        cerr << "              Master of Geant4," << endl;
        cerr << "              Master of PhD," << endl;
        cerr << "              Master of All Masters," << endl;
        cerr << "              Yulei Zhang" << endl;
        cerr << "              https://gitlab.com/yulei_zhang" << endl;
        cerr << "**************************************************************" << endl;
    }

    void PrintVersion() {
#ifdef DANA_VERSION
        cerr << "DAna " << "v" + (std::string)DANA_VERSION << std::endl;
#else
        cerr << "DAna " << "v1.5.5" << endl;
#endif
    }
}

int main(int argc, char **argv) {

    bool print_usage = false;
    bool print_version = false;
    std::string configfile;

    arg_parser::Parser parser;
    parser.Add("c,config", configfile, "config.txt", "name of the config file");
    parser.AddFlag("x", print_usage, false, "if print usage");
    parser.AddFlag("v,version", print_version, false, "if print version");
    parser.Parse(argc, argv);

    if(print_version) { PrintVersion(); exit(EXIT_SUCCESS); }

    if(if_introduction) PrintIntroduction();
    PrintVersion();

    AnaData::CreateInstance();

    auto control = new ControlManager();
    control->setOnlyPrintUsage(print_usage);

    auto evtrdr = new EventReader();
    control->setEvtReader(evtrdr);

    auto algo = new AlgoManager();
    control->setAlgo(algo);

    if (!print_usage) control->setConfMgr(new ConfigManager(configfile, algo));
    control->run();

    delete control;
    return 0;
}
