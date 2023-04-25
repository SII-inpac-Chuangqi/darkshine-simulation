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

template <class T>
class Clusterer
{
public:
    Clusterer()  {}
    ~Clusterer() {}

    void CreatePoint(const std::shared_ptr<T> &init_object, size_t dim, double *splits, double weight);

private:
    class Point
    {
    public:
        Point() = delete;
        Point(size_t dim) : dim_(dim) {splits_ = new double[dim_];}
        Point(const Point &point) : dim_(point.dim_), weight_(point.weight_), t_(point.t_)
        {
            splits_ = new double[dim_];
            for(size_t i = 0; i < dim_; i++) splits_[i] = point.splits_[i];
        }
        ~Point() {delete[] splits_;}

        const size_t dim_;
        double weight_{0.};
        std::shared_ptr<T> t_{nullptr}; 
        double *splits_{nullptr};
    };

    class Cluster
    {
    public:
        Cluster()  {}
        Cluster(const Cluster &cluster) : points_(cluster.points_)
        {
            center_ = std::make_unique<Point>(*(cluster.center_));
        }
        ~Cluster() {}

        double GetCenter(size_t i)
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

        void UpdateCenter()
        {
            if(points_.size() < 1)
            {
                std::cerr << "[WARNING] ==> No points in thiss cluster" << std::endl;
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

        std::unique_ptr<Point> center_{nullptr};
        std::vector<std::shared_ptr<Point>> points_;
    };

    std::vector<std::shared_ptr<Point>> points_;
};

template <class T>
void Clusterer<T>::CreatePoint(const std::shared_ptr<T> &init_object, size_t dim, double *splits, double weight)
{
    points_.push_back(std::make_shared<Point>(dim));
    points_.back()->weight_ = weight;
    points_.back()->t_ = init_object;
    for(size_t i = 0; i < dim; i++) points_.back()->splits_[i] = splits[i];
}

#endif // CLUSTERER_H
