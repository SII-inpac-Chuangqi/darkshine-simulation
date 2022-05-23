//
// Created by xuliang on 2022/5/22.
//

#include "Object/DDetectorIDMaps.h"

DDetectorIDMaps *dDetectorIDMaps = nullptr;

DDetectorIDMaps *DDetectorIDMaps::CreateInstance() {
    if(dDetectorIDMaps == nullptr)
        dDetectorIDMaps = new DDetectorIDMaps();
    return dDetectorIDMaps;
}

void DDetectorIDMaps::BuildMap() {
    /// Fill ECAL Map
    // tree height 2
    int reNum2 = 0;
    for (int Z2 = 1; Z2 <= (int)ECAL_Block_No.z(); Z2++) {
        for (int Y2 = 1; Y2 <= (int)ECAL_Block_No.y(); Y2++) {
            for (int X2 = 1; X2 <= (int)ECAL_Block_No.x(); X2++) {
                // tree height 1
                int reNum1 = 0;
                for (int Z1 = 1; Z1 <= (int)ECAL_Cell_No.z(); Z1++) {
                    for (int Y1 = 1; Y1 <= (int)ECAL_Cell_No.y(); Y1++) {
                        for (int X1 = 1; X1 <= (int)ECAL_Cell_No.x(); X1++) {
                            FillECALID(reNum1,
                                       reNum2,
                                       X1 + (X2 - 1) * (int)ECAL_Cell_No.x(),
                                       Y1 + (Y2 - 1) * (int)ECAL_Cell_No.y(),
                                       Z1 + (Z2 -1) * (int)ECAL_Cell_No.z());
                            reNum1++;
                        }
                    }
                }
                reNum2++;
            }
        }
    }
    /// Fill HCAL Map
    reNum2 = 0;
    for (int Y = 1; Y <= (int)HCAL_Module_No.y(); Y++) {
        for (int X = 1; X <= (int)HCAL_Module_No.x(); X++) {
            FillHCALID(reNum2,
                       X,
                       Y);
            reNum2++;
        }
    }
}
