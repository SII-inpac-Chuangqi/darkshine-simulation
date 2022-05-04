//
// Created by xuliang on 2022/5/4.
//

#ifndef DSIMU_MCPHELPER_B1_5_H
#define DSIMU_MCPHELPER_B1_5_H

#include "Object/McParticle.h"

class McPHelper_b1_5 : public DParticle {
public:
    McPHelper_b1_5() = default;

    /// getters
    int getDetector() const {
        return Detector;
    }

    int getCellId() const {
        return CellID;
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

    bool isIncoming() const {
        return is_incoming;
    }

    McParticle *getMcParticleCollectionLink() const {
        return MCParticleCollectionLink;
    }

    /// setters
    void setDetector(int detector) {
        Detector = detector;
    }

    void setCellId(int cellId) {
        CellID = cellId;
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

    void setIsIncoming(bool isIncoming) {
        is_incoming = isIncoming;
    }

    void setMcParticleCollectionLink(McParticle *mcParticleCollectionLink) {
        MCParticleCollectionLink = mcParticleCollectionLink;
    }

private:
    /// Detector information
    int Detector{nNone};

    int CellID{0};
    int CellID_X{0};
    int CellID_Y{0};
    int CellID_Z{0};

    /// true - prev step is outside the crystal, and post step is in the crystal. false - vice versa
    bool is_incoming{false};

    McParticle* MCParticleCollectionLink;

};

#endif //DSIMU_MCPHELPER_B1_5_H
