#include <iostream>

#include "TVectorD.h"

#include "Object/DMagnet.h"

//................................................................................//
//public:
//................................................................................//
//Set
//................................................................................//
//Set divisions of the parameterized region
void DMagnet::SetXDivision(const std::vector<double> &division)
{
    xDivision_.assign(division.begin(), division.end());
}

void DMagnet::SetYDivision(const std::vector<double> &division)
{
    yDivision_.assign(division.begin(), division.end());
}

void DMagnet::SetZDivision(const std::vector<double> &division)
{
    zDivision_.assign(division.begin(), division.end());
}

//................................................................................//
//Add parameterized magnet from TMultiDimFit
void DMagnet::AddMagnet(const TMultiDimFit *fitter)
{
    if(!fitter)
    {
        std::cout << "WARNING	Empty magnet to add" << std::endl;
        return;
    }

    MagSlice mag;

    mag.gNVariables = fitter->GetNVariables();
    mag.gNCoefficients = fitter->GetNCoefficients();
    mag.gDMean = fitter->GetMeanQuantity();

    auto gXMin = fitter->GetMinVariables();
    auto gXMax = fitter->GetMaxVariables();
    for(int i = 0; i < 3; i++)
    {
        mag.gXMin.push_back((*gXMin)[i]);
        mag.gXMax.push_back((*gXMax)[i]);
    }

    auto gCoefficients = fitter->GetCoefficients();
    for(int i = 0; i < mag.gNCoefficients; i++)
        mag.gCoefficients.push_back((*gCoefficients)[i]);

    auto gPowers = fitter->GetPowers();
    auto gPowerIndex = fitter->GetPowerIndex();
    for(int i = 0; i < mag.gNCoefficients; i++)
    {
        for(int j = 0; j < mag.gNVariables; j++)
            mag.gPowers.push_back(*(gPowers + gPowerIndex[i] * mag.gNVariables + j));
    }

    mags_.push_back(mag);
}

//................................................................................//
//Get
//................................................................................//
//Get field value at position (x, y, z) in mm
double DMagnet::GetField(double x, double y, double z)
{
    if(x < xDivision_.front() || x > xDivision_.back() ||
       y < yDivision_.front() || y > yDivision_.back() ||
       z < zDivision_.front() || z > zDivision_.back())
    {
        // std::cout << "WARNING	Position declared crosses the border" << std::endl;
        return 0;
    }

    int xid = xDivision_.size()*yDivision_.size()*zDivision_.size();
    int yid = yDivision_.size()*xDivision_.size();
    int zid = zDivision_.size();

    for(int i = 0; i < static_cast<int>(xDivision_.size() - 1); i++)
    {
        if(x >= xDivision_.at(i) && x <= xDivision_.at(i + 1))
            xid = i;
    }
    for(int j = 0; j < static_cast<int>(yDivision_.size() - 1); j++)
    {
        if(y >= yDivision_.at(j) && y <= yDivision_.at(j + 1))
            yid = j;
    }
    for(int k = 0; k < static_cast<int>(zDivision_.size() - 1); k++)
    {
        if(z >= zDivision_.at(k) && z <= zDivision_.at(k + 1))
            zid = k;
    }

    int id = zid*((xDivision_.size() - 1)*(yDivision_.size() - 1)) + yid*(xDivision_.size() - 1) + xid;
    if(id > static_cast<int>(mags_.size()) || id < 0)
    {
        // std::cout << "WARNING	No magnet slice matched" << std::endl;
        return 0;
    }

    double field = GetFieldValue(x, y, z, mags_.at(id));
    return field;
}

//................................................................................//
//private:
//................................................................................//
//Get field value in chosen MagSlice
double DMagnet::GetFieldValue(double x, double y, double z, const MagSlice &mag)
{
    std::vector<double> coor = {x, y, z};

    double returnValue = mag.gDMean;
    int i = 0, j = 0, k = 0;
    for (i = 0; i < mag.gNCoefficients; i++)
    {
        double term = mag.gCoefficients.at(i);
        for (j = 0; j < mag.gNVariables; j++)
        {
            int power = mag.gPowers.at(i*mag.gNVariables + j);
            double p1 = 1, p2 = 0, p3 = 0, r = 0;
            double v =  1 + 2. / (mag.gXMax.at(j) - mag.gXMin.at(j))*(coor.at(j) - mag.gXMax.at(j));

            switch(power)
            {
                case 1: r = 1; break;
                case 2: r = v; break;
                default:
                        p2 = v;
                        for (k = 3; k <= power; k++)
                        {
                            p3 = p2 * v;
                            p1 = p2; p2 = p3;
                        }
                r = p3;
            }
            term *= r;
        }
        returnValue += term;
    }

    return returnValue;
}
