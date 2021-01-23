//
// ATLAS Style, based on a style file from BaBar
//

#include <iostream>

#include "DarkStyle.h"

#include "TROOT.h"

void SetDarkSHINEStyle() {
    static TStyle *DarkSHINEStyle = nullptr;
    std::cout << "\nApplying Dark SHINE style settings...\n" << std::endl;
    if (DarkSHINEStyle == nullptr) DarkSHINEStyle = DarkStyle();
    gROOT->SetStyle("DarkSHINE");
    gROOT->ForceStyle();
}

TStyle *DarkStyle() {
    auto *darkStyle = new TStyle("DarkSHINE", "Dark SHINE style");

    // use plain black on white colors
    Int_t icol = 0; // WHITE
    darkStyle->SetFrameBorderMode(icol);
    darkStyle->SetFrameFillColor(icol);
    darkStyle->SetCanvasBorderMode(icol);
    darkStyle->SetCanvasColor(icol);
    darkStyle->SetPadBorderMode(icol);
    darkStyle->SetPadColor(icol);
    darkStyle->SetStatColor(icol);
    //darkStyle->SetFillColor(icol); // don't use: white fill color for *all* objects

    // set the paper & margin sizes
    darkStyle->SetPaperSize(20, 26);

    // set margin sizes
    darkStyle->SetPadTopMargin(0.05);
    darkStyle->SetPadRightMargin(0.05);
    darkStyle->SetPadBottomMargin(0.16);
    darkStyle->SetPadLeftMargin(0.16);

    // set title offsets (for axis label)
    darkStyle->SetTitleXOffset(1.2);
    darkStyle->SetTitleYOffset(1.4);

    // use large fonts
    //Int_t font=72; // Helvetica italics
    Int_t font = 42; // Helvetica italics
    Int_t font_i = 52; // Helvetica italics
    Double_t tsize = 0.035;
    darkStyle->SetTextFont(font);

    darkStyle->SetTextSize(tsize);
    darkStyle->SetLabelFont(font, "x");
    darkStyle->SetTitleFont(font_i, "x");
    darkStyle->SetLabelFont(font, "y");
    darkStyle->SetTitleFont(font, "y");
    darkStyle->SetLabelFont(font, "z");
    darkStyle->SetTitleFont(font, "z");

    darkStyle->SetLabelSize(tsize, "x");
    darkStyle->SetTitleSize(tsize, "x");
    darkStyle->SetLabelSize(tsize, "y");
    darkStyle->SetTitleSize(tsize, "y");
    darkStyle->SetLabelSize(tsize, "z");
    darkStyle->SetTitleSize(tsize, "z");

    darkStyle->SetNdivisions(505,"x");

    // use bold lines and markers
    darkStyle->SetMarkerStyle(20);
    darkStyle->SetMarkerSize(1.2);
    darkStyle->SetHistLineWidth(2.);
    darkStyle->SetLineStyleString(2, "[12 12]"); // postscript dashes

    // get rid of X error bars
    //darkStyle->SetErrorX(0.001);
    // get rid of error bar caps
    darkStyle->SetEndErrorSize(0.);

    // do not display any of the standard histogram decorations
    darkStyle->SetOptTitle(0);
    //darkStyle->SetOptStat(1111);
    darkStyle->SetOptStat(0);
    //darkStyle->SetOptFit(1111);
    darkStyle->SetOptFit(0);

    // put tick marks on top and RHS of plots
    darkStyle->SetPadTickX(1);
    darkStyle->SetPadTickY(1);

    // Set Legend Attributes
    darkStyle->SetLegendBorderSize(0);
    darkStyle->SetLegendFont(font);
    darkStyle->SetLegendTextSize(0.03);


    return darkStyle;
}

