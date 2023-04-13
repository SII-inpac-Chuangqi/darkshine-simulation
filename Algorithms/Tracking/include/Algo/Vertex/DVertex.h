#ifndef DVERTEX_H
#define DVERTEX_H

//................................................................................//
//C++
#include <vector>
#include <memory>

//................................................................................//
//Framework
#include "TObject.h"

//................................................................................//
//Tracking
#include "Algo/DTrack.h"

class DVertex : public TObject
{
public:
    DVertex()  {};
    ~DVertex() {};

private:
    double x_;
    double y_;
    double z_;

    std::vector<std::shared_ptr<DTrack>> tracks_;
};

#endif  // DVERTEX_H
