#include "Algo/Fitter.h"

int Fitter::GetSign(const TrkHitSPVec &track)
{
    double xl  = track.at(track.size() - 1)->GetU();
    double xlr = track.at(track.size() - 2)->GetU();
    double xr  = track.at(0)->GetU();
    double xrl = track.at(1)->GetU();

    double zl  = track.at(track.size() - 1)->GetZ();
    double zlr = track.at(track.size() - 2)->GetZ();
    double zr  = track.at(0)->GetZ();
    double zrl = track.at(1)->GetZ();

    if(zr < zl)
    {
        std::swap(xl,  xr );
        std::swap(xlr, xrl);
        std::swap(zl,  zr );
        std::swap(zlr, zrl);
    }

    int s = 0;
    s = (xr - xrl)/sqrt((xr - xrl)*(xr - xrl) + (zr - zrl)*(zr - zrl)) >
        (xlr - xl)/sqrt((xl - xlr)*(xl - xlr) + (zl - zlr)*(zl - zlr)) ? 1 : -1;
    return s;
}
