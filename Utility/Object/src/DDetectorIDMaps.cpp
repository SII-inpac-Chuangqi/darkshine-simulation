//
// Created by xuliang on 2022/5/22.
//

#include "Object/DDetectorIDMaps.h"

DDetectorIDMaps *dECALIDMaps = nullptr;
DDetectorIDMaps *dHCALIDMaps = nullptr;
DDetectorIDMaps *dSideHCALIDMaps = nullptr;

void DDetectorIDMaps::CreateInstance() {
    if(dECALIDMaps == nullptr)
        dECALIDMaps = new DDetectorIDMaps();
    if(dHCALIDMaps == nullptr)
        dHCALIDMaps = new DDetectorIDMaps();
    if(dSideHCALIDMaps == nullptr)
        dSideHCALIDMaps = new DDetectorIDMaps();
}

void DDetectorIDMaps::LoopCellID(int blockX, int blockY, int blockZ) {
    reNumber1 = 0;
    for (int cellZ = 1; cellZ <= Cell_Shape.at(2); cellZ++) {
        for (int cellY = 1; cellY <= Cell_Shape.at(1); cellY++) {
            for (int cellX = 1; cellX <= Cell_Shape.at(0); cellX++) {
                int idX = cellX + (blockX - 1) * Cell_Shape.at(0);
                int idY = cellY + (blockY - 1) * Cell_Shape.at(1);
                int idZ = cellZ + (blockZ - 1) * Cell_Shape.at(2);
                int id = idX + (idY - 1) * xID + (idZ - 1) * (xID * yID);
                FillDetectorID(idX,idY,idZ,id);
                reNumber1++;
            }
        }
    }
}

void DDetectorIDMaps::BuildMap() {
    xID =  Cell_Shape.at(0) * Block_Shape.at(0);
    yID =  Cell_Shape.at(1) * Block_Shape.at(1);
    // volume depth 2
    reNumber2 = 0;
    for (int blockZ = 1; blockZ <= Block_Shape.at(2); blockZ++) {
        for (int blockY = 1; blockY <= Block_Shape.at(1); blockY++) {
            for (int blockX = 1; blockX <= Block_Shape.at(0); blockX++) {
                // volume depth 1
                LoopCellID(blockX,blockY,blockZ);
                reNumber2++;
            }
        }
    }
}
