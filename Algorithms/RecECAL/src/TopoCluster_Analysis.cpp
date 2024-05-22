//
// Clustering Algo from Q.LIU from y2022.
//

#include <utility>
#include <numeric>
#include <deque>
#include <functional>
#include "TMatrixD.h"
#include "TVectorD.h"
#include "TMatrixDSymEigen.h"

#include "Algo/TopoCluster_Analysis.h"

// enable debugging (~1k lines/event)
// #define CLUSTER_DEBUG
#ifdef CLUSTER_DEBUG
    #pragma message "Clustering Debug mode... note your printout!!" 
#endif

// #ifndef CLUSTER_DEBUG
//     // #define EIGEN_NO_DEBUG
// #endif
// #include <Eigen/Dense>

#define MAX_SUB_CLUSTER 100
TopoCluster_Analysis::TopoCluster_Analysis(CalorimeterHitVec *clusterVec, bool isStaggered) : Cluster_Analysis(clusterVec, isStaggered) {
    
    setPOS(dAnaData->getECalPosMap());
    setSurfaceZ(dAnaData->getECalSurfaceZ());    

    allhits.reserve(dAnaData->getNECalCells());
}

std::string TopoCluster_Analysis::printCell(CHit* cell){
    return std::string(Form("%d(%d,%d,%d):%.0fMeV [%d|%d/%d] %s",
                            cell->hit->getCellId(),
                            cell->X(),cell->Y(),cell->Z(),
                            cell->E(),cell->P0(),cell->P1(),cell->P2(),cell->isLocalMax()?"*":""));
}

bool TopoCluster_Analysis::makeSortedCenterIdNeighborsCHitMap() {
    centerIdNeighborsCHit.clear();
    centerIdNeighborsCHit.resize(dAnaData->getNECalCells() + 1);
    if (m_isStaggered) {
        for(auto center : allhits) {
            for (int neighbor_id: dAnaData->getCenterIdNeighborIds_staggered().at(center->hit->getCellId())) {
                centerIdNeighborsCHit.at(neighbor_id).emplace_back(center);
            }
        }
    }

    return true;
}

const TVector3& TopoCluster_Analysis::toPos(CHit* h){ // Pos from AnaData always be mm //note CHit never need to bo const -- it designs to be const
    //first access the geometry information
    return POS().at(dAnaData->getACC(h->X(),h->Y(),h->Z()));
}

double TopoCluster_Analysis::calDistance(CHit* h, const TVector3& loc){
    auto pos=toPos(h);

    #ifdef CLUSTER_DEBUG
        std::cout<<"~ distance debug: "<<h->X()<<","<<h->Y()<<","<<h->Z()
                <<"<-->"<<pos.X()<<","<<pos.Y()<<","<<pos.Z()<<std::endl;
    #endif

    return (pos-loc).Mag() / EM_SCALE_LENGTH_mm;
}

double TopoCluster_Analysis::calWeight(double E1,  double E2, double d1, double d2){
    return E1 / ( E1 + E2 * exp (d1-d2) ) ;
}

bool TopoCluster_Analysis::ConvHits() { 
    //First build CHits 
    for (auto hit : allhits) delete hit;
    allhits.clear();
    for (auto hit: *ClusterVec) {
        auto h=new CHit(hit);
        allhits.push_back(h);
    }
    std::sort(allhits.begin(), allhits.end(), Esorter_descendingC<CHit>);
    // Do the test
    #ifdef CLUSTER_DEBUG
        std::cout<<"Check ALLHITS and POSMAP"<<std::endl
                <<" First hit: cell id(from 1) = "<<ClusterVec->at(0)->getCellIdX()
                <<","<<ClusterVec->at(0)->getCellIdY()
                <<","<<ClusterVec->at(0)->getCellIdZ()
                <<":"<<ClusterVec->at(0)->getE()<<std::endl
                <<" :CHit (id from 1) = "<<printCell(allhits.at(0))
                <<"-->POS ACC"<<ACC(allhits.at(0)->X(),allhits.at(0)->Y(),allhits.at(0)->Z())<<std::endl
                <<" :POS/mm = "<<POS().at(ACC(allhits.at(0)->X(),allhits.at(0)->Y(),allhits.at(0)->Z())).X()
                <<","<<POS().at(ACC(allhits.at(0)->X(),allhits.at(0)->Y(),allhits.at(0)->Z())).Y()
                <<","<<POS().at(ACC(allhits.at(0)->X(),allhits.at(0)->Y(),allhits.at(0)->Z())).Z()<<std::endl;
    #endif
    return true;
}

bool TopoCluster_Analysis::MakePOSMAP() {
    POSMAP.clear();
    POSMAP.resize(dAnaData->getNECalCells(), nullptr);
    for (auto h: allhits) {
        POSMAP.at(dAnaData->getACC(h->X(), h->Y(), h->Z())) = h;
    }
    return true;
}

bool TopoCluster_Analysis::Do(std::vector<std::map<std::string, double>> *ret1,std::vector<std::map<std::string, double>> *ret2, 
                CHitVec *dump) { 
    // First make Chits from calorimeter hits
    if(!ConvHits()){
        std::cerr<<"Fail to conv hits!"<<std::endl;
        return false;
    }
    if(!MakePOSMAP()){
        std::cerr<<"Fail to initialize POSMAP!"<<std::endl;
    }
    if(!makeSortedCenterIdNeighborsCHitMap()) {
        std::cerr<<"Fail to build CenterIdNeighborsCHitMap!" << std::endl;
    }
    //Then run clustering kernel
    if(!Clustering()){
        std::cerr<<"Fail to cluster hits!"<<std::endl;
        return false;
    }
    //Finally collect results
    int _i=0;
    //remember to sort all the clusters! -- here we store based on its clustering process.
    for(auto cluster:clusters){ //currently we save two clusters for debug... parent cluster of sub clsuter with parent id
        if(cluster.size()==0){ //should not happen!!
            return false;
        }
        int P0=cluster.at(0)->P0();
        int NSub=N_subcluster.at(_i++);
        #ifdef CLUSTER_DEBUG
            std::cout<<"Main P0="<<P0
                        <<std::endl;
        #endif
        //save parent cluster
        if(ret1) {
            std::map<std::string, double> m;
            double _ret[10]={0};
            m["P0"]=P0;
            m["NSub"]=NSub;
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
        #ifdef CLUSTER_DEBUG
            std::cout<<"Goto Sub of P0="<<P0<<" NSub="<<NSub
                        <<std::endl;
        #endif
        if(ret2){
            for(int i=((NSub==0)?-1:0);i<NSub;i++){
                int P1=i;
                #ifdef CLUSTER_DEBUG
                    std::cout<<"Sub P1="<<P1
                                <<std::endl;
                #endif
                std::map<std::string, double> m;
                double _ret[10]={0};
                m["P0"]=P0;
                m["P1"]=P1;
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
    
    // it is shared_ptr and no need to release
    return true;
}

bool TopoCluster_Analysis::Clustering() { 
    //Formation
    auto frees=std::deque(allhits.begin(),allhits.end()); // the hits which are not clustered
    std::sort(frees.begin(), frees.end(), Esorter_descendingC<CHit>); // always decending

    CHitVec clustereds{}; //clustered 
    clustereds.reserve(1000);  

    // CHitVec seeds_temp; //seeds at current iteration
    // neighbors_temp.reserve(100); 

    int SEED=0;
    #ifdef CLUSTER_DEBUG
        std::cout<<"Emax "<<frees.front()->E()<<std::endl;
        std::cout<<"Emin "<<frees.back()->E()<<std::endl;
    #endif
    while (frees.size() > 0) //loop all the frees until finished
    {
        // auto init_seed = frees.back(); // from maximim cell
        auto init_seed = frees.front(); // from maximim cell
        if(init_seed->E()/Enoise <= EThres_S) break; // check S, stop if the further hits can not be a seed (decending)
        init_seed->setP0(SEED); //make asosciation
        SEED++;
        // frees.pop_back(); //move from frees + 
        frees.pop_front(); //move from frees + 
        clustereds.push_back(init_seed); // + add to clustered --> also decending
        #ifdef CLUSTER_DEBUG
            std::cout<<"-- Seed E: "<<init_seed->E()<<std::endl;
        #endif

        std::deque<CHit*> seeds_temp{}; //seeds at current iteration. We do not use set as we must be confident there is no double counting
        // seeds_temp.clear(); //no need?
        seeds_temp.push_back(init_seed); // initial seed

        while(seeds_temp.size()>0){
            #ifdef CLUSTER_DEBUG
                std::cout<<"-- - Seed size: "<<seeds_temp.size()<<std::endl;
            #endif
            // auto seed=seeds_temp.back();
            auto seed=seeds_temp.front();
            #ifdef CLUSTER_DEBUG
                std::cout<<"-- - SEED got: "<<printCell(seed)
                    <<std::endl;
                #endif
            // seeds_temp.pop_back();
            seeds_temp.pop_front();
            for (auto neighbor: centerIdNeighborsCHit.at(seed->hit->getCellId())) {
                #ifdef CLUSTER_DEBUG
                    std::cout<<"-- - -- neighbor got: "<<
                        printCell(neighbor)
                        <<std::endl;
                #endif
                if(neighbor->E()/Enoise <= EThres_P) continue; 
                if( neighbor->P0()< 0){ //not associate //prevent duplacate 
                    neighbor->setP0(seed->P0()); //make association 
                    clustereds.push_back(neighbor); // + add to clustered --> this is intend to do here not in an atom operation add+remove: to check the delplicate!
                    if( neighbor->E()/Enoise > EThres_N ){
                        seeds_temp.push_back(neighbor); 
                    }
                }
            }
        }
        //clean frees --> exclusivly clustering!
        for (auto it = frees.begin(); it != frees.end();) //scan all the free cells 
        {
            auto h=*it;
            if(h->P0()>=0){
                it=frees.erase(it); // + move from frees // can be atom operation as clustereds.push_back(neighbor);
            }
            else{
                it++;
            }
        }
    }
    if(clustereds.size() > allhits.size() || clustereds.size() + frees.size() != allhits.size()){
        std::cerr<<"FATAL: clustering failed 1. Check your codes."<<std::endl;
        std::cerr<<"Check: clustereds.size, allhits.size, frees.size: "
                    <<clustereds.size()<<" "
                    <<allhits.size()<<" "
                    <<frees.size()<<" "<<std::endl;
        return false;
    }
    #ifdef CLUSTER_DEBUG
        std::cout<<"SEED="<<SEED<<std::endl;
    #endif

    //form proto-clusters --> clusters
    clusters.clear();
    int valid_count=clustereds.size();
    for (int i = 0; i < SEED; i++)
    {
        CHitVec cluster{};
        for(auto it = clustereds.begin(); it != clustereds.end();){ 
            auto h = *it;
            if (h->P0() == i)
            {
                cluster.push_back(h);
                it=clustereds.erase(it);
                valid_count--;
            }
            else {
                it++;
            }
        }
        if (cluster.size() > 0){
            std::sort(cluster.begin(), cluster.end(), Esorter_descendingC<CHit>); // descending
            clusters.push_back(cluster);
        }
        else{
            std::cerr<<"FATAL: clustering failed 3. Check your codes."<<std::endl;
            std::cerr<<"Check: we have empty cluster i="<<i<<std::endl;
            return false;
        }
    }
    if(valid_count!=0){
        std::cerr<<"FATAL: clustering failed 4. Check your codes."<<std::endl;
        std::cerr<<"Check: parent cluster form failed formed:"<<valid_count<<" clustered: "<<clustereds.size()<<std::endl;
        return false;
    }

    //lets do splittion
    N_subcluster.clear();
    for(auto cluster:clusters){
        if(cluster.size()==0) return false; // should not happen
        int id=cluster.at(0)->P0(); 
        int sub_id = 0 ;
        // find local maxima (Critical E, N , maxima)
        CHitVec localMaxs{};
        for(auto h:cluster){
            #ifdef CLUSTER_DEBUG
                std::cout<<"-- -- Scan localMax from "<<printCell(h)<<std::endl;
            #endif
            if(h->E() > Critical_E){
                int N=0;
                for (auto neighbor: centerIdNeighborsCHit.at(h->hit->getCellId())) {
                    #ifdef CLUSTER_DEBUG
                        std::cout<<"-- -- -- Find neighbor "<<printCell(neighbor)
                                    <<std::endl;
                    #endif
                    // if(neighbor->isLocalMax() || neighbor->E()>h->E()) break; //if E==E, add first if satisfy local max
                    if(neighbor->isLocalMax() || neighbor->E()>h->E()) continue; // why break??
                    if(neighbor->P0()==h->P0()) //we have to be in the same larent cluster -- of course?? except the P cut...
                        N++; 
                }
                #ifdef CLUSTER_DEBUG
                    std::cout<<"-- -- N="<<N
                                <<std::endl;
                #endif
                if(sub_id>MAX_SUB_CLUSTER){
                    std::cout<<"[WARNING] More than 100 sub_cluster found! only keep 100"<<std::endl;
                    continue;
                }
                if(N>=Critical_N){
                    #ifdef CLUSTER_DEBUG
                        std::cout<<"-- -- GOT LocalMax P0= "<<printCell(h)
                                    <<std::endl;
                    #endif
                    h->setLocalMax();
                    h->setP1(sub_id);
                    h->setP2(sub_id);
                    localMaxs.push_back(h);
                    sub_id++;
                }
            }
        }
        #ifdef CLUSTER_DEBUG
            std::cout<<"-- N_sub="<<sub_id<<std::endl;
        #endif
        N_subcluster.push_back(sub_id);
        #ifdef CLUSTER_DEBUG
                std::cout<<" N_subcluster.size="<<N_subcluster.size()
                            <<std::endl;
        #endif
        //determine sub cluster and shared cells
        if(sub_id==0){
            //no local Max, all the P1==P2==-1, P0!=-1, fine
            for(auto h:cluster){
                h->calE_sub();
            }
            continue;
        }

        for(auto h:localMaxs){ //inclusively clustering so that the doubley (shared) clustering could be found
            //clean seeded flag
            for(auto _h:allhits)
                _h->setSeeded(false);
            #ifdef CLUSTER_DEBUG
                std::cout<<"-- -- -- Expand LocalMax "<<printCell(h)<<std::endl;
            #endif
            // std::set<std::shared_ptr<CHit>> seed_cells; 
            std::deque<CHit*> seed_cells;
            //We cannot prevent dulplicate(we have to allow inclusive clustering in this stage) -- use std::set
            // BUT THE ORDER MATTERS!!!
            seed_cells.push_back(h);
            // seed_cells.insert(h);
            while(seed_cells.size()>0){
                #ifdef CLUSTER_DEBUG
                    std::cout<<"-- -- -- - SP "<<h->hit->getCellId()<<" Seed1 size: "<<seed_cells.size()<<std::endl;
                #endif
                // auto seed=*std::prev(seed_cells.end());
                auto seed=seed_cells.front();
                seed->setSeeded(); // use seeded to prevent loop when start from one localMax
                seed_cells.pop_front();
                // seed_cells.erase(std::prev(seed_cells.end()));
                #ifdef CLUSTER_DEBUG
                    std::cout<<"-- -- -- - -- Seed: "<<seed->hit->getCellId()<<std::endl;
                #endif
                for (auto neighbor: centerIdNeighborsCHit.at(seed->hit->getCellId())) {
                    #ifdef CLUSTER_DEBUG
                        std::cout<<"-- -- -- Find neighbor "<<printCell(neighbor)
                                    <<" seeded:"<<neighbor->isSeeded()
                                    <<std::endl;
                    #endif
                    if(neighbor->isSeeded() || neighbor->P0()!=id || neighbor->isLocalMax()) continue; // not in same parent cluster and not add back the localMax
                    neighbor->addNeighbor(seed); //here we keep leading two neighbors to this cell
                    // seed_cells.insert(neighbor);
                    seed_cells.push_back(neighbor);
                }
            } 
        }
        //ok finally do the splitting by the P1 and P2: first calculate E

        std::array<double,MAX_SUB_CLUSTER> E_sub={0.};
        std::array<double,MAX_SUB_CLUSTER> W={0.};
        std::array<TVector3,MAX_SUB_CLUSTER> LOC{};

        CHitVec shared_cells{};
        for(auto h:cluster){ // first adding the energy for splittiong base
            if(h->P1()>=0 && !h->isShared()){ // associate to one localMax
                double E=h->E();
                E_sub.at(h->P1())+=E;
                double w=WEIGHT(E);
                W.at(h->P1())+=w; // use log average.
                LOC.at(h->P1())+=w*toPos(h);
                h->calE_sub(); // finish energy splitting -> all splited to first subclusters
                #ifdef CLUSTER_DEBUG
                    std::cout<<"-- -- +noShared Cell "<<printCell(h)
                            <<"~"<<w<<std::endl;
                #endif
            }else if(h->isShared()){
                shared_cells.push_back(h);
                #ifdef CLUSTER_DEBUG
                    std::cout<<"-- -- +Shared Cell "<<printCell(h)<<std::endl;
                #endif
            }
        }
        for(int i=0;i<sub_id;i++){
            LOC.at(i)*=(1./W.at(i));
        }
        //then expand and use splittion formula: and note: we do this exclusively! the cell and only be considered onece when they neighbor shared cell
        std::sort(shared_cells.begin(), shared_cells.end(), Esorter_descendingC<CHit>);
        for(auto h:shared_cells){
            double d1=calDistance(h,LOC.at(h->P1()));
            double d2=calDistance(h,LOC.at(h->P2()));
            double w1 = calWeight(E_sub.at(h->P1()),E_sub.at(h->P2()),d1,d2);
            #ifdef CLUSTER_DEBUG
                std::cout<<"-- -- -- Expand shared Cell "<<printCell(h)
                            <<"->"<<d1<<"~"<<d2<<"="<<w1
                            <<std::endl;
            #endif
            h->calE_sub(w1);
            CHitVec seed_cells; //here we can ensure no double counting: since we cluster exclusively
            seed_cells.push_back(h); //initial
            while(seed_cells.size()>0){
                #ifdef CLUSTER_DEBUG
                    std::cout<<"-- -- -- - Seed2 size: "<<seed_cells.size()<<std::endl;
                #endif
                auto seed=seed_cells.back();
                seed_cells.pop_back();
                for (auto neighbor: centerIdNeighborsCHit.at(seed->hit->getCellId())) {
                    if(neighbor->P0()!=id || neighbor->P1()>=0 || neighbor->isLocalMax()) continue; // not in same parent cluster or already sub clustered, or localMax(directlt adjount)
                    neighbor->setP1(seed->P1());
                    neighbor->setP2(seed->P2());
                    double _d1=calDistance(neighbor,LOC.at(neighbor->P1()));
                    double _d2=calDistance(neighbor,LOC.at(neighbor->P2()));
                    double _w1 = calWeight(E_sub.at(neighbor->P1()),E_sub.at(neighbor->P2()),_d1,_d2);
                    #ifdef CLUSTER_DEBUG
                        std::cout<<"-- -- -- -- Detected new hit from shared cell: "
                            <<printCell(neighbor)
                            <<"->"<<_d1<<"~"<<_d2<<"="<<_w1
                            <<std::endl;
                    #endif
                    neighbor->calE_sub(_w1);
                    seed_cells.push_back(seed);
                }
            }
        }
        //loop all the CHit to check...
        for(auto h:cluster){
            if(!h->isValid()){
                std::cerr<<"FATAL: clustering failed 9. Check your codes."<<std::endl;
                std::cerr<<"Debug: locked="<<h->isLocked()<<" error="<<h->isError()
                            <<" E="<<h->E()<<" E_sub1="<<h->E_sub1()<<" E_sub2="<<h->E_sub2()<<" "<<
                            printCell(h)<<std::endl;
                return false;
            }
        }//done cluster...
        #ifdef CLUSTER_DEBUG
                std::cout<<"Done cluster!"
                            <<std::endl;
        #endif
    }//done all clusters...
    #ifdef CLUSTER_DEBUG
                std::cout<<"Done all clusters!" << "NCluster="<<clusters.size()<<" N_subcluster.size="<<N_subcluster.size()
                            <<std::endl;
    #endif
    return true;
}

void TopoCluster_Analysis::calXYZCellWidth(int P0, int P1, const CHitVec cluster, double ret[]){ //this is in id unit...
    assert(P0 >= 0 || P0 == _DUMMY);
    ret[0]=_DNAN;
    ret[1]=_DNAN;
    ret[2]=_DNAN;
    double Xmin=_DNAN,Xmax=_DNAN;
    double Ymin=_DNAN,Ymax=_DNAN;
    double Zmin=_DNAN,Zmax=_DNAN;
    for(auto h:cluster){
        if( P0 == _DUMMY || h->E(P0,P1)>0 ){
            auto pos=toPos(h);
            Xmin=Xmax=pos.X();
            Ymin=Ymax=pos.Y();
            Zmin=Zmax=pos.Z();
            break;
        }
    }
    if(std::isnan(Xmin)){
        return;
    }
    for(auto h:cluster){
        if( P0 == _DUMMY || h->E(P0,P1)>0 ){
            auto pos=toPos(h);
            Xmin=std::min(Xmin,pos.X());
            Xmax=std::max(Xmax,pos.X());
            Ymin=std::min(Ymin,pos.Y());
            Ymax=std::max(Ymax,pos.Y());
            Zmin=std::min(Zmin,pos.Z());
            Zmax=std::max(Zmax,pos.Z());
        }
    }
    ret[0]=Xmax-Xmin;
    ret[1]=Ymax-Ymin;
    ret[2]=Zmax-Zmin;           
}

void TopoCluster_Analysis::calXYZ(int P0, int P1, const CHitVec cluster, double ret[]) { //prevent return the XYZVector...
    assert(P0 >= 0 || P0 == _DUMMY);
    ret[0]=_DNAN;
    ret[1]=_DNAN;
    ret[2]=_DNAN;
    double W{0.}; 
    TVector3 LOC(0,0,0);
    for(auto h:cluster){
        double E = (P0 == _DUMMY) ? h->E() : h->E(P0,P1); 
        double w=WEIGHT(E);
        W+=w; 
        LOC+=w*toPos(h);
    }
    LOC*=(1./W);
    ret[0]=LOC.X();        
    ret[1]=LOC.Y(); 
    ret[2]=LOC.Z();
}

bool TopoCluster_Analysis::calXYEW(int P0, int P1, const CHitVec cluster,int Z, double ret[]){
    assert(P0 >= 0 || P0 == _DUMMY);
    ret[0]=_DNAN;
    ret[1]=_DNAN;
    ret[2]=_DNAN;
    ret[3]=_DNAN;
    double W{0.}; 
    double E_sum{0.};
    TVector3 LOC(0,0,0);
    bool valid=false;
    for(auto h:cluster){
        if(h->Z()!=Z) continue;
        double E = (P0 == _DUMMY) ? h->E() : h->E(P0,P1);
        if(E==0) continue;
        valid=true;
        E_sum+=E;
        double w=WEIGHT(E);
        W+=w; 
        LOC+=w*toPos(h);
    }
    if(!valid)
        return false;
    LOC*=(1./W); //we use log weighted to calculate the in-layer center
    ret[0]=LOC.X();        
    ret[1]=LOC.Y(); 
    ret[2]=LOC.Z();  
    ret[3]=E_sum; //but return the E_sum used in later layer fitting
    ret[4]=W; //also return sum_logE as alternative weight for fitting
    return true;
}

// eigen version
// bool TopoCluster_Analysis::calEtaPhiXY(int P0, int P1, const CHitVec cluster, double ret[])
// {
//     assert(P0 >= 0 || P0 == _DUMMY);
//     ret[0]=_DNAN;
//     ret[1]=_DNAN;
//     ret[2]=_DNAN;
//     ret[3]=_DNAN;
//     // emmmmm, mathmatics is s*xy!
//     //https://stackoverflow.com/questions/24747643/3d-linear-regression/67303867#67303867
//     #ifdef CLUSTER_DEBUG
//             std::cout<<"Debug of the EIGEN input:"<<std::endl;
//     #endif
//     std::vector<Eigen::Vector3d> points;
//     std::vector<double> ws;
//     for (int z = 0; z < dNZ(); z++)
//     {
//         double _ret[] = {0, 0, 0, 0, 0};
//         if(calXYEW(P0,P1,cluster, z, _ret)){
//             #ifdef CLUSTER_DEBUG
//                 std::cout<<" : "<<_ret[0]<<","<<_ret[1]<<","<<z<<":"<<_ret[2]<<"~"<<_ret[3]<<std::endl;
//             #endif

//             points.push_back(Eigen::Vector3d(_ret[0], _ret[1], _ret[2]));
//             ws.push_back(WEIGHT(_ret[3])); // we use E_sum as weight
//         }
//     }
//     int N = points.size();
//     if(N<2) return false;
//     double w_sum=0;

//     Eigen::Vector3d mean = {0, 0, 0};
//     Eigen::Matrix3d corr = Eigen::Matrix3d::Zero();
//     int _i=0;
//     for (auto p : points)
//     {
//         double w=ws.at(_i++);  // or now is logSumE, or SumlogW, or 1
//         w_sum+=w;
//         mean += p * w;
//         for (int i = 0; i < 3; i++)
//             for (int j = i; j < 3; j++)
//                 corr(i, j) += p(i) * p(j) * w;
//     }

//     corr /= w_sum;
//     mean /= w_sum;
//     Eigen::Matrix3d cov;
//     cov<<corr(0, 0) - mean(0) * mean(0), corr(0, 1) - mean(0) * mean(1), corr(0, 2) - mean(0) * mean(2),
//          corr(0, 1) - mean(0) * mean(1), corr(1, 1) - mean(1) * mean(1), corr(1, 2) - mean(1) * mean(2),
//          corr(0, 2) - mean(0) * mean(2), corr(1, 2) - mean(2) * mean(1), corr(2, 2) - mean(2) * mean(2);

//     #ifdef CLUSTER_DEBUG
//         std::cout<<"Debug of the EIGEN fitting:"<<std::endl;
//         std::cout<<"mean:"<<std::endl
//             <<mean<<std::endl
//             <<"corr:"<<std::endl
//             <<corr<<std::endl
//             <<"cov:"<<std::endl
//             <<cov<<std::endl;
//     #endif

//     Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> eigensolver(cov);
//     if (eigensolver.info() != Eigen::Success) return false;

//     //get eta and phi
//     auto _dir = eigensolver.eigenvectors().col(2);
//     TVector3 dir(_dir(0),_dir(1),_dir(2));
//     ret[0]=abs(dir.Eta());
//     ret[1]=dir.Phi();
//     //extropolate to z. ECAL surface is the 
//     double z_surface=SurfaceZ();
//     double t= ( z_surface - mean(2) ) / _dir(2);
//     auto pos = mean + t * _dir ;
//     ret[2]=pos(0);
//     ret[3]=pos(1);
//     ret[4]=dir.CosTheta();

//     #ifdef CLUSTER_DEBUG
//         std::cout<<"Debug of the EIGEN result:"<<std::endl;
//         std::cout<<"_dir:"<<std::endl
//             <<_dir<<std::endl
//             <<"t:"<<t<<std::endl
//             <<"cast pos:"<<std::endl
//             <<pos<<std::endl;            
//     #endif

//     return true;
// }

// move to TMatrix -- V2
bool TopoCluster_Analysis::calEtaPhiXY(int P0, int P1, const CHitVec cluster, double ret[])
{
    assert(P0 >= 0 || P0 == _DUMMY);
    ret[0]=_DNAN;
    ret[1]=_DNAN;
    ret[2]=_DNAN;
    ret[3]=_DNAN;
    // emmmmm, mathmatics is s*xy!
    //https://stackoverflow.com/questions/24747643/3d-linear-regression/67303867#67303867
    #ifdef CLUSTER_DEBUG
            std::cout<<"Debug of the EIGEN input:"<<std::endl;
    #endif
    std::vector<TVectorD> points;
    std::vector<double> ws;
    for (int z = 0; z < dNZ(); z++)
    {
        double _ret[] = {0, 0, 0, 0, 0};
        if(calXYEW(P0,P1,cluster, z, _ret)){
            #ifdef CLUSTER_DEBUG
                std::cout<<"["<<P0<<","<<P1<<"] : "<<_ret[0]<<","<<_ret[1]<<","<<_ret[2]<<"("<<z<<") ~ "<<_ret[3]<<std::endl;
            #endif
            points.push_back(TVectorD(3,_ret));
            ws.push_back(WEIGHT(_ret[3])); // we use E_sum as weight
        }
    }
    int N = points.size();
    if(N<2) return false;
    double w_sum=0;

    TMatrixDSym corr(3);
    corr.Zero();
    TVectorD mean(3);
    mean.Zero();
    
    int _i=0;
    for (auto p : points)
    {
        double w=ws.at(_i++);  // or now is logSumE, or SumlogW, or 1
        w_sum+=w;
        mean += p * w;
        for (int i = 0; i < 3; i++)
            for (int j = i; j < 3; j++)
                corr(i, j) += p(i) * p(j) * w;
    }
    for (int i = 0; i < 3; i++)
            for (int j = i; j < 3; j++)
                corr(j, i) = corr(i, j);

    corr *= (1/w_sum);
    mean *= (1/w_sum);

    #ifdef CLUSTER_DEBUG
        std::cout<<"Debug of the EIGEN fitting:"<<std::endl;
        std::cout<<"mean:"<<std::endl;
        mean.Print();
        // std::cout<<"corr:"<<std::endl;
        // corr.Print();
    #endif
    corr.Rank1Update(mean,-1); //inplace convert to cov
    #ifdef CLUSTER_DEBUG
        ;
        // std::cout<<"cov:"<<std::endl;
        // corr.Print();
    #endif

    // run solver adnd check
    const TMatrixDSymEigen eigen(corr);

    //get eigen
    const TVectorD eigenVal = eigen.GetEigenValues();
    TMatrixD eigenVec = eigen.GetEigenVectors();

    //get eta and phi
    int i=0;
    if(eigenVal(0) >= eigenVal(1) && eigenVal(1) >= eigenVal(2))
        ;
    else if(eigenVal(1) >= eigenVal(0) && eigenVal(1) >= eigenVal(2))
        i=1;
    else
        i=2;
    const auto _dir = TVectorD(TMatrixDColumn(eigenVec,i)); // which one??
    TVector3 dir(_dir(0),_dir(1),_dir(2));
    ret[0]=abs(dir.Eta());
    ret[1]=dir.Phi();
    //extropolate to z. ECAL surface is the 
    double z_surface=SurfaceZ();
    double t= ( z_surface - mean(2) ) / _dir(2);
    auto pos = mean + t * _dir ;
    ret[2]=pos(0);
    ret[3]=pos(1);
    ret[4]=dir.CosTheta();

    #ifdef CLUSTER_DEBUG
        std::cout<<"Debug of the EIGEN result:"<<std::endl;
        std::cout<<"evars:"<<i<<std::endl;
        eigenVal.Print();
        // std::cout<<"evarsvec:"<<std::endl;
        // eigenVec.Print();
        std::cout<<"_dir:"<<std::endl;
        _dir.Print();
        std::cout<<"t:"<<t<<std::endl;
        std::cout<<"cast pos:"<<std::endl;
        pos.Print();    
    #endif

    ret[5]=N;
    return true;
}

double TopoCluster_Analysis::calE(int P0, int P1, const CHitVec cluster) {
    assert(P0 >= 0 || P0 == _DUMMY);
    double E=0;
    for(auto h:cluster){
        E += (P0 == _DUMMY) ? h->E() : h->E(P0,P1);
    }
    return E;
}

int TopoCluster_Analysis::calNCell(int P0, int P1, const CHitVec cluster) {
    assert(P0 >= 0 || P0 == _DUMMY);
    if(P0 == _DUMMY) return cluster.size();
    int NCell=0;
    for(auto h:cluster){
        if( h->P0()==P0 && h->P1()==P1 ) NCell++;
    }
    return NCell;
}
