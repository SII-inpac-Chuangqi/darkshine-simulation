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

template <typename T>
class Clusterer
{
public:
    Clusterer()  {}
    ~Clusterer() {}

private:
    class Pointer
    {
    public:
        Pointer() = delete;
        Pointer(int dim) : dim_(dim) {splits_ = new double[dim_];}
        Pointer(const Pointer &node) : dim_(node.dim_), t(node.t)
        {
            splits_ = new double[dim_];
            for(int i = 0; i < dim_; i++) splits_[i] = node.splits_[i];
        }
        ~Pointer() {delete[] splits_;}

        const int dim_;
        std::shared_ptr<T> t{nullptr}; 
        double *splits_{nullptr};
    };

    class Cluster
    {
    public:
        Cluster()  {}
        ~Cluster() {}

        double GetCenter(int i)
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

        std::unique_ptr<Pointer> *center_{nullptr};
        std::vector<std::shared_ptr<Pointer>> points_;
    };
};

#endif // CLUSTERER_H
