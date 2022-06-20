//
// Created by Zhang Yulei on 1/2/21.
//

#ifndef DSIMU_PLOTYAML_H
#define DSIMU_PLOTYAML_H

// From ROOT
#include "TCanvas.h"
#include "TCut.h"
#include "TFile.h"
#include "TFrame.h"
#include "TH1D.h"
#include "TPaveText.h"
#include "TRatioPlot.h"
#include "TTreeReader.h"

#include "yaml-cpp/yaml.h"

#include "Plot_Util.h"

#include <map>
#include <memory>
#include <string>

typedef std::tuple<TString, TString, TCut> region_t;
typedef std::vector<region_t> region_v;

/// \brief Plot graph from yaml config file
class PlotYAML {
public:
    explicit PlotYAML(const std::string &file_in);

    void ReadRegions();

    void ReadHists(const TCut &region_cut = "1", const TString &region_name = "");

    void Plot(const region_v &region_info = region_v(1, std::make_tuple("", "",
                                                                        "1")));

    void PlotLogo_ATLAS(TCanvas *c, const YAML::detail::iterator_value &var,
                        const std::vector<TString> &region_title);

    void PlotLogo_DarkSHINE(TCanvas *c, const TString &cuts = "");

    static void PlotLogo_LLP(TCanvas *c, double x_offset = 0., double y_offset = 0.);

    std::tuple<TString, TString>
    FormatAxis(const YAML::detail::iterator_value &var);

public:
    // list of regions
    std::vector<region_v> region_list;

    std::string global_style;

private:
    YAML::Node node;

    std::string work_name;

    TCut global_selection;

    std::string global_selection_title;
    bool global_ratio_plot;
    double global_y_max_factor;
    bool global_normalize;

    bool global_log_y;
    bool global_grid_y;
    std::array<std::string,2> global_ratio_samples={"",""};
    std::array<double,2> global_ratio_range={0,2};

    // list of hists [variables][samples]
    std::map<std::string, std::vector<TH1D *>> hists_list;
};

#endif // DSIMU_PLOTYAML_H
