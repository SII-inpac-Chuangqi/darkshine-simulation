#ifndef TRACKING_GFPROPAGATOR_H
#define TRACKING_GFPROPAGATOR_H

#include <vector>

#include "Propagator/Propagator.h"

namespace genfit
{
class Track;
class AbsTrackRep;
}

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

    bool ExtroplateToPlaneWithExistingRep(const std::vector<double> &planes_z, genfit::Track *fit_track, genfit::AbsTrackRep *rep);

private:
    
};

#endif // TRACKING_GFPROPAGATOR_H
