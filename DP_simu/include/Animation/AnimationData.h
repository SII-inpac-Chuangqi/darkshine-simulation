//
// Created by Yulei on 2023/3/14.
// Copyright (c) 2023 Yulei. All rights reserved.
//

#ifndef DSIMU_ANIMATIONDATA_H
#define DSIMU_ANIMATIONDATA_H

#include <filesystem>
#include "map"
#include "string"
#include "vector"

namespace fs = std::filesystem;

using AnimationMap = std::map<std::string, std::vector<float>>;
using AnimationStepMap = std::map<int, std::vector<float>>;

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

    bool if_first_step(int TrackID);

    void setUseAni(bool useAni) {
        use_ani = useAni;
    }

    void save_geometry(const std::string& filename);

private:
    AnimationData();

    bool use_ani = false;

    fs::path out_dir;

    AnimationMap particle_data;
    AnimationStepMap step_x;
    AnimationStepMap step_y;
    AnimationStepMap step_z;
    AnimationStepMap step_t;

    unsigned int event_id{0};
};

extern AnimationData *pAniData;
#endif //DSIMU_ANIMATIONDATA_H
