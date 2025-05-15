#ifndef TRACKING_SEEDING_HELPER_H
#define TRACKING_SEEDING_HELPER_H

#include <cmath>

namespace seed_helper
{
inline void LinearFit(double x[], double y[], int n, double abr[])
{
    double xsum = 0.;
    double ysum = 0.;
    double x2sum = 0.;
    double xysum = 0.;

    for (int i = 0; i < n; i++)
    {
        xsum += x[i];
        ysum += y[i];
        x2sum += x[i] * x[i];
        xysum += x[i] * y[i];
    }
    abr[0] = (n*xysum - xsum * ysum) / (n*x2sum - xsum * xsum); //a
    abr[1] = (ysum - abr[0] * xsum) / n; //b

    double yavg = ysum/n;
    double dy2sum1 = 0.;
    double dy2sum2 = 0.;
    for (int i = 0; i < n; i++)
    {
        dy2sum1 += ((abr[0] * x[i] + abr[1]) - yavg)*((abr[0] * x[i] + abr[1]) - yavg);//numerator of r^2
        dy2sum2 += (y[i] - yavg)*(y[i] - yavg); //denominator of r^2
    }
    abr[2] = dy2sum1 / dy2sum2; //r^2
}

inline void KasaFit(double x[], double y[], size_t n, double &A, double &B, double &R, double &chi2)
{
    double X1 = 0.;
    double Y1 = 0.;
    double X2 = 0.;
    double Y2 = 0.;
    double X3 = 0.;
    double Y3 = 0.;
    double X1Y1 = 0.;
    double X1Y2 = 0.;
    double X2Y1 = 0.;

    for(size_t i = 0; i < n; i++)
    {
        X1 += x[i];
        Y1 += y[i];
        X2 += x[i]*x[i];
        Y2 += y[i]*y[i];
        X3 += x[i]*x[i]*x[i];
        Y3 += y[i]*y[i]*y[i];
        X1Y1 += x[i]*y[i];
        X1Y2 += x[i]*y[i]*y[i];
        X2Y1 += x[i]*x[i]*y[i];
    }

    double N = n;
    double C = N*X2 - X1*X1;
    double D = N*X1Y1 - X1*Y1;
    double E = N*X3 + N*X1Y2 - (X2 + Y2)*X1;
    double G = N*Y2 - Y1*Y1;
    double H = N*X2Y1 + N*Y3 - (X2 + Y2)*Y1;
    double a = (H*D - E*G)/(C*G - D*D);
    double b = (H*C - E*D)/(D*D - G*C);
    double c = -(a*X1 + b*Y1 + X2 + Y2)/N;

    A = -0.5*a;
    B = -0.5*b;
    R = 0.5*std::sqrt(a*a + b*b - 4*c);

    double s = 0.;
    for (size_t i = 0; i < n; i++)
    {
        double xi = x[i] - A;
        double yi = y[i] - B;
        double zi = sqrt(xi*xi + yi*yi);
        s += (R - zi)*(R - zi);
    }

    chi2 = 1 - sqrt(s/(n*R*R));
}

inline double PointToLineDistance(double A, double B, double C, double x, double y)
{
    return std::abs(A*x + B*y + C)/std::sqrt(A*A + B*B);
}
}

#endif // SEED_HELPER_H
