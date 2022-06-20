#include "Utility/UTIL/include/Utility/TruthManager.h"
#include <algorithm>
#include "TString.h"

// Required by Singleton
TruthManager *dTMgr = nullptr;

// Get Instance Class
TruthManager *TruthManager::CreateInstance() {
    if (dTMgr == nullptr)
        dTMgr = new TruthManager();

    return dTMgr;
}

TruthManager::TruthManager() = default;

string TruthManager::getProcessName(unsigned int idx) {
    try {
        return pDef.dPhyTypeVec.at(idx);
    } catch (const std::exception &e) {
        cerr << "[Physics Process Table] ==> Out of range error for index: " << idx << endl;
        return pDef.dPhyTypeVec.at(0);
    }
}

unsigned int TruthManager::getProcessIndex(const string &name) {
    auto tmp2 = TString(name);
    TString tmp3;
    if (tmp2.Contains("biasWrapper"))
        tmp3 = tmp2(tmp2.Index("(") + 1, tmp2.Index(")") - tmp2.Index("(") - 1);
    else
        tmp3 = tmp2;

    if (auto re = pDef.dPhyTypeMap.find(tmp3.Data()); re != pDef.dPhyTypeMap.end()) {
        return re->second;
    }
    else {
        cerr << "[Physics Process Table] ==> Out of range error for Name: " << tmp3 << endl;
        return 0;
    }
}

TruthManager::~TruthManager() {
    delete truth_particle;
}

bool TruthManager::checkInterest(const string &name) {
    // select only leak region (not SD region)
    return std::find(SD_regions.begin(), SD_regions.end(), name) == SD_regions.end();
}

string TruthManager::getDetPVName(unsigned int idx) {
    try {
        return pDet.dDetPVVec.at(idx);
    } catch (const std::exception &e) {
        cerr << "[Detector PV Table] ==> Out of range error for index: " << idx << endl;
        return pDet.dDetPVVec.at(0);
    }
}

unsigned int TruthManager::getDetPVIndex(const string &name) {
    if (auto re = pDet.dDetPVMap.find(name); re != pDet.dDetPVMap.end()) {
        return re->second;
    }
    else {
        cerr << "[Detector PV Table] ==> Out of range error for Name: " << name << endl;
        return 0;
    }
}

void TruthManager::Clean() {
    delete truth_particle;
}



