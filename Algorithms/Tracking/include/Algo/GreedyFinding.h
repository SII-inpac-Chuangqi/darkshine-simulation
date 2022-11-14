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
    GreedyFinding(TrkHitPVecMap &clusteredTrkHitsInLayer, int newMinDepth = 3, double newGoodnessCut = 0.99);
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
    void MethodLooping(const std::vector<double> &track_x, const std::vector<double> &track_y);
    double MethodKasa(const std::vector<double> &track_x, const std::vector<double> &track_y, const double &cur_A, const double &cur_B);
    double rKasa = -999.;
    double centerXKasa = -999.;
    double centerYKasa = -999.;
    double goodnessKasa = -999.;

//................................................................................//
//Choice storage
//................................................................................//
//Global choice
    int minDepth = 3;
    double goodnessCut = 0.99;

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
