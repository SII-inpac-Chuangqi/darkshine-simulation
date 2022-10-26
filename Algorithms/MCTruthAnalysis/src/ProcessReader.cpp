//
// Created by xuliang on 2022/10/12.
//

#include "Algo/ProcessReader.h"

using namespace std;

void ProcessReader::RegisterParameters() {
    // Event Type
    EvtWrt->RegisterOutVariable("MainProcessName", &MainProcessName);
    EvtWrt->RegisterOutVariable("MainProcessPVName", &MainProcessPVName);
    EvtWrt->RegisterOutVariable("MainProcessEnergy", &MainProcessEnergy);
    EvtWrt->RegisterOutVariable("MainProcessVertexZ", &MainProcessVertexZ);
    EvtWrt->RegisterOutVariable("process_HardBrem", &process_HardBrem);
    EvtWrt->RegisterOutVariable("process_HardBrem_Target",&process_HardBrem_Target);
    EvtWrt->RegisterOutVariable("process_HardBrem_ECAL",&process_HardBrem_ECAL);
    EvtWrt->RegisterOutVariable("process_GMM", &process_GMM);
    EvtWrt->RegisterOutVariable("process_GMM_Target", &process_GMM_Target);
    EvtWrt->RegisterOutVariable("process_GMM_ECAL", &process_GMM_ECAL);
    // detailed process
    EvtWrt->RegisterOutVariable("process_ID", &process_ID);
    EvtWrt->RegisterOutVariable("process_Name", &process_Name);
    EvtWrt->RegisterOutVariable("process_PVName", &process_PVName);
    EvtWrt->RegisterOutVariable("process_vertex", &process_vertex);
    EvtWrt->RegisterOutVariable("process_energy", &process_energy);
    EvtWrt->RegisterOutVariable("process_parent_pdg", &process_parent_pdg);
    EvtWrt->RegisterOutVariable("process_parent_id",&process_parent_id);
}

// Event Type Priority (from high to low)
//  GMM (E>4 GeV) / PN (E>4 GeV) / EN (E>4 GeV)
//  hardbrem (eBrem E>4 GeV)
//  EN (E<=4 GeV ) by initial e-
//  inclusive
void ProcessReader::ReadProcess(AnaEvent *Evt) {
    auto truth_particles = Evt->getTruthInfo()->getTruthParticles();
    for (auto const& tp : truth_particles) {
        vector < DTruthProcess * > v_processes = tp->sec_process_link;
        for (auto const& process: v_processes) {
            // Define Event Type (Main Process)
            if (process->E > 4000.) {
                if (process->index == eBrem_Id && process->E > MainProcessEnergy) { // HardBrem
                    if_refresh_hardbrem = true;
                    if (MainProcessEnergy > 4000.)
                        for (auto const& proc_name: InterestProcName) // prevent hardbem overwrite Interested Process with E > 4GeV
                            if (MainProcessName == proc_name) if_refresh_hardbrem = false;
                    if (if_refresh_hardbrem)
                        setMainProcess("hardbrem", process->E, process->vertex);
                } else {
                    for (auto const& proc_id : InterestProcId)
                        if (process->index == proc_id)
                            if (process->E > MainProcessEnergy || MainProcessName == "hardbrem") // GMM (E>4 GeV) / PN (E>4 GeV) / EN (E>4 GeV) can overwrite hardbrem
                                setMainProcess(process);
                }
            } else if (tp->id == 1 && process->index == EN_Id && process->E > MainProcessEnergy) { // ElectronNuclear from Initial e-
                setMainProcess(process);
            }
            // Set process flag
            if ( process->E > 4000. && process->index == eBrem_Id) { // HardBrem
                process_HardBrem++;
                if (dAnaData->getRegionName(process->vertex) == "Target") process_HardBrem_Target++;
                else if (dAnaData->getRegionName(process->vertex) == "ECAL") process_HardBrem_ECAL++;
            } else if ( process->index == GMM_Id ) { // GammaToMuPair
                process_GMM++;
                if (dAnaData->getRegionName(process->vertex) == "Target") process_GMM_Target++;
                if (dAnaData->getRegionName(process->vertex) == "ECAL") process_GMM_ECAL++;
            }
            // fill process
            process_ID.emplace_back(process->index);
            process_Name.emplace_back(pDef.dPhyTypeVec.at(process->index));
            process_PVName.emplace_back(dAnaData->getRegionName(process->vertex));
            process_vertex.emplace_back(process->vertex);
            process_energy.emplace_back(process->E);
            process_parent_pdg.emplace_back(tp->pdg);
            process_parent_id.emplace_back(tp->id);
        }
    }
}

void ProcessReader::initialization() {
    MainProcessName = "inclusive";
    MainProcessPVName = "";
    MainProcessEnergy = 0.;
    MainProcessVertexZ = -611;
    process_HardBrem = 0;
    process_HardBrem_Target = 0;
    process_HardBrem_ECAL = 0;
    process_GMM = 0;
    process_GMM_Target = 0;
    process_GMM_ECAL = 0;
    process_ID.clear(); process_ID.shrink_to_fit();
    process_Name.clear(); process_Name.shrink_to_fit();
    process_PVName.clear(); process_PVName.shrink_to_fit();
    process_vertex.clear(); process_vertex.shrink_to_fit();
    process_energy.clear(); process_energy.shrink_to_fit();
    process_parent_pdg.clear(); process_parent_pdg.shrink_to_fit();
    process_parent_id.clear(); process_parent_id.shrink_to_fit();
}