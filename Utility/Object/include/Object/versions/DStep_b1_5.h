//
// Created by Zhang Yulei on 9/20/20.
//

#ifndef DSIMU_DSTEP_B1_H
#define DSIMU_DSTEP_B1_H

#include <string>
#include <ostream>

#include "TObject.h"
#include "TString.h"

class DStep_b1_5 : public TObject {
public:
    DStep_b1_5() = default;

    DStep_b1_5(const DStep_b1_5 &rhs)  : TObject(rhs) {
        *this = rhs;
    }

    ~DStep_b1_5() override = default;

    // Operators
    bool operator==(const DStep_b1_5 &rhs) const {
        return id == rhs.id &&
               X == rhs.X &&
               Y == rhs.Y &&
               Z == rhs.Z &&
               Px == rhs.Px &&
               Py == rhs.Py &&
               Pz == rhs.Pz &&
               E == rhs.E &&
               PVName == rhs.PVName &&
               ProcessName == rhs.ProcessName;
    }

    bool operator!=(const DStep_b1_5 &rhs) const {
        return !(rhs == *this);
    }

    DStep_b1_5 &operator=(const DStep_b1_5 &rhs) {
        if (&rhs == this) { return *this; }
        id = rhs.id;
        X = rhs.X;
        Y = rhs.Y;
        Z = rhs.Z;
        Px = rhs.Px;
        Py = rhs.Py;
        Pz = rhs.Pz;
        E = rhs.E;
        PVName = rhs.PVName;
        ProcessName = rhs.ProcessName;

        return *this;
    }

    // Get Methods
    int getId() const {
        return id;
    }

    float getX() const {
        return X;
    }

    float getY() const {
        return Y;
    }

    float getZ() const {
        return Z;
    }

    float getPx() const {
        return Px;
    }

    float getPy() const {
        return Py;
    }

    float getPz() const {
        return Pz;
    }

    float getE() const {
        return E;
    }

    const std::string &getPVName() const {
        return PVName;
    }

    const std::string &getProcessName() const {
        return ProcessName;
    }

    // Dump
    friend std::ostream &operator<<(std::ostream &os, const DStep_b1_5 &step) {
        TString str(Form("| %-5d | %8.3f, %8.3f, %8.3f | %8.3f, %8.3f, %8.3f, %8.3f | %-18s  %-15s |",
                          step.id,
                          step.X, step.Y, step.Z,
                          step.Px, step.Py, step.Pz, step.E,
                          step.PVName.data(), step.ProcessName.data()));
        os << str;

        return os;
    }

    // Set Methods
    void setId(int ID) {
        id = ID;
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

    void setPx(float px) {
        Px = px;
    }

    void setPy(float py) {
        Py = py;
    }

    void setPz(float pz) {
        Pz = pz;
    }

    void setE(float e) {
        E = e;
    }

    void setPVName(const std::string &pvName) {
        PVName = pvName;
    }

    void setProcessName(const std::string &processName) {
        ProcessName = processName;
    }

private:
    int id{0};
    float X{0.};
    float Y{0.};
    float Z{0.};
    float Px{0.};
    float Py{0.};
    float Pz{0.};
    float E{0.};
    std::string PVName;
    std::string ProcessName;

ClassDefOverride(DStep_b1_5, 7);
};

#endif //DSIMU_DSTEP_H
