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

    std::vector<double> getLayerWidthTag()  const {return layer_width_tag;}
    std::vector<double> getLayerLengthTag() const {return layer_length_tag;}
    std::vector<int>    getStripNoTag() const {return strip_no_tag;}
    std::vector<double> getAnglesTag()  const {return angles_tag;}

    std::vector<double> getLayerWidthRec()  const {return layer_width_rec;}
    std::vector<double> getLayerLengthRec() const {return layer_length_rec;}
    std::vector<int>    getStripNoRec() const {return strip_no_rec;}
    std::vector<double> getAnglesRec()  const {return angles_rec;}

    double getECalCenterX() const {return ECAL_center_x;}
    double getECalCenterY() const {return ECAL_center_y;}
    double getECalCenterZ() const {return ECAL_center_z;}
    double getECalLengthX() const {return ECAL_length_x;}
    double getECalLengthY() const {return ECAL_length_y;}
    double getECalLengthZ() const {return ECAL_length_z;}
    std::vector<double> getECalCellLengthX() const {return ECal_cell_length_x;}
    std::vector<double> getECalCellLengthY() const {return ECal_cell_length_y;}
    std::vector<double> getECalCellLengthZ() const {return ECal_cell_length_z;}
    int getNECalCellX() const {return N_ECal_cell_x;}
    int getNECalCellY() const {return N_ECal_cell_y;}
    int getNECalCellZ() const {return N_ECal_cell_z;}

protected:
    vector<DMagnet*> mag_field_vec;
    TFile* root_file;

    TGeoNode* world_{nullptr};

    std::vector<double> layer_width_tag;
    std::vector<double> layer_length_tag;
    std::vector<int> strip_no_tag;
    std::vector<double> angles_tag;

    std::vector<double> layer_width_rec;
    std::vector<double> layer_length_rec;
    std::vector<int> strip_no_rec;
    std::vector<double> angles_rec;

    double ECAL_center_x{-INFINITY};
    double ECAL_center_y{-INFINITY};
    double ECAL_center_z{-INFINITY};
    double ECAL_length_x{-INFINITY};
    double ECAL_length_y{-INFINITY};
    double ECAL_length_z{-INFINITY};
    std::vector<double> ECal_cell_length_x;
    std::vector<double> ECal_cell_length_y;
    std::vector<double> ECal_cell_length_z;
    int N_ECal_cell_x{0};
    int N_ECal_cell_y{0};
    int N_ECal_cell_z{0};

private:
    AnaData();

};

extern AnaData *dAnaData;
#endif //DSIMU_ANADATA_H
