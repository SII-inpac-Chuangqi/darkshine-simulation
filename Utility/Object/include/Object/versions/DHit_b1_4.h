//
// Created by Zhang Yulei on 9/18/20.
//

#ifndef DSIMU_DHIT_B1_4_H
#define DSIMU_DHIT_B1_4_H

#include "TObject.h"

enum DetectorType {
    nTagTracker, nRecTracker, nECAL, nHCAL, nHCAL_APD, nNone
};

class DHit_b1_4 : public TObject {
public:
    // Constructor and Destructor
    DHit_b1_4() = default;

    DHit_b1_4(const DHit_b1_4 &rhs)  : TObject(rhs) {
        *this = rhs;
    }

    ~DHit_b1_4() override = default;

    bool operator==(const DHit_b1_4 &rhs) const {
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

    bool operator!=(const DHit_b1_4 &rhs) const {
        return !(rhs == *this);
    }

    DHit_b1_4 &operator=(const DHit_b1_4 &rhs) {
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
    [[nodiscard]] int getId() const {
        return id;
    }

    [[nodiscard]] int getCellId() const {
        return CellID;
    }

    [[nodiscard]] float getX() const {
        return X;
    }

    [[nodiscard]] float getY() const {
        return Y;
    }

    [[nodiscard]] double getZ() const {
        return Z;
    }

    [[nodiscard]] float getT() const {
        return T;
    }

    [[nodiscard]] float getE() const {
        return E;
    }

    [[nodiscard]] int getDetector() const {
        return Detector;
    }

    [[nodiscard]] int getCellIdX() const {
        return CellID_X;
    }

    [[nodiscard]] int getCellIdY() const {
        return CellID_Y;
    }

    [[nodiscard]] int getCellIdZ() const {
        return CellID_Z;
    }

    // Set Methods
    void setId(int ID) {
        id = ID;
    }

    void setCellId(int cellId) {
        CellID = cellId;
    }

    void setX(float x) {
        X = x;
    }

    void setY(float y) {
        Y = y;
    }

    void setZ(float z) {
        Z = z;
    }

    void setT(float t) {
        T = t;
    }

    void setE(float e) {
        E = e;
    }

    void setDetector(int detector) {
        Detector = detector;
    }

    void setCellIdX(int cellIdX) {
        CellID_X = cellIdX;
    }

    void setCellIdY(int cellIdY) {
        CellID_Y = cellIdY;
    }

    void setCellIdZ(int cellIdZ) {
        CellID_Z = cellIdZ;
    }


protected:
    // 00 00 00 00   00 00 00 00   00 00 00 00   00 00 00 00 // TObject Memory
    int id{0};
    int CellID{0};
    float T{0.};
    float E{0.};

    uint16_t CellID_X{0};
    uint16_t CellID_Y{0};
    uint16_t CellID_Z{0};
    uint16_t Detector{nNone};
    float X{0.};
    float Y{0.};

    float Z{0.};

    // 00 00 00 00   -- -- -- --   -- -- -- --   -- -- -- -- // Last line of DHit Memory

ClassDefOverride(DHit_b1_4, 5);
};

#endif //DSIMU_DHIT_B1_H
