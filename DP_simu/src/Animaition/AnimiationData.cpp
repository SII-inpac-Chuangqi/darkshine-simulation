//
// Created by Yulei on 2023/3/14.
// Copyright (c) 2023 Yulei. All rights reserved.
//

#include "Animation/AnimationData.h"
#include "nlohmann/json.hpp"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <utility>
#include <TSystem.h>
#include <G4Box.hh>
#include <sstream>
#include <cmath>

#include "TGeoManager.h"
#include "TFile.h"

using nlohmann::json;

// Required by Singleton
AnimationData *pAniData = nullptr;

float format_number(float number) {
    return number;
    
//    int int_part = static_cast<int>(number);
//    float decimal_part = std::abs(number - int_part);
//
//    std::stringstream ss;
//    ss.setf(std::ios::fixed);
//    if (decimal_part == 0) {
//        ss << std::setprecision(0) << number;
//    } else {
//        ss << std::setprecision(2) << number;
//    }
//
//    float formatted_number;
//    ss >> formatted_number;
//
//    return formatted_number;
}

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

//    std::cout << "[Animation Manager] ==> Save event " << event_id << " to " << event_out_dir << std::endl;

    json j;

    json j_particle(particle_data);
    json j_step_x(step_x);
    json j_step_y(step_y);
    json j_step_z(step_z);
    json j_step_t(step_t);
    json j_hit_e(energy_dep);
    json j_hit_t(energy_dep_time);
    json j_cell_info(cell_data);

    j["gen_event_id"] = gen_event_id;
    j["particle"] = j_particle;
    j["step_x"] = j_step_x;
    j["step_y"] = j_step_y;
    j["step_z"] = j_step_z;
    j["step_t"] = j_step_t;
    j["hit_e"] = j_hit_e;
    j["hit_t"] = j_hit_t;
    j["cell_info"] = j_cell_info;

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

    auto clean_animation_hit = [](AnimationHitMap &data) {
        for (auto &det: data) {
            for (auto &it: det.second) {
                it.second.clear();
            }
            det.second.clear();
        }
    };

    clean_animation_hit(energy_dep);
    clean_animation_hit(energy_dep_time);
    clean_animation_hit(cell_data);
}

void AnimationData::add_particle(float Ekin, int TrackID, int PDG, int MotherID, float t) {
    if (!use_ani) return;

    particle_data.at("particle_Ekin").push_back(format_number(Ekin));
    particle_data.at("particle_TrackID").push_back(static_cast<float>(TrackID));
    particle_data.at("particle_PDG").push_back(static_cast<float>(PDG));
    particle_data.at("particle_MotherID").push_back(static_cast<float>(MotherID));

    step_x.insert({TrackID, std::vector<float>()});
    step_y.insert({TrackID, std::vector<float>()});
    step_z.insert({TrackID, std::vector<float>()});
    step_t.insert({TrackID, {format_number(t)}});
}

void AnimationData::update_particle_end_time(int TrackID, float t) {
    if (!use_ani) return;
    step_t.at(TrackID).push_back(format_number(t));
}

void AnimationData::add_particle_step(int TrackID, float x, float y, float z) {
    if (!use_ani) return;

    step_x.at(TrackID).push_back(format_number(x));
    step_y.at(TrackID).push_back(format_number(y));
    step_z.at(TrackID).push_back(format_number(z));
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

    std::vector<std::string> detector_list = {"TagTrk", "RecTrk", "ECAL", "HCAL", "SideHCAL"};
    for (const auto &key: detector_list) {
        energy_dep.insert({key, CellIDMap()});
        energy_dep_time.insert({key, CellIDMap()});
        cell_data.insert({key, CellIDMap()});
    }

    out_dir = "Animation_Output";
    fs::create_directories(out_dir);
}

void AnimationData::add_hit(
        const std::string &Det_Type,
        std::array<int, 3> cellID,
        double E_dep,
        double E_t,
        SimulatedHit *hit,
        G4TouchableHistory *touchable,
        double rotation) {

    if (!use_ani) return;
    if (energy_dep.find(Det_Type) == energy_dep.end()) return;

    if (energy_dep.at(Det_Type).find(cellID) == energy_dep.at(Det_Type).end()) {
        energy_dep.at(Det_Type).insert({cellID, {format_number(static_cast<float>(E_dep))}});
        energy_dep_time.at(Det_Type).insert({cellID, {format_number(static_cast<float>(E_t))}});

        cell_data.at(Det_Type).insert(
                {
                        cellID,
                        {
                                format_number(hit->getX()),
                                format_number(hit->getY()),
                                format_number(hit->getZ()),
                                format_number(static_cast<float>(((G4Box *) touchable->GetSolid(0))->GetXHalfLength())),
                                format_number(static_cast<float>(((G4Box *) touchable->GetSolid(0))->GetYHalfLength())),
                                format_number(static_cast<float>(((G4Box *) touchable->GetSolid(0))->GetZHalfLength()))
                        }
                }
        );

        if (rotation != 0) {
            cell_data.at(Det_Type).at(cellID).push_back(static_cast<float>(rotation));
        }
    }
    energy_dep.at(Det_Type).at(cellID).push_back(static_cast<float>(E_dep));
    energy_dep_time.at(Det_Type).at(cellID).push_back(static_cast<float>(E_t));
}


