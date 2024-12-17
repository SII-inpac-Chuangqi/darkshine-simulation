//
// Created by Zhang Yulei on 11/30/20.
//

#include "Algo/ECAL_Writer.h"

void ECAL_Writer::BookTree(const TString &filename, const TString &treename, int nb_channel) {
    outFile = shared_ptr<TFile>(new TFile(filename, "RECREATE"));
    outTree = shared_ptr<TTree>(new TTree(treename, treename));

    for (int i = 0; i < nb_channel; ++i) ECAL_Hits.push_back(0.);

    for (unsigned long i = 0; i < ECAL_Hits.size(); ++i) {
        TString varname1 = TString::Format("ECAL_%lu", i);
        TString varname2 = TString::Format("ECAL_%lu/F", i);
        outTree->Branch(varname1,&ECAL_Hits.at(i),varname2);
    }
}

void ECAL_Writer::FillHits(const shared_ptr<vector<SimulatedHit *>> &hits) {
    for (auto hit: *hits){
        int cell_id = hit->getCellId();

        ECAL_Hits.at(cell_id-1) = ( hit->getE() > 1e-6 && !isnan(hit->getE()) ) ? hit->getE() : 0. ;
    }
    outTree->Fill();
}

void ECAL_Writer::SaveTree() {
    outFile->cd();
    outTree->Write("",TObject::kOverwrite);
    //outFile->Close();
}
