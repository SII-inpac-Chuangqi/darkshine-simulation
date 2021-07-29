//
// Created by Zhang Yulei on 12/19/20.
//

#include "DEventDisplay.h"

#include "TGeoBBox.h"
#include "TGeoNode.h"
#include "TString.h"
#include "TMath.h"

#include <iostream>
#include <iomanip>

void DEventDisplay::inspectMainRegion() {
    if (!gGeoManager) {
        std::cerr << "[Det Info] ==> No gGeoManager..." << std::endl;
        return;
    }

    // Print daughter nodes information
    for (int i = 0; i < world_node->GetNdaughters(); ++i) {
        auto *current_node = dynamic_cast<TGeoNode *>(world_node->GetDaughter(i));
        auto *cur_shape = dynamic_cast<TGeoBBox *>(current_node->GetVolume()->GetShape());
        auto cur_Name = TString(current_node->GetVolume()->GetName());
        /*
         * print region logic volume
         * 0: Target
         * 1: TagTracker
         * 2: RecTracker
         * 3: ECAL
         * 4: HCAL
         */
        std::cout << "=============================================================================" <<std::endl;
        std::cout << std::left;
        std::cout << "Node " << i << ") " << std::setw(10) << cur_Name << ": ";
        std::cout << "center z = " << std::setw(10) << CUNIT * current_node->GetMatrix()->GetTranslation()[2] << " [mm], ";
        std::cout << std::right;
        std::cout << "size = (" << std::setw(8) << CUNIT * 2 * cur_shape->GetDX() << ","
                  << std::setw(8) << CUNIT * 2 * cur_shape->GetDY() << ","
                  << std::setw(8) << CUNIT * 2 * cur_shape->GetDZ() << ") [mm]";
        std::cout << std::endl;
        std::cout << "-----------------------------------------------------------------------------" <<std::endl;

        /* Print Sub Region Information */
        Det_Type dt = DNone;
        if (cur_Name.Contains("Target")) dt = DTarget;
        if (cur_Name.Contains("Trk")) dt = DTracker;
        if (cur_Name.Contains("ECAL")) {
            dt = DECAL;
            ECAL_Size = TVector3(cur_shape->GetDX(), cur_shape->GetDY(), cur_shape->GetDZ());
            ECAL_Z_Move = current_node->GetMatrix()->GetTranslation()[2];
        }
        if (cur_Name.Contains("HCAL")) dt = DHCAL;

        inspectSubRegion(i, dt);
    }

}


void DEventDisplay::inspectSubRegion(int id, Det_Type dt) {
    // Print daughter region of mother volume id
    auto *current_node = dynamic_cast<TGeoNode *>(world_node->GetDaughter(id));

    // Calorimeter
    // For ECAL, only the first Z layers is counted
    int n_cell[3] = {0, 0, 0};
    int n_cell2[2] = {0, 0};
    double size_cal[3] = {0., 0., 0.};
    double size_cal2[3] = {0., 0., 0.};
    double last_pos[3] = {0., 0., -9999.};
    auto mat_cal = TString("Unknown");
    auto mat_abs = TString("Unknown");
    double size_abs[3] = {0., 0., 0.};

    for (int i = 0; i < current_node->GetNdaughters(); ++i) {
        auto *cur_node = dynamic_cast<TGeoNode *>(current_node->GetDaughter(i));
        auto *cur_shape = dynamic_cast<TGeoBBox *>(cur_node->GetVolume()->GetShape());
        auto cur_Name = TString(cur_node->GetVolume()->GetName());
        // Tracker
        if (dt == DTracker) {
            auto mother_z = current_node->GetMatrix()->GetTranslation()[2];
            auto daughter_z = CUNIT * (mother_z + cur_node->GetMatrix()->GetTranslation()[2]); // mm
            if (cur_Name.Contains("1")) {
                std::cout << std::right;
                std::cout << "    ==> " << std::setw(3) << i << ") ";
                std::cout << std::left;
                std::cout << std::setw(10) << cur_Name << ": ";
                std::cout << "center z = " << std::setw(8) << daughter_z << " [mm], ";
                std::cout << std::right;
                std::cout << "size = (" << std::setw(6) << CUNIT * 2 * cur_shape->GetDX() << ","
                          << std::setw(6) << CUNIT * 2 * cur_shape->GetDY() << ","
                          << std::setw(6) << CUNIT * 2 * cur_shape->GetDZ() << ") [mm]";
                std::cout << std::endl;
            } else {
                auto rot_mat = cur_node->GetMatrix()->GetRotationMatrix();

                std::cout << std::right;
                std::cout << "    ==> Rotation of Tracker 2: ";
                std::cout << std::setw(4) << TMath::ATan2(rot_mat[1], rot_mat[0])
                          << " [rad]" << std::endl;
                break;
            }
        }
        // ECAL
        if (dt == DECAL && !cur_Name.Contains("LVW")) {
            auto daughter_pos = cur_node->GetMatrix()->GetTranslation();
            mat_cal = TString(cur_node->GetVolume()->GetMaterial()->GetName());
            // count Z layer
            if (daughter_pos[2] != last_pos[2]) n_cell[2]++;
            // Only count the first layer
            if (n_cell[2] == 1) {
                // count Y layer
                if (daughter_pos[1] != last_pos[1]) n_cell[1]++;
                // Only count the first layer
                if (n_cell[1] == 1) {
                    // count X layer
                    if (daughter_pos[0] != last_pos[0]) n_cell[0]++;
                }
            }

            // record ecal cell size
            size_cal[0] = CUNIT * 2 * cur_shape->GetDX();
            size_cal[1] = CUNIT * 2 * cur_shape->GetDY();
            size_cal[2] = CUNIT * 2 * cur_shape->GetDZ();

            ECAL_Cell_Arr = TVector3(n_cell[0],n_cell[1],n_cell[2]);
            ECAL_Cell_Size = TVector3(size_cal[0],size_cal[1],size_cal[2]);

            for (int j = 0; j < 3; ++j)
                last_pos[j] = daughter_pos[j];

        }
        // HCAL
        if (dt == DHCAL && !cur_Name.Contains("LVW") && cur_Name.Contains("_0")) {
            auto daughter_pos = cur_node->GetMatrix()->GetTranslation();
            // Concerning Cell First
            if (!cur_Name.Contains("Abs")) {
                mat_cal = TString(cur_node->GetVolume()->GetMaterial()->GetName());
                // count Z layer
                if (daughter_pos[2] != last_pos[2]) n_cell[2]++;
                // Only count the first layer
                if (n_cell[2] == 1) {
                    // count Y layer
                    if (daughter_pos[1] != last_pos[1]) n_cell[1]++;
                    // Only count the first layer
                    if (n_cell[1] == 1) {
                        // count X layer
                        if (daughter_pos[0] != last_pos[0]) n_cell[0]++;
                    }
                    // record first layer cell size
                    size_cal[0] = CUNIT * 2 * cur_shape->GetDX();
                    size_cal[1] = CUNIT * 2 * cur_shape->GetDY();
                    size_cal[2] = CUNIT * 2 * cur_shape->GetDZ();
                }
                // Only count the second layer
                if (n_cell[2] == 2) {
                    // count Y layer
                    if (daughter_pos[1] != last_pos[1]) n_cell2[1]++;
                    // Only count the first layer
                    if (n_cell[1] == 1) {
                        // count X layer
                        if (daughter_pos[0] != last_pos[0]) n_cell2[0]++;
                    }
                    // record second layer cell size
                    size_cal2[0] = CUNIT * 2 * cur_shape->GetDX();
                    size_cal2[1] = CUNIT * 2 * cur_shape->GetDY();
                    size_cal2[2] = CUNIT * 2 * cur_shape->GetDZ();
                }
            }
            // Concerning Absorber
            else {
                mat_abs = TString(cur_node->GetVolume()->GetMaterial()->GetName());
                // record absorber size
                size_abs[0] = CUNIT * 2 * cur_shape->GetDX();
                size_abs[1] = CUNIT * 2 * cur_shape->GetDY();
                size_abs[2] = CUNIT * 2 * cur_shape->GetDZ();
            }

            for (int j = 0; j < 3; ++j)
                last_pos[j] = daughter_pos[j];

        }
    }

    if (dt == DECAL) {
        std::cout << std::right;
        std::cout << "    ==> Arrangement: ";
        std::cout << std::setw(3) << n_cell[0] << ", "
                  << std::setw(3) << n_cell[1] << ", "
                  << std::setw(3) << n_cell[2] << " ";
        std::cout << std::endl;

        std::cout << "    ==> Cell Size: ";
        std::cout << size_cal[0] << ", "
                  << size_cal[1] << ", "
                  << size_cal[2] << " [mm]";
        std::cout << std::endl;

        std::cout << "    ==> Cell Material: " << mat_cal << std::endl;
    }

    if (dt == DHCAL) {
        std::cout << std::right;
        std::cout << "    ==> Layer Type 1: " <<std::endl;
        std::cout << "        -- Arrangement: ";
        std::cout << std::setw(3) << n_cell[0] << ", "
                  << std::setw(3) << n_cell[1] << ", "
                  << std::setw(3) << n_cell[2]/2. << " ";
        std::cout << std::endl;

        std::cout << "        -- Cell Size: ";
        std::cout << size_cal[0] << ", "
                  << size_cal[1] << ", "
                  << size_cal[2] << " [mm]";
        std::cout << std::endl;

        std::cout << "    ==> Layer Type 2: " <<std::endl;
        std::cout << "        -- Arrangement: ";
        std::cout << std::setw(3) << n_cell2[0] << ", "
                  << std::setw(3) << n_cell2[1] << ", "
                  << std::setw(3) << n_cell[2]/2. << " ";
        std::cout << std::endl;

        std::cout << "        -- Cell Size: ";
        std::cout << size_cal2[0] << ", "
                  << size_cal2[1] << ", "
                  << size_cal2[2] << " [mm]";
        std::cout << std::endl;

        std::cout << "    ==> Cell Material: " << mat_cal << std::endl;

        std::cout << "    ==> Absorber Size: ";
        std::cout << size_abs[0] << ", "
                  << size_abs[1] << ", "
                  << size_abs[2] << " [mm]";
        std::cout << std::endl;
        std::cout << "    ==> Absorber Material: " << mat_abs << std::endl;

    }
}


