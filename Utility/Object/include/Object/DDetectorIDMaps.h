//
// Created by xuliang on 2022/5/22.
//

#ifndef DSIMU_DDETECTORIDMAPS_H
#define DSIMU_DDETECTORIDMAPS_H

#include "Object/versions/DDetectorIDMaps_b1_5.h"

/// @brief Detector ID maps for ECAL, HCAL, and SideHCAL. \n
/// ECAL / HCAL / SideHCAL Bounding volume hierarchy:\n
/// --> Region (depth 3)\n
/// ....--> Blcok (depth 2)\n
/// ........--> Cell (depth 1)\n
/// ............--> Crystal (depth 0)\n
/// ............--> APD (depth 0)\n
/// ECAL Cell ID XYZ: start from 1 \n
/// ECAL Cell ID: start from 0, \n
/// HCAL / SideHCAL Cell ID XYZ: start from 1 \n
/// HCAL / SideHCAL Cell ID: start from 1
using DDetectorIDMaps = DDetectorIDMaps_b1_5;

#endif //DSIMU_DDETECTORIDMAPS_H
