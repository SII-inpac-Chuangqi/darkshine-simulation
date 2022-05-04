//
// Created by xuliang on 2022/5/4.
//

#ifndef DSIMU_MCPHELPER_B1_5_H
#define DSIMU_MCPHELPER_B1_5_H

#include "Object/DHit.h"
#include "Object/McParticle.h"
#include "TVector3.h"

class McPHelper_b1_5 : public DHit {
public:
    McPHelper_b1_5() = default;

    McPHelper_b1_5(const McPHelper_b1_5 &);

    ~McPHelper_b1_5() override = default;

    bool operator == (const McPHelper_b1_5 &rhs) const;

    bool operator != (const McPHelper_b1_5 &rhs) const;

    McPHelper_b1_5 &operator=(const McPHelper_b1_5 &rhs);

    /// getters
    int getPdg() const {
        return PDG;
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

    float getP() const{
        auto p = TVector3(Px,Py,Pz);
        return p.Mag();
    }

    float getMass() const {
        return Mass;
    }

    bool isIncoming() const {
        return is_incoming;
    }

    McParticle *getMcParticle() const {
        return MCParticle;
    }
    /// setters
    void setPdg(int pdg) {
        PDG = pdg;
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

    void setMass(float mass) {
        Mass = mass;
    }

    void setIsIncoming(bool isIncoming) {
        is_incoming = isIncoming;
    }

    void setMcParticle(McParticle *mcParticle) {
        MCParticle = mcParticle;
    }

    /// Dump
    friend std::ostream &operator<<(std::ostream &os, const McPHelper_b1_5 &particle) {
        TString str(Form("| %-5d | %-6d | %8.3f, %8.3f, %8.3f | %8.3f,  %8.3f | %8.3f, %8.3f, %8.3f | %-6d %-6d | %-6d |",
                         particle.id,
                         particle.PDG,
                         particle.Px, particle.Py, particle.Pz,
                         particle.E, particle.Mass,
                         particle.X, particle.Y, particle.Z,
                         particle.Detector, particle.CellID,
                         particle.MCParticle ? particle.MCParticle->getId() : 0));
                         //particle.CreateProcess.data(), (particle.getParents()) ? particle.getParents()->getId() : 0));

        os << str;

        return os;
    }
private:
    int PDG;
    float Px;
    float Py;
    float Pz;
    float Mass;

    /// true - prev step is outside the crystal, and post step is in the crystal. false - vice versa
    bool is_incoming{false};

    McParticle* MCParticle{nullptr};

};

#endif //DSIMU_MCPHELPER_B1_5_H
