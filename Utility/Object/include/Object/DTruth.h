//
// Created by Zhang Yulei on 6/6/22.
//

#ifndef DSIMU_DTRUTH_H
#define DSIMU_DTRUTH_H

#include "TObject.h"
#include "Utility/TruthManager.h"
#include "Object/McParticle.h"
#include "Utility/PhysicsProcessDef.h"

#include <vector>
#include <map>

using std::vector, std::map, std::pair;

class DTruthProcess;

/*
 * Data structure to record MC particles in truth information
 * main features:
 * [all energies are kinetic energy]
 * 1. leaking energy
 *      1.1 Tracker
 *      1.2 ECAL
 *      1.3 HCAL
 * 2. remaining energy
 * 3. vertex position
 * 4. production process
 */
class DTruthParticle : public TObject {
public:
    int id = -1;
    int pdg = -1;
    float E_kin = 0.;
    float E_leak = 0.;
    float E_remain = 0.;
    float E_secondaries = 0.;
    float vertex[3] = {0., 0., 0.};
    unsigned int prod_process = 0;

    // map <{region index, process index}, deposit energy>
    map<pair<int, int>, float> E_deposit_details = {};

    DTruthProcess *prod_process_link = nullptr;
    vector<DTruthProcess *> sec_process_link = {};
    McParticle *mc_link = nullptr;

    ~DTruthParticle();

ClassDef(DTruthParticle, 1);
};

/*
 * Data structure to record processes in truth information
 * main features:
 * [all energies are kinetic energy]
 * 1. process energy
 *      = E_before - E_after
 * 2. vertex position
 * 3. process index/name
 * 4. in & out particles -> DTruthParticle / McParticle
 *
 * memory deleted by DTruth Particle
 */
class DTruthProcess : public TObject {
public:
    float E = 0.;
    float vertex[3] = {0., 0., 0.};
    unsigned int index = 0;

    DTruthParticle *in_p = nullptr;
    vector<DTruthParticle *> out_p = {};

ClassDef(DTruthProcess, 1);
};

/*
 * Data structure to record particle state in truth information
 * main features:
 *
 */
class DTruthState : public TObject {
public:
    float vertex[3] = {0., 0., 0.};
    float momentum[3] = {0., 0., 0.};
    float E = 0.;
    unsigned int process_index = 0;

ClassDef(DTruthState, 1);
};

/*
 *  Class to record all necessary truth information
 *
 *  EVENT TRUTH:
 *
 *  logic:
 *      particle -> process -> secondary particle -> process ...
 *
 *  particle features:
 *      1. leaking energy:
 *      2. remaining energy:
 *
 *  process importance:
 *     leaking energy and remaining energy of particles
 *
 *
 *  DETECTOR TRUTH:
 *  1. Tracker:
 *      recording selected tracks (determined by exp.)
 *
 *  2. ECAL:
 *      recording the incident particles at the surface
 */
class DTruth : public TObject {
public:

    enum class DTruthDetPV {
        All,
        Target,
        TagTrk,
        RecTrk,
        ECAL, // not available for trackers
        HCAL, // not available for trackers
    };

    map<DTruthDetPV, pair<float, float>> DetPV_Z{
            {DTruthDetPV::All,    {0.,         0.}},
            {DTruthDetPV::Target, {-0.35 / 2., +0.35 / 2.}},
            {DTruthDetPV::TagTrk, {-608.,      -0.35 / 2.}},
            {DTruthDetPV::RecTrk, {+0.35 / 2., 181}},
            {DTruthDetPV::ECAL,   {181,        630}},
            {DTruthDetPV::HCAL,   {630,        10000}},
    };

    DTruth();

    virtual ~DTruth();

    void Clean();

    void UpdateTruthParticle(DTruthParticle *dp);

    vector<DTruthState *> *InitTruthTrack(pair<int, int> idx);

    void UpdateTruthDetTrack(pair<int, int> idx, vector<DTruthState *> states);

    void removeTruthTrack(pair<int, int> idx);

    const vector<DTruthParticle *> &getTruthParticles() const {
        return truth_particles;
    }

    /** Loop the current process and link the parent process */
    void linkParentProcess(DTruthParticle *cur, int parent_id);

    /** Print the truth event topology */
    void printTruthTopology();

    void iteratorProcess(DTruthParticle *particle, const string& s1, const string& s2);

    /** Print the truth track information */
    void printTruthTracks();

    /** Print the truth Calorimeter information */
    void printTruthTracksCalorimeter();

    /*
     *  For Analysis
     */

    /** get sum of truth leak energy and escape energy */
    float getTruthLeakEnergy();
    float getTruthEscapeEnergy();

    /** Return the number of truth tracks */
    unsigned int getNTracks();

    /** Return all the keys of truth tracks (not applicable for ECAL/HCAL) */
    vector<pair<int, int>> getTracksKey(DTruthDetPV DetPV = DTruthDetPV::All, float E_min = 0);

    /** Return a vector of DTruthStates which has process energy larger than certain ratio */
    vector<DTruthState *>
    getCriticalProcessInTrack(pair<int, int> key, float ratio = 0., DTruthDetPV DetPV = DTruthDetPV::All);

    /** Return a map of tracks which in certain region */
    map<pair<int, int>, vector<DTruthState *>> getTracksInRegion(DTruthDetPV DetPV, float E_min = 0);

    /** Return a map of two DTruthStates (prev, post) of particles in certain region */
    map<pair<int, int>, pair<DTruthState *, DTruthState *>> getStatesInCalorimeter(DTruthDetPV DetPV, float E_min = 0);

    /** getStatesInCalorimeter for ECAL */
    map<pair<int, int>, pair<DTruthState *, DTruthState *>> getStatesInECAL(float E_min = 0);

    /** getStatesInCalorimeter for HCAL */
    map<pair<int, int>, pair<DTruthState *, DTruthState *>> getStatesInHCAL(float E_min = 0);

private:
    vector<DTruthParticle *> truth_particles;

    /** Store the truth tracks in tracker region (only charged particles) */
    // map< {track ID, particle PDG}, vector of states >
    map<pair<int, int>, vector<DTruthState *>> truth_tracks;

    /** Store the truth states in all detector boarders (maybe only particles with large E_kin)*/
    // map< {track ID, particle PDG}, vector of states >
    map<pair<int, int>, vector<DTruthState *>> truth_det_tracks;

    struct PhysicsDef PDtable;
    vector<string> dpdtable = PDtable.dPhyTypeVec;

ClassDef(DTruth, 1);
};


#endif //DSIMU_DTRUTH_H
