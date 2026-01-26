//
// Created by Zhang Yulei on 6/6/22.
//

#include "DTruth.h"
#include "TString.h"
#include "TVector3.h"

#include <cmath>
#include <iostream>
#include <iomanip>
#include <numeric>

DTruth::DTruth() = default;

void DTruth::UpdateTruthParticle(DTruthParticle *dp) {
    // Waiting for more algos to optimize
    truth_particles.emplace_back(dp);
}

void DTruth::Clean() {

    for (auto itr: truth_particles) {
        delete itr;
    }
    truth_particles.clear();
    truth_particles.shrink_to_fit();

    auto clean_state = [](map <pair<int, int>, vector<DTruthState *>> *maps) {
        for (auto itr: *maps) {
            for (auto item: itr.second) {
                delete item;
            }
            itr.second.clear();
            itr.second.shrink_to_fit();
        }
        maps->clear();
    };

    clean_state(&truth_tracks);
    clean_state(&truth_det_tracks);

}

void DTruth::linkParentProcess(DTruthParticle *cur, int parent_id) {
    auto position_equal = [](DTruthParticle *c, DTruthProcess *p) {
        return std::fabs(c->vertex[0] - p->vertex[0]) < 1e-6
               && std::fabs(c->vertex[1] - p->vertex[1]) < 1e-6
               && std::fabs(c->vertex[2] - p->vertex[2]) < 1e-6;
    };

    for (auto p: truth_particles) {
        if (p->id == parent_id) {
            for (auto pro: p->sec_process_link) {
                if (pro->index == cur->prod_process && position_equal(cur, pro)) {
                    cur->prod_process_link = pro;
                    pro->out_p.push_back(cur);
                    break;
                }
            }
            break;
        }
    }
}

void DTruth::iteratorProcess(DTruthParticle *particle, const string &s1, const string &s2) {
    int indent = 0;

    string ss = "                       ";
    string align = "    ";

    auto printParticle = [indent](DTruthParticle *p, const string &s) {
        auto leak_str = (p->E_leak > 0.) ? Form(", E_leak = %.1f", p->E_leak) : "";
        auto remain_str = (p->E_remain > 0.) ? Form(", E_escape = %.1f", p->E_remain) : "";
        cout << std::setw(indent) << s << Form("Particle %d (%d),  E = %.1f",
                                               p->id, p->pdg, p->E_kin) << leak_str << remain_str << endl;
    };

    auto printProcess = [indent](DTruthProcess *p, const string &s, const string &name) {
        cout << std::setw(indent)
             << s << Form(" %s, Vertex = (%4.1f,%4.1f,%4.1f), Energy = %4.1f ",
                          name.c_str(), p->vertex[0], p->vertex[1], p->vertex[2], p->E) << endl;
    };

    vector < DTruthProcess * > process = particle->sec_process_link;
    unsigned int n_pro = process.size();
    unsigned int n_par;

    string par_name = dpdtable.at(particle->prod_process);
    printParticle(particle, ss + s1);

    if (n_pro != 0) {
        for (auto pro: process) {
            string pro_name = dpdtable.at(pro->index);
            printProcess(pro, ss + s2, pro_name);
            n_par = pro->out_p.size();
            if (n_par != 0) {
                for (auto par: pro->out_p) { iteratorProcess(par, align + s1, align + s2); }
            }
        }
    }
}

void DTruth::printTruthTopology() {

    string space1;
    string space2 = "|--";

    cout << "===========> Event Topology <===========" << endl;

    cout << "[DTruth: Truth Energy Info] ==> Energy [MeV] Leakage in Rack/Wrapper: " << getTruthLeakEnergy() << endl;
    cout << "[DTruth: Truth Energy Info] ==> Energy [MeV] Escape out of the World: " << getTruthEscapeEnergy() << endl;

//    cout << " [DTruth : Particle Info] ==> Particle ID, E_kin, process_size" << endl;
//    cout << " [DTruth : Process  Info] ==> |-- process_name, Vertex_Z,  Energy" << endl;
    cout << "               " << endl;

//    for (auto par: truth_particles) {
//        cout << " [DTruth : particle] : " << endl ;
//
//        iteratorProcess(par, space1, space2);
//        cout << endl;
//    }
    if (!truth_particles.empty()) {
        cout << " [DTruth : particle] : " << endl;
        iteratorProcess(truth_particles.front(), space1, space2);
        cout << endl;
    }
}

vector<DTruthState *> *DTruth::InitTruthTrack(pair<int, int> idx) {
    if (truth_tracks.count(idx) == 0) {
        truth_tracks.insert({idx, vector<DTruthState *>({})});
    }
    return &truth_tracks.at(idx);
}

void DTruth::UpdateTruthDetTrack(pair<int, int> idx, vector<DTruthState *> states) {
    if (truth_det_tracks.count(idx) == 0) {
        truth_det_tracks.insert({idx, vector<DTruthState *>({})});
    }

    truth_det_tracks.at(idx).insert(truth_det_tracks.at(idx).end(), states.begin(), states.end());
}

DTruth::~DTruth() {
    Clean();
}

void DTruth::removeTruthTrack(pair<int, int> idx) {
    if (auto i = truth_tracks.find(idx); i != truth_tracks.end()) {
        truth_tracks.erase(i);
    } else
        cerr << "[DTruth::removeTruthTrack] ==> There is no key named: " << idx.first << ", " << idx.second << endl;
}

unsigned int DTruth::getNTracks() {
    return truth_tracks.size();
}

void DTruth::printTruthTracks() {
    cout << "[DTruth: Truth Tracks Info] ==> # of Tracks: " << getNTracks() << endl;
    auto keys = getTracksKey(DTruthDetPV::All);
    for (auto key: keys) {
        cout << "[DTruth: Truth Tracks Info] ==> Keys (trackID, PDG): " << key.first << ", " << key.second << endl;
        if (auto n = getCriticalProcessInTrack(key, 0.1, DTruthDetPV::TagTrk).size(); n > 0)
            cout << "\t\t # of processes (deltaE / E > 0.1 ) in TagTrk: " << n << endl;
        if (auto n = getCriticalProcessInTrack(key, 0.1, DTruthDetPV::RecTrk).size(); n > 0)
            cout << "\t\t # of processes (deltaE / E > 0.1 ) in RecTrk: " << n << endl;
        if (auto n = getCriticalProcessInTrack(key, 0.1, DTruthDetPV::Target).size(); n > 0)
            cout << "\t\t # of processes (deltaE / E > 0.1 ) in Target: " << n << endl;
    }
}

void DTruth::printTruthTracksCalorimeter() {
    auto ecal = getStatesInECAL();
    auto hcal = getStatesInHCAL();

    auto print = [](const map <pair<int, int>, pair<DTruthState *, DTruthState *>> &cal) {
        for (auto p: cal) {
            TVector3 mom(p.second.first->momentum[0], p.second.first->momentum[1], p.second.first->momentum[2]);

            cout << "\t\t TrackID: " << std::setw(3) << p.first.first << " PDG: " << std::setw(4) << p.first.second;
            cout << ", E: " << std::setw(8) << p.second.first->E << " [MeV]";
            cout << ", Angle with z axis: " << std::setw(8) << mom.Theta() / TMath::Pi() * 180. << " [deg]";
            cout << endl;
        }
    };

    cout << "[DTruth: Truth ECAL Info] ==> # of Tracks passing through ECAL: " << ecal.size() << endl;
    print(ecal);
    cout << "[DTruth: Truth HCAL Info] ==> # of Tracks passing through HCAL: " << hcal.size() << endl;
    print(hcal);
}

vector <pair<int, int>> DTruth::getTracksKey(DTruthDetPV DetPV, float E_min) {
    vector <pair<int, int>> keys = {};
    for (auto track: getTracksInRegion(DetPV)) {
        if (track.second.front()->E >= E_min) {
            keys.push_back(track.first);
        }
    }
    return keys;
}

map <pair<int, int>, vector<DTruthState *>> DTruth::getTracksInRegion(DTruthDetPV DetPV, float E_min, int min_hits) {
    map <pair<int, int>, vector<DTruthState *>> result = {};

    for (auto track: truth_tracks) {
        // Ensure the initial Energy should larger than the min E_kin
        if (track.second.front()->E >= E_min && track.second.size() >= static_cast<size_t>(min_hits)) {
            // Find the DTruthStates in certain Detector Region
            if (DetPV == DTruthDetPV::All) result.insert(track);
            else {
                int n_hits = 0;

                auto t = vector<DTruthState *>();
                // Loop all states in one track to retrieve states in required region
                for (auto itr = track.second.begin(); itr != track.second.end(); ++itr) {
                    if ((*itr)->vertex[2] > DetPV_Z.at(DetPV).first && (*itr)->vertex[2] < DetPV_Z.at(DetPV).second)
                    {
                        // insert the initial state before entering the region
                        if ((itr != track.second.begin()) && t.empty()) t.push_back(*(itr - 1));
                        // insert the current state if satisfied z range
                        t.push_back((*itr));

                        n_hits++;
                    }
                }
                if (!t.empty() && n_hits >= min_hits) result.insert({track.first, t});
            }
        }
    }
    return result;
}

vector<DTruthState *> DTruth::getCriticalProcessInTrack(pair<int, int> key, float ratio, DTruthDetPV DetPV) {
    auto tracks = getTracksInRegion(DetPV);
    auto track = tracks.find(key);
    if (tracks.empty() || track == tracks.end()) {
//        cerr << "[DTruth::getCriticalProcessInTrack] ==> No selected track with key: " << key.first << ", "
//             << key.second << endl;
        return {};
    }

    if ((*track).second.size() < 2) return (*track).second;

    auto result = vector<DTruthState *>();
    for (auto itr = (*track).second.begin() + 1; itr != (*track).second.end(); ++itr) {
        if (fabs((*itr)->E - (*(itr - 1))->E) > ratio * (*(itr - 1))->E) result.push_back((*itr));
    }

    return result;
}

map <pair<int, int>, pair<DTruthState *, DTruthState *>>
DTruth::getStatesInCalorimeter(DTruth::DTruthDetPV DetPV, float E_min) {
    /*
     * Output format:
     *  map<{trackID, PDG}, {prev_state, post_state}>
     *  prev_state: state just before entering the detector PV
     *  post_state: state just after entering the detector PV
     */
    map <pair<int, int>, pair<DTruthState *, DTruthState *>> result = {};

    for (auto track: truth_det_tracks) {
        auto p = pair<DTruthState *, DTruthState *>({nullptr, nullptr});

        assert(track.second.size() != 2 || track.second.size() != 4);
        for (unsigned int i = 1; i < track.second.size(); i = i + 2) {
            if (track.second.at(i)->vertex[2] > DetPV_Z.at(DetPV).first &&
                track.second.at(i)->vertex[2] <= DetPV_Z.at(DetPV).second) {
                p.first = track.second.at(i - 1);
                p.second = track.second.at(i);
            }
        }
        // Ensure the initial Energy should larger than the min E_kin
        if (p.first && p.first->E >= E_min)
            result.insert({track.first, p});
    }

    return result;
}

map <pair<int, int>, pair<DTruthState *, DTruthState *>> DTruth::getStatesInECAL(float E_min) {
    return getStatesInCalorimeter(DTruthDetPV::ECAL, E_min);
}

map <pair<int, int>, pair<DTruthState *, DTruthState *>> DTruth::getStatesInHCAL(float E_min) {
    return getStatesInCalorimeter(DTruthDetPV::HCAL, E_min);
}

float DTruth::getTruthLeakEnergy() {
    return std::accumulate(truth_particles.begin(), truth_particles.end(), float(0),
                           [](float a, DTruthParticle *b) { return a + b->E_leak; });
}

float DTruth::getTruthEscapeEnergy() {
    return std::accumulate(truth_particles.begin(), truth_particles.end(), float(0),
                           [](float a, DTruthParticle *b) { return a + b->E_remain; });
}

DTruthParticle::~DTruthParticle() {
    for (auto itr: sec_process_link) {
        delete itr;
    }
    sec_process_link.clear();
    sec_process_link.shrink_to_fit();
}
