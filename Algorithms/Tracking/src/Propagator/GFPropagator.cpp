#include "Algo/Propagator/GFPropagator.h"

#include "Algo/KalmanFit/GenFitInclude.h"

void GFPropagator::Init(std::shared_ptr<DTrack>&)
{
}

bool GFPropagator::ExtrapolateToPlane(const vector3D &mom_in, const vector3D &plane_pos, const vector3D &plane_normal,
                                      vector3D &mom_out, vector3D &pos_out)
{
    return false;
}
