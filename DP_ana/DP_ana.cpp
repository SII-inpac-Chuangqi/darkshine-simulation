//
// Created by Zhang Yulei on 9/18/20.
//

#include <iostream>

#include "Core/AlgoManager.h"
#include "Core/ControlManager.h"

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
        cerr << "DAna " << "v1.5.4" << endl;
    }

    void PrintUsage() {
        PrintIntroduction();
        if_introduction = false;

        cerr << " Usage: " << endl;
        cerr << " DAna [ -c config ] [ -x ]" << endl;
        cerr << "   note: -x option will print all avaliable AnaProcessors" << endl;
        cerr << endl;
    }
}

int main(int argc, char **argv) {
    if (argc > 3 || argc < 2) {
        PrintUsage();
        return 1;
    }

    bool PrintUsage = false;
    std::string configfile;
    if (std::string(argv[1]) == "-c") configfile = argv[2];
    else if (std::string(argv[1]) == "-x") {
        PrintUsage = true;
    } else if (std::string(argv[1]) == "-v") {
        PrintVersion();
        return EXIT_SUCCESS;
    } else return -1;

    if(if_introduction) PrintIntroduction();

    AnaData::CreateInstance();

    auto control = new ControlManager();
    control->setOnlyPrintUsage(PrintUsage);

    auto evtrdr = new EventReader();
    control->setEvtReader(evtrdr);

    auto algo = new AlgoManager();
    control->setAlgo(algo);

    if (!PrintUsage) control->setConfMgr(new ConfigManager(configfile, algo));
    control->run();

    delete control;
    return 0;
}
