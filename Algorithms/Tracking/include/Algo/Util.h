#ifndef SERENA_UTIL_H
#define SERENA_UTIL_H

//................................................................................//
//C++
#include <iostream>
#include <string>
#include <map>

//................................................................................//
//ROOT
#include "TMath.h"

//................................................................................//
//Tracking

//................................................................................//
//Constants
//................................................................................//
//Cluster control

//Physics process map
const std::map<std::string, int> ProcessTag = {{"",                      0},
                                               {"eBrem",                 1},
                                               {"conv",                  2},
                                               {"photonNuclear",         3},
                                               {"neutronInelastic",      4},
                                               {"hadElastic",            5},
                                               {"electronNuclear",       6},
                                               {"Decay",                 7},
                                               {"nCapture",              8},
                                               {"pi+Inelastic",          9},
                                               {"protonInelastic",       10},
                                               {"pi-Inelastic",          11},
                                               {"hBertiniCaptureAtRest", 12},
                                               {"compt",                 13},
                                               {"annihil",               14},
                                               {"muMinusCaptureAtRest",  15},
                                               {"eIoni",                 16},
                                               {"dInelastic",            17},
                                               {"kaon0LInelastic",       18},
                                               {"He3Inelastic",          19},
                                               {"GammaToMuPair",         20},
                                               {"phot",                  21},
                                               {"kaon0SInelastic",       22},
                                               {"alphaInelastic",        23},
                                               {"kaon+Inelastic",        24},
                                               {"tInelastic",            25},
                                               {"lambdaInelastic",       26},
                                               {"kaon-Inelastic",        27},
                                               {"sigma-Inelastic",       28},
                                               {"DMProcessDMBrem",       29}};

//................................................................................//
//Functions
//................................................................................//

//If particle pass through trackers
bool InTagTrack(double x, double y, double z);
bool InRecTrack(double x, double y, double z);

//
void LinearFit(double abr[], double x[], double y[], int n);
double PointToLineDistance(double A, double B, double C, double x, double y);
#endif
