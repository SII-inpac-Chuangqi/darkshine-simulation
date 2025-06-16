#ifndef TRACKING_WRAPPEDFITTER_H
#define TRACKING_WRAPPEDFITTER_H

#include "Algo/TypeDef.h"
#include "Algo/KalmanFit/KalmanFilterFitter.h"
#include "Algo/RiemannFit/RiemannFitter.h"

class WrappedFitter
{
public:
    WrappedFitter() = default;
    ~WrappedFitter() {delete fitter_; fitter_ = nullptr;};

    template <class ... Args_t>
    void Run(KalmanFilterFitter::Config config, Args_t&& ... args)
    {
        if (fitter_) delete fitter_;

        method = tracking::dKalman;
        fitter_ = new KalmanFilterFitter(config, std::forward<Args_t>(args) ...);
    }

    template <class ... Args_t>
    void Run(RiemannFitter::Config config, Args_t&& ... args)
    {
        if (fitter_) delete fitter_;

        method = tracking::dRiemann;
        fitter_ = new RiemannFitter(config, std::forward<Args_t>(args) ...);
    }

    Fitter* Retrieve()
    {
        return fitter_;
    }

private:
    tracking::FitMethods method;

    Fitter *fitter_{nullptr};
};

#endif // TRACKING_WRAPPEDFITTER_H
