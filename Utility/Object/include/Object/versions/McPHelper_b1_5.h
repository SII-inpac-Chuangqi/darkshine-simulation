//
// Created by xuliang on 2022/5/4.
//

#ifndef DSIMU_MCPHELPER_B1_5_H
#define DSIMU_MCPHELPER_B1_5_H

#include "Object/McParticle.h"

class McPHelper_b1_5 : public DParticle {
public:
    McPHelper_b1_5() = default;
private:
    int CellID{0};

    int CellID_X{0};
    int CellID_Y{0};
    int CellID_Z{0};

    int Detector{nNone};

    bool is_incoming{false};

    McParticle* MCParticleCollectionLink;

};

#endif //DSIMU_MCPHELPER_B1_5_H
