//
// Created by Zhang Yulei on 12/20/20.
//

#ifndef DSIMU_DSMAGNETICFIELD_H
#define DSIMU_DSMAGNETICFIELD_H

#include <TEveTrackPropagator.h>
#include "TMath.h"
#include "TEveVector.h"

#include "DisData.h"

class DSMagneticField : public TEveMagField{

public:
    DSMagneticField():TEveMagField(){}; //wrapper of dDisData Magnetes
    ~DSMagneticField() override= default;

    Double_t GetMaxFieldMagD() const override { 
        return -1.5; // FIXME: read from dmagnets
    }

    TEveVectorD GetFieldD(Double_t x_cm, Double_t y_cm, Double_t z_cm) const override //cm since it is based on TEve
    {
        // all the length inside DSimu/DAna/DDis will be mm
        auto x = x_cm * CUNIT;
        auto y = y_cm * CUNIT;
        auto z = z_cm * CUNIT;
        // std::cout<<"Checking By... "<<z<<std::endl;
        if (dDisData->isInBfieldRegion(x,y,z)) // TODO: move into dDisData
        {
            // std::cout<<"Got region By... "<<dDisData->GetMagnetYAt(x, y, z)<<std::endl;
            // note! it is inverted
            return TEveVectorD(-1 * dDisData->GetMagnetXAt(x, y, z),
                               -1 * dDisData->GetMagnetYAt(x, y, z),
                               -1 * dDisData->GetMagnetZAt(x, y, z));
        }
        // std::cout<<"Got By... "<<0<<std::endl;
        return TEveVectorD(0., 0., 0.);
    }
};

#endif //DSIMU_DSMAGNETICFIELD_H
