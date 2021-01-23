//
// Created by Zhang Yulei on 1/2/21.
//

#ifndef DSIMU_PLOTYAML_H
#define DSIMU_PLOTYAML_H

// From ROOT
#include "TTreeReader.h"
#include "TFile.h"
#include "TH1D.h"
#include "TPaveText.h"
#include "TFrame.h"
#include "TCanvas.h"

#include "yaml-cpp/yaml.h"

#include <string>
#include <memory>
#include <map>

/// \brief Plot graph from yaml config file
class PlotYAML {
public:
    explicit PlotYAML(const std::string &file_in);

    void ReadHists();

    void Plot();

    void PlotLogo(TCanvas* c, const TString &cuts="");

    static TPaveText *
    CreatePaveText(double x1, double y1, double x2, double y2, std::vector<TString> text, double textsize,
                   double textfont, int color = 1) {

        auto *tex = new TPaveText();
        tex->SetFillColor(0);
        tex->SetTextSize(0.05);
        tex->SetFillStyle(0);
        tex->SetBorderSize(0);
        int n = text.size();
        for (int i = 0; i < n; i++) tex->AddText(text[i].Data());
        tex->SetX1NDC(x1);
        tex->SetY1NDC(y1);
        tex->SetX2NDC(x2);
        tex->SetY2NDC(y2);
        tex->SetTextSize(textsize);
        tex->SetTextColor(color);
        tex->SetTextFont(textfont);
        return tex;
    }

    void GetX1Y1X2Y2(TVirtualPad *c, double &x1, double &y1, double &x2, double &y2) {
        x1 = c->GetFrame()->GetX1() + c->GetLeftMargin();
        y1 = c->GetFrame()->GetY1() + c->GetBottomMargin();
        x2 = c->GetFrame()->GetX2() - c->GetRightMargin();
        y2 = c->GetFrame()->GetY2() - c->GetTopMargin();
    }

private:
    YAML::Node node;

    std::string work_name;

    // list of hists [variables][samples]
    std::map<std::string, std::vector<std::shared_ptr<TH1D> > > hists_list;

};


#endif //DSIMU_PLOTYAML_H
