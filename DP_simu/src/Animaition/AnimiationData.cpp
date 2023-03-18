//
// Created by Yulei on 2023/3/14.
// Copyright (c) 2023 Yulei. All rights reserved.
//

#include "Animation/AnimationData.h"
#include "nlohmann/json.hpp"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <TSystem.h>

#include "TGeoManager.h"
#include "TFile.h"

using nlohmann::json;

// Required by Singleton
AnimationData *pAniData = nullptr;

// Get Instance Class
AnimationData *AnimationData::CreateInstance() {
    if (pAniData == nullptr)
        pAniData = new AnimationData();

    return pAniData;
}

AnimationData::AnimationData() = default;

void AnimationData::save_event(int gen_event_id) {
    if (!use_ani) return;

    fs::path event_out_dir = out_dir / std::string("event" + std::to_string(event_id) + ".json");

    std::cout << "[Animation Manager] ==> Save event " << event_id << " to " << event_out_dir << std::endl;

    json j;

    json j_particle(particle_data);
    json j_step_x(step_x);
    json j_step_y(step_y);
    json j_step_z(step_z);
    json j_step_t(step_t);

    j["particle"] = j_particle;
    j["step_x"] = j_step_x;
    j["step_y"] = j_step_y;
    j["step_z"] = j_step_z;
    j["step_t"] = j_step_t;

    std::ofstream out_file(event_out_dir, std::ios::out | std::ios::trunc);
    out_file << j << std::endl;
    out_file.close();

    event_id++;
    clean_data();
}

void AnimationData::clean_data() {
    if (!use_ani) return;

    for (auto &it: particle_data) {
        it.second.clear();
    }

    auto clean_animation_data = [](AnimationStepMap &data) {
        for (auto &it: data) {
            it.second.clear();
        }
        data.clear();
    };

    clean_animation_data(step_x);
    clean_animation_data(step_y);
    clean_animation_data(step_z);
    clean_animation_data(step_t);
}

void AnimationData::add_particle(float Ekin, int TrackID, int PDG, int MotherID, float t) {
    if (!use_ani) return;

    particle_data.at("particle_Ekin").push_back(Ekin);
    particle_data.at("particle_TrackID").push_back(TrackID);
    particle_data.at("particle_PDG").push_back(PDG);
    particle_data.at("particle_MotherID").push_back(MotherID);

    step_x.insert({TrackID, std::vector<float>()});
    step_y.insert({TrackID, std::vector<float>()});
    step_z.insert({TrackID, std::vector<float>()});
    step_t.insert({TrackID, {t}});
}

void AnimationData::update_particle_end_time(int TrackID, float t) {
    step_t.at(TrackID).push_back(t);
}

void AnimationData::add_particle_step(int TrackID, float x, float y, float z) {
    if (!use_ani) return;

    step_x.at(TrackID).push_back(x);
    step_y.at(TrackID).push_back(y);
    step_z.at(TrackID).push_back(z);
}

bool AnimationData::if_first_step(int TrackID) {
    if (!use_ani) return false;

    return step_x.at(TrackID).empty();
}

void AnimationData::save_geometry(const std::string &filename) {
    if (!use_ani) return;

    fs::path geo_out_file = out_dir / std::string("geometry.root");

    TFile geo_file(geo_out_file.c_str(), "RECREATE");

    gSystem->Load("libGeom");
    TGeoManager::Import(filename.data());

    geo_file.cd();
    gGeoManager->Write("DetGeoManager");
    geo_file.Close();

    std::cout << "[Animation Manager] ==> Save geometry to " << geo_out_file << std::endl;
}

void AnimationData::initialization() {
    if (!use_ani) return;
    const unsigned int particle_size = 1000;

    std::map<std::string, unsigned int> particle_map = {
            {"particle_Ekin",     particle_size},
            {"particle_TrackID",  particle_size},
            {"particle_PDG",      particle_size},
            {"particle_MotherID", particle_size},
    };

    for (const auto &[key, value]: particle_map) {
        particle_data.insert({key, std::vector<float>()});

        particle_data.at(key).reserve(value);
    }


    out_dir = "Animation_Output";
    fs::create_directories(out_dir);
}
