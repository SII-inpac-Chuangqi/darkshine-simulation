//
// Created by Zhang Yulei on 9/18/20.
//

#ifndef DSIMU_DPARTICLE_H
#define DSIMU_DPARTICLE_H

#include <string>
#include <cmath>

#include "TObject.h"

class DParticle : public TObject {
public:
    // Constructor and Destructor
    DParticle() = default;

    DParticle(const DParticle &rhs) : TObject(rhs) {
        *this = rhs;
    }

    ~DParticle() override = default;

    bool operator==(const DParticle &rhs) const {
        return id == rhs.id &&
               Name == rhs.Name &&
               CreateProcess == rhs.CreateProcess &&
               PDG == rhs.PDG &&
               Mass == rhs.Mass &&
               Energy == rhs.Energy &&
               Px == rhs.Px &&
               Py == rhs.Py &&
               Pz == rhs.Pz &&
               P == rhs.P &&
               VertexX == rhs.VertexX &&
               VertexY == rhs.VertexY &&
               VertexZ == rhs.VertexZ &&
               EndPointX == rhs.EndPointX &&
               EndPointY == rhs.EndPointY &&
               EndPointZ == rhs.EndPointZ;
    }

    bool operator!=(const DParticle &rhs) const {
        return !(rhs == *this);
    }

    DParticle &operator=(const DParticle &rhs) {
        if (&rhs == this) { return *this; }
        id = rhs.id;
        Name = rhs.Name;
        CreateProcess = rhs.CreateProcess;
        PDG = rhs.PDG;
        Mass = rhs.Mass;
        Energy = rhs.Energy;
        Px = rhs.Px;
        Py = rhs.Py;
        Pz = rhs.Pz;
        P = rhs.P;
        VertexX = rhs.VertexX;
        VertexY = rhs.VertexY;
        VertexZ = rhs.VertexZ;
        EndPointX = rhs.EndPointX;
        EndPointY = rhs.EndPointY;
        EndPointZ = rhs.EndPointZ;

        return *this;
    }

    // Get Methods
    int getId() const {
        return id;
    }

    int getPdg() const {
        return PDG;
    }

    double getMass() const {
        return Mass;
    }

    double getEnergy() const {
        return Energy;
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

    double getVertexX() const {
        return VertexX;
    }

    double getVertexY() const {
        return VertexY;
    }

    double getVertexZ() const {
        return VertexZ;
    }

    double getEndPointX() const {
        return EndPointX;
    }

    double getEndPointY() const {
        return EndPointY;
    }

    double getEndPointZ() const {
        return EndPointZ;
    }

    const std::string &getName() const {
        return Name;
    }

    const std::string &getCreateProcess() const {
        return CreateProcess;
    }

    double getP() const {
        return sqrt(Px * Px + Py * Py + Pz * Pz);
    }

    // Set Methods
    void setId(int ID) {
        DParticle::id = ID;
    }

    void setPdg(int pdg) {
        PDG = pdg;
    }

    void setMass(double mass) {
        Mass = mass;
    }

    void setEnergy(double energy) {
        Energy = energy;
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

    void setVertexX(double vertexX) {
        VertexX = vertexX;
    }

    void setVertexY(double vertexY) {
        VertexY = vertexY;
    }

    void setVertexZ(double vertexZ) {
        VertexZ = vertexZ;
    }

    void setEndPointX(double endPointX) {
        EndPointX = endPointX;
    }

    void setEndPointY(double endPointY) {
        EndPointY = endPointY;
    }

    void setEndPointZ(double endPointZ) {
        EndPointZ = endPointZ;
    }

    void setName(const std::string &name) {
        Name = name;
    }

    void setCreateProcess(const std::string &createProcess) {
        CreateProcess = createProcess;
    }

    void setP(double p) {
        P = p;
    }

protected:
    // internal debug only
    int id{0};

    // internal String
    std::string Name;
    std::string CreateProcess;

    int PDG{0};
    double Mass{0.};
    double Energy{0.};
    double Px{0.};
    double Py{0.};
    double Pz{0.};
    double P{0.};
    double VertexX{0.};
    double VertexY{0.};
    double VertexZ{0.};
    double EndPointX{0.};
    double EndPointY{0.};
    double EndPointZ{0.};

ClassDef(DParticle, 3);
};

#endif //DSIMU_DPARTICLE_H
