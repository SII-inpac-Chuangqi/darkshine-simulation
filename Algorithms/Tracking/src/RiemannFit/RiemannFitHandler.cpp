#include "Algo/RiemannFit/RiemannFitHandler.h"

RiemannFitHandler *dRFitHandler = nullptr;

RiemannFitHandler *RiemannFitHandler::CreateInstance()
{
    if (dRFitHandler == nullptr)
        dRFitHandler = new RiemannFitHandler();
    return dRFitHandler;
}

RiemannFitHandler::RiemannFitHandler()
{
}
