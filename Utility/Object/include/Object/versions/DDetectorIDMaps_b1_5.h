//
// Created by xuliang on 2022/5/22.
//

#ifndef DSIMU_DDETECTORIDMAPS_B1_5_H
#define DSIMU_DDETECTORIDMAPS_B1_5_H

#include <map>
#include "TObject.h"
#include "TVector3.h"
#include "DHit.h"

class DDetectorIDMaps_b1_5 : public TObject {
public:
    DDetectorIDMaps_b1_5(const DDetectorIDMaps_b1_5 &) = delete;
    DDetectorIDMaps_b1_5 &operator=(DDetectorIDMaps_b1_5 const &) = delete;

    static void CreateInstance();

    /// Getters

    int GetIDX(int reNum1, int reNum2) {
        return IDXMap.at(std::make_pair(reNum1, reNum2));
    }

    int GetIDY(int reNum1, int reNum2) {
        return IDYMap.at(std::make_pair(reNum1, reNum2));
    }

    int GetIDZ(int reNum1, int reNum2) {
        return IDZMap.at(std::make_pair(reNum1, reNum2));
    }

    int GetID(int reNum1, int reNum2) {
        return IDMap.at(std::make_pair(reNum1, reNum2));
    }

    /// Setters

    void SetBlockShape(Int_t x, Int_t y, Int_t z) {
        Block_Shape = {x, y, z};
    }

    void SetCellShape(Int_t x, Int_t y, Int_t z) {
        Cell_Shape = {x, y, z};
    }

    void SetDetectorType(DetectorType in) {dType = in;}

    void FillDetectorID(int idX, int idY,int idZ, int id) {
        IDXMap[std::make_pair(reNumber1, reNumber2)] = idX;
        IDYMap[std::make_pair(reNumber1, reNumber2)] = idY;
        IDZMap[std::make_pair(reNumber1, reNumber2)] = idZ;
        IDMap[std::make_pair(reNumber1, reNumber2)] = id;
    }

    void BuildMap();

private:
    void LoopCellID(int blockX, int blockY, int blockZ);

private:
    DDetectorIDMaps_b1_5() = default;

    std::map<std::pair<int, int>, int> IDXMap{};
    std::map<std::pair<int, int>, int> IDYMap{};
    std::map<std::pair<int, int>, int> IDZMap{};
    std::map<std::pair<int, int>, int> IDMap{};

    std::vector<int> Block_Shape;
    std::vector<int> Cell_Shape;
    int xID;
    int yID;

    int reNumber1;
    int reNumber2;
    DetectorType dType;

ClassDefOverride(DDetectorIDMaps_b1_5, 1);
};

extern DDetectorIDMaps_b1_5 *dECALIDMaps;
extern DDetectorIDMaps_b1_5 *dHCALIDMaps;
extern DDetectorIDMaps_b1_5 *dSideHCALIDMaps;

#endif //DSIMU_DDETECTORIDMAPS_B1_5_H
