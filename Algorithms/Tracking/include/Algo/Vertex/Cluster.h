#ifndef CLUSTER_H
#define CLUSTER_H

//................................................................................//
//C++
#include <memory>

template <typename T>
class Cluster
{
public:
    Cluster()  {}
    ~Cluster() {}

private:
    class Node
    {
    public:
        Node() = delete;
        Node(int dim) : dim_(dim) {splits_ = new double[dim_];}
        Node(const Node &node) : dim_(node.dim_), t(node.t)
        {splits_ = new double[dim_]; for(int i = 0; i < dim_; i++) splits_[i] = node.splits_[i];}
        ~Node() {delete[] splits_;}

        const int dim_;
        std::shared_ptr<T> t{nullptr}; 
        double *splits_{nullptr};
    };
};

#endif // CLUSTER_H
