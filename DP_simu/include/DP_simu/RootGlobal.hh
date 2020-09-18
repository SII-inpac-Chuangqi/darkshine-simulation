//
// Created by Zhang Yulei on 9/9/20.
//
#include <vector>
#include "TString.h"

#ifndef DSIMU_ROOTGLOBAL_HH
#define DSIMU_ROOTGLOBAL_HH

struct ParticleSteps
{
    double Pos_X;
    double Pos_Y;
    double Pos_Z;
    double Mom_Px;
    double Mom_Py;
    double Mom_Pz;
    TString PVName;
    TString ProcessName;
};

#endif //DSIMU_ROOTGLOBAL_HH
