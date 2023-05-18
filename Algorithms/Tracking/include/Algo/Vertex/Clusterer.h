#ifndef CLUSTERER_H
#define CLUSTERER_H

#ifndef RETURN
#define RETURN std::nan("RETURN")
#endif

//................................................................................//
//C++
#include <cmath>
#include <memory>
#include <vector>
#include <iostream>

//................................................................................//
//ROOT
#include "TString.h"

template <class T>
class Clusterer
{
public:
    Clusterer()  {}
    ~Clusterer()
    {
        for(size_t i = 0; i < clusters_.size(); i++) {delete clusters_.at(i); clusters_.at(i) = nullptr;}
        clusters_.clear();
    }

    void SetClusterWidth(double cluster_width) {cluster_width_ = cluster_width;}

    void CreatePoint(const std::shared_ptr<T> &init_object, size_t dim, double *splits, double weight);
    void ShowPoints();
    void FindClusters();
    std::vector<T> GetListOfClusteredObjects(int i) const;

private:
//................................................................................//
//class to hold the object
    class Point
    {
    public:
        Point() = delete;
        Point(size_t dim) : dim_(dim) {splits_ = new double[dim_];}
        Point(const Point &point) : dim_(point.dim_), weight_(point.weight_), t_(point.t_), if_clustered_(point.if_clustered_)
        {
            splits_ = new double[dim_];
            for(size_t i = 0; i < dim_; i++) splits_[i] = point.splits_[i];
        }
        ~Point() {delete[] splits_;}

        friend std::ostream &operator<<(std::ostream &os, const Point &point)
        {
            TString splits;
            for(size_t i = 0; i < point.dim_; i++)
                splits += TString::Format("%.2f\t", point.splits_[i]);
            auto str = TString::Format("object at %p, dim %ld, weight %.2f, splits %s\n", (void *)point.t_.get(), point.dim_, point.weight_, splits.Data());
            os << str;
            return os;
        }

        double Distance(const Point &point) const
        {
            if(dim_ != point.dim_)
            {
                std::cerr << "[WARNING] Point dimensions don't converge, " << point.dim_ << " provided, " << dim_ << " needed" << std::endl;
                return RETURN;
            }

            double dis = 0.;
            for(size_t i = 0; i < dim_; i++) dis += (splits_[i] - point.splits_[i])*(splits_[i] - point.splits_[i]);
            return std::sqrt(dis);
        }

        const size_t dim_;
        double weight_{0.};
        std::shared_ptr<T> t_{nullptr}; 
        double *splits_{nullptr};
        bool if_clustered_{false};
    };

//................................................................................//
//
    class Cluster
    {
    public:
        Cluster()  {}
        Cluster(const Cluster &cluster) : points_(cluster.points_)
        {
            center_ = std::make_unique<Point>(*(cluster.center_));
        }
        ~Cluster() {}

        double GetCenter(size_t i) const
        {
            if(!center_)
            {
                std::cerr << "[WARNING] ==> Center doesn't exist" << std::endl;
                return RETURN;
            }

            if(i < 0 || i >= center_->dim_)
            {
                std::cerr << "[Error] ==> n (which is " << i << ") >= this->size() (which is " << center_->dim_ << ")" << std::endl;
                return RETURN;
            }

            return center_->splits_[i];
        }

        void SetCenter(const Point &center)
        {
            center_.reset();
            center_ = std::make_unique<Point>(center);

            points_.clear();
        }

        void UpdateCenter()
        {
            if(points_.size() < 1)
            {
                std::cerr << "[WARNING] ==> No points in this cluster" << std::endl;
                return;
            }

            size_t dim = points_.at(0)->dim_;
            double sum_of_weights = 0.;
            double *sum_of_splits = new double[dim];

            for(const auto &point : points_)
            {
                sum_of_weights += point->weight_;

                for(size_t i = 0; i < dim; i++)
                    sum_of_splits[i] += point->weight_*point->splits_[i];
            }

            center_.reset();
            center_ = std::make_unique<Point>(dim);
            for(size_t i = 0; i < dim; i++)
                center_->splits_[i] = sum_of_splits[i];
        }

        void AddPoint(const std::shared_ptr<Point> &point, bool if_update = true)
        {
            points_.push_back(point);

            if(!if_update) return;
            this->UpdateCenter();
        }

        std::unique_ptr<Point> center_{nullptr};
        std::vector<std::shared_ptr<Point>> points_;
    };

//................................................................................//
//Get cluster seed
    int GetNextSeed() const;

//................................................................................//
    std::vector<std::shared_ptr<Point>> points_;
    std::vector<Cluster*>               clusters_;
    double max_shift_{3e-2};
    double cluster_width_{10.};
};

template <class T>
void Clusterer<T>::CreatePoint(const std::shared_ptr<T> &init_object, size_t dim, double *splits, double weight)
{
    points_.push_back(std::make_shared<Point>(dim));
    points_.back()->weight_ = weight;
    points_.back()->t_ = init_object;
    for(size_t i = 0; i < dim; i++) points_.back()->splits_[i] = splits[i];
}

template <class T>
void Clusterer<T>::ShowPoints()
{
    int i = 0;
    for(const auto &point : points_) std::cout << "point " << i++ << "\n" << *point << std::endl;
}

template <class T>
void Clusterer<T>::FindClusters()
{
    int n_lonely_point(points_.size());
    int seed = this->GetNextSeed();

    while(n_lonely_point > 0 && seed >= 0)
    {
        auto cluster = new Cluster();
        cluster->SetCenter(*(points_.at(seed)));

        double shift(INFINITY);

        while(std::abs(shift) > max_shift_)
        {
            auto last_center = new Point(*(cluster->center_));

            for(auto &point : points_)
            {
                if(point->if_clustered_ || cluster->center_->Distance(*point) >= cluster_width_)
                    continue;

                cluster->AddPoint(point, false);
                point->if_clustered_ = true;
                n_lonely_point--;
            }

            cluster->UpdateCenter();
            shift = last_center->Distance(*(cluster->center_));

            delete last_center; last_center = nullptr;
        }

        clusters_.push_back(cluster);

        seed = this->GetNextSeed();
        //std::cout << "seed: " << seed << "\tlonely: " << n_lonely_point << std::endl;
    }

    std::cout << "cluster size: " << clusters_.size() << std::endl;
}

template <class T>
int Clusterer<T>::GetNextSeed() const
{
    for(auto point = points_.begin(); point != points_.end(); ++point)
    {
        if(!(*point)->if_clustered_) return point - points_.begin();
    }

    return -1;
}

template <class T>
std::vector<T> Clusterer<T>::GetListOfClusteredObjects(int i) const
{
    std::vector<T> list;

    for(const auto &point : clusters_.at(i)->points_)
        list.push_back(point);

    return list;
}

#endif // CLUSTERER_H
