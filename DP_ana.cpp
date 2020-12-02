//
// Created by Zhang Yulei on 9/18/20.
//

#include <iostream>

#include "Core/AlgoManager.h"
#include "Core/ControlManager.h"

using namespace std;

namespace {
    void PrintUsage() {
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

    bool GenerateConfig = false;
    std::string configfile;
    if (std::string(argv[1]) == "-c") configfile = argv[2];
    else if (std::string(argv[1]) == "-x") {
        GenerateConfig = true; // unused
        return 0;
    } else return -1;

    auto control = new ControlManager();

    auto evtrdr = new EventReader();
    control->setEvtReader(evtrdr);

    auto algo = new AlgoManager();
    control->setAlgo(algo);
    control->setConfMgr(new ConfigManager(configfile, algo));
    control->run();

    delete control;
    return 0;
}