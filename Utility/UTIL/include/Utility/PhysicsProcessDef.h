//
// Created by Zhang Yulei on 6/6/22.
//

#ifndef DSIMU_PHYSICSPROCESSDEF_H
#define DSIMU_PHYSICSPROCESSDEF_H

#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>

using std::cout, std::endl, std::cerr;
using std::unordered_map, std::vector, std::string;

struct PhysicsDef {

    /** the vector of process types */
    vector<string> dPhyTypeVec = {
            "Unknown",                          // 0
            "Transportation",                   // 1
            "msc",                              // 2
            "electronNuclear",                  // 3
            "Decay",                            // 4
            "conv",                             // 5
            "pi+Inelastic",                     // 6
            "eBrem",                            // 7
            "neutronInelastic",                 // 8
            "lambdaInelastic",                  // 9
            "dInelastic",                       // 10
            "protonInelastic",                  // 11
            "hBertiniCaptureAtRest",            // 12
            "photonNuclear",                    // 13
            "tInelastic",                       // 14
            "kaon-Inelastic",                   // 15
            "pi-Inelastic",                     // 16
            "He3Inelastic",                     // 17
            "compt",                            // 18
            "kaon0LInelastic",                  // 19
            "kaon0SInelastic",                  // 20
            "eIoni",                            // 21
            "kaon+Inelastic",                   // 22
            "muMinusCaptureAtRest",             // 23
            "sigma-Inelastic",                  // 24
            "alphaInelastic",                   // 25
            "sigma+Inelastic",                  // 26
            "annihil",                          // 27
            "ionInelastic",                     // 28
            "hadElastic",                       // 29
            "phot",                             // 30
            "xi0Inelastic",                     // 31
            "nCapture",                         // 32
            "hIoni",                            // 33
            "ionIoni",                          // 34
            "CoulombScat",                      // 35
            "DMProcessDMBrem",                  // 36
            "Rayl",                             // 37
            "muIoni",                           // 38
            "muPairProd",                       // 39
            "GammaToMuPair",                    // 40
            "muonNuclear",                      // 41
            "nKiller",                          // 42
    };
    /** the map of process types */
    unordered_map<string, int> dPhyTypeMap;

    /**Default constructor*/
    PhysicsDef() {
        for (unsigned int i = 0; i < dPhyTypeVec.size(); ++i) {
            dPhyTypeMap.insert({dPhyTypeVec.at(i), i});
        }

        cout << "[Physics Process Table] ==> Construction completed with " << dPhyTypeVec.size() << " processes."
             << endl;
    }
};

#endif //DSIMU_PHYSICSPROCESSDEF_H
