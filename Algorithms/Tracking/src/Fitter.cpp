#include "Algo/Fitter.h"

#include "Algo/Object/DTrack.h"

int Fitter::GetSign(const TrkHitSPVec &track)
{
    double xl  = track.at(track.size() - 1)->GetX();
    double xlr = track.at(track.size() - 2)->GetX();
    double xr  = track.at(0)->GetX();
    double xrl = track.at(1)->GetX();

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
