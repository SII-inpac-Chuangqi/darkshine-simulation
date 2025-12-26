//
// Created by Zhang Yulei on 8/3/21.
//

#ifndef DSIMU_FASTSMEAR_H
#define DSIMU_FASTSMEAR_H

#include "Event/EventStoreAndWriter.h"
#include <memory>
#include <string>
#include <map>
#include <vector>
#include <cfloat>
#include <TRandom3.h>
#include <TF1.h>


using std::string;
using std::shared_ptr;
using std::map;
using std::vector;
using std::pair;

struct Calibration_Table {
    Double_t (TRandom::*funcPtr)(Double_t, Double_t); // Performance much better than TF1.GetRandom()
    // Energy Unit: MeV
    vector<double> E;
    vector<double> A;
    vector<double> B;
    vector<double> C;
};

class FastSmear {
public:
    explicit FastSmear(const shared_ptr<EventStoreAndWriter> &evtWrt) : EvtWrt(evtWrt) {
        cal_info.insert(pair<string, vector<Calibration_Table>>(ECAL_Name, vector<Calibration_Table>()));
        cal_info.insert(pair<string, vector<Calibration_Table>>(HCAL_Name, vector<Calibration_Table>()));
        cal_info.insert(pair<string,vector<Calibration_Table>>(SideHCAL_Name,vector<Calibration_Table>()));
    }

    void RegisterParameters(const std::string& cf, bool aecal, bool ahcal, int seed);

    void ReadConfigFile();

    void Process(AnaEvent* evt, const string& det_name, const string &cal_name);

    static double CalculateSigma(double E, const Calibration_Table& ct);

    static double MidPoint(double x1, double x2, double y1, double y2, double x);

private:
    // Random Number
    TRandom3 rnd;

    shared_ptr<EventStoreAndWriter> EvtWrt;

    string config_file;
    bool apply_to_ecal{};
    bool apply_to_hcal{};

    string ECAL_Name = "ECAL";
    string HCAL_Name = "HCAL";
    string SideHCAL_Name = "SideHCAL";
    map<string, vector<Calibration_Table>> cal_info;
};


#endif //DSIMU_FASTSMEAR_H
