//
// Created by zhuxu on 2021/7/29.
//

#ifndef DSIMU_ANADATA_H
#define DSIMU_ANADATA_H

#include "yaml-cpp/yaml.h"

#include "TFile.h"
#include "TGeoBBox.h"
#include "TGeoManager.h"

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

    void readGeometryDetails();
    void printGeometryDetails() const;

    int    getStripNoTag()     const {return strip_no_tag;}
    double getStripWidthTag()  const {return strip_width_tag;}
    double getStripLengthTag() const {return strip_length_tag;}
    std::vector<double> getAnglesTag() const {return angles_tag;}

    int    getStripNoRec()     const {return strip_no_rec;}
    double getStripWidthRec()  const {return strip_width_rec;}
    double getStripLengthRec() const {return strip_length_rec;}
    std::vector<double> getAnglesRec() const {return angles_rec;}

    int getNECalCellX() const {return N_ECal_cell_x;}
    int getNECalCellY() const {return N_ECal_cell_y;}
    int getNECalCellZ() const {return N_ECal_cell_z;}

protected:
    vector<DMagnet*> mag_field_vec;
    TFile* root_file;

    TGeoNode* world_{nullptr};

    double strip_width_tag{-INFINITY};
    double strip_length_tag{-INFINITY};
    int strip_no_tag{-1};
    std::vector<double> angles_tag;

    double strip_width_rec{-INFINITY};
    double strip_length_rec{-INFINITY};
    int strip_no_rec{-1};
    std::vector<double> angles_rec;

    int N_ECal_cell_x{0};
    int N_ECal_cell_y{0};
    int N_ECal_cell_z{0};

private:
    AnaData();


};

extern AnaData *dAnaData;
#endif //DSIMU_ANADATA_H
