#ifndef TRACKING_NULLCALIBRATOR_H
#define TRACKING_NULLCALIBRATOR_H

#include "Algo/Calibrator/Calibrator.h"

namespace tracking
{

class NullCalibrator : public Calibrator
{
public:
    virtual ~NullCalibrator() {}
    std::tuple<double, double> GetCalibratedP(double pp, double pl) { return std::make_tuple(pp, pl); }

private:
};

}

#endif // TRACKING_NULLCALIBRATOR_H
