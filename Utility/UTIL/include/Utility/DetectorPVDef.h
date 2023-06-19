//
// Created by Zhang Yulei on 6/6/22.
//

#ifndef DSIMU_DETECTORPVDEF_H
#define DSIMU_DETECTORPVDEF_H

#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>

using std::cout, std::endl, std::cerr;
using std::unordered_map, std::vector, std::string;

struct DetPVDef {

    /** the vector of process types */
    vector<string> dDetPVVec = {
            "Unknown",                             // 0
            "World",                               // 1
            "TAGTrk",                              // 2
            "TagTrk1_PV",                          // 3
            "TagTrk2_PV",                          // 4
            "TagTrk_Block_PV",                     // 5
            "TagTrk_Strip_PV",                     // 6
            "Target_PV",                           // 7
            "RECTrk",                              // 8
            "RecTrk1_PV",                          // 9
            "RecTrk2_PV",                          // 10
            "RecTrk_Block_PV",                     // 11
            "RecTrk_Strip_PV",                     // 12
            "ECAL",                                // 13
            "ECAL_PV_h1",                          // 14
            "ECAL_UnitPV",                         // 15
            "ECAL_APDWorld_PV",                    // 16
            "ECAL_PV",                             // 17
            "HCAL",                                // 18
            "HCAL_AbsPV",                          // 19
            "HCAL_LayerPV",                        // 20
            "HCAL_PV_h1",                          // 21
            "HCAL_UnitPV",                         // 22
            "HCAL_APDWorld_PV",                    // 23
            "HCAL_Fiber_PV",                       // 24
            "HCAL_FiberClad_PV",                   // 25
            "HCAL_PV",                             // 26
            "MagnetShield",                        // 27
            "SideHCAL",                            // 28
            "SideHCAL_AbsPV",                      // 29
            "SideHCAL_LayerPV",                    // 30
            "SideHCAL_APDWorld_PV",                // 31
            "SideHCAL_PV",                         // 32
            "SideHCAL_FiberClad_PV",               // 33
            "SideHCAL_Fiber_PV",                   // 34
    };
    /** the map of process types */
    unordered_map<string, int> dDetPVMap;

    /**Default constructor*/
    DetPVDef() {
        for (unsigned int i = 0; i < dDetPVVec.size(); ++i) {
            dDetPVMap.insert({dDetPVVec.at(i), i});
        }

        cout << "[Detector PV Table] ==> Construction completed with " << dDetPVVec.size() << " PV names."
             << endl;
    }

};

#endif //DSIMU_DETECTORPVDEF_H
