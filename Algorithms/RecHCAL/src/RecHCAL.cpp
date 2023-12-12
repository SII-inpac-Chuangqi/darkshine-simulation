//
// Created by xuliang on 2023/11/17.
//

#include "Algo/RecHCAL.h"

RecHCAL::RecHCAL(string name, shared_ptr<EventStoreAndWriter> evtwrt) : AnaProcessor(std::move(name),
                                                                                     std::move(evtwrt)) {
    Description = "Hadronic Calorimeter Reconstruction Processor";
    // Register input parameters
    RegisterIntParameter("Verbose", "Verbosity Variable", &verbose, 0);
    RegisterStringParameter("HCollectionToUse", "Calorimeter (HCAL) Collection to Use", &hcal_col_use, "FS0");
}

void RecHCAL::Begin() {
    ReadCollections();
    hcal_col_size = static_cast<int>(hcal_cols.size());
    sidehcal_col_size = static_cast<int>(sidehcal_cols.size());
    // Register Output Variable
    if (EvtWrt) {
        EvtWrt->RegisterOutVariable("HCAL_COL_SIZE"      ,&hcal_col_size      ,"Vector size of Main HCAL variables");
        EvtWrt->RegisterOutVariable("HCAL_E_total"       ,&HCAL_total         ,"[0] Truth total Main HCAL energy");
        EvtWrt->RegisterOutVariable("HCAL_E_Max_Cell"    ,&HCAL_E_Max_Cell    ,"[0] Truth maximum Main HCAL cell energy");
        EvtWrt->RegisterOutVariable("SideHCAL_COL_SIZE"  ,&sidehcal_col_size  ,"Vector size of Side HCAL variables");
        EvtWrt->RegisterOutVariable("SideHCAL_E_total"   ,&SideHCAL_total     ,"[0] Truth total Side HCAL energy");
        EvtWrt->RegisterOutVariable("SideHCAL_E_Max_Cell",&SideHCAL_E_Max_Cell,"[0] Truth maximum Side HCAL cell energy");
        EvtWrt->RegisterOutVariable("HCAL_E_Cali"        ,&HCAL_E_Cali        ,"Preliminary truth calibration result");
    }
}

void RecHCAL::ProcessEvt(AnaEvent *evt) {
    // Initialization
    InitEvt();
    if(verbose>1) std::cout<<"=========="<<std::endl;
    const auto &HitCollection = evt->getCalorimeterHitCollection();

    for (const auto &HCAL_Collection_Name: hcal_cols) {
        // temporary HCAL Analyzer
        double HCAL_E = 0;
        double HCAL_E_Max_cell = 0;
//        for (int i = 0; i < 1; i++) {
        auto Collection_String = Form("HCAL_%s", HCAL_Collection_Name.c_str());
        if (HitCollection.count(Collection_String) != 0) {
            const auto &hits = HitCollection.at(Collection_String);

            for (auto hit : *hits) {
                HCAL_E += hit->getE();
                HCAL_E_Max_cell = (HCAL_E_Max_cell >= hit->getE()) ? HCAL_E_Max_cell : hit->getE();
            }
        }
//        }
        // calibration part and record particle information
        double HCAL_E_Front_total = 0;
        double HCAL_E_cali = 0;
        std::map<int, double> mp_Front_HCAL;
        std::map<int, double> E_HCAL_Cali;
        for(auto& p_state : evt->getTruthInfo()->getStatesInHCAL()){
            int Pdg_Front_HCAL = p_state.first.second;
            auto prev_E_Front_HCAL = p_state.second.first->E;
            HCAL_E_Front_total += prev_E_Front_HCAL;
            mp_Front_HCAL[Pdg_Front_HCAL] += prev_E_Front_HCAL;
        }
        /*
        if(cali_pdg == 2112){//incident partcile neutron
            return 3.537*pow(E_before_cali, 1.02) - 20.63;
        }
        else if(cali_pdg == 2212){//proton
            if(E_before_cali <= 120){
                auto tmp_res_1 = 2.012*E_before_cali-169;
                return tmp_res_1 < 0 ? 0 : tmp_res_1;
            }
            else
                return 1.331*pow(E_before_cali, 1.15)+4.938;
        }
        else if(cali_pdg == 111){//pi0

        }
        else if(cali_pdg == 211){//pi+

        }
        else if(cali_pdg == -211){//pi-
        }
        else if(cali)
        else if(cali_pdg == 13){//mu-
            if(E_before_cali <= 180)
                return 2.572*E_before_cali-24.67;
            else if(E_before_cali >= 230)
                return 1.713e-23*pow(E_before_cali, 10)-4.272e-10*pow(E_before_cali, 5)+0.0384*pow(E_before_cali, 2)-971.1;
        }
        else if(cali_pdg == -13){//mu+
            if(E_before_cali <= 180)
                return 2.572*E_before_cali-24.67;
            else if(E_before_cali >= 230)
                return 1.699e-23*pow(E_before_cali, 10)-4.291e-10*pow(E_before_cali, 5)+0.0388*pow(E_before_cali, 2)-998.8;
        }*/
        for(auto& item_mp_Front_HCAL : mp_Front_HCAL){
            // mp[item_mp_Front_HCAL.first]
            //item_mp_Front_HCAL.second
            auto E_before_cali= (item_mp_Front_HCAL.second/HCAL_E_Front_total)*HCAL_E;
            HCAL_E_cali += TruthCalibration(item_mp_Front_HCAL.first, E_before_cali);
        }
        HCAL_E_Cali.push_back(HCAL_E_cali);
        HCAL_total.push_back(HCAL_E);
        HCAL_E_Max_Cell.push_back(HCAL_E_Max_cell);
    }
    for (const auto &SideHCAL_Collection_Name: sidehcal_cols) {
        // temporary Side HCAL Analyzer
        double sideHCAL_E = 0;
        double sideHCAL_E_Max_Cell = 0;
        auto Collection_String = Form("SideHCAL_%s", SideHCAL_Collection_Name.c_str());
        if (HitCollection.count(Collection_String) != 0) {
            const auto &hits = HitCollection.at(Collection_String);
            for (auto hit: *hits) {
                sideHCAL_E += hit->getE();
                if (sideHCAL_E_Max_Cell < hit->getE()) sideHCAL_E_Max_Cell = hit->getE();
            }
        }
        SideHCAL_total.emplace_back(sideHCAL_E);
        SideHCAL_E_Max_Cell.emplace_back(sideHCAL_E_Max_Cell);
    }
}

void RecHCAL::CheckEvt(AnaEvent* /*evt*/) {
    //cout<<"Check!"<<endl;
}

void RecHCAL::End() {
    //cout<<"End!"<<endl;
    dAnaData->printProcessMap();
}

void RecHCAL::ReadCollections() {
    auto format_str = [](const string &str, vector<string> &cols) {
        stringstream s_stream(str); //create string stream from the string
        while (s_stream.good()) {
            string substr;
            getline(s_stream, substr, ','); //get first string delimited by comma


            substr.erase(remove(substr.begin(), substr.end(), ' '), substr.end());
            cols.push_back(substr);
        }
    };
    format_str(hcal_col_use, hcal_cols);
    sidehcal_col_use = hcal_col_use;
    format_str(sidehcal_col_use, sidehcal_cols);
}

double RecHCAL::TruthCalibration(int cali_pdg, double E_before_cali){//calibration function
    auto tmp_res_1 = E_before_cali;
    switch(cali_pdg){
        case 2112: {//neutron
            tmp_res_1 = 3.537*pow(E_before_cali, 1.02) - 20.63;
            break;
        }
        case 2212:{//proton{}
            if(E_before_cali <= 120)
                tmp_res_1 = 2.012*E_before_cali-169;
            else
                tmp_res_1 = 1.331*pow(E_before_cali, 1.15)+4.938;
            break;
        }
        case 111:{//pi0
            if(E_before_cali <= 100)
                tmp_res_1 = E_before_cali*3.818+2.95234;
            else
                tmp_res_1 =  1.265*pow(E_before_cali, 1.11)+83.4566;
            break;
        }
        case 211:{//pi+
            tmp_res_1 =  1.045*pow(E_before_cali, 1.18)-21.70;
            break;
        }
        case -211:{//pi-
            tmp_res_1 = 1.298*pow(E_before_cali, 1.15)-58.12;
            break;
        }
        case 311:{//kaon0
            tmp_res_1 = 2.138*pow(E_before_cali, 1.07)-136.7;
            break;
        }
        case 321:{//kaon+
            tmp_res_1 = 0.3139*pow(E_before_cali, 1.35)-24.51;
            break;
        }
        case -321:{//kaon-
            if(E_before_cali <= 100)
                tmp_res_1 = 1.083*E_before_cali-31.09;
            else
                tmp_res_1 = 0.5562*pow(E_before_cali, 1.26)-133.37;
            break;
        }
        case 13:{//mu-
            if(E_before_cali <= 180)
                tmp_res_1 =  2.572*E_before_cali-24.67;
            else
                tmp_res_1 =  (1.713e-23)*pow(E_before_cali, 10)-(4.272e-10)*pow(E_before_cali, 5)+0.0384*pow(E_before_cali, 2)-971.1;
            break;
        }
        case -13:{//mu+
            if(E_before_cali <= 180)
                tmp_res_1 =  2.572*E_before_cali-24.67;
            else
                tmp_res_1 =  (1.699e-23)*pow(E_before_cali, 10)-(4.291e-10)*pow(E_before_cali, 5)+0.0388*pow(E_before_cali, 2)-998.8;
            break;
        }
    }
//        return tmp_res_1 <= 0 ? E_before_cali : tmp_res_1;
    // return tmp_res_1;
    return (tmp_res_1 == E_before_cali ? E_before_cali*2.8 : tmp_res_1);

}
