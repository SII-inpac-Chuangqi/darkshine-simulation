#ifndef FINDING_H
#define FINDING_H

//................................................................................//
//CPP STL
#include <utility>

//................................................................................//
//FRAMEWORK
#include "TMath.h"

//................................................................................//
//FRAMEWORK

//................................................................................//
//TRACKING
#include "Algo/TrkHit.h"

#define NUM 50

class Finding
{
public:
//................................................................................//
//Constructor
//................................................................................//
    Finding(TrkHitPVecMap &clusteredTrkHitsInLayer);
    ~Finding() {}

    Finding(const Finding&) = delete;
    Finding& operator =(const Finding&) = delete;

//................................................................................//
//Get
//................................................................................//
    double GetR       (int i) const {return r[i];       }
    double GetTheta   (int i) const {return theta[i];   }
    double GetCenterX (int i) const {return centerX[i]; }
    double GetCenterY (int i) const {return centerY[i]; }
    double GetGoodness(int i) const {return goodness[i];}
    int    GetTrackNo (     ) const {return circleNo;}
    std::vector<TrkHitPVec>::iterator Begin() {return VecHitChosen.begin();}
    std::vector<TrkHitPVec>::iterator End  () {return VecHitChosen.end();  }

private:
//................................................................................//
//Calculate deflection
//................................................................................//
    void Theta(int cirNo);

//................................................................................//
//Finding method
//................................................................................//
//Finding control
    void GreedyFinding(TrkHitPVecMap &clusteredTrkHitsInLayer);
    void GreedyFinding(TrkHitPVecMap &clusteredTrkHitsInLayer,
                       TrkHitPVecMap::iterator itMap,
                       int cirNo);

//................................................................................//
//Kasa method
    void MethodKasa(const std::vector<double> RecTrk2_x, const std::vector<double> RecTrk2_y);
    double rKasa = -999.;
    double centerXKasa = -999.;
    double centerYKasa = -999.;
    double goodnessKasa = -999.;

//................................................................................//
//Choice storage
//................................................................................//
//Global choice
    int circleNo = 0;
    double r[NUM] = {-999.};
    double theta[NUM] = {-999.};
    double centerX[NUM] = {-999.};
    double centerY[NUM] = {-999.};
    double goodness[NUM] = {-999.};
    std::vector<TrkHitPVec> VecHitChosen;

//................................................................................//
//Current choice
    TrkHitPVec hitChosen;
    std::vector<int> hitNoChosen;

//................................................................................//
//Temp Choice
    TrkHitPVec hitStore;
    std::vector<double> xStore;
    std::vector<double> yStore;
    std::vector<int> hitNoStore;
};

#endif

