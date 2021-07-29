//
// Created by zhuxu on 2021/7/29.
//

#include <iostream>
#include "Core/AnaData.h"

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