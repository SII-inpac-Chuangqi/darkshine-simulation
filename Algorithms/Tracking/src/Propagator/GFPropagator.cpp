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

bool GFPropagator::ExtrapolateToPlane([[maybe_unused]] const vector3D &mom_in, [[maybe_unused]] const vector3D &plane_pos, [[maybe_unused]] const vector3D &plane_normal,
                                      [[maybe_unused]] vector3D &mom_out, [[maybe_unused]] vector3D &pos_out)
{
    return false;
}

bool GFPropagator::ExtroplateToPlanesWithExistingRep(const std::vector<double> &planes_z,
                                                     genfit::Track *fit_track, genfit::AbsTrackRep *rep,
                                                     std::vector<vector3D> &mom_outs, std::vector<vector3D> &pos_outs)
{
    using namespace dunits;

    mom_outs.clear();
    pos_outs.clear();
    mom_outs.reserve(planes_z.size());
    pos_outs.reserve(planes_z.size());

    if(!rep || !fit_track)
    {
        if(verbose_ > 0)
            std::cerr << "[WARNING] ==> No hits to extrapolate" << std::endl;

        return false;
    }

    genfit::TrackPoint* tp = fit_track->getPointWithMeasurementAndFitterInfo(0, rep);
    genfit::KalmanFittedStateOnPlane kfsop(*(static_cast<genfit::KalmanFitterInfo*>(tp->getFitterInfo(rep))->getBackwardUpdate()));

    std::vector<double> extrapolated;
    for(const auto &plane_z : planes_z)
    {
        genfit::SharedPlanePtr plane(new genfit::DetPlane(TVector3(0.,
                                                                   0.,
                                                                   plane_z * dss_to_genfit::mm),
                                                          TVector3(1, 0, 0),
                                                          TVector3(0, 1, 0)));

        try
        {
            rep->extrapolateToPlane(kfsop, plane);
            const auto &mom = kfsop.getMom();
            const auto &state = kfsop.getState();

            mom_outs.push_back({-mom[0]*genfit_to_dss::GeV, mom[1]*genfit_to_dss::GeV, -mom[2]*genfit_to_dss::GeV});
            pos_outs.push_back({state[3]*genfit_to_dss::cm, state[4]*genfit_to_dss::cm, plane_z});
            //pflow_qop_ = 1./state[0]*1000.;
            //pflow_dirct_x_ = state[1];
            //pflow_dirct_y_ = state[2];
        }
        catch(genfit::Exception& e)
        {
            if(verbose_ > 1)
            {
                std::cerr << "[WARNING] ==> When extrapolating hits at z=" << plane_z << "mm:" << std::endl;
                std::cerr << "              " << e.what();
            }

            mom_outs.push_back({RETURN, RETURN, RETURN});
            pos_outs.push_back({RETURN, RETURN, plane_z});
        }
    }

    return true;
}

bool GFPropagator::ExtrapolateToPlanes([[maybe_unused]] const vector3D &mom_in, [[maybe_unused]] const std::vector<vector3D> &plane_poss, [[maybe_unused]] const std::vector<vector3D> &plane_normals,
                                       [[maybe_unused]] std::vector<vector3D> &mom_outs, [[maybe_unused]] std::vector<vector3D> &pos_outs)
{
    return false;
}
