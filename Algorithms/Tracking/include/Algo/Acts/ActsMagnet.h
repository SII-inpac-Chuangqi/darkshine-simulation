#if BUILD_ACTS

#ifndef TRACKING_ACTS_MAGNET_H
#define TRACKING_ACTS_MAGNET_H

#include "Utility/Units.h"
#include "Core/AnaData.h"
#include "Object/DMagnet.h"
#include "Algo/Acts/ActsHelper.h"
#include <TEveTrackPropagator.h>

#include "Acts/Definitions/Algebra.hpp"
#include "Acts/Definitions/Units.hpp"
#include "Acts/MagneticField/MagneticFieldProvider.hpp"
#include "Acts/MagneticField/MagneticFieldContext.hpp"

class DMagnetField final : public Acts::MagneticFieldProvider
{
public:
    struct Cache
    {
        Cache(const Acts::MagneticFieldContext&)
        {
        }
    };

    DMagnetField() = default;
    

    Acts::MagneticFieldProvider::Cache makeCache(const Acts::MagneticFieldContext& mctx) const override
    {
        return Acts::MagneticFieldProvider::Cache::make<Cache>(mctx);
    }

    Acts::Result<Acts::Vector3> getField(const Acts::Vector3& position, Acts::MagneticFieldProvider::Cache&) const override
    {
        return Acts::Result<Acts::Vector3>::success(getField(position));
    }

    Acts::Vector3 getField(const Acts::Vector3& position) const
    {
        using namespace ActsHelper;

        Acts::Vector3 position_dss(fromActsReferenceFrameV3(position));
        auto field = dAnaData->getMagnetFieldAt({position_dss(0), position_dss(1), position_dss(2)});
        return toActsReferenceFrameV3(Acts::Vector3({field.at(0),
                                                     field.at(1),
                                                     field.at(2)}))*Acts::UnitConstants::T;
    }

    Acts::Result<Acts::Vector3> getFieldGradient(const Acts::Vector3& position, Acts::ActsMatrix<3, 3>&,
                                                 Acts::MagneticFieldProvider::Cache&) const override
    {
        return Acts::Result<Acts::Vector3>::success(getField(position));
    }
    
};

class DActsTEveMagField : public TEveMagField{

public:
    DActsTEveMagField():TEveMagField(){};
    ~DActsTEveMagField() override= default;

    Double_t GetMaxFieldMagD() const override {
        return -1.5; // FIXME: read from dmagnets
    }

    TEveVectorD GetFieldD(Double_t x, Double_t y, Double_t z) const override
    {
        using namespace dunits;
        using namespace ActsHelper;

        auto pos_dss = fromActsReferenceFrameV3(Acts::Vector3(x * root_to_dss::ul, y * root_to_dss::ul, z * root_to_dss::ul)); // ACTS has same default length as DSS
        auto field_dss = dAnaData->getMagnetFieldAt({pos_dss.x(), pos_dss.y(), pos_dss.z()});
        auto filed_acts = ActsHelper::toActsReferenceFrameV3(Acts::Vector3(field_dss.at(0), field_dss.at(1), field_dss.at(2)));

        return TEveVectorD(-filed_acts.x(), -filed_acts.y(), -filed_acts.z()); // No unit change here
    }
};

#endif // TRACKING_ACTS_MAGNET_H

#endif
