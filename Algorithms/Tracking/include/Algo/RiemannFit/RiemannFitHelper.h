#ifndef RIEMANNFIT_RIEMANN_FIT_HELPER_H
#define RIEMANNFIT_RIEMANN_FIT_HELPER_H

#include "Algo/RiemannFit/RiemannFitting.h"

class RiemannFitHelper
{
public:
    RiemannFitHelper(const RiemannFitHelper&) = delete;
    RiemannFitHelper &operator=(RiemannFitHelper const&) = delete;

    //static RiemannFitHelper *CreateInstance();

    static void SetVerbose(int verbose) {verbose_ = verbose;}

private:
    RiemannFitHelper();

    static int verbose_;
};

//extern RiemannFitHelper *dRFitHelper;

#endif
