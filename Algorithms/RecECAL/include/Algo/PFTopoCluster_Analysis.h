//
// Created by Zhang Yulei on 12/27/20.
//

#ifndef DANA_PFTOPOCLUSTER_ANALYSIS_H
#define DANA_PFTOPOCLUSTER_ANALYSIS_H

#include "Core/AnaProcessor.h"
#include "Core/AnaData.h"

//never include any templete header like Eigen, XYZVector, so on. see https://stackoverflow.com/questions/67529835/eigen3-take-a-long-time-to-compile-and-very-slow-when-debug

#include "Algo/TopoCluster_Analysis.h"
typedef std::pair<int,DTruthState*> r_state;

/// \brief Base Analysis Class for Cluster
/// \note The input data vector will be automatically sorted in desending order by energy
class PFTopoCluster_Analysis : public TopoCluster_Analysis {
public:
    PFTopoCluster_Analysis() = default;

    explicit PFTopoCluster_Analysis(CalorimeterHitVec* clusterVec, bool isStaggered=true);

    virtual ~PFTopoCluster_Analysis() = default;

    bool Do(std::vector<std::map<std::string, double>> *ret1,std::vector<std::map<std::string, double>> *ret2, 
                CHitVec *dump=nullptr) override;
    
    // bool AddReferencePoints(int recTrkID, DTruthState* state) ;
    bool AddReferencePoints(int recTrkID, double X, double Y, double PX, double PY, double PZ, double E=-1);
   
private:
// cluster index: clusters.at(i)
//P0: h->P0
    std::vector<r_state> refs; // recTrkID: ECAL state
    std::map<int,std::tuple<int,double,int,double>> distance_map; // recTrkID: nearest clusterID,distance,2nd near cluster(for future usage), distance
    std::vector<int> N_matchTrk{}; // the considered matched track (i is the index for clusters)
    std::vector<int> N_matchTrk_orig{}; // the actual matched track

    // functions
    double calDisClusterState(CHitVec cluster, r_state state, int P1=-1)   ;
    r_state getClosetState(CHitVec cluster, std::vector<r_state> states, int P1)   ;
    bool setMatchID(int ID, CHitVec cluster, int P1=-1)  ;
    bool isMatchedAnySub(int ID, CHitVec cluster)  ;

    int findCluster(int P0)  ;
    int getP0(int i);

    int findMax(int P0)  ;
    int findMax(int P0, int P1)  ;
    std::string printCluster(CHitVec cluster);
    std::string printState(r_state s);
    bool MatchClusters();
};


#endif //DANA_PFTOPOCLUSTER_ANALYSIS_H
