//................................................................................//
//C++
#include <iostream>

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

void LinearFit(double abr[], double x[], double y[], int n)
{
   double xsum{0.};
   double ysum{0.};
   double x2sum{0.};
   double xysum{0.};

   for (int  i = 0; i < n; i++)
   {
       xsum += x[i];
       ysum += y[i];
       x2sum += x[i] * x[i];
       xysum += x[i] * y[i];
   }
   abr[0] = (n*xysum - xsum * ysum) / (n*x2sum - xsum * xsum);//a
   abr[1] = (ysum - abr[0] * xsum) / n;//b

   double yavg = ysum/n;
   double dy2sum1{0.}, dy2sum2{0.};
   for (int i = 0; i < n; i++)
   {
       dy2sum1 += ((abr[0] * x[i] + abr[1]) - yavg)*((abr[0] * x[i] + abr[1]) - yavg);//r^2的分子
       dy2sum2 += (y[i] - yavg)*(y[i] - yavg);//r^2的分母
   }
   abr[2] = dy2sum1 / dy2sum2;//r^2
}

double PointToLineDistance(double A, double B, double C, double x, double y)
{
    return std::abs(A*x + B*y + C)/std::sqrt(A*A + B*B);
}
