#ifndef TRACKING_CALIBRATOR_H
#define TRACKING_CALIBRATOR_H

#include <tuple>

namespace tracking
{

class Calibrator
{
public:
    Calibrator() {}
    virtual ~Calibrator() {}

    std::tuple<double, double> GetCalibratedP(double, double) { return std::make_tuple(0., 0.); }

protected:
};

}

#endif // TRACKING_CALIBRATOR_H
