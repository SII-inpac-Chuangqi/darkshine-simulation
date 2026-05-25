#ifndef TRACKING_WRAPPEDFITTER_H
#define TRACKING_WRAPPEDFITTER_H

// Tracking
#include "Algo/TypeDef.h"
#include <memory>
#include "Algo/KalmanFit/KalmanFilterFitter.h"
#include "Algo/RiemannFit/RiemannFitter.h"
#include "Algo/Propagator/GFPropagator.h"

class WrappedFitter
{
public:
    WrappedFitter() = default;
    ~WrappedFitter() = default;

    template <class ... Args_t>
    void Run(KalmanFilterFitter::Config config, Args_t&& ... args)
    {
        method = tracking::dKalman;
        fitter_ = std::make_unique<KalmanFilterFitter>(config, std::forward<Args_t>(args) ...);
    }

    template <class ... Args_t>
    void Run(RiemannFitter::Config config, Args_t&& ... args)
    {
        method = tracking::dRiemann;
        fitter_ = std::make_unique<RiemannFitter>(config, std::forward<Args_t>(args) ...);
    }

    Fitter* Retrieve()
    {
        return fitter_.get();
    }

private:
    tracking::FitMethods method;

    std::unique_ptr<Fitter> fitter_;
};

#endif // TRACKING_WRAPPEDFITTER_H
