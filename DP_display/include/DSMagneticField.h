//
// Created by Zhang Yulei on 12/20/20.
//

#ifndef DSIMU_DSMAGNETICFIELD_H
#define DSIMU_DSMAGNETICFIELD_H

#include <TEveTrackPropagator.h>
#include "TMath.h"

class DSMagneticField : public TEveMagField{

public:
    DSMagneticField():TEveMagField(){};
    ~DSMagneticField() override= default;
    using   TEveMagField::GetField;

    TEveVectorD GetFieldD(Double_t /*x*/, Double_t /*y*/, Double_t z) const override
    {
        if (TMath::Abs(z) >= -60.7825 && TMath::Abs(z) <= 18.0225)
            return TEveVectorD(0, -1.5, 0.);
//        else if (TMath::Abs(z) >= 0.7725 && TMath::Abs(z) <= 18.0225)
//            return TEveVectorD(0, -0.5, 0.);
        return TEveVectorD(0., 0., 0.);
    }

};

#endif //DSIMU_DSMAGNETICFIELD_H
