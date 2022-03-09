#ifndef GREEDY_FINDING_H
#define GREEDY_FINDING_H

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
#include "Algo/Finding.h"

#define NUM 50

class GreedyFinding : public Finding
{
public:
//................................................................................//
//Constructor
//................................................................................//
    GreedyFinding(TrkHitPVecMap &clusteredTrkHitsInLayer);
    ~GreedyFinding() {}

    GreedyFinding(const GreedyFinding&) = delete;
    GreedyFinding& operator =(const GreedyFinding&) = delete;

//................................................................................//
//Get
//................................................................................//
    virtual double GetR      (int i) const override {return r[i];       }
    virtual double GetCenterX(int i) const override {return centerX[i]; } //x direction in detector!
    virtual double GetCenterY(int i) const override {return centerY[i]; } //z direction in detector!
    virtual double GetChi2   (int i) const override {return goodness[i];}
    virtual int    GetTrackNo(     ) const override {return circleNo;   }
    virtual std::vector<TrkHitPVec>::iterator First() override {return VecHitChosen.begin();}
    virtual std::vector<TrkHitPVec>::iterator Last () override {return VecHitChosen.end();  }

private:
//................................................................................//
//Calculate deflection
//................................................................................//

//................................................................................//
//Finding method
//................................................................................//
//Finding control
    void GreedyLooping(TrkHitPVecMap &clusteredTrkHitsInLayer);
    void GreedyLooping(TrkHitPVecMap &clusteredTrkHitsInLayer,
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
