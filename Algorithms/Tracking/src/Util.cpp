#ifndef _OFF_TRACKING
//................................................................................//
//CPP STL
#include <iostream>
#include <map>
#include <vector>

//................................................................................//
//ROOT
#include "TMath.h"

//................................................................................//
//Tracking
#include "Algo/Util.h"

//If particle pass through trackers
bool InTagTrack(double x, double y, double z)
{
    if(z > -607.5 && z < -7.5 &&
       y > -100   && y <  100 &&
       x > -50    && x <  50    )
        return true;

    return false;
}

bool InRecTrack(double x, double y, double z)
{
    if(z >  7.5 && z < 180.5 &&
       y > -100 && y < 100   &&
       x > -50  && x < 50      )
        return true;

    return false;
}
#endif