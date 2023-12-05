//
// Clustering Algo from Q.LIU from y2023.
//

#include <utility>
#include <numeric>
#include <deque>
#include <tuple>
#include <functional>
#include "TMatrixD.h"
#include "TVectorD.h"
#include "TMatrixDSymEigen.h"

#include "Algo/PFTopoCluster_Analysis.h"
// #define PFCLUSTER_DEBUG

PFTopoCluster_Analysis::PFTopoCluster_Analysis(CalorimeterHitVec *clusterVec, bool isStaggered) : TopoCluster_Analysis(clusterVec, isStaggered) {
    ;
}

// bool PFTopoCluster_Analysis::AddReferencePoints(int recTrkID, DTruthState* state){ 
//     refs.push_back(std::make_pair(recTrkID,state));
//     std::cout<<"[DEBUG] receieve track "<<printState(refs.back())<<std::endl;
//     return true;
// }

bool PFTopoCluster_Analysis::AddReferencePoints(int recTrkID, double X, double Y, double PX, double PY, double PZ, double E){ 
    auto state = new DTruthState();
    state->vertex[0]=X;
    state->vertex[1]=Y;
    state->vertex[2]=dAnaData->getECalSurfaceZ();
    state->momentum[0]=PX;
    state->momentum[1]=PY;
    state->momentum[2]=PZ;
    state->E=(E>=0)?E:sqrt(PX*PX+PY*PY+PZ*PZ);
    refs.push_back(std::make_pair(recTrkID,state));
    // std::cout<<"[DEBUG] receieve track "<<printState(refs.back())<<std::endl;
    return true;
}

double PFTopoCluster_Analysis::calDisClusterState(CHitVec cluster, r_state state, int P1)  { 
    // distance defain as
    // W = L * (deltaE/E)
    // directly match to the center
    double _ret[10]={0};
    if(P1<0){
        calXYZ(cluster,_ret);
    }else{
        int P0=cluster.at(0)->P0(); 
        calXYZ(P0,P1,cluster,_ret);
    }
    
    TVector3 a(state.second->vertex);
    TVector3 n(state.second->momentum);
    n.SetMag(1);
    TVector3 p(_ret);
    //https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line
    auto dis= ((p - a).Cross(n)).Mag();
    // 
    double E_true=0;
    if(P1<0){
        E_true=calE(cluster);
    }else{
        int P0=cluster.at(0)->P0(); 
        E_true=calE(P0,P1,cluster);
    }

    double dis_E=abs(state.second->E/max(E_true,1.)-1);
    return dis * dis_E;
}

r_state PFTopoCluster_Analysis::getClosetState(CHitVec cluster, std::vector<r_state> states, int P1)  { 
    // // std::cout<<"[DEBUG] input states to sort ";
    // for(auto p:states)
    //     std::cout<<printState(p)<<" ";
    // std::cout<<std::endl;
    assert(states.size()>1);
    std::vector<int> indices(states.size()); // TODO: make it const
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(),
        [&](int A, int B) -> bool {
                return calDisClusterState(cluster,states.at(A),P1) < calDisClusterState(cluster,states.at(B),P1);
        });
    // std::cout<<"[DEBUG] distances w/ P1="<<P1<<" ";
    // for(auto i:indices)
    //     std::cout<<i<<":"<<printState(states.at(i))<<" "<<calDisClusterState(cluster,states.at(i),P1)<<" ";
    // std::cout<<std::endl;
    return states.at(indices.front());
}

bool PFTopoCluster_Analysis::setMatchID(int ID, CHitVec cluster, int P1) { 
    for(auto c:cluster){
        if(P1<0 || c->P1()==P1)
            c->setmatchedTrk(ID);
    }
    return true;
}

bool PFTopoCluster_Analysis::isMatchedAnySub(int ID, CHitVec cluster) { 
    for(auto c:cluster){
        if(c->matchedTrk()==ID)
            return true;
    }
    return false;
}

// P0->index
int PFTopoCluster_Analysis::findCluster(int P0) { 
    size_t i=0;
    for(i=0;i<clusters.size();i++){
        auto cluster=clusters.at(i);
        if(cluster.at(0)->P0()==P0)
            return i;
    }
    return -1;
}

// index -> P0
int PFTopoCluster_Analysis::getP0(int i) { 
    auto cluster = clusters.at(i);
    if(cluster.size()==0) return -1;
    auto ret=cluster.at(0)->P0();
    for(auto h:cluster)
        assert(h->P0()==ret);
    return ret;
}

int PFTopoCluster_Analysis::findMax(int P0) { 
    int i=findCluster(P0);
    if(i<0) return -1;
    auto cluster=clusters.at(i);
    if(cluster.size()==0) return -1;
    if(cluster.size()==1) return 0;
    std::vector<int> indices(cluster.size()); // TODO: make it const
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(),
        [&](int A, int B) -> bool {
                return cluster.at(A)->E() > cluster.at(B)->E();
        });
    return indices.front();
}

int PFTopoCluster_Analysis::findMax(int P0, int P1) { 
    int i=findCluster(P0);
    if(i<0) return -1;
    auto cluster=clusters.at(i);
    if(cluster.size()==0) return -1;
    if(cluster.size()==1) return 0;
    std::vector<int> indices(cluster.size()); // TODO: make it const
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(),
        [&](int A, int B) -> bool {
                return (cluster.at(A)->P1()==P1)*(cluster.at(A)->E()) > (cluster.at(A)->P1()==P1)*(cluster.at(B)->E());
        });
    return indices.front();
}

std::string PFTopoCluster_Analysis::printCluster(CHitVec cluster){
    int P0=cluster.at(0)->P0();
    auto _i=findCluster(P0);
    int NSub=N_subcluster.at(_i);
    int NMatch = N_matchTrk.empty()?-1:N_matchTrk.at(_i);
    int NMatch_orig = N_matchTrk_orig.empty()?-1:N_matchTrk_orig.at(_i); //
    return std::string(Form("(%d)Cluster P0=%d size=%ld NSub=%d NMatch=%d(%d) NCell=%d %.0fMeV",_i,
                            P0,cluster.size(),NSub,NMatch,NMatch_orig,calNCell(cluster),calE(cluster)));
}

std::string PFTopoCluster_Analysis::printState(r_state s){
    return std::string(Form("%d(%.0f,%.0f,%.0fmm)(%.0f,%.0f,%.0fMeV)",s.first,
                                s.second->vertex[0],s.second->vertex[1],s.second->vertex[2],
                                s.second->momentum[0],s.second->momentum[1],s.second->momentum[2]));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool PFTopoCluster_Analysis::MatchClusters(){
    if(refs.size()==0){
        #ifdef PFCLUSTER_DEBUG
            std::cout<<"Warning: NO rerference track!!"<<std::endl;
        #endif
        std::fill_n(back_inserter(N_matchTrk), clusters.size(), 0);
        std::fill_n(back_inserter(N_matchTrk_orig), clusters.size(), 0);
        return true;
    }
    if(refs.size()>1)
        std::sort(refs.begin(), refs.end(),
                [](r_state A, r_state B) -> bool {
                        return A.second->E > B.second->E;
                        });

    // match track to cluster
    // find nearest primary cluster
    // if more track match to same cluster --> bind and match the sub
    // if only one match --> bind
    // finally make the P0 belongs to the recTrkID and P1 P2 reform, recalculte the Esub1 and Esub2
    for(auto p:refs){
        if(clusters.size()==1){
            distance_map.insert(
            {   p.first,
                std::make_tuple(
                            0, calDisClusterState(clusters.at(0),p),
                            -1, -1
                        )
                });
            continue;
        }
        std::vector<int> indices(clusters.size()); // TODO: make it const
        std::iota(indices.begin(), indices.end(), 0);
        std::sort(indices.begin(), indices.end(),
            [&](int A, int B) -> bool {
                    return calDisClusterState(clusters.at(A),p) < calDisClusterState(clusters.at(B),p);
            });
        #ifdef PFCLUSTER_DEBUG
            std::cout<<"distance from recTrk "<<p.first<<std::endl;
            std::cout<<"n clusters "<<clusters.size()<<std::endl;
            for(auto i:indices)
                std::cout<<printCluster(clusters.at(i))<<std::endl;
        #endif
        distance_map.insert({p.first,std::make_tuple(
                            indices.at(0), calDisClusterState(clusters.at(indices.at(0)),p),
                            indices.at(1), calDisClusterState(clusters.at(indices.at(1)),p)
                            )});
    }
    #ifdef PFCLUSTER_DEBUG
        std::cout<<"[DEBUG] reference seeds"<<std::endl;
        for(auto p:refs)
            std::cout<<printState(p)<<std::endl;
        std::cout<<"[DEBUG] distance map"<<std::endl;
        for(auto p:distance_map)
            std::cout<<"-->trk "<<p.first<<":"<<std::get<0>(p.second)<<","<<std::get<1>(p.second)<<"mm;"<<std::get<2>(p.second)<<","<<std::get<3>(p.second)<<"mm"<<std::endl;
    #endif
    int _i=0;
    int maxP0=0;
    std::vector<r_state> non_matched_track{};
    for(auto cluster:clusters){
        if(cluster.size()==0){ 
            return false;
        }
        int P0=cluster.at(0)->P0();
        maxP0=max(maxP0,P0);
        int NSub=N_subcluster.at(_i++);
        std::vector<r_state> matched_trks;
        for(auto p:refs){
            if(getP0(std::get<0>(distance_map.at(p.first))) == P0)
                matched_trks.push_back(p);
        }
        N_matchTrk.push_back(matched_trks.size());
        N_matchTrk_orig.push_back(matched_trks.size());
        if(matched_trks.size()==0){ // non match, do nothing, matchID=-1
            ;
        }else if(matched_trks.size()==1){ // one-to-one match
            setMatchID(matched_trks.at(0).first,cluster);
        }else if(matched_trks.size()>1){ // sub match
            // if(matched_trks.size()>Nsub) ...; // TODO: dis-match
            #ifdef PFCLUSTER_DEBUG
                std::cout<<"[DEBUG] multi match: "<<matched_trks.size()<<std::endl;
            #endif
            for(int P1=(NSub==0)?-1:0;P1<NSub;P1++){
                auto p=getClosetState(cluster,matched_trks,P1);
                setMatchID(p.first,cluster,P1);
                #ifdef PFCLUSTER_DEBUG
                    std::cout<<"\t P1= "<<P1 <<" closetState "<<printState(p)<<std::endl;
                #endif
            }
            // some track might match to primary but not any sub, recored them for later usage
            for(auto p:matched_trks){
                if(!isMatchedAnySub(p.first,cluster)){
                    non_matched_track.push_back(p);
                    N_matchTrk.back()-=1;
                    #ifdef PFCLUSTER_DEBUG
                        std::cout<<"[DEBUG] non matched track "<<printState(p)<<" taken out of from cluster "<<printCluster(cluster)<<std::endl;
                    #endif
                }
            }
        }
    }
    double E_nonmatch=0.;
    for(auto p:non_matched_track)
        E_nonmatch+=p.second->E;

    #ifdef PFCLUSTER_DEBUG
        std::cout<<"[DEBUG] total non matched track "<<non_matched_track.size()<<" total energy "<<E_nonmatch<<std::endl;
    #endif
    // TODO: dis-match
    // //finally match non matched track and non matched cluster -- not used for now
    // // if the 2nd closet cluster to non matched track, match them ,if not remain un match
    // for(auto p:non_matched_track){
    //     auto P0 = std::get<2>(distance_map.at(p.first));
    //     auto i=findCluster(P0);
    //     auto cluster = clusters.at(i);
    //     int NSub=N_subcluster.at(i);

    //     if(N_matchTrk.at(i)==0){
    //         std::cout<<"[DEBUG] non matched track "<<p.first<<" match to 2nd cluster: "<< P0 <<" already match to 0"<<std::endl;
    //         setMatchID(p.first,cluster);
    //         N_matchTrk.at(i)=1;
    //     }else{ // later can use other method.
    //         std::cout<<"[DEBUG] non matched track "<<p.first<<" match to 2nd cluster: "<< P0 <<" already match to "<<N_matchTrk.at(i)<<std::endl;
    //         std::vector<r_state> matched_trks(p);
    //         for(auto p:refs){
    //             if(std::get<0>(distance_map.at(p.first)) == P0)
    //                 matched_trks.push_back(p);
    //             }
    //         for(int P1=(NSub==0)?-1:0;P1<NSub;P1++){
    //             auto p=getClosetState(cluster,matched_trks,P1);
    //             setMatchID(p.first,cluster,P1);
    //         }
    //         N_matchTrk.at(i)++;
    //     }
    // }
    _i=0;
    for(auto cluster:clusters){
        if(cluster.size()==0){ 
            return false;
        }
        int P0=cluster.at(0)->P0();
        int NSub=N_subcluster.at(_i);
        int NMatch = N_matchTrk.at(_i);
        _i+=1;
        if(NMatch==0){
            // 0 match, merge all the hits
            #ifdef PFCLUSTER_DEBUG
                std::cout<<"[DEBUG] cluster "<<printCluster(cluster)<<" No match track-->remove sub"<<std::endl;
            #endif
            for(auto h:cluster)
                h->cleanSub();
        }else if(NMatch==1){
            // for those do not have sub, make 1 sub
            #ifdef PFCLUSTER_DEBUG
                std::cout<<"[DEBUG] cluster "<<printCluster(cluster)<<" match one track-->form 1 sub"<<std::endl;
            #endif
            for(auto h:cluster)
                h->assignSub(0);
            // pick localMax
            cluster.at(findMax(P0))->assignSub(0,true);
        }else{
            // TODO: dis-match
            // if(NSub==0 || NSub<NMatch){ 
            //      ;
            // }
            #ifdef PFCLUSTER_DEBUG
                std::cout<<"[DEBUG] cluster "<<printCluster(cluster)<<" match more track..."<<std::endl;
            #endif
            assert(NSub>1 && NSub>=NMatch);
            // build track-local relationship
            std::map<int,std::vector<int>> recTrk_P1s;
            for(auto p:refs){ // will form NMatched sub cluster
                std::vector<int> P1s; //first match local max
                for(auto h:cluster)
                    if(h->matchedTrk()==p.first && h->isLocalMax())
                        P1s.push_back(h->P1());
                recTrk_P1s.insert({p.first,P1s});
            }
            // then reform sub
            for(auto h:cluster){
                if(h->isLocalMax()){
                    h->assignSub(h->matchedTrk());
                }else if(h->P2()<0){// non splitting cells
                    h->assignSub(h->matchedTrk());
                }else{ // splitting cells
                    auto P1s = recTrk_P1s.at(h->matchedTrk());
                    if(std::find(P1s.begin(), P1s.end(), h->P2()) != P1s.end()){
                        // cell belong to same track-cluster --> cancel split
                        h->assignSub(h->matchedTrk());
                    }else{
                        // cell share by two trakc-cluster, find another trk
                        auto it = std::find_if(recTrk_P1s.begin(), recTrk_P1s.end(), 
                            [&](std::pair<int,std::vector<int>> s) -> bool { 
                                return std::find(s.second.begin(), s.second.end(), h->P2()) != s.second.end();
                            });
                        assert(it!=recTrk_P1s.end());
                        h->assignSub(h->matchedTrk(),it->first,h->E_sub1());
                    }
                }
            }
            // set localMax
            for(auto k:recTrk_P1s){
                int P1=k.first;
                cluster.at(findMax(P0,P1))->assignSub(P1,true);
            }
            // rename P1/P2 (start from 0)
            int j=0;
            for(auto s:recTrk_P1s){
                for(auto h:cluster)
                    h->renameSub(s.first,j);
                j+=1;
            }
            #ifdef PFCLUSTER_DEBUG
                std::cout<<"[DEBUG] new cluster "<<printCluster(cluster)<<std::endl;
            #endif
        }
    }
    return true;
}

bool PFTopoCluster_Analysis::Do(
                std::vector<std::map<std::string, double>> *ret1,
                std::vector<std::map<std::string, double>> *ret2, 
                CHitVec *dump) { 
    /* method1: cell match
     - first read track info (trkSeed or truthSeed)
     - then increase cone, until all the cells with in are clustered and energy <= P_track and excluded from the topo clustering
     - then run the topo clustering for other cells
    */

    /* method2: cluster match * 
     - first read track info (trkSeed or truthSeed)
     - then increase cone, until all the subclusters with in are clustered and energy <= P_track and excluded from the topo clustering
     - then run the topo clustering for other cells
    */
    
    // first run the clustering
    if(!ConvHits() || !MakeHMAP() || !MakePOSMAP() || !makeSortedCenterIdNeighborsCHitMap() || !Clustering()){
        std::cerr<<"Fail to build initial cluster!"<<std::endl;
        return false;
    }

    // then do the match and re form the primary clusters
    if(!MatchClusters()){
        std::cerr<<"Fail to match the cluster!"<<std::endl;
        return false;
    }

    //Finally collect results as usual, now the subCLuster will hold the information!
    int _i=0;
    //remember to sort all the clusters! -- here we store based on its clustering process.
    for(auto cluster:clusters){ 
        if(cluster.size()==0){ //should not happen!!
            return false;
        }
        int P0=cluster.at(0)->P0();
        int NSub=N_subcluster.at(_i);
        int NMatch = N_matchTrk.at(_i);
        int NMatch_orig = N_matchTrk_orig.at(_i);
        _i+=1;
        #ifdef PFCLUSTER_DEBUG
            std::cout<<printCluster(cluster)<<std::endl;
        #endif
        //save parent cluster
        if(ret1) {
            std::map<std::string, double> m;
            double _ret[10]={0};
            m["P0"]=P0;
            m["NSub"]=NMatch; // modified
            m["NSub_orig"]=NSub; // modified
            m["NMatch_orig"]=NMatch_orig; // modified
            m["E"]=calE(cluster);
            calXYZ(cluster,_ret);
            m["X"]=_ret[0];    
            m["Y"]=_ret[1];
            m["Z"]=_ret[2];
            calXYZCellWidth(cluster,_ret);
            m["Width_X"]=_ret[0];
            m["Width_Y"]=_ret[1];
            m["Width_Z"]=_ret[2];
            m["NCell"]=calNCell(cluster); //cluster.size(); 
            auto status=calEtaPhiXY(cluster,_ret);
            if(!status){ // fallback to center method
                _ret[0]=-5;
                _ret[1]=-5;
                _ret[2]=m["X"];
                _ret[3]=m["Y"];
                _ret[4]=-5;
                #ifdef CLUSTER_DEBUG
                    std::cout<<"Eta fitting failed(n_Z<2): parent cluster P0="<<P0
                                <<" NCell="<<m["NCell"]
                                <<" E="<<m["E"]
                                <<" NSub"<<m["NSub"]
                                <<std::endl;
                #endif
            }
            m["eta"]=_ret[0];
            m["phi"]=_ret[1];
            m["X_cast"]=_ret[2];
            m["Y_cast"]=_ret[3];
            m["cosTheta"]=_ret[4];

            ret1->push_back(m);
        }
        if(ret2){
            for(int i=((NMatch==0)?-1:0);i<NMatch;i++){
                int P1=i;
                #ifdef PFCLUSTER_DEBUG
                    std::cout<<"Sub P1="<<P1
                                <<std::endl;
                #endif
                std::map<std::string, double> m;
                double _ret[10]={0};
                m["P0"]=P0;
                m["P1"]=P1;
                m["matchRecTrk"]=cluster.at(findMax(P0,P1))->matchedTrk();
                m["E"]=calE(P0,P1,cluster);
                m["NCell"]=calNCell(P0,P1,cluster);
                calXYZ(P0,P1,cluster,_ret);
                m["X"]=_ret[0];    
                m["Y"]=_ret[1];
                m["Z"]=_ret[2];
                calXYZCellWidth(P0,P1,cluster,_ret);
                m["Width_X"]=_ret[0];
                m["Width_Y"]=_ret[1];
                m["Width_Z"]=_ret[2];
                auto status=calEtaPhiXY(P0,P1,cluster,_ret);
                if(!status){
                    _ret[0]=-5;
                    _ret[1]=-5;
                    _ret[2]=m["X"];
                    _ret[3]=m["Y"];
                    _ret[4]=-5;
                    #ifdef CLUSTER_DEBUG
                        std::cout<<"Eta fitting failed(n_Z<2): sub cluster P0="<<P0<<" P1="<<P1
                                    <<" NCell="<<m["NCell"]
                                    <<" E="<<m["E"]
                                    <<std::endl;
                    #endif
                }
                m["eta"]=_ret[0];
                m["phi"]=_ret[1];
                m["X_cast"]=_ret[2];
                m["Y_cast"]=_ret[3];
                m["cosTheta"]=_ret[4];

                ret2->push_back(m);
            }
        }
    }
    if(dump){
        dump->clear();
        for(auto h:allhits){
            dump->push_back(std::move(h));
        }
        return true;
    }

    // clean
    for(auto p:refs)
        delete p.second;
    refs.clear();
    // it is shared_ptr and no need to release
    return true;
}
