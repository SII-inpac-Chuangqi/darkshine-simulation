//
// Created by zhuxu on 2021/7/29.
//

#ifndef DSIMU_ANADATA_H
#define DSIMU_ANADATA_H

#ifndef RETURN
#define RETURN std::nan("RETURN")
#endif

#include "yaml-cpp/yaml.h"

#include "TFile.h"
#include "TGeoBBox.h"
#include "TGeoManager.h"
#include "TVector3.h"

#include "Object/DTruth.h"
#include "Object/DMagnet.h"
#include "Object/McPHelper.h"

#include <vector>
#include <tuple>
#include <unordered_map>

using std::vector, std::tuple;

#define MAX_ECAL_CELLS (25*25*15)

class AnaData {
public:
    AnaData(const AnaData &) = delete;

    AnaData &operator=(AnaData const &) = delete;

    static AnaData *CreateInstance();

    void ReadMagField();

    [[nodiscard]] const vector<DMagnet *> &getMagFieldVec() const {
        return mag_field_vec;
    }
    void setConstMagnetField(const vector<double> &const_value);
    [[nodiscard]] const vector<double> getMagnetFieldAt(const vector<double> &pos) const;

    [[nodiscard]] TFile *getRootFile() const {
        return root_file;
    }

    void setRootFile(TFile *rootFile) {
        root_file = rootFile;
        ReadMagField();
    }

//................................................................................//
//Geometry manager
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

    const std::array<TVector3,MAX_ECAL_CELLS>& getECalPosMap() const {return ECAL_posmap;};
    [[maybe_unused]] double getECalCenterX() const {return ECAL_center_x;}
    [[maybe_unused]] double getECalCenterY() const {return ECAL_center_y;}
    [[maybe_unused]] double getECalCenterZ() const {return ECAL_center_z;}
    [[maybe_unused]] double getECalLengthX() const {return ECAL_length_x;}
    [[maybe_unused]] double getECalLengthY() const {return ECAL_length_y;}
    [[maybe_unused]] double getECalLengthZ() const {return ECAL_length_z;}
    [[maybe_unused]] std::vector<double> getECalCellLengthX() const {return ECal_cell_length_x;}
    [[maybe_unused]] std::vector<double> getECalCellLengthY() const {return ECal_cell_length_y;}
    [[maybe_unused]] std::vector<double> getECalCellLengthZ() const {return ECal_cell_length_z;}
    int getNECalCellX() const {return N_ECal_cell_x;}
    int getNECalCellY() const {return N_ECal_cell_y;}
    int getNECalCellZ() const {return N_ECal_cell_z;}
    double getECalSurfaceZ() const {return ECAL_center_z - 0.5*ECAL_length_z;}

    int getProcessId(const std::string& n);
    void printProcessMap();

//................................................................................//
//Truth helper manager
    //void LoadTruthMcPHelper(const MCPHelperMap &helper_collection);
    void LoadTruthInfo(DTruth *truth);
    //void PrintTruthMcPHelper() const;
    void PrintTruthInfo() const;
    //const McPHelper* getInitialElectron() const;
    std::vector<const DTruthState*> getTruthTracksAtECalFront() const;
    unsigned int getNTruthTracks(DTruth::DTruthDetPV DetPV, double min_energy = 50., int min_hits = 4) const;
    const DTruth* getInitialElectron() const;

protected:
    TFile* root_file;

    bool if_const_field_{false};
    vector<DMagnet*> mag_field_vec;
    vector<double>   const_mag_field_vec;

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
    double ECAL_cell_dx{0};
    double ECAL_cell_dy{0};
    double ECAL_cell_dz{0};
    TVector3* ECAL_pos0{nullptr};
    std::array<TVector3,MAX_ECAL_CELLS> ECAL_posmap{};

    //MCPHelperVec* helper{nullptr};
    DTruth *truth_{nullptr};

    std::unordered_map<std::string,int> processMap{};
    std::unordered_map<int,std::string> rev_processMap{};

private:
    AnaData();

};

extern AnaData *dAnaData;
#endif //DSIMU_ANADATA_H
