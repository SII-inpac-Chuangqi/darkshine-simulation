#include <iostream>
#include <malloc.h>

#include "TVectorD.h"

#include "Object/DMagnet.h"

//................................................................................//
//public:
//................................................................................//
//Constructor & destructor
//................................................................................//
//Copy constructor
DMagnet::DMagnet(const DMagnet &old) : TNamed(old),
                                       mags_(old.mags_),
                                       xDivision_(old.xDivision_),
                                       yDivision_(old.yDivision_),
                                       zDivision_(old.zDivision_)
{}

//................................................................................//
//Operator =
DMagnet& DMagnet::operator =(const DMagnet &rhs)
{
    if(this == &rhs) return *this;

    TNamed::operator = (rhs);
    this->mags_.assign(rhs.mags_.begin(), rhs.mags_.end());
    this->xDivision_.assign(rhs.xDivision_.begin(), rhs.xDivision_.end());
    this->yDivision_.assign(rhs.yDivision_.begin(), rhs.yDivision_.end());
    this->zDivision_.assign(rhs.zDivision_.begin(), rhs.zDivision_.end());

    return *this;
}

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
    mag.gChi2 = fitter->GetChi2()/(fitter->GetSampleSize() - fitter->GetNCoefficients());

    mag.gDMean = fitter->GetMeanQuantity();
    mag.gDMin = fitter->GetMeanQuantity() + fitter->GetMinQuantity();
    mag.gDMax = fitter->GetMeanQuantity() + fitter->GetMaxQuantity();

    auto gXMin = fitter->GetMinVariables();
    auto gXMax = fitter->GetMaxVariables();
    for(int i = 0; i < 3; i++)
    {
        mag.gXMin.at(i) = (*gXMin)[i];
        mag.gXMax.at(i) = (*gXMax)[i];
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
        return 0.;

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

    int id = zid*(xDivision_.size() - 1)*(yDivision_.size() - 1) + yid*(xDivision_.size() - 1) + xid;
    if(id > static_cast<int>(mags_.size()) || id < 0)
        return 0.;

    double field = GetSliceField(x, y, z, mags_.at(id));
    return field;
}

//................................................................................//
//Print
void DMagnet::Print(Option_t *option) const
{
    std::cout << "******************************************************************************" << std::endl
              << "*" << " DMagnet:" << "	name: " << this->GetName() << "	title: " << this->GetTitle() << std::endl
              << "******************************************************************************" << std::endl;
    for(int i = 0; i < static_cast<int>(mags_.size()); i++)
    {
        int zid = i/((xDivision_.size() - 1)*(yDivision_.size() - 1));
        int yid = (i - zid*(xDivision_.size() - 1)*(yDivision_.size() - 1))/(xDivision_.size() - 1);
        int xid = i - zid*(xDivision_.size() - 1)*(yDivision_.size() - 1) - yid*(xDivision_.size() - 1);

        if(i) std::cout << std::endl;

        std::cout << "* magnet " << i << ":" << std::endl
                  << "* x range/mm:   " << xDivision_.at(xid) << " " << xDivision_.at(xid + 1) << std::endl
                  << "* y range/mm:   " << yDivision_.at(yid) << " " << yDivision_.at(yid + 1) << std::endl
                  << "* z range/mm:   " << zDivision_.at(zid) << " " << zDivision_.at(zid + 1) << std::endl
                  << "* min field/T:  " << mags_.at(i).gDMin << std::endl
                  << "* max field/T:  " << mags_.at(i).gDMax << std::endl
                  << "* reduced chi2: " << mags_.at(i).gChi2 << std::endl;
    }
    std::cout << "******************************************************************************" << std::endl;
}

//................................................................................//
//private:
//................................................................................//
//Get field value in chosen MagSlice
double DMagnet::GetSliceField(double x, double y, double z, const MagSlice &mag)
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

