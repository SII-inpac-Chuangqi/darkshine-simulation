//
// Created by Zhang Yulei on 9/24/20.
//

#include "Algo/SecondaryMaxEFinder.h"

void SecondaryMaxEFinder::RegisterParameters() {
    // Register Output Variables
    EvtWrt->RegisterDoubleVariable("Secondary_MaxE", &Secondary_MaxE, "Secondary_MaxE/D");
    EvtWrt->RegisterStrVariable("Secondary_MaxE_PVName", &Secondary_MaxE_PVName);
    EvtWrt->RegisterStrVariable("Secondary_MaxE_Process", &Secondary_MaxE_Process);
}
