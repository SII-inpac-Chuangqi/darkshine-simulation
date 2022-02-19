//
// Created by Zhang Yulei on 9/18/20.
//

#ifndef DSIMU_DHIT_B1_H
#define DSIMU_DHIT_B1_H

#include "TObject.h"

enum DetectorType {
    nNone, nTracker, nECAL, nHCAL
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
               CellID_X == rhs.CellID_X &&
               CellID_Y == rhs.CellID_Y &&
               CellID_Z == rhs.CellID_Z &&
               Detector == rhs.Detector &&
               X == rhs.X &&
               Y == rhs.Y &&
               Z == rhs.Z &&
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
        CellID_X = rhs.CellID_X;
        CellID_Y = rhs.CellID_Y;
        CellID_Z = rhs.CellID_Z;
        Detector = rhs.Detector;
        X = rhs.X;
        Y = rhs.Y;
        Z = rhs.Z;
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

    double getX() const {
        return X;
    }

    double getY() const {
        return Y;
    }

    double getZ() const {
        return Z;
    }

    double getT() const {
        return T;
    }

    double getE() const {
        return E;
    }

    DetectorType getDetector() const {
        return Detector;
    }

    int getCellIdX() const {
        return CellID_X;
    }

    int getCellIdY() const {
        return CellID_Y;
    }

    int getCellIdZ() const {
        return CellID_Z;
    }

    // Set Methods
    void setId(int ID) {
        DHit_b1_5::id = ID;
    }

    void setCellId(int cellId) {
        CellID = cellId;
    }

//    void setX(double x) {
//        X = x;
//    }
//
//    void setY(double y) {
//        Y = y;
//    }
//
//    void setZ(double z) {
//        Z = z;
//    }

    void setT(double t) {
        T = t;
    }

    void setE(double e) {
        E = e;
    }

    void setDetector(DetectorType detector) {
        Detector = detector;
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

//    double X{0.};
//    double Y{0.};
//    double Z{0.};
    double T{0.};
    double E{0.};

ClassDefOverride(DHit_b1_5, 5);
};

#endif //DSIMU_DHIT_B1_H
