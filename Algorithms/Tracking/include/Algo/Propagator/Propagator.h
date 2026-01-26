#ifndef TRACKING_PROPAGATOR_H
#define TRACKING_PROPAGATOR_H

#include <array>
#include <memory>

#include "Algo/TypeDef.h"

class DTrack;

class Propagator
{
protected:
    using vector3D = std::array<double, 3>;

public:
    Propagator() = default;
    virtual ~Propagator() = default;

    virtual void Init(std::shared_ptr<DTrack>&) = 0;
    virtual bool ExtrapolateToPlane(const vector3D &mom_in, const vector3D &plane_pos, const vector3D &plane_normal,
                                    vector3D &mom_out, vector3D &pos_out) = 0;

    virtual bool ExtrapolateToPlanes(const vector3D &mom_in, const std::vector<vector3D> &plane_pos, const std::vector<vector3D> &plane_normal,
                                     std::vector<vector3D> &mom_out, std::vector<vector3D> &pos_out) = 0;
    
protected:
    int verbose_;
    tracking::direction extrop_dir_;
};

#endif // TRACKING_PROPAGATOR_H
