#ifndef DSIMU_TRUTHMANAGER_H
#define DSIMU_TRUTHMANAGER_H

#include <map>
#include <string>
#include "Object/Util/PhysicsProcessDef.h"
#include "Object/Util/DetectorPVDef.h"
#include "Object/DTruth.h"

using std::map;

class DTruthParticle;

class DTruthProcess;

class DTruthState;

class DTruth;

class TruthManager {
public:
    static TruthManager *CreateInstance();

    ~TruthManager();

private:
    TruthManager();

public:
    // Delete Copy constructor
    TruthManager(const TruthManager &) = delete;

    TruthManager &operator=(TruthManager const &) = delete;

private:
    PhysicsDef pDef;
    DetPVDef pDet;

    DTruthParticle *truth_particle{};
    DTruthProcess *truth_process{};
    vector<DTruthState *> *truth_state{};

    vector<string> SD_regions = {
            "TagTrk_Strip_PV",
            "RecTrk_Strip_PV",
            "ECAL_PV",
            "HCAL_PV"
            "HCAL_APDWorld_PV",
    };

public:
    /** Get Physics Process Name from Index */
    string getProcessName(unsigned int idx);

    /** Get Physics Process Index from Name */
    unsigned int getProcessIndex(const string &name);

    /** Get Detector PV Name from Index */
    string getDetPVName(unsigned int idx);

    /** Get Detector PV Index from Name */
    unsigned int getDetPVIndex(const string &name);

    DTruthParticle *getTruthParticle() const {
        return truth_particle;
    }

    void setTruthParticle(DTruthParticle *truthParticle) {
        truth_particle = truthParticle;
    }

    DTruthProcess *getTruthProcess() const {
        return truth_process;
    }

    vector<DTruthState *> *getTruthState() const {
        return truth_state;
    }

    void setTruthState(vector<DTruthState *> *truthState) {
        truth_state = truthState;
    }

    /** Check if region is in the interested leak regions */
    bool checkInterest(const string &name);

    void Clean();
};


extern TruthManager *dTMgr;

#endif //DSIMU_TRUTHMANAGER_H
