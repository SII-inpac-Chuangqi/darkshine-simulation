//
// Created by xuliang on 2022/10/12.
//

#ifndef DSIMU_PROCESSREADER_H
#define DSIMU_PROCESSREADER_H

#include <utility>
#include <tuple>
#include "TString.h"
#include "Object/McParticle.h"
#include "Object/DTruth.h"
#include "Core/AnaProcessor.h"
#include "Core/AnaData.h"
#include "Object/Util/PhysicsProcessDef.h"
#include "TVector3.h"

class ProcessReader {

public:
    explicit ProcessReader(const shared_ptr<EventStoreAndWriter> &evtWrt) : EvtWrt(evtWrt) {
        for (auto const& proc_name : InterestProcName)
            InterestProcId.emplace_back(pDef.dPhyTypeMap[proc_name]);
        eBrem_Id = pDef.dPhyTypeMap["eBrem"];
        EN_Id = pDef.dPhyTypeMap["electronNuclear"];
        GMM_Id = pDef.dPhyTypeMap["GammaToMuPair"];
        PN_Id = pDef.dPhyTypeMap["photonNuclear"];
    };

    void initialization();

    void RegisterParameters();

    void ReadProcess(AnaEvent* Evt);

    void SetInitialE(DStep_b1_5 *step) {
        initial_E = step->getE();
        minProcE = 0.5 * initial_E;
    };


private:

    void setMainProcess(const std::string processName, const float energy, const float vertex[3]) {
        MainProcessName = processName;
        MainProcessPVName = dAnaData->getRegionName(vertex);
        MainProcessEnergy = energy;
        MainProcessVertexZ = vertex[2];
    };

    void setMainProcess(const DTruthProcess* proc) {
        MainProcessName = pDef.dPhyTypeVec.at(proc->index);
        MainProcessPVName = dAnaData->getRegionName(proc->vertex);
        MainProcessEnergy = proc->E;
        MainProcessVertexZ = proc->vertex[2];
    };

private:

    const shared_ptr<EventStoreAndWriter> &EvtWrt;
    PhysicsDef pDef;
    const std::vector<std::string> InterestProcName = {"GammaToMuPair", "photonNuclear", "electronNuclear"};
    std::vector<unsigned int> InterestProcId{};
    unsigned int eBrem_Id{};
    unsigned int EN_Id{};
    unsigned int GMM_Id{};
    unsigned int PN_Id{};
    Bool_t if_refresh_hardbrem;

    double initial_E; // energy of the first initial track step
    double minProcE;

    // Event Type
    TString  MainProcessName;
    TString  MainProcessPVName;
    float  MainProcessEnergy;
    float  MainProcessVertexZ;
    int process_HardBrem;
    int process_HardBrem_Target; // if exist hardbrem (eBrem with E > 4GeV ) process in the Target region
    int process_HardBrem_ECAL; // if exist hardbrem  process in the ECAL region
    int process_GMM;
    int process_GMM_Target;
    int process_GMM_ECAL;
    int process_EN;
    int process_EN_Target;
    int process_EN_ECAL;
    int process_PN;
    int process_PN_Target;
    int process_PN_ECAL;
    // detailed process
    std::vector<Int_t> process_ID; // Process index defined in PhysicsDef::dPhyTypeVec
    std::vector<TString>  process_PVName;
    std::vector<TVector3> process_vertex;
    std::vector<float>  process_energy;
    std::vector<Int_t> process_parent_pdg;
    std::vector<Int_t> process_parent_id; // Unique particle id
};

#endif //DSIMU_PROCESSREADER_H
