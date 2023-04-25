//................................................................................//
//Tracking
/*
#include "Algo/Vertex/Clusterer.h"
#include "Algo/DTrack.h"

template <class T>
void Clusterer<T>::CreatePoint(const std::shared_ptr<T> &init_object, size_t dim, double *splits, double weight)
{
    points_.push_back(std::make_shared<Point>(dim));
    points_.back()->weight_ = weight;
    points_.back()->t_ = init_object;
    for(size_t i = 0; i < dim; i++) points_.back()->splits_[i] = splits[i];
}

template class Clusterer<DTrack>;
*/
