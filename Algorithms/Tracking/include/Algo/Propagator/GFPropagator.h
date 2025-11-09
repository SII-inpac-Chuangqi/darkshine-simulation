#ifndef TRACKING_GFPROPAGATOR_H
#define TRACKING_GFPROPAGATOR_H

#include "Propagator/Propagator.h"

class GFPropagator final : public Propagator
{
private:
    using vector3D = std::array<double, 3>;

public:
    GFPropagator() = default;
    ~GFPropagator() = default;

    virtual void Init(std::shared_ptr<DTrack>&) override;
    virtual bool ExtrapolateToPlane(const vector3D &mom_in, const vector3D &plane_pos, const vector3D &plane_normal,
                                    vector3D &mom_out, vector3D &pos_out) override;

    bool ExtroplateToPlaneWithExistingRep();
private:

};

#endif // TRACKING_GFPROPAGATOR_H
