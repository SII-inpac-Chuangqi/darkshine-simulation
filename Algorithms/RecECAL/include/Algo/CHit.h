// Kernel type of clustering algo -- Q.L.
#ifndef DANA_CHIT_H
#define DANA_CHIT_H

//used for dummy value for cluster id
#define _DUMMY -2

class CHit { 
    public: 
        CHit(CalorimeterHit *h){
            _E=h->getE();
            _X=h->getCellIdX(); //this should start from 1
            _Y=h->getCellIdY();
            _Z=h->getCellIdZ();
            hit=h; // keep the link to reco hit
        };
        #ifdef CLUSTER_DEBUG
            CHit(SimulatedHit *h){ // only used for debugging...
                _E=h->getE();
                _X=h->getCellIdX(); //this should start from 1
                _Y=h->getCellIdY();
                _Z=h->getCellIdZ();
                isTruth=true;
                hit=h; // keep the link to truth hit
            };
        #endif
        double E(){return _E;};
        double E(int P0, int P1){
            assert(P0 >= 0);
            if(isClustered(P0,P1)){
                return (P1==_P1)?E_sub1():E_sub2();
            }else
                return 0;
        };
        int X(){return _X;};
        int Y(){return _Y;};
        int Z(){return _Z;};
        int P0(){return _P0;};
        int P1(){return _P1;};
        int P2(){return _P2;};
        int matchedTrk(){return _matchedTrk;};
        void setmatchedTrk(int v){_matchedTrk=v;}; // used for match
        bool isClustered(int P0, int P1){
            assert(P0 >= 0);
            return (P0==_P0 && (P1==_P1 || P1==_P2));
        };
        bool isSeeded(){return _isSeeded;};
        void setSeeded(bool v=true){_isSeeded=v;};
        bool isLocalMax(){return _isLocalMax;};
        bool isShared(){return (!_isLocalMax && _P1>=0 && _P2>=0);};
        bool isLocked(){return locked;};
        bool isError(){return error;};
        bool isValid(){
            if(error) return false;
            if(!locked) return false; // when check it should be locked(energy splitted)
            if((_E_sub1 + _E_sub2 - _E) >= 0.001*_E || (_E_sub1 + _E_sub2 - _E) <= -0.001*_E) return false;
            if(_isLocalMax && _P1==_P2 && _P1>=0) //localMax
                return true;
            if(!_isLocalMax && _P1==_P2 && _P1==-1) // no localMax
                return true;
            if(!_isLocalMax && _P1>=0) //assiciated to one/two localMaxima
                return true;
            return false;
        };

        double E_sub1(){return locked?_E_sub1:_DNAN;};
        double E_sub2(){return locked?_E_sub2:_DNAN;};

        void calE_sub(double w1)
        {
            if (!locked) //prevent ouble claculte the splitting!!
            {
                _E_sub1 = _E * w1;
                _E_sub2 = _E * (1 - w1);
                locked = true;
            }
            else
                error = true;
        };
        void calE_sub()
        {
            if (!locked)
            {
                if (_isLocalMax || _P2 < 0) //prevent splitting cell enter this func
                {
                    _E_sub1 = _E;
                    _E_sub2 = 0;
                    locked = true;
                }
                else
                    error = true;
            }
            else
                error = true;
        };

        void addNeighbor(CHit* h){
            //assert h->isLocalMax()
            //assert h->P1()==h->P2()
            if(_P1<0){
                _P1=h->P1();
                _E_sub1=h->E();
            }
            else if(_P2<0){
                if(h->E()>_E_sub1){
                    if(h->P1()!=_P1){
                        _P2=_P1;
                        _P1=h->P1();
                        _E_sub2=_E_sub1;
                    }
                    _E_sub1=h->E();
                }else if(h->P1()!=_P1){
                    _P1=h->P1();
                    _E_sub1=h->E();
                }
            }
            else{
                //method1: use desencding localMax and keep only first P1 P2
                // ;
                //method2: use leading two neighbors
                if(h->E()>_E_sub1){
                    if(h->P1()!=_P1){
                        _P2=_P1;
                        _P1=h->P1();
                        _E_sub2=_E_sub1;
                     }
                     _E_sub1=h->E();
                }else if(h->E()>_E_sub2 && h->P1()!=_P1){
                        _P2=h->P1();
                        _E_sub2=h->E();
                }else{
                   ; 
                }
                //method3: determined by cluster energy -- need recalculation
                //;
            }
        };

        void setP0(int v){_P0=v;};
        void setP1(int v){_P1=v;};
        void setP2(int v){_P2=v;};
        void setLocalMax(){_isLocalMax=true;};

        void cleanSub(){
            //cleanSub
            _P1=-1;
            _P2=-1;
            _E_sub1=_E;
            _E_sub2=0;
        };
        void assignSub(int P1, int P2, double E_sub1){ // used for match-reform
            _P1=P1;
            _P2=P2;
            _E_sub1=E_sub1;
            _E_sub2=_E - E_sub1;
        };
        void assignSub(int P1, bool isLocalMax=false){ // used for match-reform
            _P1=P1;
            _P2=(isLocalMax?P1:-1);
            _E_sub1=_E;
            _E_sub2=0;
            _isLocalMax=isLocalMax;
        };
        void renameSub(int from,int to){ // used for match-reform
            if(_P1>=0 && _P1==from)
                _P1=to;
            if(_P2>=0 && _P2==from)
                _P2=to;
        };
        
        // bool isNeighbor(std::shared_ptr<CHit> b){
        //     return (std::abs(X-b->X)+std::abs(Y-b->Y)+std::abs(Z-b->Z)==1);
        // };
        // bool isNeighbor(CHit b){
        //     return (std::abs(X-b.X)+std::abs(Y-b.Y)+std::abs(Z-b.Z)==1);
        // };
        bool isTruth{false}; // TODO: better to be private
        #ifdef CLUSTER_DEBUG
            DHit *hit; // allow for simulationHit directly cluster (no smearing to caloHit)
        #else
            CalorimeterHit *hit;
        #endif
        void reset(){
            _P0=-1; 
            _P1=-1; 
            _P2=-1; 
            _E_sub1=0;
            _E_sub2=0;
            locked=false;
            _isLocalMax=false;
            error=false;
            _isSeeded=false;
            _matchedTrk=-1;
        };

    private:
        double _E{0};
        int _X{-1};
        int _Y{-1};
        int _Z{-1};
        int _P0{-1}; // id of parent cluster
        int _P1{-1}; // id of first sassociated sub cluster inside the parent cluster
        int _P2{-1}; // id of potential second associated sub cluster inside the parent cluster
        double _E_sub1{0};//neighbor cell energy, used in splitting; or splitted energy used in final summation
        double _E_sub2{0};
        // double _E_matched{0}; // the energy belongs to the matchedTrk
        bool locked{false}; //when locaked, the neighbor could not be added and the E_sub means the splitted energy not the neoghbor energy
        bool _isLocalMax{false}; //mark whether this is the maxima cell (obviously, is the localMaxima of P0_P1)
        bool error{false}; // mark fatal error
        bool _isSeeded{false};
        int _matchedTrk{-1}; 
}; 

// template<class CHit>
// bool Esorter_ascendingC( std::shared_ptr<CHit> a,  std::shared_ptr<CHit> b) { return (a->E() < b->E()); } // ascending
template<class CHit>
bool Esorter_descendingC( CHit* a,  CHit* b) { return (a->E() > b->E()); } // descending

typedef std::vector<CHit*> CHitVec;

#endif //DANA_CHIT_H