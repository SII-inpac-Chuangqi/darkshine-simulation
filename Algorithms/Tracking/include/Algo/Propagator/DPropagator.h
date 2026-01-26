//
// Created by xuliang on 2024/4/25.
//

#ifndef TRACKING_ACTS_DPROPAGATOR_H
#define TRACKING_ACTS_DPROPAGATOR_H

#include <iostream>

#include "TEveTrackPropagator.h"

#include "Algo/Propagator/Propagator.h"

class DPropagator final: public TEveTrackPropagator, public Propagator {
public:
    DPropagator(): TEveTrackPropagator() {};
    virtual ~DPropagator() override = default;

    virtual void Init([[maybe_unused]] std::shared_ptr<DTrack>& track) override
    {
//        InitTrack(TEveVectorD(), -1);
    }

    virtual bool ExtrapolateToPlane(const vector3D &mom_in, const vector3D &plane_pos, const vector3D &plane_normal,
                                    vector3D &mom_out, vector3D &pos_out) override
    {
        TEveVectorD mom_itsect;
        TEveVectorD pos_itsect;

        auto result = HelixIntersectPlane({mom_in.at(0), mom_in.at(1), mom_in.at(2)},
                                          {plane_pos.at(0), plane_pos.at(1), plane_pos.at(2)},
                                          {plane_normal.at(0), plane_normal.at(1), plane_normal.at(2)},
                                          mom_itsect, pos_itsect);

        if(!result)
        {
            std::cerr << "[WARNING] ==> Error detected in extrapolation at ("
                          << plane_pos.at(0) << ", " << plane_pos.at(0) << ", " << plane_pos.at(0) << ")"
                          << std::endl;

            mom_out = vector3D();
            pos_out = vector3D();

            return false;
        }

        mom_out = {mom_itsect[0], mom_itsect[1], mom_itsect[2]};
        pos_out = {pos_itsect[0], pos_itsect[1], pos_itsect[2]};

        return result;
    }

    virtual bool ExtrapolateToPlanes(const vector3D &mom_in, const std::vector<vector3D> &plane_poss, const std::vector<vector3D> &plane_normals,
                                     std::vector<vector3D> &mom_outs, std::vector<vector3D> &pos_outs) override
    {
        bool result = true;

        mom_outs.clear();
        pos_outs.clear();

        mom_outs.reserve(plane_pos.size());
        pos_outs.reserve(plane_pos.size());

        vector3D mom_out;
        vector3D pos_out;
        for(auto &&[plane_pos, plane_normal] : utils::make_zip(plane_poss, plane_normals))
        {
            auto result = ExtrapolateToPlane(mom_in, plane_pos, plane_normal, mom_out, pos_out);

            if(!result)
            {
                std::cerr << "[WARNING] ==> Error detected in extrapolation at ("
                          << plane_pos.at(0) << ", " << plane_pos.at(0) << ", " << plane_pos.at(0) << ")"
                          << std::endl;
                result = false;

                mom_outs.push_back(vector3D());
                pos_outs.push_back(vector3D());
                continue;
            }

            mom_outs.push_back(mom_out);
            pos_outs.push_back(pos_out);
        }

        return result;
    }

    bool HelixIntersectPlane(const TEveVectorD& mom_in, const TEveVectorD& plane_pos, const TEveVectorD& plane_normal,
                             TEveVectorD& mom_out, TEveVectorD& pos_out) 
    {
        TEveVectorD mom(mom_in);
        TEveVectorD pos(fV);
        if (fMagFieldObj->IsConst())
            fH.UpdateHelix(mom, fMagFieldObj->GetFieldD(pos), kFALSE, kFALSE);

        TEveVectorD n(plane_normal);
        TEveVectorD delta = pos - plane_pos;
        Double_t d = delta.Dot(n);
        if (d > 0) {
            n.NegateXYZ(); // Turn normal around so that we approach from negative side of the plane
            d = -d;
        }

        TEveVector4D forwV;
        TEveVectorD  forwP;
        TEveVector4D pos4(pos);
        while (kTRUE)
        {
            Update(pos4, mom);
            Step(pos4, mom, forwV , forwP);
            Double_t new_d = (forwV - plane_pos).Dot(n);
            if (new_d < d)
            {
                // We are going further away ... fail intersect.
//                Warning("HelixIntersectPlane", "going away from the plane.");
                return kFALSE;
            }
            if (new_d > 0)
            {
                delta  = forwV - pos4;
                pos_out = pos4 + delta * ((plane_pos - pos4).Dot(n) / delta.Dot(n));
                mom_out = forwP;
                return kTRUE;
            }
            pos4 = forwV;
            mom  = forwP;
        }
    }
};


#endif //TRACKING_ACTS_DPROPAGATOR_H
