#ifndef GREEDY_FINDER_H
#define GREEDY_FINDER_H

//................................................................................//
//CPP Libraries
#include <utility>
#include <limits>

//................................................................................//
//FRAMEWORK
#include "TMath.h"

//................................................................................//
//FRAMEWORK

//................................................................................//
//TRACKING
#include "Algo/TrkHit.h"
#include "Algo/Finder.h"

#ifndef RETURN
#define RETURN std::nan("RETURN")
#endif

//#ifndef NEGINF_DOUBLE
//#define NEGINF_DOUBLE -std::numeric_limits<double>::infinity()
//#endif

#ifndef MAX_CIRCLE
#define MAX_CIRCLE 50
#endif

class GreedyFinder : public Finder
{
public:
//................................................................................//
//Constructor
//................................................................................//
    GreedyFinder(TrkHitPVecMap &clusteredTrkHitsInLayer, int newMinDepth = 3, double newGoodnessCut = 0.99);
    ~GreedyFinder() {}

    GreedyFinder(const GreedyFinder&) = delete;
    GreedyFinder& operator =(const GreedyFinder&) = delete;

//................................................................................//
//Get
//................................................................................//
    virtual double GetR      (int i) const override {return r_.at(i);       }
    virtual double GetCenterX(int i) const override {return center_x_.at(i); } //x direction in detector!
    virtual double GetCenterY(int i) const override {return center_y_.at(i); } //z direction in detector!
    virtual double GetChi2   (int i) const override {return goodness_.at(i);}
    virtual int    GetTrackNo(     ) const override {return VecHitChosen.size();}
    virtual std::vector<TrkHitPVec>::iterator First() override {return VecHitChosen.begin();}
    virtual std::vector<TrkHitPVec>::iterator Last () override {return VecHitChosen.end();  }

    int GetCircleNo() const {return circleNo;}

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
    void MethodLooping(const std::vector<double> &track_x, const std::vector<double> &track_y,
                       double &cur_A, double &cur_B, double &cur_R, double &cur_goodness);
    double MethodKasa(const std::vector<double> &track_x, const std::vector<double> &track_y,
                      double &cur_A, double &cur_B, double &cur_R, double &cur_goodness);
    double r_Kasa_{RETURN};
    double center_x_Kasa_{RETURN};
    double center_y_Kasa_{RETURN};
    double goodness_Kasa_{-INFINITY};

//................................................................................//
//Choice storage
//................................................................................//
//Global choice
    int minDepth = 3;
    double goodnessCut = 0.99;

    int circleNo{0};
    std::vector<double> r_;
    std::vector<double> center_x_;
    std::vector<double> center_y_;
    std::vector<double> goodness_;
    //double r[NUM] = {RETURN};
    //double centerX[NUM] = {RETURN};
    //double centerY[NUM] = {RETURN};
    //double goodness[NUM] = {RETURN};
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
