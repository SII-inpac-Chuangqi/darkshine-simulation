#ifndef TRACKING_PROPAGATOR_H
#define TRACKING_PROPAGATOR_H

#include <array>

class Propagator
{
private:
    using vector3D = std::array<double, 3>;

public:
    Propagator() = default;
    virtual ~Propagator() = default;

    virtual void Init() = 0;
    virtual bool ExtrapolateToPlane(const vector3D &mom_in, const vector3D &plane_pos, const vector3D &plane_normal,
                                    vector3D &mom_out, vector3D &pos_out) = 0;
};

#endif // TRACKING_PROPAGATOR_H
