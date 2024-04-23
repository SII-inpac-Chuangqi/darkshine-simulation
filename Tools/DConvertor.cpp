// This is a dummy modified version, adapting for various signal process production.
// Should be able to merge filling part @ end of event according to particle type.
// electron: 11 | photon: 22 | dark photon: 2200 | target: 2201 | axion: 2202

#include<string>
#include<sstream>
#include<fstream>
#include<iostream>
#include<vector>
#include "TFile.h"
#include "TObject.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include<algorithm>
#include<math.h>
#include<TVector3.h>

using namespace std;
namespace {
    void PrintUsage() {
        cout<< "Missed anything?"<<endl;        
        cout<< "Useage: "<<endl;
        cout<< "  DConvertor [lhe] [root] [signal process] [mass point]"<< endl;
        cout<< "  -----------------------------------------"<< endl;
        cout<< "  [lhe] - lhe file from calcHEP"<< endl;
        cout<< "  [root] - output root file name"<< endl;
        cout<< "  [signal process] - one of the following signal process: "<< endl;
        cout<< "  --> DP_inv - dark photon invisible decay"<< endl;
        cout<< "  --> DP_vis - dark photon visible decay"<< endl;
        cout<< "  --> ALP - axion like particle production"<< endl;
        cout<< "  --> DAP - dark axion portal"<< endl;
        cout<< "  --> DP_e+diagramA - dark photon production via e+ on target"<< endl;
        cout<< "  --> DP_e+diagramB - dark photon production via annihilation, with a photon"<< endl;
        cout<< "  --> DP_e+diagramC - (model missing) dark photon production via annihilation"<< endl;
        cout<< "  [mass point] - name of the 2D LUT file (e.g.: 1MEV)"<< endl;
    }
}

//Turn a string line (consisted of number) to vector of double
void line_to_vec(vector<double>& output, string input)
{
    //First turn string line to vector of string
    vector<string> res;
    string temp;
    stringstream instring(input);
    string::size_type sz;
    while(instring>>temp)
    {
        res.push_back(temp);
    }

    //Then turn vector of string to vector of double
    for(unsigned int i=0;i<res.size();i++)
    {
        string number = res[i];
        double number_temp = stod(number, &sz);
        output.push_back(number_temp);
        //cout<< res[i] << endl;
    }
}

// compute angle
double get_eta(vector<double>& particle){
    double particle_theta = TMath::ACos(particle[8] / TMath::Sqrt(particle[6] * particle[6] + particle[7] * particle[7] + particle[8] * particle[8]));
    return particle_theta;
}
double get_phi(vector<double>& particle){
    double particle_phi = TMath::ATan2(particle[7], particle[6]);
    return particle_phi;
}

int main(int argc,char **argv)
{

cout<<"This tool converts a lhe file into truth-level root file."<<endl;
cout<<"Irrelevant branches (e.g., axion in dark photon process) will be produced but kept empty."<<endl;
cout<<"------------------------------------------------"<<endl;
    if (argc <5){
        PrintUsage();
        return 1;
    }

    string input_file_name, output_root_name, signal_process, mass_point;
    input_file_name = argv[1];
    output_root_name = argv[2];
    signal_process = argv[3];
    mass_point = argv[4];
    
    std::ifstream file(input_file_name);
    int event_begin = 0;
    int event_end = 0;
    int ptr_line=0;
 
    // dummy way to check valid signal process
    if(signal_process != "DP_inv" && signal_process != "DP_vis" &&
       signal_process != "ALP" && signal_process != "DAP" &&
       signal_process != "DP_e+diagramA" && signal_process != "DP_e+diagramB" && signal_process != "DP_e+diagramC"){
        cout<<"Unknown signal process ["<<signal_process<<"], please check again!"<<endl;
        return 1;
    }

    cout<<"current signal process: "<<signal_process<<endl;

    // To make a root file
    TFile* f = new TFile(output_root_name.c_str(),"RECREATE");
    TTree* outputTree = new TTree("lhe","lhe");
    // for LUT
    TH2F * h2D = new TH2F(mass_point.c_str(),mass_point.c_str(),100, 0., 8., 100, 0., 3.);
    // define variables
    double init_e_p[4];
    double init_E_p[4];
    double dp_mass;
    double dp_phi;
    double dp_theta;
    double dp_p[4];
    double dp_px;
    double dp_py;
    double dp_pz;
    double dp_E;
    double axion_mass;
    double axion_phi;
    double axion_theta;
    double axion_p[4];
    double axion_px;
    double axion_py;
    double axion_pz;
    double axion_E;
    double recoil_e_phi;
    double recoil_e_theta;
    double recoil_e_p[4];
    double recoil_e_px;
    double recoil_e_py;
    double recoil_e_pz;
    double recoil_e_E;
    double recoil_E_phi;
    double recoil_E_theta;
    double recoil_E_p[4];
    double recoil_E_px;
    double recoil_E_py;
    double recoil_E_pz;
    double recoil_E_E;
    double decay_e_p[4];
    double decay_e_px;
    double decay_e_py;
    double decay_e_pz;
    double decay_e_E;
    double decay_E_p[4];
    double decay_E_px;
    double decay_E_py;
    double decay_E_pz;
    double decay_E_E;
    double fin_photon_phi;
    double fin_photon_theta;
    double fin_photon_p[4];
    double fin_photon_px;
    double fin_photon_py;
    double fin_photon_pz;
    double fin_photon_E;

    vector<double> dp;
    vector<double> axion;
    vector<double> init_e;
    vector<double> init_E;
    vector<double> recoil_e;
    vector<double> recoil_E;
    vector<double> decay_e;
    vector<double> decay_E;
    vector<double> photon;   

    TVector3 vector3_dp;
    TVector3 vector3_axion;
    TVector3 vector3_recoil_e;
    TVector3 vector3_recoil_E;

    //truth info
    // initial electron
    outputTree->Branch("init_e_p", &init_e_p, "init_e_p[4]/D");
    // initial positron
    outputTree->Branch("init_E_p", &init_E_p, "init_E_p[4]/D");
    // dark photon
    outputTree->Branch("dp_mass",  &dp_mass,  "dp_mass/D");
    outputTree->Branch("dp_phi",   &dp_phi,   "dp_phi/D");
    outputTree->Branch("dp_theta", &dp_theta, "dp_theta/D");
    outputTree->Branch("dp_p",     &dp_p,     "dp_p[4]/D");
    outputTree->Branch("dp_px",    &dp_px,    "dp_px/D");
    outputTree->Branch("dp_py",    &dp_py,    "dp_py/D");
    outputTree->Branch("dp_pz",    &dp_pz,    "dp_pz/D");
    outputTree->Branch("dp_E",     &dp_E,     "dp_E/D");
    // axion
    outputTree->Branch("axion_mass",  &axion_mass,  "axion_mass/D");
    outputTree->Branch("axion_phi",   &axion_phi,   "axion_phi/D");
    outputTree->Branch("axion_theta", &axion_theta, "axion_theta/D");
    outputTree->Branch("axion_p",     &axion_p,     "axion_p[4]/D");
    outputTree->Branch("axion_px",    &axion_px,    "axion_px/D");
    outputTree->Branch("axion_py",    &axion_py,    "axion_py/D");
    outputTree->Branch("axion_pz",    &axion_pz,    "axion_pz/D");
    outputTree->Branch("axion_E",     &axion_E,     "axion_E/D");
    // recoil electron
    outputTree->Branch("recoil_e_phi",   &recoil_e_phi,   "recoil_e_phi/D");
    outputTree->Branch("recoil_e_theta", &recoil_e_theta, "recoil_e_theta/D");
    outputTree->Branch("recoil_e_p",     &recoil_e_p,     "recoil_e_p[4]/D");
    outputTree->Branch("recoil_e_px",    &recoil_e_px,    "recoil_e_px/D");
    outputTree->Branch("recoil_e_py",    &recoil_e_py,    "recoil_e_py/D");
    outputTree->Branch("recoil_e_pz",    &recoil_e_pz,    "recoil_e_pz/D");
    outputTree->Branch("recoil_e_E",     &recoil_e_E,     "recoil_e_E/D");
    // recoil positron
    outputTree->Branch("recoil_E_phi",   &recoil_E_phi,   "recoil_E_phi/D");
    outputTree->Branch("recoil_E_theta", &recoil_E_theta, "recoil_E_theta/D");
    outputTree->Branch("recoil_E_p",     &recoil_E_p,     "recoil_E_p[4]/D");
    outputTree->Branch("recoil_E_px",    &recoil_E_px,    "recoil_E_px/D");
    outputTree->Branch("recoil_E_py",    &recoil_E_py,    "recoil_E_py/D");
    outputTree->Branch("recoil_E_pz",    &recoil_E_pz,    "recoil_E_pz/D");
    outputTree->Branch("recoil_E_E",     &recoil_E_E,     "recoil_E_E/D");
    // for visible decay
    outputTree->Branch("decay_e_p",  &decay_e_p,  "decay_e_p[4]/D");
    outputTree->Branch("decay_E_p",  &decay_E_p,  "decay_E_p[4]/D");
    outputTree->Branch("decay_e_px", &decay_e_px, "decay_e_px/D");
    outputTree->Branch("decay_e_py", &decay_e_py, "decay_e_py/D");
    outputTree->Branch("decay_e_pz", &decay_e_pz, "decay_e_pz/D");
    outputTree->Branch("decay_e_E",  &decay_e_E,  "decay_e_E/D");
    outputTree->Branch("decay_E_px", &decay_E_px, "decay_E_px/D");
    outputTree->Branch("decay_E_py", &decay_E_py, "decay_E_py/D");
    outputTree->Branch("decay_E_pz", &decay_E_pz, "decay_E_pz/D");
    outputTree->Branch("decay_E_E",  &decay_E_E,  "decay_E_E/D");
    // final state photon
    outputTree->Branch("fin_photon_phi",   &fin_photon_phi,   "fin_photon_phi/D");
    outputTree->Branch("fin_photon_theta", &fin_photon_theta, "fin_photon_theta/D");
    outputTree->Branch("fin_photon_p",     &fin_photon_p,     "fin_photon_p[4]/D");
    outputTree->Branch("fin_photon_px",    &fin_photon_px,    "fin_photon_px/D");
    outputTree->Branch("fin_photon_py",    &fin_photon_py,    "fin_photon_py/D");
    outputTree->Branch("fin_photon_pz",    &fin_photon_pz,    "fin_photon_pz/D");
    outputTree->Branch("fin_photon_E",     &fin_photon_E,     "fin_photon_E/D");    


    if (!file.is_open()) {
        cout << "cannot open file!" << endl;
        return 1;
    }
    cout << "convert begin..." << endl;
//-------------------------------------------------------------------
    string line;
    while (getline(file, line))
    {
        if( line == "</LesHouchesEvents>") break; // end of lhe file
        //to get the line I want, and Fill four momentum of particles
        if ( line == "<event>" ) // begin of a event
        {
            event_begin++;
            //cout<<event_begin<<endl;
        }
//-------------------------------------------------------------------
        if(signal_process == "DP_inv"){
            if (event_begin > event_end )
            {
                ptr_line++;
                // dark photon invisible decay
                if(ptr_line == 3) line_to_vec(init_e,   line);
                if(ptr_line == 5) line_to_vec(dp,       line);
                if(ptr_line == 6) line_to_vec(recoil_e, line);
            }
            if ( line == "</event>" ) // end of a event
            {
                event_end++;
                dp_mass = dp[10];
                dp_px = dp[6];
                dp_py = dp[7];
                dp_pz = dp[8];
                dp_E = dp[9];
                recoil_e_px = recoil_e[6];
                recoil_e_py = recoil_e[7];
                recoil_e_pz = recoil_e[8];
                recoil_e_E = recoil_e[9];
                for(int i=0;i<4;i++){
                    dp_p[i]       = dp[i+6];
                    init_e_p[i]   = init_e[i+6];
                    recoil_e_p[i] = recoil_e[i+6];
                }
                //angle
                vector3_dp.SetXYZ(dp_p[0],dp_p[1],dp_p[2]);
                vector3_recoil_e.SetXYZ(recoil_e_p[0],recoil_e_p[1],recoil_e_p[2]);
                dp_phi = vector3_dp.Phi();
                dp_theta = vector3_dp.Theta();
                recoil_e_phi = vector3_recoil_e.Phi();
                recoil_e_theta = vector3_recoil_e.Theta();
            }
        } // DP_inv
//-------------------------------------------------------------------
        if(signal_process == "DP_vis"){
            if (event_begin > event_end )
            {
                ptr_line++;
                // dark photon visible decay
                if(ptr_line == 3) line_to_vec(init_e,   line);
                if(ptr_line == 5) line_to_vec(dp,       line);
                if(ptr_line == 6) line_to_vec(recoil_e, line);
                if(ptr_line == 8) line_to_vec(decay_e,  line);
                if(ptr_line == 9) line_to_vec(decay_E,  line);
            }
            if ( line == "</event>" ) // end of a event
            {
                event_end++;
                dp_mass = dp[10];
                dp_px = dp[6];
                dp_py = dp[7];
                dp_pz = dp[8];
                dp_E = dp[9];
                recoil_e_px = recoil_e[6];
                recoil_e_py = recoil_e[7];
                recoil_e_pz = recoil_e[8];
                recoil_e_E = recoil_e[9];
                decay_e_px = decay_e[6];
                decay_e_py = decay_e[7];
                decay_e_pz = decay_e[8];
                decay_e_E = decay_e[9];
                decay_E_px = decay_E[6];
                decay_E_py = decay_E[7];
                decay_E_pz = decay_E[8];
                decay_E_E = decay_E[9];
                for(int i=0;i<4;i++){
                    dp_p[i]       = dp[i+6];
                    init_e_p[i]   = init_e[i+6];
                    recoil_e_p[i] = recoil_e[i+6];
                    decay_e_p[i]  = decay_e[i+6];
                    decay_E_p[i]  = decay_E[i+6];
                }
            }
        } // DP_vis
//-------------------------------------------------------------------
        if(signal_process == "ALP"){
            if (event_begin > event_end )
            {
                ptr_line++;
                // axion like particle production
                if(ptr_line == 3) line_to_vec(init_e,   line);
                if(ptr_line == 5) line_to_vec(axion,    line);
                if(ptr_line == 6) line_to_vec(recoil_e, line);
            }
            if ( line == "</event>" ) // end of a event
            {
                event_end++;
                axion_mass = axion[10];
                axion_px = axion[6];
                axion_py = axion[7];
                axion_pz = axion[8];
                axion_E = axion[9];
                recoil_e_px = recoil_e[6];
                recoil_e_py = recoil_e[7];
                recoil_e_pz = recoil_e[8];
                recoil_e_E = recoil_e[9];
                for(int i=0;i<4;i++){
                    axion_p[i]    = axion[i+6];
                    init_e_p[i]   = init_e[i+6];
                    recoil_e_p[i] = recoil_e[i+6];
                }
                //angle
                vector3_axion.SetXYZ(axion_p[0],axion_p[1],axion_p[2]);
                vector3_recoil_e.SetXYZ(recoil_e_p[0],recoil_e_p[1],recoil_e_p[2]);
                axion_phi = vector3_axion.Phi();
                axion_theta = vector3_axion.Theta();
                recoil_e_phi = vector3_recoil_e.Phi();
                recoil_e_theta = vector3_recoil_e.Theta();
            }
        } // ALP
//-------------------------------------------------------------------
        if(signal_process == "DAP"){
            if (event_begin > event_end )
            {
                ptr_line++;
                // dark axion portal
                if(ptr_line == 3) line_to_vec(init_e,   line);
                if(ptr_line == 5) line_to_vec(dp,       line);
                if(ptr_line == 6) line_to_vec(axion,    line);
                if(ptr_line == 7) line_to_vec(recoil_e, line);
            }
            if ( line == "</event>" ) // end of a event
            {
                event_end++;
                dp_mass = dp[10];
                dp_px = dp[6];
                dp_py = dp[7];
                dp_pz = dp[8];
                dp_E = dp[9];
                axion_mass = axion[10];
                axion_px = axion[6];
                axion_py = axion[7];
                axion_pz = axion[8];
                axion_E = axion[9];
                recoil_e_px = recoil_e[6];
                recoil_e_py = recoil_e[7];
                recoil_e_pz = recoil_e[8];
                recoil_e_E = recoil_e[9];
                for(int i=0;i<4;i++){
                    dp_p[i]       = dp[i+6];
                    axion_p[i]    = axion[i+6];
                    init_e_p[i]   = init_e[i+6];
                    recoil_e_p[i] = recoil_e[i+6];
                }
                //angle
                vector3_dp.SetXYZ(dp_p[0],dp_p[1],dp_p[2]);
                vector3_axion.SetXYZ(axion_p[0],axion_p[1],axion_p[2]);
                vector3_recoil_e.SetXYZ(recoil_e_p[0],recoil_e_p[1],recoil_e_p[2]);
                /*
                dp_phi = vector3_dp.Phi();
                dp_theta = vector3_dp.Theta();
                axion_phi = vector3_axion.Phi();
                axion_theta = vector3_axion.Theta();
                recoil_e_phi = vector3_recoil_e.Phi();
                recoil_e_theta = vector3_recoil_e.Theta();  */
                dp_phi = get_phi(dp);
                dp_theta = get_eta(dp);
                axion_phi = get_phi(axion);
                axion_theta = get_eta(axion);
                recoil_e_phi = get_phi(recoil_e);
                recoil_e_theta = get_eta(recoil_e);
            }
        } // DAP
//-------------------------------------------------------------------
        if(signal_process == "DP_e+diagramA"){
            if (event_begin > event_end )
            {
                ptr_line++;
                // dark photon production via e+ on target
                if(ptr_line == 3) line_to_vec(init_E,   line);
                if(ptr_line == 5) line_to_vec(dp,       line);
                if(ptr_line == 6) line_to_vec(recoil_E, line);
            }
            if ( line == "</event>" ) // end of a event
            {
                event_end++;
                dp_mass = dp[10];
                dp_px = dp[6];
                dp_py = dp[7];
                dp_pz = dp[8];
                dp_E = dp[9];
                recoil_E_px = recoil_E[6];
                recoil_E_py = recoil_E[7];
                recoil_E_pz = recoil_E[8];
                recoil_E_E = recoil_E[9];
                for(int i=0;i<4;i++){
                    dp_p[i]       = dp[i+6];
                    init_E_p[i]   = init_E[i+6];
                    recoil_E_p[i] = recoil_E[i+6];
                }
                //angle
                vector3_dp.SetXYZ(dp_p[0],dp_p[1],dp_p[2]);
                vector3_recoil_E.SetXYZ(recoil_E_p[0],recoil_E_p[1],recoil_E_p[2]);
                dp_phi = vector3_dp.Phi();
                dp_theta = vector3_dp.Theta();
                recoil_E_phi = vector3_recoil_E.Phi();
                recoil_E_theta = vector3_recoil_E.Theta();
            }
        } // DP_e+diagramA
//-------------------------------------------------------------------
       if(signal_process == "DP_e+diagramB"){
         if (event_begin > event_end )
       {
         ptr_line++;
         // dark photon production via annihilation
         if(ptr_line == 3) line_to_vec(init_E,   line);
         if(ptr_line == 5) line_to_vec(photon,   line);
         if(ptr_line == 6) line_to_vec(dp,       line);         
       }
       if ( line == "</event>" ) // end of a event
       {
         event_end++;
         dp_mass = dp[10];
         dp_px = dp[6];
         dp_py = dp[7];
         dp_pz = dp[8];
         dp_E = dp[9];
         fin_photon_px = photon[6];
         fin_photon_py = photon[7];
         fin_photon_pz = photon[8];
         fin_photon_E = photon[9];
         for(int i=0;i<4;i++){
             dp_p[i]       = dp[i+6];
             init_E_p[i]   = init_E[i+6];
             fin_photon_p[i] = photon[i+6];
             }
         // angle
                dp_phi = get_phi(dp);
                dp_theta = get_eta(dp);
                fin_photon_phi = get_phi(photon);
                fin_photon_theta = get_eta(photon);
       }

       }// DP_e+diagramB
//-------------------------------------------------------------------
        // done with this event
        if ((event_begin == event_end) && (event_begin>0) && (event_end>0))
        {
            ptr_line=0;
            outputTree->Fill();
            //cout<<"just filled, begin: "<<event_begin<<" end: "<<event_end<<endl;
            //if(signal_process=="DAP")hist2D_dp_axion_theta->Fill(dp_theta,axion_theta);

            // make LUT

            if(signal_process == "DP_e+diagramA"){ // rec. positron
              h2D->Fill(recoil_E_E, recoil_E_theta);
              }
            else if(signal_process == "DP_e+diagramB"){ // final state photon
              h2D->Fill(fin_photon_E, fin_photon_theta);
              }
            else h2D->Fill(recoil_e_E, recoil_e_theta); // rec. electron


            init_e.clear();
            init_E.clear();
            dp.clear();
            axion.clear();
            recoil_e.clear();
            recoil_E.clear();
            decay_e.clear();
            decay_E.clear();
            photon.clear();           
 
            vector3_dp.SetXYZ(0,0,0);
            vector3_axion.SetXYZ(0,0,0);
            vector3_recoil_e.SetXYZ(0,0,0);
            vector3_recoil_E.SetXYZ(0,0,0);
        }
    } // end of while
    
    cout << "convert finished." << endl;
    file.close();
    
    f->cd();
    h2D->Write("",TObject::kOverwrite);
    outputTree->Write("",TObject::kOverwrite);
    f->Close();
    f->Delete();

    if(signal_process == "DP_e+diagramA"){ // rec. positron
      cout<<"2D LUT (energy, theta) for recoil positron is produced, name: "<<mass_point<<endl;
      }
    else if(signal_process == "DP_e+diagramB"){ // final state photon
      cout<<"2D LUT (energy, theta) for final state photon is produced, name: "<<mass_point<<endl;
      }
    else{ // rec. electron
      cout<<"2D LUT (energy, theta) for recoil electron is produced, name: "<<mass_point<<endl; 
      }
    cout << "total " << event_end << " events processed." <<endl;
}
