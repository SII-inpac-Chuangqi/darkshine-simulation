//
// Created by Yulei on 2023/3/14.
// Copyright (c) 2023 Yulei. All rights reserved.
//

#ifndef DSIMU_ANIMATIONDATA_H
#define DSIMU_ANIMATIONDATA_H

#include <filesystem>
#include <array>
#include <unordered_map>
#include "map"
#include "string"
#include "vector"
#include "G4ThreeVector.hh"
#include "G4TouchableHistory.hh"
#include "SimulatedHit.h"

namespace fs = std::filesystem;


struct ThreeIntegersHash {
    size_t operator()(std::array<int, 3> ti) const {
        // combine the three integers into a single hash value
        size_t hash_value = 0;
        hash_value |= ti[0];
        hash_value <<= 8;
        hash_value |= ti[1];
        hash_value <<= 8;
        hash_value |= ti[2];
        return hash_value;
    }
};

struct ThreeIntegersEqual {
    bool operator()(std::array<int, 3> ti1, std::array<int, 3> ti2) const {
        // compare the three integers for equality
        return ti1[0] == ti2[0] && ti1[1] == ti2[1] && ti1[2] == ti2[2];
    }
};

using CellIDMap = std::unordered_map<std::array<int, 3>, std::vector<float>, ThreeIntegersHash, ThreeIntegersEqual>;

using AnimationMap = std::map<std::string, std::vector<float>>;
using AnimationStepMap = std::map<int, std::vector<float>>;
using AnimationHitMap = std::map<std::string, CellIDMap>;

class AnimationData {

public:
    AnimationData(const AnimationData &) = delete;

    AnimationData &operator=(AnimationData const &) = delete;

    static AnimationData *CreateInstance();

    void initialization();

    void save_event(int gen_event_id);

    void clean_data();

    void add_particle(float Ekin, int TrackID, int PDG, int MotherID, float t);

    void update_particle_end_time(int TrackID, float t);

    void add_particle_step(int TrackID, float x, float y, float z);

    void add_hit(
            const std::string &Det_Type, std::array<int, 3> cellID, double E_dep, double E_t,
            SimulatedHit *hit, G4TouchableHistory *touchable
    );

    bool if_first_step(int TrackID);

    void setUseAni(bool useAni) {
        use_ani = useAni;
    }

    void save_geometry(const std::string &filename);

private:
    AnimationData();

    bool use_ani = false;

    fs::path out_dir;

    // record properties of each particle
    AnimationMap particle_data;
    // record steps according to trackID
    AnimationStepMap step_x;
    AnimationStepMap step_y;
    AnimationStepMap step_z;
    AnimationStepMap step_t;

    // record E_dep with key "CellID" according to detectors
    AnimationHitMap energy_dep;
    AnimationHitMap energy_dep_time;
    AnimationHitMap cell_data;


    unsigned int event_id{0};
};

extern AnimationData *pAniData;
#endif //DSIMU_ANIMATIONDATA_H
