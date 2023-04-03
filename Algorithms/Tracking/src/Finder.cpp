//................................................................................//
//C++
#include <iostream>
#include <vector>

//................................................................................//
//ROOT

//................................................................................//
//Framework
#include "Core/AnaData.h"

//................................................................................//
//Tracking
#include "Algo/Finder.h"

std::vector<double> Finder::GetMagnetFieldAt(double x, double y, double z)
{
    return dAnaData->getMagnetFieldAt({x, y, z});
}
