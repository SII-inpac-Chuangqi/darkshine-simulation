#ifndef DIGITIZATION_H
#define DIGITIZATION_H

#ifdef RM_UNIT
#define CUNIT 1
#else
#define CUNIT 10
#endif

#ifndef RETURN
#define RETURN std::nan("RETURN")
#endif

//................................................................................//
//C++
#include <map>
#include <vector>

//................................................................................//
//ROOT
//#include "TGeoNode.h"
#include "TRandom.h"

//................................................................................//
//Framework
#include "Object/SimulatedHit.h"

//................................................................................//
//Tracking
#include "Algo/TypeDef.h"
#include "Algo/TrkHit.h"

namespace tracking
{
    enum detector {tag = 1, rec = 2};
}

class Digitization
{
public:

    Digitization() = default;
    ~Digitization() = default;

    void SetIfSmear(bool if_smear);
//................................................................................//
//Get tracker geo information
    void ReadTrackerInfo(bool if_strip);
//................................................................................//
//Separate tracker hits into vectors by layers
    void Layering(const std::vector<TrkHit> &trk1_hits, const std::vector<TrkHit> &trk2_hits, TrkHitPVecMap &recoTrkHitMap,
                  int detector);
//................................................................................//
//Set strip cluster width
    void SetClusterWidth(double cluster_width) {cluster_width_ = cluster_width;}

    double GetClusterWidth() const {return cluster_width_;}

private:
//................................................................................//
//Struct to construct cluster
//    struct Point
//    {
//        double x{RETURN};
//        double w{RETURN};
//        bool cluster{false};
//    };

//................................................................................//
//Cluster
//    class Cluster
//    {
//    public:
//        ~Cluster()
//        {
//            delete center; center = nullptr;
//            //for(size_t i = 0; i < points.size(); i++)
//            //{delete points.at(i); points.at(i) = nullptr;}
//        }
//
//        double GetCenterX() const {return center->x;}
//
//        double GetNewCenterX()
//        {
//            double sum_of_w(0.);
//            double sum_of_x(0.);
//
//            for(const auto &point : points)
//            {
//                sum_of_w += point->w;
//                sum_of_x += point->w*point->x;
//            }
//
//            if(sum_of_w == 0.) return center->x;
//            return sum_of_x/sum_of_w;
//        }
//
//        void SetCenter(double x, double w)
//        {
//            if(center) {center->x = x; center->w = w;}
//            else
//            {
//                center = new Point({x, w});
//            }
//        }
//
//        Point *center = new Point();
//        std::vector<Point*> points;
//    };

    void InitHitMap(const TrkHitPVec &trk_hits, TrkHitPVecMap &trk_hit_map);
    void InsertHitMap(const TrkHitP &trk_hit, TrkHitPVecMap &trk_hit_map);

//    double GetClusterDistance(const Point &point1, const Point &point2) {return std::abs(point1.x - point2.x);}
//    int GetNextClusterSeed(const vector<Point> &points);
//    void MakeClusterPoints(const TrkHitPVec &trk_hits, std::vector<Point> &points);
//    void MakeCluster(std::vector<Point> &points, std::vector<Cluster*> &clusters);
//    void MergeCluster(std::vector<Cluster*> &clusters);

    bool if_strip_{true};
    bool if_smear_{true};

    double cluster_width_ = 0.2;

    std::vector<double> layer_width_tag_ = {};
    std::vector<double> layer_length_tag_ = {};
    std::vector<int>    strip_no_tag_ = {};
    std::vector<double> angles_tag_ = {};

    std::vector<double> layer_width_rec_ = {};
    std::vector<double> layer_length_rec_ = {};
    std::vector<int>    strip_no_rec_ = {};
    std::vector<double> angles_rec_ = {};

    TRandom rnd_;
};

#endif
