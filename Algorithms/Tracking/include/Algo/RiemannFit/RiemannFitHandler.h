#ifndef RIEMANNFIT_RIEMANN_FIT_HANDLER_H
#define RIEMANNFIT_RIEMANN_FIT_HANDLER_H

#include "Algo/RiemannFit/RiemannFitting.h"

class RiemannFitHandler
{
public:
    RiemannFitHandler(const RiemannFitHandler&) = delete;
    RiemannFitHandler &operator=(RiemannFitHandler const&) = delete;

    static RiemannFitHandler *CreateInstance();

    void SetVerbose(int verbose) {verbose_ = verbose;}

private:
    RiemannFitHandler();

    int verbose_{0};
};

extern RiemannFitHandler *dRFitHandler;

#endif
