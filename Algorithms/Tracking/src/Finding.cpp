//................................................................................//
//CPP STL
#include <iostream>
#include <vector>

//................................................................................//
//ROOT

//................................................................................//
//Framework
#include "Core/AnaData.h"

//................................................................................//
//Tracking
#include "Algo/Finding.h"

std::vector<double> Finding::GetMagnetFieldAt(double x, double y, double z)
{
    return dAnaData->getMagnetFieldAt({x, y, z});
}
