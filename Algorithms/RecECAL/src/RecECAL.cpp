//
// Created by Zhang Yulei on 9/19/20.
//

#include "Algo/RecECAL.h"
#include "Algo/Cluster_Analysis.h"

#include <cmath>
#include <iostream>
#include <numeric>
#include <iomanip>
#include <utility>
#include <sstream>
#include "TVector3.h"
#include "TLorentzVector.h"
#ifndef _isVALID
    #define _isVALID(v) (!std::isnan(v)&&(v>-INFINITY)&&(v<+INFINITY))
#endif

RecECAL::RecECAL(string name, shared_ptr<EventStoreAndWriter> evtwrt) : AnaProcessor(std::move(name),
                                                                                     std::move(evtwrt)) {

    // Add description for this AnaProcessor
    Description = "ECAL Reconstruction Processor";

    // Register parameters
    RegisterIntParameter("Verbose", "Verbosity Variable", &verbose, 0);
    RegisterIntParameter("SkipEmpty", "Skip Empty Hits", &SkipEmpty, 0);
    RegisterStringParameter("ECollectionToUse", "Calorimeter (ECAL) Collection to Use", &ecal_col_use, "ECAL_FS0");
    RegisterStringParameter("HCollectionToUse", "Calorimeter (HCAL) Collection to Use", &hcal_col_use, "FS0");
    RegisterIntParameter("E_n_fraction", "the n-th large E fraction", &n_fraction, 20);
    RegisterIntParameter("Advance", "Advanced analysis level", &enAda, 2);
    RegisterIntParameter("SaveTrackInfo", "SaveTrackInfo", &SaveTrackInfo, 0);
    RegisterIntParameter("SaveTruthInfo", "SaveTruthInfo(from MCparticle)", &SaveTruthInfo, 0);

    //extracted the parameter
    setNX(dAnaData->getNECalCellX());
    setNY(dAnaData->getNECalCellY());
    setNZ(dAnaData->getNECalCellZ());
    setSurfaceZ(dAnaData->getECalSurfaceZ());
}

void RecECAL::Begin() {

    ReadCollections();
    ecal_col_size = static_cast<int>(ecal_cols.size());
    hcal_col_size = static_cast<int>(hcal_cols.size());

    // Register Output Variable
    if (EvtWrt) {
        EvtWrt->RegisterIntVariable("ECAL_COL_SIZE", &ecal_col_size, "ECAL_COL_SIZE/I");

//        EvtWrt->RegisterDoubleVariable("ECAL_E_total", &E_total, "ECAL_E_total/D");
//        EvtWrt->RegisterDoubleVariable("ECAL_E_max", &E_max, "ECAL_E_max/D");
//        EvtWrt->RegisterDoubleVariable("ECAL_E_frac", &E_frac, "ECAL_E_frac/D");
//        EvtWrt->RegisterDoubleVariable("ECAL_Moment_Lat", &Moments_Lat, "ECAL_Moment_Lat/D");
//        EvtWrt->RegisterDoubleVariable("ECAL_E_frac_vec", E_frac_vec, "ECAL_E_frac_vec[8]/D");
//        EvtWrt->RegisterDoubleVariable("ECAL_Moment_R", Moments_R, "ECAL_Moment_R[4]/D");
//        EvtWrt->RegisterDoubleVariable("ECAL_Moment_X", Moments_X, "ECAL_Moment_X[4]/D");
//        EvtWrt->RegisterDoubleVariable("ECAL_Moment_Y", Moments_Y, "ECAL_Moment_Y[4]/D");
//        EvtWrt->RegisterDoubleVariable("ECAL_Moment_Z", Moments_Z, "ECAL_Moment_Z[4]/D");
//
//        EvtWrt->RegisterDoubleVariable("HCAL_E_total", &HCAL_total, "HCAL_E_total/D");
//        EvtWrt->RegisterDoubleVariable("HCAL_E_Max_Cell", &HCAL_E_Max_Cell, "HCAL_E_Max_Cell/D");


        EvtWrt->RegisterOutVariable("ECAL_E_total", &E_total);
        EvtWrt->RegisterOutVariable("ECAL_E_max", &E_max);
        EvtWrt->RegisterOutVariable("ECAL_E_frac", &E_frac);
        EvtWrt->RegisterOutVariable("ECAL_Moment_Lat", &Moments_Lat);
        EvtWrt->RegisterOutVariable("ECAL_E_frac_vec", &E_frac_vec);
        EvtWrt->RegisterOutVariable("ECAL_Moment_R", &Moments_R);
        EvtWrt->RegisterOutVariable("ECAL_Moment_X", &Moments_X);
        EvtWrt->RegisterOutVariable("ECAL_Moment_Y", &Moments_Y);
        EvtWrt->RegisterOutVariable("ECAL_Moment_Z", &Moments_Z);

        EvtWrt->RegisterIntVariable("HCAL_COL_SIZE", &hcal_col_size, "HCAL_COL_SIZE/I");
        EvtWrt->RegisterOutVariable("HCAL_E_total", &HCAL_total);
        EvtWrt->RegisterOutVariable("HCAL_E_Max_Cell", &HCAL_E_Max_Cell);

        if(enAda>3){
            EvtWrt->RegisterOutVariable("ECAL_ECell_XY", &ECAL_ECell_XY);
            EvtWrt->RegisterOutVariable("ECAL_ECell_XZ", &ECAL_ECell_XZ);
            EvtWrt->RegisterOutVariable("ECAL_ECell_YZ", &ECAL_ECell_YZ);
            EvtWrt->RegisterOutVariable("ECAL_ECell_Z", &ECAL_ECell_Z);
            EvtWrt->RegisterOutVariable("ECAL_ECell_XYZ", &ECAL_ECell_XYZ);
        }
        if(enAda>2){
            EvtWrt->RegisterOutVariable("ECAL_ClusterSub_E",&ECAL_ClusterSub_E);
            EvtWrt->RegisterOutVariable("ECAL_ClusterSub_X",&ECAL_ClusterSub_X);
            EvtWrt->RegisterOutVariable("ECAL_ClusterSub_Y",&ECAL_ClusterSub_Y);
            EvtWrt->RegisterOutVariable("ECAL_ClusterSub_Z",&ECAL_ClusterSub_Z);
            EvtWrt->RegisterOutVariable("ECAL_ClusterSub_Width_X",&ECAL_ClusterSub_Width_X);
            EvtWrt->RegisterOutVariable("ECAL_ClusterSub_Width_Y",&ECAL_ClusterSub_Width_Y);
            EvtWrt->RegisterOutVariable("ECAL_ClusterSub_Width_Z",&ECAL_ClusterSub_Width_Z);
            EvtWrt->RegisterOutVariable("ECAL_ClusterSub_NCell",&ECAL_ClusterSub_NCell);
            EvtWrt->RegisterOutVariable("ECAL_ClusterSub_P0",&ECAL_ClusterSub_P0);
            EvtWrt->RegisterOutVariable("ECAL_ClusterSub_P1",&ECAL_ClusterSub_P1);
            EvtWrt->RegisterOutVariable("ECAL_ClusterSub_cosTheta",&ECAL_ClusterSub_cosTheta);
            EvtWrt->RegisterOutVariable("ECAL_ClusterSub_phi",&ECAL_ClusterSub_phi);
            EvtWrt->RegisterOutVariable("ECAL_ClusterSub_X_cast",&ECAL_ClusterSub_X_cast);
            EvtWrt->RegisterOutVariable("ECAL_ClusterSub_Y_cast",&ECAL_ClusterSub_Y_cast);
            // -_-
            EvtWrt->RegisterIntVariable("ECAL_ClusterSub_NCell_total", &ECAL_ClusterSub_NCell_total, "ECAL_ClusterSub_NCell_total/I");
            EvtWrt->RegisterIntVariable("ECAL_ClusterSub_N", &ECAL_ClusterSub_N, "ECAL_ClusterSub_N/I");
            EvtWrt->RegisterDoubleVariable("ECAL_ClusterSub_E_total", &ECAL_ClusterSub_E_total, "ECAL_ClusterSub_E_total/D");
        }
        if(enAda>1){
            EvtWrt->RegisterOutVariable("ECAL_Cluster_E",&ECAL_Cluster_E);
            EvtWrt->RegisterOutVariable("ECAL_Cluster_X",&ECAL_Cluster_X);
            EvtWrt->RegisterOutVariable("ECAL_Cluster_Y",&ECAL_Cluster_Y);
            EvtWrt->RegisterOutVariable("ECAL_Cluster_Z",&ECAL_Cluster_Z);
            EvtWrt->RegisterOutVariable("ECAL_Cluster_Width_X",&ECAL_Cluster_Width_X);
            EvtWrt->RegisterOutVariable("ECAL_Cluster_Width_Y",&ECAL_Cluster_Width_Y);
            EvtWrt->RegisterOutVariable("ECAL_Cluster_Width_Z",&ECAL_Cluster_Width_Z);
            EvtWrt->RegisterOutVariable("ECAL_Cluster_NCell",&ECAL_Cluster_NCell);
            EvtWrt->RegisterOutVariable("ECAL_Cluster_NSub",&ECAL_Cluster_NSub);
            EvtWrt->RegisterOutVariable("ECAL_Cluster_P0",&ECAL_Cluster_P0);
            EvtWrt->RegisterOutVariable("ECAL_Cluster_cosTheta",&ECAL_Cluster_cosTheta);
            EvtWrt->RegisterOutVariable("ECAL_Cluster_phi",&ECAL_Cluster_phi);
            EvtWrt->RegisterOutVariable("ECAL_Cluster_X_cast",&ECAL_Cluster_X_cast);
            EvtWrt->RegisterOutVariable("ECAL_Cluster_Y_cast",&ECAL_Cluster_Y_cast);
            // -_-
            EvtWrt->RegisterIntVariable("ECAL_Cluster_NCell_total", &ECAL_Cluster_NCell_total, "ECAL_Cluster_NCell_total/I");
            EvtWrt->RegisterIntVariable("ECAL_Cluster_N", &ECAL_Cluster_N, "ECAL_Cluster_N/I");
            EvtWrt->RegisterDoubleVariable("ECAL_Cluster_E_total", &ECAL_Cluster_E_total, "ECAL_Cluster_E_total/D");
        }
        if(enAda>0){
            EvtWrt->RegisterOutVariable("ECAL_NCell_max_XY", &ECAL_NCell_max_XY);
            EvtWrt->RegisterOutVariable("ECAL_NCell_XY", &ECAL_NCell_XY);
            if(SaveTrackInfo){
                EvtWrt->RegisterOutVariable("ECAL_trkSeed_X",&ECAL_trkSeed_X);
                EvtWrt->RegisterOutVariable("ECAL_trkSeed_Y",&ECAL_trkSeed_Y);
                EvtWrt->RegisterOutVariable("ECAL_trkSeed_POQ",&ECAL_trkSeed_POQ);
                EvtWrt->RegisterOutVariable("ECAL_trkSeed_cosTheta",&ECAL_trkSeed_cosTheta);
                EvtWrt->RegisterOutVariable("ECAL_trkSeed_phi",&ECAL_trkSeed_phi);
            }
            if(SaveTruthInfo){
                EvtWrt->RegisterOutVariable("ECAL_truthSeed_X",&ECAL_truthSeed_X);
                EvtWrt->RegisterOutVariable("ECAL_truthSeed_Y",&ECAL_truthSeed_Y);
                EvtWrt->RegisterOutVariable("ECAL_truthSeed_E",&ECAL_truthSeed_E);
                EvtWrt->RegisterOutVariable("ECAL_truthSeed_P",&ECAL_truthSeed_P);
                EvtWrt->RegisterOutVariable("ECAL_truthSeed_PDG",&ECAL_truthSeed_PDG);
                EvtWrt->RegisterOutVariable("ECAL_truthSeed_ID",&ECAL_truthSeed_ID);
                EvtWrt->RegisterOutVariable("ECAL_truthSeed_parentPDG",&ECAL_truthSeed_parentPDG);
                EvtWrt->RegisterOutVariable("ECAL_truthSeed_parentID",&ECAL_truthSeed_parentID);
                EvtWrt->RegisterOutVariable("ECAL_truthSeed_genProcess",&ECAL_truthSeed_genProcess);
                EvtWrt->RegisterOutVariable("ECAL_truthSeed_cosTheta",&ECAL_truthSeed_cosTheta);
                EvtWrt->RegisterOutVariable("ECAL_truthSeed_phi",&ECAL_truthSeed_phi);
            }
        }
    }
}

void RecECAL::ProcessEvt(AnaEvent *evt) {
    // Initialization
    initialization();
    if(verbose>1) std::cout<<"=========="<<std::endl;

    // Get Simulated Hits for the current event
    //const auto &HitCollection = evt->getSimulatedHitCollection();
    const auto &HitCollection = evt->getCalorimeterHitCollection();
    int smearing_id=0;
    for (const auto &HitCollectionName: ecal_cols) {
        // define the collection name (RawMCParticle) to find.
        // IMPORTANT: check if the collection exists
        if (HitCollection.count(HitCollectionName) != 0) {
            const auto &hits = HitCollection.at(HitCollectionName);
            if (hits->empty()) {
                if((verbose>0 && smearing_id==0) || verbose>1 ) std::cout<<HitCollectionName<<" Empty!!"<<std::endl;
                if(SkipEmpty) continue;
                E_total.push_back(0);
                E_max.push_back(0);
                E_frac.push_back(0);
                Moments_Lat.push_back(_DNAN);
                ECAL_Cluster_E_total=0;
                ECAL_Cluster_N=0;
                ECAL_Cluster_NCell_total=0;
                ECAL_ClusterSub_N=0;
                ECAL_ClusterSub_E_total=0;
                ECAL_ClusterSub_NCell_total=0;
                continue;
            }
            // Calculate some cluster parameters ( moments...)
            auto cluster_ana = std::shared_ptr<Cluster_Analysis>(new Cluster_Analysis(hits));
            if(verbose>1) std::cout<<"T_total "<<smearing_id<<" "<<cluster_ana->FindETotal()<<std::endl;
            E_total.push_back(cluster_ana->FindETotal());
            E_max.push_back(cluster_ana->FindMaxEHit()->getE());
            E_frac.push_back(cluster_ana->FindEFraction(n_fraction));
            Moments_Lat.push_back(cluster_ana->FindLatMoment());

            auto temp_v = vector<double>();
            int j = 0;
            for (auto i : {5, 10, 20, 50, 100, 125, 150, 200}) {
                if(HitCollectionName == "ECAL_FS3" && j == 7)
                    [[maybe_unused]] double a = 0;
                temp_v.push_back(cluster_ana->FindEFraction(i));
                j++;
            }
            E_frac_vec.push_back(temp_v);

            temp_v.clear();
            for (unsigned i = 1; i <= 4; ++i) temp_v.push_back(cluster_ana->FindMoment(i, 0, true));
            Moments_R.push_back(temp_v);

            temp_v.clear();
            for (unsigned i = 1; i <= 4; ++i) temp_v.push_back(cluster_ana->FindMoment(i, 1, true));
            Moments_X.push_back(temp_v);

            temp_v.clear();
            for (unsigned i = 1; i <= 4; ++i) temp_v.push_back(cluster_ana->FindMoment(i, 2, true));
            Moments_Y.push_back(temp_v);

            temp_v.clear();
            for (unsigned i = 1; i <= 4; ++i) temp_v.push_back(cluster_ana->FindMoment(i, 3, true));
            Moments_Z.push_back(temp_v);

            if ((enAda > 0) && (smearing_id==0))
            {
                //level 1: basic advance variable, the N_cells
                int ti[MAX_ECAL_CELLS]={0};
                if (cluster_ana->maxXY(ti))
                {
                    ECAL_NCell_max_XY.clear();
                    for (int i = 0; i < dNX() * dNY(); ++i)
                        ECAL_NCell_max_XY.push_back(ti[i]);
                    for (int i = 0; i < dNX() * dNY(); ++i)
                        ti[i] = 0;
                }
                if (cluster_ana->NXY(ti))
                {
                    ECAL_NCell_XY.clear();
                    for (int i = 0; i < dNX() * dNY(); ++i)
                        ECAL_NCell_XY.push_back(ti[i]);
                    for (int i = 0; i < dNX() * dNY(); ++i)
                        ti[i] = 0;
                }

                //dump track informations
                if(SaveTrackInfo){ 
                    auto _ECal_seed_x = EvtWrt->FindOutVariable<std::vector<double>>("ECal_seed_x");
                    auto _ECal_seed_y = EvtWrt->FindOutVariable<std::vector<double>>("ECal_seed_y");
                    auto _ECal_seed_px = EvtWrt->FindOutVariable<std::vector<double>>("ECal_seed_px");
                    auto _ECal_seed_py = EvtWrt->FindOutVariable<std::vector<double>>("ECal_seed_py");
                    auto _ECal_seed_pz = EvtWrt->FindOutVariable<std::vector<double>>("ECal_seed_pz");
                    auto _RecTrk2_track_chi2=EvtWrt->FindOutVariable<std::vector<double>>("RecTrk2_track_chi2");

                    std::vector<double> _ECAL_trkSeed_X{};
                    std::vector<double> _ECAL_trkSeed_Y{};
                    std::vector<double> _ECAL_trkSeed_POQ{};
                    std::vector<double> _ECAL_trkSeed_cosTheta{};
                    std::vector<double> _ECAL_trkSeed_phi{};

                    assert(_ECal_seed_x->size()==_RecTrk2_track_chi2.size());
                    for(int i=0;i<_ECal_seed_x->size();i++){
                        TVector3 P(_ECal_seed_px->at(i),_ECal_seed_py->at(i),_ECal_seed_pz->at(i));
                        double seed_x=_ECal_seed_x->at(i);
                        double seed_y=_ECal_seed_y->at(i);
                        double chi2 = _RecTrk2_track_chi2->at(i);

                        if(!_isVALID(P.Mag()))
                            continue;
                        if(chi2>=2)
                            continue;

                        _ECAL_trkSeed_X.push_back(seed_x);
                        _ECAL_trkSeed_Y.push_back(seed_y);
                        _ECAL_trkSeed_POQ.push_back(P.Mag()); 
                        _ECAL_trkSeed_cosTheta.push_back(P.CosTheta());
                        _ECAL_trkSeed_phi.push_back(P.Phi());
                    }
                    //sort the track, desending.
                     std::vector<int> indices(_ECAL_trkSeed_POQ.size());
                     std::iota(indices.begin(), indices.end(), 0);
                     std::sort(indices.begin(), indices.end(),
                            [&](int A, int B) -> bool {
                                    return _ECAL_trkSeed_POQ[A] > _ECAL_trkSeed_POQ[B];
                            });
                     for(auto i:indices){
                        ECAL_trkSeed_X.push_back(_ECAL_trkSeed_X.at(i));
                        ECAL_trkSeed_Y.push_back(_ECAL_trkSeed_Y.at(i));
                        ECAL_trkSeed_POQ.push_back(_ECAL_trkSeed_POQ.at(i));
                        ECAL_trkSeed_cosTheta.push_back(_ECAL_trkSeed_cosTheta.at(i));
                        ECAL_trkSeed_phi.push_back(_ECAL_trkSeed_phi.at(i));
                     }
                }
                if (SaveTruthInfo)
                {
                    const auto &MCCollection = evt->getMcParticleCollection();
                    const auto &MPHCollection = evt->getMcPHelperCollection();

                    // define the collection name (RawMCParticle) to find.
                    std::string MCCollectionName = "RawMCParticle";
                    std::string MPHCollectionName = "MCPHelper";

                    vector<double> _ECAL_truthSeed_X{};
                    vector<double> _ECAL_truthSeed_Y{};
                    vector<double> _ECAL_truthSeed_E{};
                    vector<double> _ECAL_truthSeed_P{};
                    vector<int> _ECAL_truthSeed_PDG{};
                    vector<int> _ECAL_truthSeed_ID{};
                    vector<int> _ECAL_truthSeed_parentPDG{};
                    vector<int> _ECAL_truthSeed_parentID{};
                    vector<int> _ECAL_truthSeed_genProcess{};
                    vector<double> _ECAL_truthSeed_cosTheta{};
                    vector<double> _ECAL_truthSeed_phi{};

                    // IMPORTANT: check if the collection exists
                    if (MCCollection.count(MCCollectionName) != 0 && MPHCollection.count(MPHCollectionName) != 0)
                    {

                        [[maybe_unused]] const auto &raws = MCCollection.at(MCCollectionName);
                        const auto &particles = MPHCollection.at(MPHCollectionName);
                        std::unordered_map<int, double> uniqueParticle{}; // for same id only same the largest E one.

                        for (auto p : *particles)
                        {
                            if ( abs(p->getZ() - SurfaceZ()) > 1 ) continue; //+-1mm
                            if ( p->getPz() < 0 ) continue; //non indcident
                            bool add = false;
                            int id = p->getId();
                            int pdg = p->getPdg();
                            int X=p->getX();
                            int Y=p->getY();
                            TLorentzVector p4;
                            p4.SetPxPyPzE(p->getPx(), p->getPy(), p->getPz(), sqrt(p->getP() * p->getP() + p->getMass() * p->getMass()));
                            auto p3 = p4.Vect();
                            if (uniqueParticle.count(id) == 0)
                            {
                                uniqueParticle[id] = p4.E();
                                add = true;
                            }
                            else if (p4.E() > uniqueParticle[id])
                            {
                                uniqueParticle[id] = p4.E();
                                add = true;
                            }
                            if(!add) continue;
                            if( !p->isIncoming() || p->getPz()<0 ) continue; //not save backward particle... incoming not using now...
                            //find parent
                            int parentPDG=0;
                            int parentID=-9; //error default
                            int genProcess=-9;
                            auto mc=p->getMcParticle();
                            if(mc){
                                if( mc->getParents()){
                                    parentPDG=mc->getParents()->getPdg();
                                    parentID=mc->getParents()->getId();
                                }else{
                                    parentPDG=0;
                                    parentID=-2; //no parent
                                    genProcess=-2;
                                }
                                if(id==1){
                                    genProcess=0;
                                    parentPDG=0;
                                    parentID=-1; //initial
                                }else{
                                    genProcess=dAnaData->getProcessId(mc->getCreateProcess());
                                }
                            }else{
                                parentPDG=0;
                                parentID=-1; //no raw mc
                                genProcess=-1;
                            }
                            _ECAL_truthSeed_X.push_back(X);
                            _ECAL_truthSeed_Y.push_back(Y);
                            _ECAL_truthSeed_E.push_back(p4.E());
                            _ECAL_truthSeed_P.push_back(p4.P());
                            _ECAL_truthSeed_PDG.push_back(pdg);
                            _ECAL_truthSeed_ID.push_back(id);
                            _ECAL_truthSeed_parentPDG.push_back(parentPDG);
                            _ECAL_truthSeed_parentID.push_back(parentID);
                            _ECAL_truthSeed_genProcess.push_back(genProcess);
                            _ECAL_truthSeed_cosTheta.push_back(p3.CosTheta());
                            _ECAL_truthSeed_phi.push_back(p3.Phi());
                        }
                        std::vector<int> indices(_ECAL_truthSeed_X.size());
                        std::iota(indices.begin(), indices.end(), 0);
                        std::sort(indices.begin(), indices.end(),
                                [&](int A, int B) -> bool {
                                        return _ECAL_truthSeed_E[A] > _ECAL_truthSeed_E[B];
                                });
                        for(auto i:indices){
                            ECAL_truthSeed_X.push_back(_ECAL_truthSeed_X.at(i));
                            ECAL_truthSeed_Y.push_back(_ECAL_truthSeed_Y.at(i));
                            ECAL_truthSeed_E.push_back(_ECAL_truthSeed_E.at(i));
                            ECAL_truthSeed_P.push_back(_ECAL_truthSeed_P.at(i));
                            ECAL_truthSeed_PDG.push_back(_ECAL_truthSeed_PDG.at(i));
                            ECAL_truthSeed_ID.push_back(_ECAL_truthSeed_ID.at(i));
                            ECAL_truthSeed_parentPDG.push_back(_ECAL_truthSeed_parentPDG.at(i));
                            ECAL_truthSeed_parentID.push_back(_ECAL_truthSeed_parentID.at(i));
                            ECAL_truthSeed_genProcess.push_back(_ECAL_truthSeed_genProcess.at(i));
                            ECAL_truthSeed_cosTheta.push_back(_ECAL_truthSeed_cosTheta.at(i));
                            ECAL_truthSeed_phi.push_back(_ECAL_truthSeed_phi.at(i));
                        }
                    }
                }

                //level 2: dump parent cluster
                if (enAda > 1)
                {                  
                    std::vector<std::map<std::string, double>> ret1;
                    ret1.reserve(100);
                    std::vector<std::map<std::string, double>> ret2;
                    ret2.reserve(100);
                    bool status=false;
                    if(enAda>2){
                        status=cluster_ana->Do(&ret1,&ret2);
                    }else{
                        status=cluster_ana->Do(&ret1);
                    }
                    if(status){
                        std::sort(ret1.begin(), ret1.end(), Esorter_descendingCluster()); //we must sprt the clusters!
                        ECAL_Cluster_N=ret1.size();
                        if(verbose>0) std::cout<<"NCluster="<<ECAL_Cluster_N<<std::endl;
                        for(auto m:ret1){
                            if(verbose>1) std::cout<<"-->Cluster E="<<m["E"]<<std::endl;
                            ECAL_Cluster_E.push_back(m.at("E"));
                            ECAL_Cluster_X.push_back(m.at("X"));
                            ECAL_Cluster_Y.push_back(m.at("Y"));
                            ECAL_Cluster_Z.push_back(m.at("Z"));
                            ECAL_Cluster_Width_X.push_back(m.at("Width_X"));
                            ECAL_Cluster_Width_Y.push_back(m.at("Width_Y"));
                            ECAL_Cluster_Width_Z.push_back(m.at("Width_Z"));
                            ECAL_Cluster_NCell.push_back(m.at("NCell"));
                            ECAL_Cluster_NSub.push_back(m.at("NSub"));
                            ECAL_Cluster_P0.push_back(m.at("P0"));
                            ECAL_Cluster_cosTheta.push_back(abs(m.at("cosTheta"))); //since th fitting do no consider the direction +-
                            ECAL_Cluster_phi.push_back(m.at("phi"));
                            ECAL_Cluster_X_cast.push_back(m.at("X_cast"));
                            ECAL_Cluster_Y_cast.push_back(m.at("Y_cast"));
                            ECAL_Cluster_E_total+=m.at("E");
                            ECAL_Cluster_NCell_total+=m.at("NCell");
                        }
                        if(enAda>2){
                            std::sort(ret2.begin(), ret2.end(), Esorter_descendingCluster()); //we must sprt the clusters!
                            ECAL_ClusterSub_N=ret2.size();
                            if(verbose>0) std::cout<<"NSubCluster="<<ECAL_ClusterSub_N<<std::endl;
                            for(auto m:ret2){
                                if(verbose>1) std::cout<<"-->SubCluster E="<<m["E"]<<std::endl;
                                ECAL_ClusterSub_E.push_back(m.at("E"));
                                ECAL_ClusterSub_X.push_back(m.at("X"));
                                ECAL_ClusterSub_Y.push_back(m.at("Y"));
                                ECAL_ClusterSub_Z.push_back(m.at("Z"));
                                ECAL_ClusterSub_Width_X.push_back(m.at("Width_X"));
                                ECAL_ClusterSub_Width_Y.push_back(m.at("Width_Y"));
                                ECAL_ClusterSub_Width_Z.push_back(m.at("Width_Z"));
                                ECAL_ClusterSub_NCell.push_back(m.at("NCell"));
                                ECAL_ClusterSub_P0.push_back(m.at("P0"));
                                ECAL_ClusterSub_P1.push_back(m.at("P1"));
                                ECAL_ClusterSub_cosTheta.push_back(abs(m.at("cosTheta")));
                                ECAL_ClusterSub_phi.push_back(m.at("phi"));
                                ECAL_ClusterSub_X_cast.push_back(m.at("X_cast"));
                                ECAL_ClusterSub_Y_cast.push_back(m.at("Y_cast"));
                                ECAL_ClusterSub_E_total+=m.at("E");
                                ECAL_ClusterSub_NCell_total+=m.at("NCell");
                            }
                        }
                    }
                }
                //level 4: dump all original hits
                if (enAda > 3)
                {
                    double t[MAX_ECAL_CELLS]={0};
                    if (cluster_ana->FineECellXY(t))
                    {
                        ECAL_ECell_XY.clear();
                        for (int i = 0; i < dNX() * dNY(); ++i)
                            ECAL_ECell_XY.push_back(t[i]);
                        for (int i = 0; i < dNX() * dNY() * dNZ(); ++i)
                            t[i] = 0;
                    }
                    if (cluster_ana->FineECellXZ(t))
                    {
                        ECAL_ECell_XZ.clear();
                        for (int i = 0; i < dNX() * dNZ(); ++i)
                            ECAL_ECell_XZ.push_back(t[i]);
                        for (int i = 0; i < dNX() * dNY() * dNZ(); ++i)
                            t[i] = 0;
                    }
                    if (cluster_ana->FineECellYZ(t))
                    {
                        ECAL_ECell_YZ.clear();
                        for (int i = 0; i < dNY() * dNZ(); ++i)
                            ECAL_ECell_YZ.push_back(t[i]);
                        for (int i = 0; i < dNX() * dNY() * dNZ(); ++i)
                            t[i] = 0;
                    }
                    if (cluster_ana->FineECellXYZ(t))
                    {
                        ECAL_ECell_XYZ.clear();
                        for (int i = 0; i < dNX() * dNY() * dNZ(); ++i)
                            ECAL_ECell_XYZ.push_back(t[i]);
                        for (int i = 0; i < dNX() * dNY() * dNZ(); ++i)
                            t[i] = 0;
                    }
                    if (cluster_ana->FineECellZ(t))
                    {
                        ECAL_ECell_Z.clear();
                        for (int i = 0; i < dNZ(); ++i)
                            ECAL_ECell_Z.push_back(t[i]);
                        for (int i = 0; i < dNX() * dNY() * dNZ(); ++i)
                            t[i] = 0;
                    }
                }
            }
        } else {
            // if not exists, print out error
            cerr << HitCollectionName << " not found" << endl;
        }
        smearing_id++;
    }

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
        HCAL_total.push_back(HCAL_E);
        HCAL_E_Max_Cell.push_back(HCAL_E_Max_cell);
    }
}

void RecECAL::CheckEvt(AnaEvent* /*evt*/) {
    //cout<<"Check!"<<endl;
}

void RecECAL::End() {
    //cout<<"End!"<<endl;
    dAnaData->printProcessMap();
}

void RecECAL::ReadCollections() {

    auto format_str = [](const string &str, vector<string> &cols) {
        stringstream s_stream(str); //create string stream from the string
        while (s_stream.good()) {
            string substr;
            getline(s_stream, substr, ','); //get first string delimited by comma


            substr.erase(remove(substr.begin(), substr.end(), ' '), substr.end());
            cols.push_back(substr);
        }
    };

    format_str(ecal_col_use, ecal_cols);
    format_str(hcal_col_use, hcal_cols);
}


