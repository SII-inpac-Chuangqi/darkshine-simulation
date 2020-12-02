//
// Created by Zhang Yulei on 9/20/20.
//

#ifndef DSIMU_DSTEP_H
#define DSIMU_DSTEP_H

#include <string>

#include "TObject.h"

class DStep : public TObject {
public:
    DStep() = default;

    DStep(const DStep &rhs)  : TObject(rhs) {
        *this = rhs;
    }

    ~DStep() override = default;

    // Operators
    bool operator==(const DStep &rhs) const {
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

    bool operator!=(const DStep &rhs) const {
        return !(rhs == *this);
    }

    DStep &operator=(const DStep &rhs) {
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

    double getX() const {
        return X;
    }

    double getY() const {
        return Y;
    }

    double getZ() const {
        return Z;
    }

    double getPx() const {
        return Px;
    }

    double getPy() const {
        return Py;
    }

    double getPz() const {
        return Pz;
    }

    double getE() const {
        return E;
    }

    const std::string &getPVName() const {
        return PVName;
    }

    const std::string &getProcessName() const {
        return ProcessName;
    }

    // Set Methods
    void setId(int ID) {
        id = ID;
    }

    void setX(double x) {
        X = x;
    }

    void setY(double y) {
        Y = y;
    }

    void setZ(double z) {
        Z = z;
    }

    void setPx(double px) {
        Px = px;
    }

    void setPy(double py) {
        Py = py;
    }

    void setPz(double pz) {
        Pz = pz;
    }

    void setE(double e) {
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
    double X{0.};
    double Y{0.};
    double Z{0.};
    double Px{0.};
    double Py{0.};
    double Pz{0.};
    double E{0.};
    std::string PVName;
    std::string ProcessName;

ClassDef(DStep,7);
};

#endif //DSIMU_DSTEP_H
