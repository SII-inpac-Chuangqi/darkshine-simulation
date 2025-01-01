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

    template <class ... Args>
    void Run(KalmanFilterFitter::Config config, Args&& ... args)
    {
        fitter_ = new KalmanFilterFitter(config, std::forward<Args>(args) ...);
    }

    template <class ... Args>
    void Run(RiemannFitter::Config config, Args&& ... args)
    {
        fitter_ = new RiemannFitter(config, std::forward<Args>(args) ...);
    }

private:
    Fitter *fitter_{nullptr};
};

#endif // TRACKING_WRAPPEDFITTER_H
