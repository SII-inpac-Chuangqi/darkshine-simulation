// Cook some ingredients for perspective study. -- Yufeng
// -- branching ratio from main process name
// -- customized cutflow
// -- fast print signal efficiency

#include <vector>
#include <string>
#include <iostream>
#include <filesystem>
#include "yaml-cpp/yaml.h"
#include "DarkStyle.h"
#include "PlotYAML.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TH1D.h"
#include "TCut.h"

using namespace std;
namespace fs = std::filesystem;

namespace {
    void PrintUsage() {
        cerr << " Usage: " << endl;
        cerr << " DCutflow [ -c config ]" << endl;
        cerr << endl;
    }
} // namespace

void makeBranchingRatio(const char* treename_in, TFile* afile_in, TFile* afile_out){
    
    TTreeReader dsReader(treename_in, afile_in);
    // variables for branching ratio
    TTreeReaderValue<TString> MainProcessName   (dsReader, "MainProcessName");
    TTreeReaderValue<TString> MainProcessPVName (dsReader, "MainProcessPVName");
    TTreeReaderValue<float>   MainProcessEnergy (dsReader, "MainProcessEnergy");
    
    cout<<"-----> making branching ratio now!"<<endl;
    // note that this inclusive is not real "inclusive"
    string binLabels_type[8] = {"inclusive", "hardbrem", "en_ecal", "en_target", "pn_ecal", "pn_target", "gmm_ecal","gmm_target"};
    string binLabels_HCAL[5] = {"inclusive","hardbrem","EN","PN","GMM"};

    TH1D *Type_Count_raw = new TH1D("Type_Count_raw","Type_Count_raw",8,0,8);
    for(int l=1;l<9;l++)Type_Count_raw->GetXaxis()->SetBinLabel(l,binLabels_type[l-1].c_str());
    TH1D *Type_Count_4GeV = new TH1D("Type_Count_4GeV","Type_Count_4GeV",8,0,8);
    for(int l=1;l<9;l++)Type_Count_4GeV->GetXaxis()->SetBinLabel(l,binLabels_type[l-1].c_str());
    TH1D *Type_Count_HCAL = new TH1D("Type_Count_HCAL","Type_Count_HCAL",5,0,5);
    for(int l=1;l<6;l++)Type_Count_HCAL->GetXaxis()->SetBinLabel(l,binLabels_HCAL[l-1].c_str());
    TH1D *Type_Count_SideHCAL = new TH1D("Type_Count_SideHCAL","Type_Count_SideHCAL",5,0,5);
    for(int l=1;l<6;l++)Type_Count_SideHCAL->GetXaxis()->SetBinLabel(l,binLabels_HCAL[l-1].c_str());
 
    int Ntot = 0;

    while (dsReader.Next()){
    Ntot++;
    // filling
    if(*MainProcessName == "inclusive")                                         Type_Count_raw->Fill(0.,1.);
    if(*MainProcessName == "hardbrem")                                          Type_Count_raw->Fill(1.,1.);
    if(*MainProcessName == "electronNuclear" && *MainProcessPVName == "ECAL")   Type_Count_raw->Fill(2.,1.);
    if(*MainProcessName == "electronNuclear" && *MainProcessPVName == "Target") Type_Count_raw->Fill(3.,1.);
    if(*MainProcessName == "photonNuclear" && *MainProcessPVName == "ECAL")     Type_Count_raw->Fill(4.,1.);
    if(*MainProcessName == "photonNuclear" && *MainProcessPVName == "Target")   Type_Count_raw->Fill(5.,1.);
    if(*MainProcessName == "GammaToMuPair" && *MainProcessPVName == "ECAL")     Type_Count_raw->Fill(6.,1.);
    if(*MainProcessName == "GammaToMuPair" && *MainProcessPVName == "Target")   Type_Count_raw->Fill(7.,1.);

    if(*MainProcessEnergy>4000.){
    if(*MainProcessName == "inclusive")                                         Type_Count_4GeV->Fill(0.,1.);
    if(*MainProcessName == "hardbrem")                                          Type_Count_4GeV->Fill(1.,1.);
    if(*MainProcessName == "electronNuclear" && *MainProcessPVName == "ECAL")   Type_Count_4GeV->Fill(2.,1.);
    if(*MainProcessName == "electronNuclear" && *MainProcessPVName == "Target") Type_Count_4GeV->Fill(3.,1.);
    if(*MainProcessName == "photonNuclear" && *MainProcessPVName == "ECAL")     Type_Count_4GeV->Fill(4.,1.);
    if(*MainProcessName == "photonNuclear" && *MainProcessPVName == "Target")   Type_Count_4GeV->Fill(5.,1.);
    if(*MainProcessName == "GammaToMuPair" && *MainProcessPVName == "ECAL")     Type_Count_4GeV->Fill(6.,1.);
    if(*MainProcessName == "GammaToMuPair" && *MainProcessPVName == "Target")   Type_Count_4GeV->Fill(7.,1.);
    }

    if(*MainProcessPVName == "HCAL"){
      if(*MainProcessName == "inclusive")       Type_Count_HCAL->Fill(0.,1.);
      if(*MainProcessName == "hardbrem")        Type_Count_HCAL->Fill(1.,1.);
      if(*MainProcessName == "electronNuclear") Type_Count_HCAL->Fill(2.,1.);
      if(*MainProcessName == "photonNuclear")   Type_Count_HCAL->Fill(3.,1.);
      if(*MainProcessName == "GammaToMuPair")   Type_Count_HCAL->Fill(4.,1.);
    }

    if(*MainProcessPVName == "SideHCAL"){
      if(*MainProcessName == "inclusive")       Type_Count_SideHCAL->Fill(0.,1.);
      if(*MainProcessName == "hardbrem")        Type_Count_SideHCAL->Fill(1.,1.);
      if(*MainProcessName == "electronNuclear") Type_Count_SideHCAL->Fill(2.,1.);
      if(*MainProcessName == "photonNuclear")   Type_Count_SideHCAL->Fill(3.,1.);
      if(*MainProcessName == "GammaToMuPair")   Type_Count_SideHCAL->Fill(4.,1.);
    }
  }// while dsReader
  cout<<"------------------------------------------------"<<endl;
  cout<<"Branching ratio has been filled."<<endl;
  cout<<"* HCAL processes are not given here."<<endl;
  cout<<"Total events: "<<Ntot<<endl;
  cout<<"-------- raw branching ratio --------"<<endl;
  cout<<"- inclusive: "  <<(double)Type_Count_raw->GetBinContent(1)/Ntot<<endl;
  cout<<"- hardbrem: "   <<(double)Type_Count_raw->GetBinContent(2)/Ntot<<endl;
  cout<<"- en_ecal: "    <<(double)Type_Count_raw->GetBinContent(3)/Ntot<<endl;
  cout<<"- en_target: "  <<(double)Type_Count_raw->GetBinContent(4)/Ntot<<endl;
  cout<<"- pn_ecal: "    <<(double)Type_Count_raw->GetBinContent(5)/Ntot<<endl;
  cout<<"- pn_target: "  <<(double)Type_Count_raw->GetBinContent(6)/Ntot<<endl;
  cout<<"- gmm_ecal: "   <<(double)Type_Count_raw->GetBinContent(7)/Ntot<<endl;
  cout<<"- gmm_target: " <<(double)Type_Count_raw->GetBinContent(8)/Ntot<<endl;
  cout<<"---- (process energy > 4GeV) branching ratio ----"<<endl;
  cout<<"- inclusive: "  <<(double)Type_Count_4GeV->GetBinContent(1)/Ntot<<endl;
  cout<<"- hardbrem: "   <<(double)Type_Count_4GeV->GetBinContent(2)/Ntot<<endl;
  cout<<"- en_ecal: "    <<(double)Type_Count_4GeV->GetBinContent(3)/Ntot<<endl;
  cout<<"- en_target: "  <<(double)Type_Count_4GeV->GetBinContent(4)/Ntot<<endl;
  cout<<"- pn_ecal: "    <<(double)Type_Count_4GeV->GetBinContent(5)/Ntot<<endl;
  cout<<"- pn_target: "  <<(double)Type_Count_4GeV->GetBinContent(6)/Ntot<<endl;
  cout<<"- gmm_ecal: "   <<(double)Type_Count_4GeV->GetBinContent(7)/Ntot<<endl;
  cout<<"- gmm_target: " <<(double)Type_Count_4GeV->GetBinContent(8)/Ntot<<endl;
  cout<<"------------------------------------------------"<<endl;

afile_out->cd();
Type_Count_raw->Write();
Type_Count_4GeV->Write();
Type_Count_HCAL->Write();
Type_Count_SideHCAL->Write();
//afile_out->Write();
Ntot = 0;

}// make branching ratio

//------------------------------------------------------------------------------------------
int main(int argc, char **argv) {
cout<<"Cutflow D.I.Y starts!"<<endl;
cout<<"------------------------------------------------"<<endl;
cout<<"------------------------------------------------"<<endl;
    if (argc > 3 || argc < 2) {
        PrintUsage();
        return 1;
    }
    string cutflow_cfg;
    if (std::string(argv[1]) == "-c")
        cutflow_cfg = argv[2];
    else {
        PrintUsage();
        return -1;
    }
    
    YAML::Node node;
    try{
        node = YAML::LoadFile(cutflow_cfg);
        cout<<"config file: "<<cutflow_cfg<<endl;
    }
    catch(YAML::ParserException &e){
        cerr<<"[Reading YAML] ==> " <<e.msg<<endl;
    }

    string path_name;
    string output_file_name;
    path_name = node["path_name"].as<std::string>();
    output_file_name = node["output_file_name"].as<std::string>();

    string outputfile = path_name + "/" + output_file_name;

    fs::create_directories(path_name);
    cout<<"output path: "<<path_name<<endl;
    auto file_out = new TFile(outputfile.data(),"RECREATE");
    cout<<"output file: "<<output_file_name<<endl;
cout<<"------------------------------------------------"<<endl;
cout<<"global options:"<<endl;
    bool doBranchingRatio = node["make_branching_ratio"].as<bool>();
    cout<<"make branching ratio: "<< doBranchingRatio <<endl;
    
    bool makeSigEff = node["print_sig_efficiency"].as<bool>();
    bool printCuts = node["print_cuts"].as<bool>();
    bool printEfficiency = node["print_efficiency"].as<bool>();
    bool doGapVeto = node["require_gap_veto"].as<bool>();
    bool dropLargeTrkP = node["drop_large_track_momentum"].as<bool>();

    cout<<"print efficiencies for all input signal sample: "<< makeSigEff <<endl;
    cout<<"require gap veto: "<< doGapVeto <<endl;
    cout<<"drop event with large track momentum: "<< dropLargeTrkP <<endl;

    // for signal efficiency
    map<string, double>map_sigeff;
    int sample_size = node["samples"].size();
//------------------------------------------------------------------------------------------
cout<<">> now start loop over input samples."<<endl;
cout<<"------------------------------------------------"<<endl;
    for (auto sample : node["samples"]) { // loop over input samples
        auto name = sample["name"].as<std::string>();
        auto filename =sample["file_name"].as<std::string>();
        auto treename = sample["tree_name"].as<std::string>();
        
        auto file_in  = new TFile(filename.data());
        file_in->cd();
        cout<<"sample: "<<filename<<endl;
        cout<<"tree: "<<treename<<endl;
        auto tree_in = (TTree*)file_in->Get(treename.data());

        if(doBranchingRatio)makeBranchingRatio(treename.c_str(), file_in, file_out);       
    
        int Nbins = 1;
        int ibin = 0;
        map<int, TCut>map_cut;
        // pre-cuts considered as 1 cut
        if(doGapVeto || dropLargeTrkP)Nbins+=1;

        // prepare hist and cuts
        Nbins+=node["layer_of_cuts"].size();
        //cout<<"number of cutflow bins: "<<Nbins<<endl;
        map<string, TH1D*>map_cutflow;
        auto cutflow_name = "cutflow_" + name;
        map_cutflow[cutflow_name] = new TH1D(cutflow_name.c_str(),cutflow_name.c_str(),Nbins,0,Nbins);
        map<int, string>binLabels;
        // 0: total
        map_cut[ibin] = "1";
        binLabels[ibin] = "All";
        ibin++;
        // 1: pre-selection
        if(doGapVeto || dropLargeTrkP) binLabels[ibin] = "Pre.";
        TCut GapVeto, LargeTrkP;
        if(doGapVeto){
           GapVeto = node["cut_gap_veto"].IsDefined()
            ? TCut(node["cut_gap_veto"].as<std::string>().c_str())
            : "1";
        }
        if(dropLargeTrkP){
           LargeTrkP = node["cut_large_trkP"].IsDefined()
            ? TCut(node["cut_large_trkP"].as<std::string>().c_str())
            : "1";
        }
        map_cut[ibin] = GapVeto && LargeTrkP;
        ibin++;
       
        // skip a layer if the variable doesn't exist
        bool skip;
        // -------------------------------------------------------------------
        // loop over the rest of cut layers
        cout<<"-----> making cutflow now!"<<endl;
        for(auto layer : node["layer_of_cuts"]){
          skip = false;
          auto var_name = layer["name"].as<std::vector<string>>();
          // check if all variables exist
          for(size_t v=0; v<var_name.size(); v++){
            if(!tree_in->GetBranch(var_name[v].c_str())){
              cout<<"!! variable "<<var_name[v]<<" does not exist!"<<endl;
              skip = true;
              }
          }
          if(skip)map_cut[ibin] = "1";
          else map_cut[ibin] = layer["tcut"].IsDefined()
            ? TCut(layer["tcut"].as<std::string>().c_str())
            : "1";

          binLabels[ibin] = layer["label"].IsDefined()
            ? layer["label"].as<std::string>()
            : "null";

          ibin++;
          var_name.clear();
        }// cut layers
        //-------------------------------------------------------------------
        // fill cutflow
        TCut thiscut = "1";
        for(int l=1;l<Nbins+1;l++){
          map_cutflow[cutflow_name]->GetXaxis()->SetBinLabel(l,binLabels[l-1].c_str());
          thiscut = thiscut && map_cut[l-1];
          if(printCuts)cout<<"layer "<<l<<" | "<<binLabels[l-1]<<" : "<<thiscut<<endl;
          map_cutflow[cutflow_name]->Fill(l-1, tree_in->GetEntries(thiscut)); 
        }

        if(printEfficiency){
          cout<<"-----> cutflow efficiency:"<<endl;
          for(int e=1;e<Nbins+1;e++){
            cout<<binLabels[e-1]<<" : "<<(double)(map_cutflow[cutflow_name]->GetBinContent(e)/map_cutflow[cutflow_name]->GetBinContent(1))*100<<"%"<<endl;
            if(makeSigEff) map_sigeff[name] = map_cutflow[cutflow_name]->GetBinContent(e)/map_cutflow[cutflow_name]->GetBinContent(1);
          }
        }

        file_out->cd();
        map_cutflow[cutflow_name]->Write();
        file_out->Write();
        cout<<"this sample done."<<endl;
cout<<"------------------------------------------------"<<endl;
    } // loop over input samples

    if(makeSigEff){
      cout<<"-----> signal efficiency:"<<endl;
      for(auto isample: node["samples"]){
        auto iname = isample["name"].as<std::string>();
        auto imass = iname.substr(0, iname.length()-3);
        cout<<imass<<" MeV: "<<map_sigeff[iname]<<endl;
      }
      cout<<"-----> for limit computation:"<<endl;
      int ii=0;
      cout<<"double mass["<< sample_size <<"] = {";
      for(auto isample: node["samples"]){
        auto iname = isample["name"].as<std::string>();
        auto imass = iname.substr(0, iname.length()-3);
        cout<<imass;
        if( ii < sample_size -1 )cout<<",";
        ii++;
      }
cout<<"};"<<endl;

      ii=0;
      cout<<"double signal_eff["<< sample_size <<"] = {";
      for(auto isample: node["samples"]){
        auto iname = isample["name"].as<std::string>();
        auto imass = iname.substr(0, iname.length()-3);
        cout<<map_sigeff[iname];
        if( ii < sample_size -1 )cout<<",";
        ii++;
      }
cout<<"};"<<endl;


    } // print signal efficiency
cout<<"------------------------------------------------"<<endl;
cout<<"Cutflow finished, enjoy."<<endl;


   return 0;
}
