#ifndef DVERTEX_H
#define DVERTEX_H

//................................................................................//
//C++
#include <vector>
#include <memory>
#include <algorithm>

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

    void AddTrack(const std::shared_ptr<DTrack> &track)
    {
        if(std::find(tracks_.begin(), tracks_.end(), track) == tracks_.end())
            tracks_.push_back(track);
    }

    void SetZ(double z) {z_ = z;}

    double GetZ() const {return z_;}

private:
    [[maybe_unused]] double x_;
    [[maybe_unused]] double y_;
    double z_;

    std::vector<std::shared_ptr<DTrack>> tracks_;
};

#endif // DVERTEX_H
