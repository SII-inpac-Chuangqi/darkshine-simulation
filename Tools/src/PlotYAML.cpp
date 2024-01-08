//
// Created by Zhang Yulei on 1/2/21.
// Little modified by Qibin, 20/06/2022 to adapt into DSS fw.
//

// From ROOT
#include "TCanvas.h"
#include "TCut.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TTree.h"

#include "DarkStyle.h"
#include "PlotYAML.h"

#include <algorithm>
#include <cassert>
#include <filesystem>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>

namespace fs = std::filesystem;

TPaveText *CreatePaveText(double x1, double y1, double x2, double y2,
                          const TString &text, double textsize, double textfont,
                          int color = 1) {

    auto *tex = new TPaveText();
    tex->SetFillColor(0);
    tex->SetTextSize(0.05);
    tex->SetFillStyle(0);
    tex->SetBorderSize(0);

    tex->AddText(text.Data());
    tex->SetX1NDC(x1);
    tex->SetY1NDC(y1);
    tex->SetX2NDC(x2);
    tex->SetY2NDC(y2);
    tex->SetTextSize(textsize);
    tex->SetTextColor(color);
    tex->SetTextFont(textfont);
    return tex;
}

void GetX1Y1X2Y2(TVirtualPad *c, double &x1, double &y1, double &x2,
                 double &y2) {
    x1 = c->GetFrame()->GetX1() + c->GetLeftMargin();
    y1 = c->GetFrame()->GetY1() + c->GetBottomMargin();
    x2 = c->GetFrame()->GetX2() - c->GetRightMargin();
    y2 = c->GetFrame()->GetY2() - c->GetTopMargin();
}

void SetAttr(YAML::detail::iterator_value sample, TH1D *h) {
    if (auto s = sample["line_attr"]; s.IsDefined()) {
        h->SetLineColor(s[0].as<int>());
        h->SetLineWidth(s[1].as<int>());
        if (s.size() == 3)
            h->SetLineStyle(s[2].as<int>());
    }
    if (auto s = sample["marker_attr"]; s.IsDefined()) {
        h->SetMarkerColorAlpha(s[0].as<int>(), s[1].as<double>());
        h->SetMarkerSize(s[2].as<int>());
        h->SetMarkerStyle(s[3].as<int>());
    }
    if (auto s = sample["fill_attr"]; s.IsDefined()) {
        h->SetFillColorAlpha(s[0].as<int>(), s[1].as<double>());
        h->SetFillStyle(s[2].as<int>());
    }
}

PlotYAML::PlotYAML(const std::string &file_in) {
    try {
        node = YAML::LoadFile(file_in);
    } catch (YAML::ParserException &e) {
        std::cerr << "[Reading YAML] ==> " << e.msg << std::endl;
    }

    // Prepare work space
    work_name = node["name"].as<std::string>();
    // create a directory if not exists
    fs::create_directories(work_name);

    // Global Configuration
    global_selection =
            node["global_selection"].IsDefined()
            ? TCut(node["global_selection"].as<std::string>().c_str())
            : "1";
    global_selection_title = node["global_selection_title"].IsDefined()
                         ? node["global_selection_title"].as<std::string>():"";
    global_ratio_plot = node["global_ratio_plot"].IsDefined()?
                            node["global_ratio_plot"].as<bool>():
                            false;

    global_y_max_factor = node["global_y_scale"].IsDefined()
                                 ? node["global_y_scale"].as<double>()
                                 : 1.5;

    global_normalize = node["global_normalize"].IsDefined() ?
                            node["global_normalize"].as<bool>():
                            false;

    global_log_y=node["global_log_y"].IsDefined() ?
                            node["global_log_y"].as<bool>():
                            false;
    global_grid_y=node["global_grid_y"].IsDefined() ?
                            node["global_grid_y"].as<bool>():
                            true;
    if(node["global_ratio_samples"].IsDefined())
        global_ratio_samples=node["global_ratio_samples"].as<std::array<std::string,2>>();
    if(node["global_ratio_range"].IsDefined())
        global_ratio_range= node["global_ratio_range"].as<std::array<double,2>>();
}

/// \brief read histograms from root file
void PlotYAML::ReadHists(const TCut &region_cut, const TString &region_name) {

    auto inject_hist = node["inject_hist"].IsDefined() && node["inject_hist"].as<bool>();

    auto c1 = new TCanvas("c1", "c1", 1200, 1000);

    // Initialize a map storing all histograms [var_name]:[hists of all samples]
    for (auto var : node["variables"])
        hists_list.try_emplace(var["name"].as<std::string>(),
                               std::vector<TH1D *>());

    int n = 0;
    // For each file
    for (auto sample : node["samples"]) {
        auto name = sample["name"].as<std::string>();
        // Read in tree
        //    auto file =
        //        std::make_unique<TFile>(sample["file_name"].as<std::string>().data());
        auto file = new TFile(sample["file_name"].as<std::string>().data());
        file->cd();

        TTree *tree = nullptr;
        if (!inject_hist)
            tree = dynamic_cast<TTree *>(
                    file->Get(sample["tree_name"].as<std::string>().data()));

        // For each tree, draw the variables
        // Loop variables
        int vn = 0;
        for (auto var : node["variables"]) {
            auto var_name = var["name"].as<std::string>();

            bool var_insert = true;

            bool ratio_plot = var["ratio_plot"].IsDefined()?
                                var["ratio_plot"].as<bool>():
                                global_ratio_plot;

            if (ratio_plot) {
                auto ratio_samples=var["ratio_samples"].IsDefined()?
                                        var["ratio_samples"].as<std::array<std::string,2>>():
                                        global_ratio_samples;
                var_insert = (name == ratio_samples[0]) ||
                             (name == ratio_samples[1]);
            }

            if (var_insert) {
                TCut selection =
                        sample["selection"].IsDefined()
                        ? TCut(sample["selection"].as<std::string>().c_str())
                        : global_selection;

                TCut weight = sample["weight"].IsDefined()
                              ? TCut(sample["weight"].as<std::string>().c_str())
                              : "1";

                TH1D *h = nullptr;
                if (!inject_hist) {
                    auto vbins = var["vbins"].IsDefined()?var["vbins"].as<std::vector<double>>():std::vector<double>{};
                    if(vbins.size()>0){
                        h = new TH1D(("h_" + std::to_string(n)).data(), name.data(), vbins.size()-1,
                                    &vbins[0]);
                    }
                    else{
                        auto xbin = var["range"][0].as<int>();
                        auto xmin = var["range"][1].as<double>();
                        auto xmax = var["range"][2].as<double>();
                        h = new TH1D(("h_" + std::to_string(n)).data(), name.data(), xbin,
                                    xmin, xmax);
                    }
                    std::string draw_text = var_name + ">>" + std::string(h->GetName());

                    tree->Draw(draw_text.data(),
                               weight * (selection && global_selection && region_cut),
                               "");

                    SetAttr(sample, h);
                    hists_list.at(var_name).push_back(h);
                } else {
                    TString hist_name;
                    hist_name.Form("fig%s_%s", region_name.Data(),
                                   std::to_string(vn).data());
                    std::cout << hist_name << std::endl;
                    h = (TH1D *) (file->Get(hist_name))->Clone();
                    h->SetTitle(name.data());

                    SetAttr(sample, h);
                    hists_list.at(var_name).push_back(h);
                }
                n++;
                vn++;
            }
        }
    }
    delete c1;
}

void PlotYAML::Plot(const region_v &region_info) {
    std::vector<TString> region_name;
    std::vector<TString> region_title;
    TCut region_cut = "1";
    std::string sub_path;

    if (auto cn = node["extra_title"];  node["extra_title"].IsDefined()) {
        for (auto ccn: cn) {
            region_title.emplace_back(ccn.as<std::string>());
        }
    }

    for (auto ri : region_info) {
        region_name.push_back(std::get<0>(ri));
        region_title.push_back(std::get<1>(ri));
        region_cut = region_cut && std::get<2>(ri);

        sub_path.append("/" + std::get<0>(ri));
    }

    fs::create_directories(work_name + sub_path);

    std::string region_str;
    for (const auto &r_s : region_name) {
        if (r_s.IsNull())
            continue;
        region_str.append("_");
        region_str.append(r_s);
    }

    ReadHists(region_cut, region_str);

    int n = 0;
    for (auto var : node["variables"]) {

        // plot variables and save to corresponding folder
        auto c1 = new TCanvas("c1", "c1", 2000, 2200);
        c1->SetMargin(0.13, 0.05, 0.13, 0.05);

        bool log_x = (var["log_x"].IsDefined()) && var["log_x"].as<bool>();
        if (log_x)
            c1->SetLogx();
        bool log_y = var["log_y"].IsDefined()?
                        var["log_y"].as<bool>():
                        global_log_y;
        if (log_y)
            c1->SetLogy();
        bool normalize = (global_normalize) ||
                         ((var["normalize"].IsDefined()) && var["normalize"].as<bool>());
        bool grid_y = var["grid_y"].IsDefined()?
                        var["grid_y"].as<bool>():
                        global_grid_y;
        if (grid_y)
            c1->SetGridy();

        bool ratio_plot = var["ratio_plot"].IsDefined()?
                                var["ratio_plot"].as<bool>():
                                global_ratio_plot;

        auto var_name = var["name"].as<std::string>();

        // Calculate the maximal and minimal y
        double y_max_factor = var["y_scale"].IsDefined()
                              ? var["y_scale"].as<double>()
                              : global_y_max_factor;
        double y_max = 0.;
        double y_min_tmp = DBL_MAX;
        double min_entries = DBL_MAX;

        if (normalize) {
            for (const auto &h_var : hists_list.at(var_name)) {
                double total = h_var->Integral();
                h_var->Scale(1. / total);
            }
        }

        for (const auto &h_var : hists_list.at(var_name)) {
            y_max = std::max(h_var->GetMaximum(), y_max);
            min_entries = std::min(h_var->GetEntries(), min_entries);
            y_min_tmp = std::min(h_var->GetMinimum(1e-9), y_min_tmp);
        }

        //    if (normalize) {
        //      double total = h->Integral();
        //      h->Scale(1. / total);
        //      if (i == 0) {
        //        y_max /= total;
        //        y_min /= total;
        //      }
        //      std::cout<<"Integral: "<<total<<", y_max: "<<y_max<<", h_max:
        //      "<<h->GetMaximum()<<std::endl;
        //    }

        double y_min = (log_y) ? 0.5 * y_min_tmp : 1e-6 / min_entries;
        // if (log_y)
        //     y_max_factor = pow(10, y_max_factor - 1);
        std::cout<<"y_max: "<<y_max<<"  y_max_factor:"<<y_max_factor<<std::endl;
        y_max *= y_max_factor;

        auto[x_title, y_title] = FormatAxis(var);
        double ratio_up = (var["ratio_range"].IsDefined())
                          ? var["ratio_range"][1].as<double>()
                          : 1.5;
        double ratio_lo = (var["ratio_range"].IsDefined())
                          ? var["ratio_range"][0].as<double>()
                          : 0.5;

        // Final plot
        auto x1 = 0.68, x2 = 0.87, y2 = 0.91,
                y1 = y2 - hists_list.at(var_name).size() * 0.04;
        auto leg = new TLegend(x1, y1, x2, y2);
        leg->SetTextAlign(12);
        leg->SetFillStyle(0);

        for (unsigned i = 0; i < hists_list.at(var_name).size(); ++i) {
            auto h = hists_list.at(var_name).at(i);

            if (i == 0) {
                if (!ratio_plot) {
                    gStyle->SetPadLeftMargin(0.10);
                    h->GetYaxis()->SetTitle(y_title);
                    h->GetYaxis()->CenterTitle();
                }
                h->GetXaxis()->SetTitle(x_title);
                h->GetYaxis()->SetRangeUser(y_min, y_max);

                if (!ratio_plot)
                    h->Draw("hist");
            } else {
                if (!ratio_plot)
                    h->Draw("hist same");
            }

            // Build Legend
            if (h->GetFillColor() != 0)
                leg->AddEntry(h, h->GetTitle(), "f");
            else
                leg->AddEntry(h, h->GetTitle(), "l");
        }

        TRatioPlot *rp = nullptr;
        if (ratio_plot) {
            rp = new TRatioPlot(hists_list.at(var_name).at(0),
                                hists_list.at(var_name).at(1));

            // rp->SetH1DrawOpt("HIST E1"); //no need for upper pad
            // rp->SetH2DrawOpt("HIST E1");
            // rp->SetGraphDrawOpt("P X E1"); //this opt is not correct!
            rp->Draw();
            auto ratio_samples=var["ratio_samples"].IsDefined()?
                                        var["ratio_samples"].as<std::array<std::string,2>>():
                                        global_ratio_samples;
            auto up_y = TString::Format(
                    "%s/%s", ratio_samples[0].data(),
                    ratio_samples[1].data());

            rp->GetLowerRefYaxis()->SetTitle(up_y);
            rp->GetLowerRefYaxis()->CenterTitle();
            // rp->GetUpperRefYaxis()->SetTitle(y_title);
            rp->SetSeparationMargin(0.0);
            rp->GetLowerRefGraph()->SetMinimum(ratio_lo);
            rp->GetLowerRefGraph()->SetMaximum(ratio_up);
            rp->GetLowerRefGraph()->SetMarkerSize(1);

            if (auto s = var["ratio_marker_attr"]; s.IsDefined()) {
                rp->GetLowerRefGraph()->SetMarkerColorAlpha(s[0].as<int>(),
                                                            s[1].as<double>());
                rp->GetLowerRefGraph()->SetMarkerSize(s[2].as<int>());
                rp->GetLowerRefGraph()->SetMarkerStyle(s[3].as<int>());
            }
            c1->Update();
        }

        leg->Draw("same");

        TString select = (var["selection_title"].IsDefined())
                         ? var["selection_title"].as<std::string>()
                         : global_selection_title;

        PlotLogo_DarkSHINE(c1, select);

        // Save
        std::string path = work_name + sub_path + "fig" + region_str + "_" +
                           std::to_string(n) + ".png";
        c1->SaveAs(path.c_str());
        delete c1;
        delete rp;

        n++;
    }

    // Clear all the memory allocation
    for (auto hist_map : hists_list) {
        for (auto h : hist_map.second) {
            delete h;
        }
        hist_map.second.clear();
        hist_map.second.shrink_to_fit();
    }
    hists_list.clear();
}

void PlotYAML::PlotLogo_DarkSHINE(TCanvas *c, const TString &cuts) {
    double x1, x2, y1, y2;
    GetX1Y1X2Y2(c, x1, y1, x2, y2);

    TString str = "";

    double x, y;

    x = x1 + 0.08;
    y = y1 + 0.70;

    str = "Dark SHINE";
    TPaveText *DStext = CreatePaveText(x, y, x, y, str, 0.06, 72, 1);
    DStext->Draw("same");

    y = y - 0.042;
    str = "Beam: 10^{14} EOT @ 8 GeV ";
    TPaveText *DSEtext = CreatePaveText(x, y, x, y, str, 0.032, 42, 1);
    DSEtext->Draw("same");

    y = y - 0.042;
    str = cuts;
    TPaveText *DS2text = CreatePaveText(x, y, x, y, str, 0.032, 42, 1);
    if (cuts != "")
        DS2text->Draw("same");
}

std::tuple<TString, TString>
PlotYAML::FormatAxis(const YAML::detail::iterator_value &var) {
    // Format Y Axis
    bool normalize =
            (var["normalize"].IsDefined()) && var["normalize"].as<bool>();
    auto y_label = (normalize) ? "A.U." : "Yields";
    TString y_title;
    if(! var["vbins"].IsDefined()){
        double bin_size = 
            (var["range"][2].as<double>() - var["range"][1].as<double>()) /
            var["range"][0].as<int>();
        if (bin_size - floor(bin_size) == 0.)
            y_title = TString::Format("%s / %.f %s", y_label, bin_size,
                                    var["label"][1].as<std::string>().data());
        else if (bin_size - floor(bin_size) > 0. &&
                log(bin_size - floor(bin_size)) / log(10) >= -2.)
            y_title = TString::Format("%s / %.2f %s", y_label, bin_size,
                                    var["label"][1].as<std::string>().data());
        else if (bin_size - floor(bin_size) > 0. &&
                log(bin_size - floor(bin_size)) / log(10) < -2.)
            y_title = TString::Format("%s / %.5f %s", y_label, bin_size,
                                    var["label"][1].as<std::string>().data());
    }
    
    // Format X Axis
    TString x_title;
    if (!var["label"][1].as<std::string>().empty())
        x_title =
                TString::Format("%s [%s]", var["label"][0].as<std::string>().data(),
                                var["label"][1].as<std::string>().data());
    else
        x_title = TString::Format("%s", var["label"][0].as<std::string>().data());

    return std::make_tuple(x_title, y_title);
}

void PlotYAML::ReadRegions() {
    auto re_node = node["regions"];
    if (!re_node.IsDefined())
        return;

    std::vector<region_v> tmp_data;
    std::vector<size_t> tmp_size;
    for (auto r_n : re_node) {
        region_v sub_vec;

        // sanity check
        assert(r_n["name"].size() == r_n["title"].size());
        assert(r_n["title"].size() == r_n["selection"].size());
        for (size_t i = 0; i < r_n["name"].size(); ++i) {
            sub_vec.push_back(std::make_tuple(
                    r_n["name"][i].as<std::string>(), r_n["title"][i].as<std::string>(),
                    TCut(TString(r_n["selection"][i].as<std::string>()))));
        }
        tmp_size.push_back(r_n["name"].size());
        tmp_data.push_back(sub_vec);
    }

    size_t total_region_num = (std::accumulate(tmp_size.begin(), tmp_size.end(),
                                               1, std::multiplies<>()));
    // Make Cartesian Product
    std::vector<size_t> tmp_n(re_node.size(), 0);
    for (size_t i = 0; i < total_region_num; ++i) {
        region_v tmp_re;
        for (size_t j = 0; j < tmp_n.size(); ++j) {
            tmp_re.push_back(tmp_data.at(j).at(tmp_n.at(j)));
        }

        for (int j = tmp_n.size() - 1; j >= 0; --j) {
            if (tmp_n.at(j) + 1 >= tmp_size.at(j)) {
                tmp_n.at(j) = 0;
            } else {
                tmp_n.at(j)++;
                break;
            }
        }
        region_list.push_back(tmp_re);
    }
}
