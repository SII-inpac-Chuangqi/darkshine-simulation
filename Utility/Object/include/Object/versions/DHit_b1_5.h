//
// Created by Zhang Yulei on 9/18/20.
//

#ifndef DSIMU_DHIT_B1_5_H
#define DSIMU_DHIT_B1_5_H

#include "TObject.h"

enum DetectorType {
    nTagTracker, nRecTracker, nECAL, nHCAL, nHCAL_APD, nNone
};

class DHit_b1_5 : public TObject {
public:
    // Constructor and Destructor
    DHit_b1_5() = default;

    DHit_b1_5(const DHit_b1_5 &rhs)  : TObject(rhs) {
        *this = rhs;
    }

    ~DHit_b1_5() override = default;

    bool operator==(const DHit_b1_5 &rhs) const {
        return id == rhs.id &&
        CellID == rhs.CellID &&
        CellType == rhs.CellType &&
//        CellID_X == rhs.CellID_X &&
//        CellID_Y == rhs.CellID_Y &&
//        CellID_Z == rhs.CellID_Z &&
        Detector == rhs.Detector &&
//        X == rhs.X &&
//        Y == rhs.Y &&
//        Z == rhs.Z &&
        T == rhs.T &&
        E == rhs.E;
    }

    bool operator!=(const DHit_b1_5 &rhs) const {
        return !(rhs == *this);
    }

    DHit_b1_5 &operator=(const DHit_b1_5 &rhs) {
        if (&rhs == this) { return *this; }
        id = rhs.id;
        CellID = rhs.CellID;
        CellType = rhs.CellType;
//        CellID_X = rhs.CellID_X;
//        CellID_Y = rhs.CellID_Y;
//        CellID_Z = rhs.CellID_Z;
        Detector = rhs.Detector;
//        X = rhs.X;
//        Y = rhs.Y;
//        Z = rhs.Z;
        T = rhs.T;
        E = rhs.E;

        return *this;
    }

    // Get Methods
    int getId() const {
        return id;
    }

    int getCellId() const {
        return CellID;
    }

    virtual float getX() const {
        return 0; // TODO: read from map
    }

    virtual float getY() const {
        return 0;
    }

    virtual float getZ() const {
        return 0;
    }

    float getT() const {
        return T;
    }

    float getE() const {
        return E;
    }

    DetectorType getDetector() const {
        return Detector;
    }

    int getCellType() const {
        return CellType;
    }

    virtual int getCellIdX() const {
        return 0;
    }

    virtual int getCellIdY() const{
        return 0;
    }

    virtual int getCellIdZ() const{
        return 0;
    }

    // Set Methods
    void setId(int ID) {
        DHit_b1_5::id = ID;
    }

    void setCellId(int cellId) {
        CellID = cellId;
    }

//    void setX(float x) {
//        X = x;
//    }
//
//    void setY(float y) {
//        Y = y;
//    }
//
//    void setZ(float z) {
//        Z = z;
//    }

    void setT(float t) {
        T = t;
    }

    void setE(float e) {
        E = e;
    }

    void setDetector(DetectorType detector) {
        Detector = detector;
    }

    void setCellType(int cellType) {
        CellType = cellType;
    }

//    void setCellIdX(int cellIdX) {
//        CellID_X = cellIdX;
//    }
//
//    void setCellIdY(int cellIdY) {
//        CellID_Y = cellIdY;
//    }
//
//    void setCellIdZ(int cellIdZ) {
//        CellID_Z = cellIdZ;
//    }


protected:
    int id{0};
    int CellID{0};
    int CellType{0};

//    int CellID_X{0};
//    int CellID_Y{0};
//    int CellID_Z{0};

    DetectorType Detector{nNone};

//    float X{0.};
//    float Y{0.};
//    float Z{0.};
    float T{0.};
    float E{0.};

ClassDefOverride(DHit_b1_5, 5);
};

#endif //DSIMU_DHIT_B1_5_H
