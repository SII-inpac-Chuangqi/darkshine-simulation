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

    if (! root_file->Get("magnet0") ) {
        std::cerr << "[READFILE WARNING] ==> Magnet x not found" << std::endl;
    }
    if (! root_file->Get("magnet1") ) {
        std::cerr << "[READFILE WARNING] ==> Magnet y not found" << std::endl;
    }
    if (! root_file->Get("magnet2") ) {
        std::cerr << "[READFILE WARNING] ==> Magnet z not found" << std::endl;
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

    layer_width_tag.clear();
    layer_length_tag.clear();
    strip_no_tag.clear();
    angles_tag.clear();

    layer_width_rec.clear();
    layer_length_rec.clear();
    strip_no_rec.clear();
    angles_rec.clear();

    ECal_cell_length_x.clear();
    ECal_cell_length_y.clear();
    ECal_cell_length_z.clear();

    N_ECal_cell_x = 0;
    N_ECal_cell_y = 0;
    N_ECal_cell_z = 0;
    double last_pos[3] = {0., 0., -INFINITY};

    for (int i = 0; i < world_->GetNdaughters(); ++i) {
        auto *detector = dynamic_cast<TGeoNode*>(world_->GetDaughter(i));
        auto detector_name = TString(detector->GetVolume()->GetName());

        if(detector_name.Contains("Trk")) { // TAGTrk or RECTrk
            for(int j = 0; j < detector->GetNdaughters(); j++) {
                auto *layer = dynamic_cast<TGeoNode*>(detector->GetDaughter(j));
                auto *layer_shape = dynamic_cast<TGeoBBox*>(layer->GetVolume()->GetShape());
                auto layer_name = TString(layer->GetVolume()->GetName());
                auto rotation = layer->GetMatrix()->GetRotationMatrix();

                if(layer_name.Contains("Tag")) { // TagTrk1_LV or TagTRk2_LV
                    layer_width_tag.push_back(2.*CUNIT*layer_shape->GetDX());
                    layer_length_tag.push_back(2.*CUNIT*layer_shape->GetDY());
                    auto *block0 = dynamic_cast<TGeoNode*>(layer->GetDaughter(0));
                    strip_no_tag.push_back(layer->GetNdaughters() * block0->GetNdaughters());
                    angles_tag.push_back(std::asin(rotation[1]));
                }
                else if(layer_name.Contains("Rec")) {
                    layer_width_rec.push_back(2.*CUNIT*layer_shape->GetDX());
                    layer_length_rec.push_back(2.*CUNIT*layer_shape->GetDY());
                    auto *block0 = dynamic_cast<TGeoNode*>(layer->GetDaughter(0));
                    strip_no_rec.push_back(layer->GetNdaughters() * block0->GetNdaughters());
                    angles_rec.push_back(std::asin(rotation[1]));
                }
            }
        }



        if(detector_name.Contains("ECAL")) {
            auto *detector_shape = dynamic_cast<TGeoBBox*>(detector->GetVolume()->GetShape());
            ECAL_center_x = CUNIT*detector->GetMatrix()->GetTranslation()[0];
            ECAL_center_y = CUNIT*detector->GetMatrix()->GetTranslation()[1];
            ECAL_center_z = CUNIT*detector->GetMatrix()->GetTranslation()[2];
            ECAL_length_x = CUNIT*2*detector_shape->GetDX();
            ECAL_length_y = CUNIT*2*detector_shape->GetDY();
            ECAL_length_z = CUNIT*2*detector_shape->GetDZ();

            for (int j = 0; j < detector->GetNdaughters(); ++j) {
                auto *block = dynamic_cast<TGeoNode*>(detector->GetDaughter(j));
                auto block_name = TString(block->GetVolume()->GetName());
                for (int k = 0; k < block->GetNdaughters(); ++k ) {
                    auto *subdetector = dynamic_cast<TGeoNode*>(block->GetDaughter(k));
                    auto subdetector_name = TString(subdetector->GetVolume()->GetName());

                    if (subdetector_name.Contains("LVW")) {
                        auto *crystal = dynamic_cast<TGeoNode*>(subdetector->GetDaughter(0));
                        auto *crystal_shape = dynamic_cast<TGeoBBox*>(crystal->GetVolume()->GetShape());
                        ECal_cell_length_x.push_back(CUNIT*2*crystal_shape->GetDX());
                        ECal_cell_length_y.push_back(CUNIT*2*crystal_shape->GetDY());
                        ECal_cell_length_z.push_back(CUNIT*2*crystal_shape->GetDZ());

                        auto subdetector_pos = subdetector->GetMatrix()->GetTranslation();

                        if (subdetector_pos[2] != last_pos[2]) N_ECal_cell_z++;

                        if (N_ECal_cell_z == 1) {
                            if (subdetector_pos[1] != last_pos[1]) N_ECal_cell_y++;
                            if (N_ECal_cell_y == 1) {
                                if (subdetector_pos[0] != last_pos[0]) N_ECal_cell_x++;
                            }
                        }

                        for (int l = 0; l < 3; ++l)
                        last_pos[l] = subdetector_pos[l];
                    }
                }
            }
        }
    }
}

void AnaData::printGeometryDetails() const {
    std::cerr << "[INFO] ==> Geometry details:" << std::endl;
    if(strip_no_tag.size() && layer_width_tag.size() && layer_length_tag.size() &&
       strip_no_rec.size() && layer_width_rec.size() && layer_length_rec.size()   ) {
        std::cerr << "           Tag tracker: strip No.    ";
        for(size_t i = 0; i < strip_no_tag.size();     i += 2) std::cerr << strip_no_tag.at(i) << ", ";
        std::cerr << std::endl;
        std::cerr << "                        layer width  ";
        for(size_t i = 0; i < layer_width_tag.size();  i += 2) std::cerr << layer_width_tag.at(i)  << " mm, ";
        std::cerr << std::endl;
        std::cerr << "                        layer length ";
        for(size_t i = 0; i < layer_length_tag.size(); i += 2) std::cerr << layer_length_tag.at(i) << " mm, ";
        std::cerr << std::endl;

        std::cerr << "           Rec tracker: strip No.    ";
        for(size_t i = 0; i < strip_no_rec.size();     i += 2) std::cerr << strip_no_rec.at(i) << ", ";
        std::cerr << std::endl;
        std::cerr << "                        layer width  ";
        for(size_t i = 0; i < layer_width_rec.size();  i += 2) std::cerr << layer_width_rec.at(i)  << " mm, ";
        std::cerr << std::endl;
        std::cerr << "                        layer length ";
        for(size_t i = 0; i < layer_length_rec.size(); i += 2) std::cerr << layer_length_rec.at(i) << " mm, ";
        std::cerr << std::endl;
    }
    if(ECal_cell_length_x.size() && ECal_cell_length_y.size() && ECal_cell_length_z.size())
        std::cerr << "           ECal:        center x at  " << ECAL_center_x            << " mm" << std::endl
                  << "                        center y at  " << ECAL_center_y            << " mm" << std::endl
                  << "                        center z at  " << ECAL_center_z            << " mm" << std::endl
                  << "                        length x     " << ECAL_length_x            << " mm" << std::endl
                  << "                        length y     " << ECAL_length_y            << " mm" << std::endl
                  << "                        length z     " << ECAL_length_z            << " mm" << std::endl
                  << "                        cell size x  " << ECal_cell_length_x.at(0) << " mm" << std::endl
                  << "                        cell size y  " << ECal_cell_length_y.at(0) << " mm" << std::endl
                  << "                        cell size z  " << ECal_cell_length_z.at(0) << " mm" << std::endl
                  << "                        cell No. x   " << N_ECal_cell_x            << std::endl
                  << "                        cell No. y   " << N_ECal_cell_y            << std::endl
                  << "                        cell No. z   " << N_ECal_cell_z            << std::endl;
}
