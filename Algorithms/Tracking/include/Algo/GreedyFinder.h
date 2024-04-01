#ifndef GREEDY_FINDER_H
#define GREEDY_FINDER_H

//................................................................................//
//C++
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
#define MAX_CIRCLE 1000000 // 10^7, (max n hit)^(n layers)
#endif

class GreedyFinder : public Finder
{
public:
//................................................................................//
//Constructor
//................................................................................//
    GreedyFinder(Pool *pool, int newMinDepth = 3, double newGoodnessCut = 0.999);
    ~GreedyFinder() {}

    GreedyFinder(const GreedyFinder&) = delete;
    GreedyFinder& operator =(const GreedyFinder&) = delete;

//................................................................................//
//Get
//................................................................................//
    virtual void FillTracks(std::vector<std::shared_ptr<DTrack>> *tracks) override;
    std::vector<std::shared_ptr<DTrack>>* GetTracks();

    virtual double GetR      (int i) const override {return r_.at(i);       }
    virtual double GetCenterX(int i) const override {return center_x_.at(i); } //x direction in detector!
    virtual double GetCenterY(int i) const override {return center_y_.at(i); } //z direction in detector!
    virtual double GetChi2   (int i) const override {return goodness_.at(i);}
    virtual int    GetTrackNo(     ) const override {return tracks_chosen_.size();}
    virtual std::vector<TrkHitSPVec>::iterator First() override {return tracks_chosen_.begin();}
    virtual std::vector<TrkHitSPVec>::iterator Last () override {return tracks_chosen_.end();  }

    int GetCircleNo() const {return circle_No_;}

private:
//................................................................................//
//Calculate deflection
//................................................................................//

//................................................................................//
//Finding method
//................................................................................//
//Finding control
//................................................................................//
    struct BadGuy
    {
        size_t id;
        size_t how_bad;
    };

    void GreedyLooping(Pool *pool);
    TrkHitSPVecMap GetTempHitMap(Pool *pool);

    bool GreedyLooping(TrkHitSPVecMap &clusteredTrkHitsInLayer,
                       TrkHitSPVecMap::iterator itMap,
                       int cirNo);
    void CutTracks();
    void SortHits();

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
//Final choice
    int min_depth_ = 3;
    double goodness_cut_ = 0.999;

    int circle_No_{0};
    std::vector<double> r_;
    std::vector<double> center_x_;
    std::vector<double> center_y_;
    std::vector<double> goodness_;
    std::vector<TrkHitSPVec> tracks_chosen_;

//................................................................................//
//Current choice
    TrkHitSPVec hits_chosen_;
    std::vector<int> hits_no_chosen_;

//................................................................................//
//Temp Choice
    void StoredPushBack(double x, double y, double oth, int hits_no, const TrkHitSP &hit)
    {
        hits_store_.push_back(hit);
        x_store_.push_back(x);
        y_store_.push_back(y);
        oth_store_.push_back(oth);
        hits_no_store_.push_back(hits_no);
    }

    void StoredEraseEnd()
    {
        hits_store_.erase(hits_store_.end() - 1);
        x_store_.erase(x_store_.end() - 1);
        y_store_.erase(y_store_.end() - 1);
        oth_store_.erase(oth_store_.end() - 1);
        hits_no_store_.erase(hits_no_store_.end() - 1);
    }

    void StoredClear()
    {
        hits_store_.clear();
        x_store_.clear();
        y_store_.clear();
        oth_store_.clear();
        hits_no_store_.clear();
    }

    TrkHitSPVec hits_store_;
    std::vector<double> x_store_;
    std::vector<double> y_store_;
    std::vector<double> oth_store_;
    std::vector<int> hits_no_store_;
};

#endif
