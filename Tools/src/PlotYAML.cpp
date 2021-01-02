//
// Created by Zhang Yulei on 1/2/21.
//

// From ROOT
#include "TCanvas.h"
#include "TTree.h"
#include "TLegend.h"

#include "PlotYAML.h"

#include <iostream>
#include <filesystem>
#include <memory>

namespace fs = std::filesystem;

PlotYAML::PlotYAML(const std::string &file_in) {
    try {
        node = YAML::LoadFile(file_in);
    }
    catch (YAML::ParserException &e) {
        std::cerr << "[Reading YAML] ==> " << e.msg << std::endl;
    }

    // Prepare work space
    work_name = node["name"].as<std::string>();
    // create a directory if not exists
    fs::create_directories(work_name);
}

/// \brief read histograms from root file
void PlotYAML::ReadHists() {

    auto c1 = new TCanvas("c1", "c1", 1200, 1000);

    // Initialize a map storing all histograms [var_name]:[hists of all samples]
    for (auto var : node["variables"])
        hists_list.try_emplace(var["name"].as<std::string>(), std::vector<std::shared_ptr<TH1D>>());

    int n = 0;
    // For each file
    for (auto sample : node["samples"]) {
        auto name = sample["name"].as<std::string>();
        // Read in tree
        auto file = std::make_unique<TFile>(sample["file_name"].as<std::string>().data());
        auto tree = std::unique_ptr<TTree>(
                dynamic_cast<TTree *>(file->Get(sample["tree_name"].as<std::string>().data())));

        // For each tree, draw the variables
        // Loop variables
        for (auto var : node["variables"]) {
            auto var_name = var["name"].as<std::string>();
            auto xbin = var["range"][0].as<int>();
            auto xmin = var["range"][1].as<double>();
            auto xmax = var["range"][2].as<double>();

            std::string selection = sample["selection"].IsDefined() ? sample["selection"].as<std::string>() : "";

            auto h = std::make_shared<TH1D>(("h_" + std::to_string(n)).data(), name.data(), xbin, xmin, xmax);
            std::string draw_text = var_name + ">>" + std::string(h->GetName());
            tree->Draw(draw_text.data(), selection.data(), "");
            if (auto s = sample["line_attr"]; s.IsDefined()) {
                h->SetLineColor(s[0].as<int>());
                h->SetLineWidth(s[1].as<int>());
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
            hists_list.at(var_name).push_back(h);
            n++;
        }
    }
    delete c1;
}


void PlotYAML::Plot() {
    ReadHists();

    int n = 0;
    for (auto var : node["variables"]) {
        // plot variables and save to corresponding folder
        auto c1 = new TCanvas("c1", "c1", 2500, 2000);

        bool log_x = (var["log_x"].IsDefined()) ? var["log_x"].as<bool>() : false;
        if (log_x) c1->SetLogx();
        bool log_y = (var["log_y"].IsDefined()) ? var["log_y"].as<bool>() : false;
        if (log_y) c1->SetLogy();
        bool normalize = (var["normalize"].IsDefined()) ? var["normalize"].as<bool>() : false;
        bool grid_y = (var["grid_y"].IsDefined()) ? var["grid_y"].as<bool>() : false;
        if (grid_y) c1->SetGridy();

        auto var_name = var["name"].as<std::string>();

        // Calculate the maximal and minimal y
        double y_max_factor = var["y_scale"].IsDefined() ? var["y_scale"].as<double>() : 1.5;
        double y_max = 1.;
        double min_entries = 1;
        for (const auto &h_var : hists_list.at(var_name)) {
            y_max = std::max(h_var->GetMaximum(), y_max);
            min_entries = std::min(h_var->GetEntries(), min_entries);
        }
        double y_min = (log_y) ? 0.5 / min_entries : 0.;
        if (log_y) y_max_factor = pow(10, y_max_factor);
        y_max *= y_max_factor;

        // Final plot
        auto x1 = 0.7, x2 = 0.9, y2 = 0.91, y1 = y2 - hists_list.at(var_name).size() * 0.06;
        auto leg = new TLegend(x1, y1, x2, y2);
        for (unsigned i = 0; i < hists_list.at(var_name).size(); ++i) {
            auto h = hists_list.at(var_name).at(i);
            if (normalize) {
                double total = h->Integral();
                h->Scale(1. / total);
                y_max /= total;
                y_min /= total;
            }
            if (i == 0) {
                // Format Y Axis
                auto y_label = (normalize) ? "A.U." : "Yields";
                double bin_size =
                        (var["range"][2].as<double>() - var["range"][1].as<double>()) / var["range"][0].as<int>();
                TString y_title;
                if (bin_size - floor(bin_size) == 0.)
                    y_title = TString::Format("%s / %.f%s", y_label, bin_size,
                                              var["label"][1].as<std::string>().data());
                else if (bin_size - floor(bin_size) > 0. && log(bin_size - floor(bin_size)) / log(10) >= -2.)
                    y_title = TString::Format("%s / %.2f%s", y_label, bin_size,
                                              var["label"][1].as<std::string>().data());
                else if (bin_size - floor(bin_size) > 0. && log(bin_size - floor(bin_size)) / log(10) < -2.)
                    y_title = TString::Format("%s / %.5f%s", y_label, bin_size,
                                              var["label"][1].as<std::string>().data());

                // Format X Axis
                TString x_title;
                if (!var["label"][1].as<std::string>().empty())
                    x_title = TString::Format("%s [%s]", var["label"][0].as<std::string>().data(),
                                              var["label"][1].as<std::string>().data());
                else
                    x_title = TString::Format("%s", var["label"][0].as<std::string>().data());

                h->GetXaxis()->SetTitle((x_title));
                h->GetYaxis()->SetTitle((y_title));
                h->GetYaxis()->CenterTitle();
                h->GetYaxis()->SetRangeUser(y_min, y_max);

                h->Draw("hist");
            } else h->Draw("hist same");

            // Build Legend
            if (h->GetFillColor() != 0) leg->AddEntry(h.get(), h->GetTitle(), "f");
            else leg->AddEntry(h.get(), h->GetTitle(), "l");
        }
        leg->Draw("same");

        TString select = (var["selection_title"].IsDefined()) ? var["selection_title"].as<std::string>() : "";

        PlotLogo(c1, select);

        // Save
        std::string path = work_name + "/fig_" + std::to_string(n) + ".png";
        c1->SaveAs(path.c_str());
        delete c1;

        n++;
    }
}

void PlotYAML::PlotLogo(TCanvas *c, const TString &cuts) {
    double x1, x2, y1, y2;
    GetX1Y1X2Y2(c, x1, y1, x2, y2);

    double x, y;

    x = x1 + 0.08;
    y = y1 + 0.70;
    std::vector<TString> DS;
    DS.emplace_back("Dark SHINE");
    TPaveText *DStext = CreatePaveText(x, y, x, y, DS, 0.06, 72, 1);
    DStext->Draw("same");

    y = y - 0.042;
    std::vector<TString> DSE;
    DSE.emplace_back("Beam: 10^{14} EOT @ 8 GeV ");
    TPaveText *DSEtext = CreatePaveText(x, y, x, y, DSE, 0.032, 42, 1);
    DSEtext->Draw("same");

    y = y - 0.042;
    std::vector<TString> DS2;
    DS2.emplace_back(cuts);
    TPaveText *DS2text = CreatePaveText(x, y, x, y, DS2, 0.032, 42, 1);
    if (cuts != "") DS2text->Draw("same");

}
