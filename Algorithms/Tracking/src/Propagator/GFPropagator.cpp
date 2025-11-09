#include "Algo/Propagator/GFPropagator.h"

#include <vector>
#include <cmath>

#include "KalmanFittedStateOnPlane.h"
#include "KalmanFitterInfo.h"

#include "Utility/Units.h"

#include "Algo/TypeDef.h"
#include "Algo/KalmanFit/GenFitInclude.h"

#ifndef RETURN
#define RETURN std::nan("RETURN")
#endif

void GFPropagator::Init(std::shared_ptr<DTrack>&)
{
}

bool GFPropagator::ExtrapolateToPlane(const vector3D &mom_in, const vector3D &plane_pos, const vector3D &plane_normal,
                                      vector3D &mom_out, vector3D &pos_out)
{
    return false;
}

bool GFPropagator::ExtroplateToPlaneWithExistingRep(const std::vector<double> &planes_z,
                                                    genfit::Track *fit_track, genfit::AbsTrackRep *rep)
{
    using namespace dunits;

    if(!rep || !fit_track)
    {
        if(verbose_ > 0)
            std::cerr << "[WARNING] ==> No hits to extrapolate" << std::endl;

        return false;
    }

    std::vector<double> extrapolated;
    for(const auto &plane_z : planes_z)
    {
        genfit::TrackPoint* tp = fit_track->getPointWithMeasurementAndFitterInfo(0, rep);
        genfit::KalmanFittedStateOnPlane kfsop(*(static_cast<genfit::KalmanFitterInfo*>(tp->getFitterInfo(rep))->getBackwardUpdate()));
        genfit::SharedPlanePtr plane(new genfit::DetPlane(TVector3(0.,
                                                                   0.,
                                                                   plane_z * dss_to_genfit::mm),
                                                          TVector3(1, 0, 0),
                                                          TVector3(0, 1, 0)));

        try
        {
            rep->extrapolateToPlane(kfsop, plane);
            const TVectorD& state = kfsop.getState();
            if     (extrop_dir_ == tracking::dX) extrapolated.push_back(state[3]*10);
            else if(extrop_dir_ == tracking::dY) extrapolated.push_back(state[4]*10);
        }
        catch(genfit::Exception& e)
        {
            if(verbose_ > 1)
            {
                std::cerr << "[WARNING] ==> When extrapolating hits at z=" << plane_z << "mm:" << std::endl;
                std::cerr << "              " << e.what();
            }

            if     (extrop_dir_ == tracking::dX) extrapolated.push_back(RETURN);
            else if(extrop_dir_ == tracking::dY) extrapolated.push_back(RETURN);
        }
    }

    return false;
}
