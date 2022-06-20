//
// Created by Zhang Yulei on 1/2/21.
// Little modified by Qibin, 20/06/2022 to adapt into DSS fw.
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
} // namespace

int main(int argc, char **argv) {
    if (argc > 3 || argc < 2) {
        PrintUsage();
        return 1;
    }

    std::string configfile;
    if (std::string(argv[1]) == "-c")
        configfile = argv[2];
    else {
        PrintUsage();
        return -1;
    }

    auto plot = new PlotYAML(configfile);

    // Setup global style

    SetDarkSHINEStyle();

    plot->ReadRegions();

    for (const auto &rl : plot->region_list) {
        std::cout << "==> plotting region: ";
        for (auto r : rl) {
            std::cout << std::get<0>(r) << " ";
        }
        std::cout << std::endl;

        plot->Plot(rl);
    }

    if (plot->region_list.empty())
        plot->Plot();
    return 0;
}