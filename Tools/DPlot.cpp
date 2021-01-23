//
// Created by Zhang Yulei on 1/2/21.
//

#include "DarkStyle.h"
#include "PlotYAML.h"

#include <iostream>

using namespace std;

namespace {
    void PrintUsage() {
        cerr << " Usage: " << endl;
        cerr << " DPlot [ -c config ]" << endl;
        cerr << endl;
    }
}

int main(int argc, char **argv) {
    if (argc > 3 || argc < 2) {
        PrintUsage();
        return 1;
    }

    std::string configfile;
    if (std::string(argv[1]) == "-c") configfile = argv[2];
    else {
        PrintUsage();
        return -1;
    }

    // Setup global style
    SetDarkSHINEStyle();

    auto plot = new PlotYAML(configfile);
    plot->Plot();


    return 0;
}