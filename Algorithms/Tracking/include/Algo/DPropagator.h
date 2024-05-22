//
// Created by xuliang on 2024/4/25.
//

#ifndef TRACKING_ACTS_DPROPAGATOR_H
#define TRACKING_ACTS_DPROPAGATOR_H

#include "TEveTrackPropagator.h"

class DPropagator: public TEveTrackPropagator {
public:
    DPropagator(): TEveTrackPropagator(){};
    ~DPropagator() override=default;

    Bool_t HelixIntersectPlane(const TEveVectorD& p, const TEveVectorD& point, const TEveVectorD& normal,
                               TEveVectorD& itsect_pos, TEveVectorD& itsect_mom) {
        TEveVectorD pos(fV);
        TEveVectorD mom(p);
        if (fMagFieldObj->IsConst())
            fH.UpdateHelix(mom, fMagFieldObj->GetFieldD(pos), kFALSE, kFALSE);

        TEveVectorD n(normal);
        TEveVectorD delta = pos - point;
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
            Double_t new_d = (forwV - point).Dot(n);
            if (new_d < d)
            {
                // We are going further away ... fail intersect.
//                Warning("HelixIntersectPlane", "going away from the plane.");
                return kFALSE;
            }
            if (new_d > 0)
            {
                delta  = forwV - pos4;
                itsect_pos = pos4 + delta * ((point - pos4).Dot(n) / delta.Dot(n));
                itsect_mom = forwP;
                return kTRUE;
            }
            pos4 = forwV;
            mom  = forwP;
        }
    }
};


#endif //TRACKING_ACTS_DPROPAGATOR_H
