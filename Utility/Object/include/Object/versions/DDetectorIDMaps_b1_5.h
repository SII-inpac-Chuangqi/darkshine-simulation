//
// Created by xuliang on 2022/5/22.
//

#ifndef DSIMU_DDETECTORIDMAPS_B1_5_H
#define DSIMU_DDETECTORIDMAPS_B1_5_H

#include <map>
#include "TObject.h"
#include "TVector3.h"

class DDetectorIDMaps_b1_5 : public TObject {
public:
    DDetectorIDMaps_b1_5(const DDetectorIDMaps_b1_5 &) = delete;
    DDetectorIDMaps_b1_5 &operator=(DDetectorIDMaps_b1_5 const &) = delete;

    static DDetectorIDMaps_b1_5 *CreateInstance();

    /// Getters

    int GetECALIDX(int reNumber1, int reNumber2) {
        return ECALIDXMap.at(std::make_pair(reNumber1, reNumber2));
    }

    int GetECALIDY(int reNumber1, int reNumber2) {
        return ECALIDYMap.at(std::make_pair(reNumber1, reNumber2));
    }

    int GetECALIDZ(int reNumber1, int reNumber2) {
        return ECALIDZMap.at(std::make_pair(reNumber1, reNumber2));
    }

    int GetHCALIDX(int reNumber) {
        return HCALIDXMap.at(reNumber);
    }

    int GetHCALIDY(int reNumber) {
        return HCALIDYMap.at(reNumber);
    }

    /// Setters

    void SetECALBlockNo(Double_t x, Double_t y, Double_t z) {
        ECAL_Block_No = TVector3(x,y,z);
    }

    void SetECALCellNo(Double_t x, Double_t y, Double_t z) {
        ECAL_Cell_No = TVector3(x,y,z);
    }

    void SetHCALModuleNo(Double_t x, Double_t y, Double_t z) {
        HCAL_Module_No = TVector3(x,y,z);
    }

    void FillECALID(int reNumber1, int reNumber2, int idX, int idY,int idZ) {
        ECALIDXMap[std::make_pair(reNumber1, reNumber2)] = idX;
        ECALIDYMap[std::make_pair(reNumber1, reNumber2)] = idY;
        ECALIDZMap[std::make_pair(reNumber1, reNumber2)] = idZ;
    }

    void FillHCALID(int reNumber, int idX, int idY) {
        HCALIDXMap.insert(std::make_pair(reNumber, idX));
        HCALIDYMap.insert(std::make_pair(reNumber, idY));
    }

    void BuildMap();

private:
    DDetectorIDMaps_b1_5() = default;

    std::map<std::pair<int, int>, int> ECALIDXMap{};
    std::map<std::pair<int, int>, int> ECALIDYMap{};
    std::map<std::pair<int, int>, int> ECALIDZMap{};

    std::map<int, int> HCALIDXMap{};
    std::map<int, int> HCALIDYMap{};

    TVector3 ECAL_Block_No;
    TVector3 ECAL_Cell_No;
    TVector3 HCAL_Module_No;

ClassDefOverride(DDetectorIDMaps_b1_5, 1);
};

extern DDetectorIDMaps_b1_5 *dDetectorIDMaps;
#endif //DSIMU_DDETECTORIDMAPS_B1_5_H
