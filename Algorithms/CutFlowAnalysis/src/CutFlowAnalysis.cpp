//
// Created by Zhang Yulei on 9/19/20.
//

#include "Algo/CutFlowAnalysis.h"

#include <utility>
#include <algorithm>

#include <string>
#include <sstream>

CutFlowAnalysis::CutFlowAnalysis(string name, shared_ptr<EventStoreAndWriter> evtwrt)
        : AnaProcessor(std::move(name), std::move(evtwrt)) {

    // Add description for this AnaProcessor
    Description = "Automatically Generate Cut Flow.";

    cut_chain.emplace_back("Raw Events");
    cut_chain.emplace_back("Single Track");
    cut_chain.emplace_back("Momentum Loss");
    cut_chain.emplace_back("Missing ECAL Energy");
    cut_chain.emplace_back("Min HCAL Energy");
    cut_chain.emplace_back("Max HCAL Cell Energy");

    RegisterDoubleParameter("momentum_diff", "Pi - Pf", &momentum_diff, 4000.);
    RegisterDoubleParameter("ECAL_E_Min", "Min. ECAL Energy", &ECAL_E_Min, 4000.);
    RegisterDoubleParameter("HCAL_E_Max", "Max. HCAL Energy", &HCAL_E_Max, 10.);
    RegisterDoubleParameter("HCAL_E_Cell_Max", "Max. HCAL Energy per Cell", &HCAL_E_Cell_Max, 1.0);

    RegisterStringParameter("weight", "event weight", &weight, "1");
    RegisterIntParameter("SaveToNewRoot", "Save Result to a new File", &save_new, 0);

}


void CutFlowAnalysis::Begin() {
    h_cut = new TH1F("cutflow_total", "cutflow_total", 7, 0, 7);

    h_tmp = new TH1F("cutflow_ex", "cutflow_tx", 400, 0, 8000.);

    for (int i = 0; i < 6; ++i) {
        h_cut->Fill(cut_chain.at(i).data(), 0.);
    }

//    TagTrk2_track_No = EvtWrt->FindIntVar("TagTrk2_track_No");
//    RecTrk2_track_No = EvtWrt->FindIntVar("RecTrk2_track_No");
//    TagTrk2_pp = EvtWrt->FindDoubleVar("TagTrk2_pp");
//    RecTrk2_pp = EvtWrt->FindDoubleVar("RecTrk2_pp");

    TagTrk2_pp = EvtWrt->FindDoubleVar("Truth_Pi");
    RecTrk2_pp = EvtWrt->FindDoubleVar("Truth_Pf");
    ECAL_E_total = EvtWrt->FindDoubleVar("ECAL_E_total");
    HCAL_E_total = EvtWrt->FindDoubleVar("HCAL_E_total");
    HCAL_E_Max_Cell = EvtWrt->FindDoubleVar("HCAL_E_Max_Cell");
}

void CutFlowAnalysis::ProcessEvt(AnaEvent *evt) {
    // Raw Events
    h_cut->Fill(cut_chain.at(0).data(), 1.);

    //auto c1 = (*TagTrk2_track_No == 1 && *RecTrk2_track_No == 1);
    auto c1 = (1);
    auto c2 = ((*TagTrk2_pp - *RecTrk2_pp) >= momentum_diff);
    auto c3 = (*ECAL_E_total <= ECAL_E_Min);
    auto c4 = (*HCAL_E_total <= HCAL_E_Max);
    auto c5 = (*HCAL_E_Max_Cell <= HCAL_E_Cell_Max);

    if (c1)
        h_cut->Fill(cut_chain.at(1).data(), 1.);
    if (c1 && c2)
        h_cut->Fill(cut_chain.at(2).data(), 1.);
    if (c1 && c2 && c3)
        h_cut->Fill(cut_chain.at(3).data(), 1.);
    if (c1 && c2 && c3 && c4)
        h_cut->Fill(cut_chain.at(4).data(), 1.);
    if (c1 && c2 && c3 && c4 && c5)
        h_cut->Fill(cut_chain.at(5).data(), 1.);

    // For extrapolation
    if (c4 && c5)
        h_tmp->Fill(*ECAL_E_total);
}

void CutFlowAnalysis::CheckEvt(AnaEvent *evt) {
    //cout<<"Check!"<<endl;

}

void CutFlowAnalysis::End() {
    auto h_cumulative = h_tmp->GetCumulative();

    if (save_new) {
        TFile fout("dp_cutflow.root","RECREATE");
        fout.cd();
        h_cut->Write("",TObject::kOverwrite);
        h_cumulative->Write("",TObject::kOverwrite);
        fout.Close();
    } else {
        EvtWrt->SaveObjectToFile(h_cut, "");
        EvtWrt->SaveObjectToFile(h_cumulative, "");
    }
}
