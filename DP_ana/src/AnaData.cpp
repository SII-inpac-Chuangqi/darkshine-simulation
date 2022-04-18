//
// Created by zhuxu on 2021/7/29.
//

#include <iostream>
#include "Core/AnaData.h"

#ifdef RM_UNIT
#define CUNIT 1
#else
#define CUNIT 10
#endif

// Required by Singleton
AnaData *dAnaData = nullptr;

// Get Instance Class
AnaData *AnaData::CreateInstance() {
    if (dAnaData == nullptr)
        dAnaData = new AnaData();
    return dAnaData;
}

AnaData::AnaData() {

}

void AnaData::ReadMagField() {
    if (! root_file->IsOpen() ) {
        std::cerr << "[READFILE ERROR] ==> InputGeoFile does not exist." << std::endl;
        exit(1);
    }
    mag_field_vec = std::vector<DMagnet*>({dynamic_cast<DMagnet*>(root_file->Get("magnet0")),
                                        dynamic_cast<DMagnet*>(root_file->Get("magnet1")),
                                        dynamic_cast<DMagnet*>(root_file->Get("magnet2"))});

}


void AnaData::readGeometryDetails() {
    world_ = dynamic_cast<TGeoNode*>(gGeoManager->GetListOfNodes()->At(0));
    if(!world_) {
        std::cerr << "[WARNING] ==> No world node ..." << std::endl;
        return;
    }

    strip_width_tag = -INFINITY;
    strip_length_tag = -INFINITY;
    strip_no_tag = -1;
    angles_tag.clear();

    strip_width_rec = -INFINITY;
    strip_length_rec = -INFINITY;
    strip_no_rec = -1;
    angles_rec.clear();

    N_ECal_cell_x = 0;
    N_ECal_cell_y = 0;
    N_ECal_cell_z = 0;
    double last_pos[3] = {0., 0., -INFINITY};

    for (int i = 0; i < world_->GetNdaughters(); ++i) {
        auto *detector = dynamic_cast<TGeoNode*>(world_->GetDaughter(i));
        auto detector_name = TString(detector->GetVolume()->GetName());

        if(detector_name.Contains("Trk")) {
            auto *detector_shape = dynamic_cast<TGeoBBox*>(detector->GetVolume()->GetShape());

            if(detector_name.Contains("TAG")) {
                strip_no_tag = detector->GetDaughter(0)->GetNdaughters();
                strip_width_tag = CUNIT*detector_shape->GetDX();
                strip_length_tag = CUNIT*detector_shape->GetDY();
            }
            else if(detector_name.Contains("REC")) {
                strip_no_rec = detector->GetDaughter(0)->GetNdaughters();
                strip_width_rec = CUNIT*detector_shape->GetDX();
                strip_length_rec = CUNIT*detector_shape->GetDY();
            }

            for(int j = 0; j < detector->GetNdaughters(); j++) {
                auto *layer = dynamic_cast<TGeoNode*>(detector->GetDaughter(j));
                auto strip_name = TString(layer->GetVolume()->GetName());
                auto rotation = layer->GetMatrix()->GetRotationMatrix();
                if(strip_name.Contains("Tag"))
                    angles_tag.push_back(std::asin(rotation[1]));
                else if(strip_name.Contains("Rec"))
                    angles_rec.push_back(std::asin(rotation[1]));
            }
        }

        if(detector_name.Contains("ECAL")) {
            auto *cur_shape = dynamic_cast<TGeoBBox*>(detector->GetVolume()->GetShape());
            ECAL_center_x = CUNIT*detector->GetMatrix()->GetTranslation()[0];
            ECAL_center_y = CUNIT*detector->GetMatrix()->GetTranslation()[1];
            ECAL_center_z = CUNIT*detector->GetMatrix()->GetTranslation()[2];
            ECAL_length_x = CUNIT*2*cur_shape->GetDX();
            ECAL_length_y = CUNIT*2*cur_shape->GetDY();
            ECAL_length_z = CUNIT*2*cur_shape->GetDZ();

            for (int j = 0; j < detector->GetNdaughters(); ++j) {
                auto *subdetector = dynamic_cast<TGeoNode*>(detector->GetDaughter(j));
                auto subdetector_name = TString(subdetector->GetVolume()->GetName());

                if (subdetector_name.Contains("LVW")) {
                    auto subdetector_pos = subdetector->GetMatrix()->GetTranslation();

                    if (subdetector_pos[2] != last_pos[2]) N_ECal_cell_z++;

                    if (N_ECal_cell_z == 1) {
                        if (subdetector_pos[1] != last_pos[1]) N_ECal_cell_y++;
                        if (N_ECal_cell_y == 1) {
                            if (subdetector_pos[0] != last_pos[0]) N_ECal_cell_x++;
                        }
                    }

                    for (int k = 0; k < 3; ++k)
                    last_pos[k] = subdetector_pos[k];
                }
            }
        }
    }
}

void AnaData::printGeometryDetails() const {
    std::cerr << "[INFO] ==> Geometry details:" << std::endl
              << "           Tag tracker: strip No.    " << strip_no_tag     << std::endl
              << "                        strip width  " << strip_width_tag  << std::endl
              << "                        strip length " << strip_length_tag << std::endl
              << "           Rec tracker: strip No.    " << strip_no_rec     << std::endl
              << "                        strip width  " << strip_width_rec  << " mm" << std::endl
              << "                        strip length " << strip_length_rec << " mm" << std::endl
              << "           ECal:        center x at  " << ECAL_center_x    << " mm" << std::endl
              << "                        center y at  " << ECAL_center_y    << " mm" << std::endl
              << "                        center z at  " << ECAL_center_z    << " mm" << std::endl
              << "                        length x     " << ECAL_length_x    << " mm" << std::endl
              << "                        length y     " << ECAL_length_y    << " mm" << std::endl
              << "                        length z     " << ECAL_length_z    << " mm" << std::endl
              << "                        cell No. x   " << N_ECal_cell_x    << std::endl
              << "                        cell No. y   " << N_ECal_cell_y    << std::endl
              << "                        cell No. z   " << N_ECal_cell_z    << std::endl;
}
