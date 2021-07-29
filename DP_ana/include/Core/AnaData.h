//
// Created by zhuxu on 2021/7/29.
//

#ifndef DSIMU_ANADATA_H
#define DSIMU_ANADATA_H

#include "yaml-cpp/yaml.h"

#include "TFile.h"

#include "Object/DMagnet.h"

#include <vector>
#include <tuple>

using std::vector,std::tuple;

class AnaData {
public:
    AnaData(const AnaData &) = delete;

    AnaData &operator=(AnaData const &) = delete;

    static AnaData *CreateInstance();

    void ReadMagField();

    [[nodiscard]] const vector<DMagnet *> &getMagFieldVec() const {
        return mag_field_vec;
    }

    [[nodiscard]] TFile *getRootFile() const {
        return root_file;
    }

    void setRootFile(TFile *rootFile) {
        root_file = rootFile;
        ReadMagField();
    }

protected:

    vector<DMagnet*> mag_field_vec;
    TFile* root_file;

private:
    AnaData();


};

extern AnaData *dAnaData;
#endif //DSIMU_ANADATA_H
