//................................................................................//
//C++
#include <iostream>

//................................................................................//
//ROOT
#include "TMath.h"

//................................................................................//
//Framework
#include "Core/AnaData.h"

//................................................................................//
//Tracking
#include "Algo/Utils/Util.h"

//If particle pass through trackers
bool InTagTrack(double x, double y, double z)
{
    double x_max = dAnaData->getCenterXTag() + 0.5*dAnaData->getLengthXTag();
    double x_min = dAnaData->getCenterXTag() - 0.5*dAnaData->getLengthXTag();
    double y_max = dAnaData->getCenterYTag() + 0.5*dAnaData->getLengthYTag();
    double y_min = dAnaData->getCenterYTag() - 0.5*dAnaData->getLengthYTag();
    double z_max = dAnaData->getCenterZTag() + 0.5*dAnaData->getLengthZTag();
    double z_min = dAnaData->getCenterZTag() - 0.5*dAnaData->getLengthZTag();

    if(z > z_min && z < z_max &&
       y > y_min && y < y_max &&
       x > x_min && x < x_max   )
        return true;

    return false;
}

bool InRecTrack(double x, double y, double z)
{
    double x_max = dAnaData->getCenterXRec() + 0.5*dAnaData->getLengthXRec();
    double x_min = dAnaData->getCenterXRec() - 0.5*dAnaData->getLengthXRec();
    double y_max = dAnaData->getCenterYRec() + 0.5*dAnaData->getLengthYRec();
    double y_min = dAnaData->getCenterYRec() - 0.5*dAnaData->getLengthYRec();
    double z_max = dAnaData->getCenterZRec() + 0.5*dAnaData->getLengthZRec();
    double z_min = dAnaData->getCenterZRec() - 0.5*dAnaData->getLengthZRec();

    if(z > z_min && z < z_max &&
       y > y_min && y < y_max &&
       x > x_min && x < x_max   )
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
       dy2sum1 += ((abr[0] * x[i] + abr[1]) - yavg)*((abr[0] * x[i] + abr[1]) - yavg);//numerator of r^2
       dy2sum2 += (y[i] - yavg)*(y[i] - yavg);//denominator of r^2
   }
   abr[2] = dy2sum1 / dy2sum2;//r^2
}

double PointToLineDistance(double A, double B, double C, double x, double y)
{
    return std::abs(A*x + B*y + C)/std::sqrt(A*A + B*B);
}
