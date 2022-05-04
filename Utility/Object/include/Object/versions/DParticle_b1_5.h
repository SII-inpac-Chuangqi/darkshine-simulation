//
// Created by Zhang Yulei on 9/18/20.
//

#ifndef DSIMU_DPARTICLE_B1_5_H
#define DSIMU_DPARTICLE_B1_5_H

#include <string>
#include <cmath>

#include "TObject.h"

class DParticle_b1_5 : public TObject {
public:
    // Constructor and Destructor
    DParticle_b1_5() = default;

    DParticle_b1_5(const DParticle_b1_5 &rhs) : TObject(rhs) {
        *this = rhs;
    }

    ~DParticle_b1_5() override = default;

    bool operator==(const DParticle_b1_5 &rhs) const {
        return id == rhs.id &&
               Name == rhs.Name &&
               CreateProcess == rhs.CreateProcess &&
               PDG == rhs.PDG &&
               Mass == rhs.Mass &&
               Energy == rhs.Energy &&
               Px == rhs.Px &&
               Py == rhs.Py &&
               Pz == rhs.Pz &&
               VertexX == rhs.VertexX &&
               VertexY == rhs.VertexY &&
               VertexZ == rhs.VertexZ &&
               EndPointX == rhs.EndPointX &&
               EndPointY == rhs.EndPointY &&
               EndPointZ == rhs.EndPointZ;
    }

    bool operator!=(const DParticle_b1_5 &rhs) const {
        return !(rhs == *this);
    }

    DParticle_b1_5 &operator=(const DParticle_b1_5 &rhs) {
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

    float getMass() const {
        return Mass;
    }

    float getEnergy() const {
        return Energy;
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

    float getVertexX() const {
        return VertexX;
    }

    float getVertexY() const {
        return VertexY;
    }

    float getVertexZ() const {
        return VertexZ;
    }

    float getEndPointX() const {
        return EndPointX;
    }

    float getEndPointY() const {
        return EndPointY;
    }

    float getEndPointZ() const {
        return EndPointZ;
    }

    const std::string &getName() const {
        return Name;
    }

    const std::string &getCreateProcess() const {
        return CreateProcess;
    }

    float getP() const {
        return sqrt(Px * Px + Py * Py + Pz * Pz);
    }

    // Set Methods
    void setId(int ID) {
        DParticle_b1_5::id = ID;
    }

    void setPdg(int pdg) {
        PDG = pdg;
    }

    void setMass(float mass) {
        Mass = mass;
    }

    void setEnergy(float energy) {
        Energy = energy;
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

    void setVertexX(float vertexX) {
        VertexX = vertexX;
    }

    void setVertexY(float vertexY) {
        VertexY = vertexY;
    }

    void setVertexZ(float vertexZ) {
        VertexZ = vertexZ;
    }

    void setEndPointX(float endPointX) {
        EndPointX = endPointX;
    }

    void setEndPointY(float endPointY) {
        EndPointY = endPointY;
    }

    void setEndPointZ(float endPointZ) {
        EndPointZ = endPointZ;
    }

    void setName(const std::string &name) {
        Name = name;
    }

    void setCreateProcess(const std::string &createProcess) {
        CreateProcess = createProcess;
    }


protected:
    // internal debug only
    int id{0};

    // internal String
    std::string Name;
    std::string CreateProcess;

    int PDG{0};
    float Mass{0.};
    float Energy{0.};
    float Px{0.};
    float Py{0.};
    float Pz{0.};
    float VertexX{0.};
    float VertexY{0.};
    float VertexZ{0.};
    float EndPointX{0.};
    float EndPointY{0.};
    float EndPointZ{0.};

ClassDefOverride(DParticle_b1_5, 3);
};

#endif //DSIMU_DPARTICLE_B1_5_H
