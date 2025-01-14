//
// Created by xuliang on 2023/11/17.
//

#include "Algo/RecHCAL.h"

RecHCAL::RecHCAL(string name, shared_ptr<EventStoreAndWriter> evtwrt) : AnaProcessor(std::move(name),
                                                                                     std::move(evtwrt)) {
    Description = "Hadronic Calorimeter Reconstruction Processor";
    // Register input parameters
    RegisterIntParameter("Verbose", "Verbosity Variable", &verbose, 0);
    RegisterIntParameter("ParticleStore", "Particles' info in HCAL", &particle_store, 0);
    RegisterIntParameter("HitStore", "Hits' info in HCAL", &hit_store, 0);
    RegisterIntParameter("BDTVarStore", "BDT variables", &bdt_var_store, 0);
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
        if (particle_store) {
            EvtWrt->RegisterOutVariable("HCAL_Particle_Pdg"   ,&HCAL_Particle_Pdg     ,"Truth Particle PDG");
            EvtWrt->RegisterOutVariable("HCAL_Particle_X"     ,&HCAL_Particle_X      ,"Truth Particle X position");
            EvtWrt->RegisterOutVariable("HCAL_Particle_Y"     ,&HCAL_Particle_Y      ,"Truth Particle Y position");
            EvtWrt->RegisterOutVariable("HCAL_Particle_Z"     ,&HCAL_Particle_Z      ,"Truth Particle Z position");
            EvtWrt->RegisterOutVariable("HCAL_Particle_E"     ,&HCAL_Particle_E      ,"Truth Particle Energy");
            EvtWrt->RegisterOutVariable("HCAL_Particle_Px"    ,&HCAL_Particle_Px     ,"Truth Particle Px");
            EvtWrt->RegisterOutVariable("HCAL_Particle_Py"    ,&HCAL_Particle_Py     ,"Truth Particle Py");
            EvtWrt->RegisterOutVariable("HCAL_Particle_Pz"    ,&HCAL_Particle_Pz     ,"Truth Particle Pz");
            EvtWrt->RegisterOutVariable("HCAL_Particle_Process",&HCAL_Particle_Process,"Truth Production Process");
        }
        if (hit_store) {
            EvtWrt->RegisterOutVariable("HCAL_Hit_X"          ,&HCAL_Hit_X           ,"HCAL Hit X position");
            EvtWrt->RegisterOutVariable("HCAL_Hit_Y"          ,&HCAL_Hit_Y           ,"HCAL Hit Y position");
            EvtWrt->RegisterOutVariable("HCAL_Hit_Z"          ,&HCAL_Hit_Z           ,"HCAL Hit Z position");
            EvtWrt->RegisterOutVariable("HCAL_Hit_IDX"          ,&HCAL_Hit_IDX           ,"HCAL Hit ID X");
            EvtWrt->RegisterOutVariable("HCAL_Hit_IDY"          ,&HCAL_Hit_IDY           ,"HCAL Hit ID Y");
            EvtWrt->RegisterOutVariable("HCAL_Hit_IDZ"          ,&HCAL_Hit_IDZ           ,"HCAL Hit ID Z");
            EvtWrt->RegisterOutVariable("HCAL_Cell_E"          ,&HCAL_Cell_E           ,"HCAL cell energy");
            EvtWrt->RegisterOutVariable("SideHCAL_Hit_X"          ,&SideHCAL_Hit_X           ,"SideHCAL Hit X position");
            EvtWrt->RegisterOutVariable("SideHCAL_Hit_Y"          ,&SideHCAL_Hit_Y           ,"SideHCAL Hit Y position");
            EvtWrt->RegisterOutVariable("SideHCAL_Hit_Z"          ,&SideHCAL_Hit_Z           ,"SideHCAL Hit Z position");
            EvtWrt->RegisterOutVariable("SideHCAL_Hit_IDX"          ,&SideHCAL_Hit_IDX           ,"SideHCAL Hit ID X");
            EvtWrt->RegisterOutVariable("SideHCAL_Hit_IDY"          ,&SideHCAL_Hit_IDY           ,"SideHCAL Hit ID Y");
            EvtWrt->RegisterOutVariable("SideHCAL_Hit_IDZ"          ,&SideHCAL_Hit_IDZ           ,"SideHCAL Hit ID Z");
            EvtWrt->RegisterOutVariable("SideHCAL_Cell_E"          ,&SideHCAL_Cell_E           ,"SideHCAL cell energy");
            EvtWrt->RegisterOutVariable("HCAL_E_front"        ,&HCAL_E_front         ,"HCAL Hit Energy");
            EvtWrt->RegisterOutVariable("HCAL_E_back"         ,&HCAL_E_back          ,"HCAL Hit Energy");
        }
        if (bdt_var_store) {
            EvtWrt->RegisterOutVariable("HCAL_layer1_E"          ,&HCAL_layer1_E           ,"HCAL first layer energy in each bar");
            EvtWrt->RegisterOutVariable("HCAL_X_average"          ,&HCAL_X_average           ,"HCAL energy per bar in X-layers");
            EvtWrt->RegisterOutVariable("HCAL_Y_average"          ,&HCAL_Y_average           ,"HCAL energy per bar in Y-layers");
            EvtWrt->RegisterOutVariable("HCAL_E_per_layer"          ,&HCAL_E_per_layer           ,"HCAL E per layer");
            EvtWrt->RegisterOutVariable("HCAL_E_Min_Cell"          ,&HCAL_E_Min_Cell           ,"HCAL min E");
            EvtWrt->RegisterOutVariable("HCAL_E_concentration"          ,&HCAL_E_concentration           ,"HCAL (max_cell - sub_max_cell)/(max_cell + sub_max_cell)");
            EvtWrt->RegisterOutVariable("HCAL_E_Max_layer"          ,&HCAL_E_Max_layer           ,"HCAL layer number with max E");
            EvtWrt->RegisterOutVariable("HCAL_E_Max_layer_num"          ,&HCAL_E_Max_layer_num           ,"HCAL layer with max E");
            EvtWrt->RegisterOutVariable("HCAL_lighted_cells_per_lighted_layer"          ,&HCAL_lighted_cells_per_lighted_layer           ,"HCAL average number of cells per lighted layer");
            EvtWrt->RegisterOutVariable("HCAL_total_length"          ,&HCAL_total_length           ,"HCAL lighted length");
            EvtWrt->RegisterOutVariable("HCAL_total_lighted_layer"          ,&HCAL_total_lighted_layer           ,"HCAL total lighter layer number");
            EvtWrt->RegisterOutVariable("HCAL_E_main_side_ratio"          ,&HCAL_E_main_side_ratio           ,"Ratio between main HCAL E and Side HCAL E");
        }
    }
}

void RecHCAL::ProcessEvt(AnaEvent *evt) {
    // Initialization
    InitEvt();
    if(verbose>1) std::cout<<"=========="<<std::endl;
    //Truth particle information
    // std::cout<<"Truth information collection"<<std::endl;
    if (particle_store) {
        const auto &_McPMap = evt->getMcParticleCollection();
        for(const auto &eMcPMap:_McPMap){
            for(const auto &eMcP:*eMcPMap.second){
                if(abs(eMcP->getPdg())==2112 || abs(eMcP->getPdg())==13 || abs(eMcP->getPdg())==2212 || abs(eMcP->getPdg())==111 || abs(eMcP->getPdg())==211 || abs(eMcP->getPdg())==311 || abs(eMcP->getPdg())==321 || abs(eMcP->getPdg())==130 || abs(eMcP->getPdg())==310 || abs(eMcP->getPdg())==22){ //Neutron or Muon
                    HCAL_Particle_Pdg.emplace_back(eMcP->getPdg());
                    HCAL_Particle_E.emplace_back(eMcP->getEnergy());
                    HCAL_Particle_Px.emplace_back(eMcP->getPx());
                    HCAL_Particle_Py.emplace_back(eMcP->getPy());
                    HCAL_Particle_Pz.emplace_back(eMcP->getPz());
                    HCAL_Particle_X.emplace_back(eMcP->getVertexX());
                    HCAL_Particle_Y.emplace_back(eMcP->getVertexY());
                    HCAL_Particle_Z.emplace_back(eMcP->getVertexZ());
                    HCAL_Particle_Process.emplace_back(eMcP->getCreateProcess());
                }
            }
        }
    }

    const auto &HitCollection = evt->getCalorimeterHitCollection();

    double E_HCAL;
    double E_sideHCAL;
    double E_ratio_main_side;
    for (const auto &HCAL_Collection_Name: hcal_cols) {
        // temporary HCAL Analyzer
        double HCAL_E = 0;
        double HCAL_E_Max_cell = 0;

        // Temp BDT variables
        int n_cell = 0;
        double temp_max_cell = 0;
        double E_sec_max = 0;
        int iLayer = 0;
        int iCell = 0;
        double E_cell_1[90][30] = {0};
        double E_cell_2[90][30] = {0};
        double E_min = 8000;
        int x_num = 0;
        int y_num = 0;
        double x_all = 0.;
        double y_all = 0.;
        double E_per_layer = 0.;
        double E_max_layer = 0.;
        int Max_layer_num = 0;
        vector<int> layer_n;

        auto Collection_String = Form("HCAL_%s", HCAL_Collection_Name.c_str());
        if (HitCollection.count(Collection_String) != 0) {
            const auto &hits = HitCollection.at(Collection_String);

            for (auto hit : *hits) {
                if (hit_store) {
                    HCAL_Hit_X.emplace_back(hit->getX());
                    HCAL_Hit_Y.emplace_back(hit->getY());
                    HCAL_Hit_Z.emplace_back(hit->getZ());
                    HCAL_Hit_IDX.emplace_back(hit->getCellIdX());
                    HCAL_Hit_IDY.emplace_back(hit->getCellIdY());
                    HCAL_Hit_IDZ.emplace_back(hit->getCellIdZ());
                    HCAL_Cell_E.emplace_back(hit->getE());
                    if(hit->getZ()<2048. )HCAL_E_front += hit->getE();
                    else {HCAL_E_back += hit->getE();}
                }
                HCAL_E += hit->getE();
                n_cell++;
                HCAL_E_Max_cell = (HCAL_E_Max_cell >= hit->getE()) ? HCAL_E_Max_cell : hit->getE();
                if (temp_max_cell != HCAL_E_Max_cell) {
                    E_sec_max = temp_max_cell;
                    temp_max_cell = HCAL_E_Max_cell;
                }else if (E_sec_max < hit->getE()) {
                    E_sec_max = hit->getE();
                }

                if (bdt_var_store) {
                    iLayer = hit->getCellIdZ() / 2 + 1;
                    iCell = hit->getCellIdX(); 

                    if (iLayer == 1) {HCAL_layer1_E.emplace_back(hit->getE());} 
                    if (hit->getCellIdY() == 1) {E_cell_1[iLayer - 1][iCell - 1] = hit->getE();}
                    else {E_cell_2[iLayer - 1][iCell - 1] = hit->getE();}
                   
                    if (E_min > hit->getE()) {E_min = hit->getE();}

                    if (iLayer %2 == 1) {
                        x_num++;
                        x_all = x_all + hit->getX();
                    } else {
                        y_num++;
                        y_all = y_all + hit->getY();
                    }
                }
            }
            if (bdt_var_store) {
              if (E_min == 8000) E_min = 0;
            }
        }

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
        for(auto& item_mp_Front_HCAL : mp_Front_HCAL){
            // mp[item_mp_Front_HCAL.first]
            //item_mp_Front_HCAL.second
            auto E_before_cali= (item_mp_Front_HCAL.second/HCAL_E_Front_total)*HCAL_E;
            HCAL_E_cali += TruthCalibration(item_mp_Front_HCAL.first, E_before_cali);
        }
        HCAL_E_Cali.push_back(HCAL_E_cali);
        HCAL_total.push_back(HCAL_E);
        HCAL_E_Max_Cell.push_back(HCAL_E_Max_cell);

        //BDT variables
        if (bdt_var_store) {
            if (x_num != 0) {HCAL_X_average.emplace_back(x_all / x_num);}
            if (y_num != 0) {HCAL_Y_average.emplace_back(y_all / y_num);}

            for (int iL = 0; iL < 90; iL++) {
                E_per_layer = 0;
                for (int iC = 0; iC < 30; iC++) {
                    E_per_layer = E_per_layer + E_cell_1[iL][iC] + E_cell_2[iL][iC];

                    E_cell_1[iL][iC] = 0;
                    E_cell_2[iL][iC] = 0;
                }
                HCAL_E_per_layer.emplace_back(E_per_layer);
                if (E_max_layer < E_per_layer) {
                    E_max_layer = E_per_layer;
                    Max_layer_num = iL + 1;
                }
                if (E_per_layer > 0) {
                    layer_n.push_back(iL + 1);
                }
            }
            HCAL_E_Min_Cell.emplace_back(E_min);
            HCAL_E_concentration.emplace_back((HCAL_E_Max_cell - E_sec_max) / (HCAL_E_Max_cell + E_sec_max));
            HCAL_E_Max_layer.emplace_back(E_max_layer);
            HCAL_E_Max_layer_num.emplace_back(Max_layer_num);
            if (n_cell == 0) {
                HCAL_lighted_cells_per_lighted_layer.emplace_back(0);
            } else {
                HCAL_total_length.emplace_back(layer_n.back() - layer_n.front() + 1);
                HCAL_total_lighted_layer.emplace_back(layer_n.size());
                HCAL_lighted_cells_per_lighted_layer.emplace_back(n_cell / layer_n.size());
            }
        
            E_HCAL = HCAL_E;
        }
    }
    for (const auto &SideHCAL_Collection_Name: sidehcal_cols) {
        // temporary Side HCAL Analyzer
        double sideHCAL_E = 0;
        double sideHCAL_E_Max_Cell = 0;
        auto Collection_String = Form("SideHCAL_%s", SideHCAL_Collection_Name.c_str());
        if (HitCollection.count(Collection_String) != 0) {
            const auto &hits = HitCollection.at(Collection_String);
            for (auto hit: *hits) {
                if (hit_store) {
                    SideHCAL_Hit_X.emplace_back(hit->getX());
                    SideHCAL_Hit_Y.emplace_back(hit->getY());
                    SideHCAL_Hit_Z.emplace_back(hit->getZ());
                    SideHCAL_Hit_IDX.emplace_back(hit->getCellIdX());
                    SideHCAL_Hit_IDY.emplace_back(hit->getCellIdY());
                    SideHCAL_Hit_IDZ.emplace_back(hit->getCellIdZ());
                    SideHCAL_Cell_E.emplace_back(hit->getE());
                }
                sideHCAL_E += hit->getE();
                if (sideHCAL_E_Max_Cell < hit->getE()) sideHCAL_E_Max_Cell = hit->getE();
            }
        }
        SideHCAL_total.emplace_back(sideHCAL_E);
        SideHCAL_E_Max_Cell.emplace_back(sideHCAL_E_Max_Cell);
        if (bdt_var_store) {
            E_sideHCAL = sideHCAL_E;
        }
    }
    if (bdt_var_store) {
        if (E_sideHCAL != 0) {
            E_ratio_main_side = E_HCAL / E_sideHCAL;
        }
        HCAL_E_main_side_ratio.emplace_back(E_ratio_main_side);
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
